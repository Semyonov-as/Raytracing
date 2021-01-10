#pragma once

#include "Vector3.hpp"
#include "Vector3DF.hpp"

#include <iostream>

using ColorD = Vector3D;
using ColorF = Vector3F;

template<typename T>
void write_color(std::ostream &out, Vector3<T> pixel_color) noexcept {
    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(255.999 * pixel_color.x()) << ' '
        << static_cast<int>(255.999 * pixel_color.y()) << ' '
        << static_cast<int>(255.999 * pixel_color.z()) << '\n';
}
