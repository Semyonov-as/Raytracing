#pragma once

#include <cmath>
#include <limits>
#include <memory>
#include <random>


// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

template<typename T>
T random(T min, T max) {
    static std::uniform_real_distribution<T> distribution(min, max);
    static std::mt19937 generator;
    return distribution(generator);
}

template<typename T>
T clamp(T x, T min, T max) {
    if (x < min)
        return min;
    if (x > max)
        return max;
    return x;
}

