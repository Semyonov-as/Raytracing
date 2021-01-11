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
T random(T min = 0.0, T max = 1.0) {
    static std::uniform_real_distribution<T> distribution(min, max);
    static std::mt19937 generator;
    return distribution(generator);
}

