#pragma once
#include "PerlinNoise.h"
#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <glm.hpp>
#include <chrono>
#include <fstream>
#include <vector>
#include <cstdint>
#include <string>
#include <cmath>
#include <ctime>
#include <algorithm>

// Default constructor initializes seed using a high-resolution clock
PerlinNoise::PerlinNoise()
{
    auto now = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    // keep seed in a signed 32-bit range
    seed = static_cast<int>(now & 0x7fffffff);
}

PerlinNoise::PerlinNoise(int seedValue) 
{
    seed = seedValue;
}

double PerlinNoise::RandomValue(int x, int y) 
{
    int noise = x + y * seed;
    noise = (noise << 13) ^ noise;
    int t = (noise * (noise * noise * 15731 + 789221) + 1376312589) & 0x7fffffff;
    double randomVal = 1.0 - static_cast<double>(t) * 0.93132257461548515625e-9;
    return randomVal;
}

double PerlinNoise::Smooth(int x, int y) 
{
    double corners = (RandomValue(x - 1, y - 1) +
        RandomValue(x + 1, y - 1) +
        RandomValue(x - 1, y + 1) +
        RandomValue(x + 1, y + 1)) / 16.0;

    double sides = (RandomValue(x - 1, y) +
        RandomValue(x + 1, y) +
        RandomValue(x, y - 1) +
        RandomValue(x, y + 1)) / 8.0;

    double center = RandomValue(x, y) / 4.0;

    return corners + sides + center;
}

// Linear Interpolation
double PerlinNoise::LinearInterpolate(double a, double b, double x) 
{
    return a * (1.0 - x) + b * x;
}

// Cosine Interpolation
double PerlinNoise::CosineInterpolate(double a, double b, double x) 
{
    const double PI = 3.14159265358979323846;
    double ft = x * PI;
    double f = (1.0 - cos(ft)) * 0.5;
    return a * (1.0 - f) + b * f;
}

// Smooth Bilinear Interpolation using Cosine Interpolation
double PerlinNoise::SmoothInterpolate(double x, double y) 
{
    int truncatedX = static_cast<int>(x);
    int truncatedY = static_cast<int>(y);

    double fractX = x - static_cast<double>(truncatedX);
    double fractY = y - static_cast<double>(truncatedY);

    // Smoothing corners/edges
    double v1 = Smooth(truncatedX, truncatedY);
    double v2 = Smooth(truncatedX + 1, truncatedY);
    double v3 = Smooth(truncatedX, truncatedY + 1);
    double v4 = Smooth(truncatedX + 1, truncatedY + 1);

    // Interpolates
    double interpolate_1 = CosineInterpolate(v1, v2, fractX);
    double interpolate_2 = CosineInterpolate(v3, v4, fractX);
    double finalVal = CosineInterpolate(interpolate_1, interpolate_2, fractY);

    return finalVal;
}

double PerlinNoise::TotalNoisePerPoint(double x, double y) 
{
    int octaves = 4;
    float wavelength = 128.0f;
    float gain = 0.5f;
    float lacunarity = 2.0f;

    float maxValue = 0.0f;
    double total = 0.0;

    for (int i = 0; i < octaves; i++) 
    {
        double frequency = pow(lacunarity, i) / wavelength;
        double amplitude = pow(gain, i);
        maxValue += amplitude;

        total += SmoothInterpolate(x * frequency, y * frequency) * amplitude;
    }

    return total / maxValue;
}

// Generates pixels and exports RAW (heightmap) and JPG files
void PerlinNoise::GenerateAndSaveNoise(unsigned int width, unsigned int height, const std::string& saveFilePath) 
{
    // Re-seed on each generation using high-resolution time so repeated scene loads generate different noise textures.
    auto now = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    seed = static_cast<int>(now & 0x7fffffff);

    // Single channel grayscale pixels
    std::vector<uint8_t> greyPixels(width * height);

    auto computeGrey = [&](int x, int y) -> uint8_t {
        double n = TotalNoisePerPoint(x, y);
        double mapped = (n + 1.0) * 0.5 * 255.0;
        mapped = glm::clamp(mapped, 0.0, 255.0);
        return static_cast<uint8_t>(mapped);
    };

    // Fill grayscale pixels
    for (unsigned int j = 0; j < height; ++j)
    {
        for (unsigned int i = 0; i < width; ++i)
        {
            greyPixels[j * width + i] = computeGrey(i, j);
        }
    }

    // Save RAW file
    std::ofstream rawFile(saveFilePath + ".raw", std::ios_base::binary);
    if (rawFile)
    {
        rawFile.write(reinterpret_cast<char*>(greyPixels.data()), static_cast<std::streamsize>(greyPixels.size()));
        rawFile.close();
    }

    // Write grayscale JPG
    std::string greyPath = saveFilePath + "_grey_" + std::to_string(seed) + ".jpg";
    stbi_write_jpg(greyPath.c_str(), width, height, 1, greyPixels.data(), 100);

    // Create RGB gradient image (white, yellow, red, black)
    std::vector<uint8_t> gradPixels(width * height * 3);
    auto lerp = [](double a, double b, double t) { return a * (1.0 - t) + b * t; };
    auto mapToGradient = [&](uint8_t v) {
        double t = static_cast<double>(v) / 255.0;
        // gradient stops: 0.0 = black, 0.33 = red, 0.66 = yellow, 1.0 = white
        glm::vec3 c0(0.0f, 0.0f, 0.0f); // black
        glm::vec3 c1(1.0f, 0.0f, 0.0f); // red
        glm::vec3 c2(1.0f, 1.0f, 0.0f); // yellow
        glm::vec3 c3(1.0f, 1.0f, 1.0f); // white

        glm::vec3 col;
        if (t < 0.33)
        {
            double localT = t / 0.33;
            col = glm::vec3(lerp(c0.r, c1.r, localT), lerp(c0.g, c1.g, localT), lerp(c0.b, c1.b, localT));
        }
        else if (t < 0.66)
        {
            double localT = (t - 0.33) / 0.33;
            col = glm::vec3(lerp(c1.r, c2.r, localT), lerp(c1.g, c2.g, localT), lerp(c1.b, c2.b, localT));
        }
        else
        {
            double localT = (t - 0.66) / 0.34;
            col = glm::vec3(lerp(c2.r, c3.r, localT), lerp(c2.g, c3.g, localT), lerp(c2.b, c3.b, localT));
        }
        return glm::u8vec3(static_cast<uint8_t>(glm::clamp(col.r, 0.0f, 1.0f) * 255.0f),
                           static_cast<uint8_t>(glm::clamp(col.g, 0.0f, 1.0f) * 255.0f),
                           static_cast<uint8_t>(glm::clamp(col.b, 0.0f, 1.0f) * 255.0f));
    };

    for (unsigned int j = 0; j < height; ++j)
    {
        for (unsigned int i = 0; i < width; ++i)
        {
            uint8_t g = greyPixels[j * width + i];
            glm::u8vec3 c = mapToGradient(g);
            int idx = (j * width + i) * 3;
            gradPixels[idx + 0] = c.r;
            gradPixels[idx + 1] = c.g;
            gradPixels[idx + 2] = c.b;
        }
    }

    std::string gradPath = saveFilePath + "_grad_" + std::to_string(seed) + ".jpg";
    stbi_write_jpg(gradPath.c_str(), width, height, 3, gradPixels.data(), 100);

    // Create an animated sprite sheet (columns x rows frames)
    const int columns = 4;
    const int rows = 4;
    const int frames = columns * rows;
    int sheetW = static_cast<int>(width) * columns;
    int sheetH = static_cast<int>(height) * rows;
    std::vector<uint8_t> sheetPixels(sheetW * sheetH * 3);

    for (int f = 0; f < frames; ++f)
    {
        int tileX = f % columns;
        int tileY = f / columns;
        // small phase offset per frame for looping effect
        double phase = static_cast<double>(f) / static_cast<double>(frames) * 6.283185307179586;
        for (int j = 0; j < static_cast<int>(height); ++j)
        {
            for (int i = 0; i < static_cast<int>(width); ++i)
            {
                double sample = TotalNoisePerPoint(i + static_cast<int>(phase * 10.0), j + f * 7);
                double mapped = (sample + 1.0) * 0.5 * 255.0;
                mapped = glm::clamp(mapped, 0.0, 255.0);
                uint8_t gv = static_cast<uint8_t>(mapped);
                glm::u8vec3 c = mapToGradient(gv);

                int sx = tileX * static_cast<int>(width) + i;
                int sy = tileY * static_cast<int>(height) + j;
                int sidx = (sy * sheetW + sx) * 3;
                sheetPixels[sidx + 0] = c.r;
                sheetPixels[sidx + 1] = c.g;
                sheetPixels[sidx + 2] = c.b;
            }
        }
    }

    std::string animPath = saveFilePath + "_anim_" + std::to_string(seed) + ".jpg";
    stbi_write_jpg(animPath.c_str(), sheetW, sheetH, 3, sheetPixels.data(), 100);
}