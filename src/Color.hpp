#pragma once

#include "Vector3.hpp"
#include "Vector3DF.hpp"
#include "General.hpp"

#include <iostream>
#include <cmath>

using ColorD = Vector3D;
using ColorF = Vector3F;

template<typename T>
void write_color(std::ostream &out, Vector3<T> pixel_color, int spp) noexcept {
    T r = pixel_color.x();
    T g = pixel_color.y();
    T b = pixel_color.z();

    T scale = 1.0/spp;
    // gamma = 2
    r = sqrt(r*scale);
    g = sqrt(g*scale);
    b = sqrt(b*scale);

    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(255.999 * clamp<T>(r, 0, 0.999)) << ' '
        << static_cast<int>(255.999 * clamp<T>(g, 0, 0.999)) << ' '
        << static_cast<int>(255.999 * clamp<T>(b, 0, 0.999)) << '\n';
}
