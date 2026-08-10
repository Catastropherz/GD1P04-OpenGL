#ifndef PERLIN_NOISE_H
#define PERLIN_NOISE_H

#pragma once
#include <cmath>
#include <cstdint>
#include <ctime>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm> 
#include <glew.h>
#include <glfw3.h>

class PerlinNoise {
public:
    int seed;

    // Constructors
    PerlinNoise();
    PerlinNoise(int seedValue);

    // Core Noise Methods
    double RandomValue(int x, int y);
    double Smooth(int x, int y);
    double LinearInterpolate(double a, double b, double x);
    double CosineInterpolate(double a, double b, double x);
    double SmoothInterpolate(double x, double y);
    double TotalNoisePerPoint(double x, double y);

    // File Generation Helpers
    void GenerateAndSaveNoise(unsigned int width, unsigned int height, const std::string& saveFilePath);
};

#endif // PERLIN_NOISE_H