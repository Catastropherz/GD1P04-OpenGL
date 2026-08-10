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

// Default constructor initializes seed using a high-resolution clock to
// ensure different seeds even when the scene is reloaded quickly.
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
    // Re-seed on each generation using high-resolution time so repeated
    // scene loads generate different noise textures.
    auto now = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    seed = static_cast<int>(now & 0x7fffffff);

    std::vector<uint8_t> pixels(width * height);
    int index = 0;

    for (unsigned int col = 0; col < height; col++) 
    {
        for (unsigned int row = 0; row < width; row++) 
        {
            float noise = static_cast<float>(TotalNoisePerPoint(row, col));

            // Map/scale value from [-1, 1] range to [0, 255] byte range
            noise = (noise + 1.0f) * 0.5f * 255.0f;

            // Clamp value to safe 8-bit bounds
            noise = glm::clamp(noise, 0.0f, 255.0f);

            pixels[index++] = static_cast<uint8_t>(noise);
        }
    }

    // Create RAW file
    std::ofstream rawFile(saveFilePath + ".raw", std::ios_base::binary);
    if (rawFile)
    {
        rawFile.write(reinterpret_cast<char*>(pixels.data()), static_cast<std::streamsize>(pixels.size()));
        rawFile.close();
    }

    // Create JPG file using stb_image_write
    std::string jpgPath = saveFilePath + "_" + std::to_string(seed) + ".jpg";
    stbi_write_jpg(jpgPath.c_str(), width, height, 1, pixels.data(), 100);
}