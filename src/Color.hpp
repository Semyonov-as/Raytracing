#pragma once

#include "Vector3.hpp"
#include "Vector3DF.hpp"
#include "General.hpp"

#include <iostream>
#include <cmath>
#include <vector>

using ColorD = Vector3D;
using ColorF = Vector3F;

class PIXEL {
public:
    int r, g, b;

    PIXEL() {}
    PIXEL(int _r, int _g, int _b) : r(_r), g(_g), b(_b) {}
};

class PPM_IMAGE {
public:
    std::vector<std::vector<PIXEL>> data;
    int width;
    int height;

    PPM_IMAGE(int _width, int _height) {
        width = _width;
        height = _height;
        data = std::vector<std::vector<PIXEL>>(height, std::vector<PIXEL>(width, PIXEL(0, 0, 0)));
    }

    //add exception!
    void set_pixel(int w, int h, PIXEL p) { data[w][h] = p;}

    friend std::ostream& operator<<(std::ostream &out, const PPM_IMAGE& img) {
        out << "P3\n" << img.width << ' ' << img.height << "\n255\n";
        for(int h = img.height - 1; h >= 0; --h) {
            for(int w = 0; w < img.width; ++w)
                out << img.data[h][w].r << ' ' << img.data[h][w].g << ' ' << img.data[h][w].b << '\n';
        }

        return out;
    }
};


template<typename T>
void write_color(PPM_IMAGE& img, int h, int w, Vector3<T> pixel_color, int spp) noexcept {
    T r = pixel_color.x();
    T g = pixel_color.y();
    T b = pixel_color.z();

    T scale = 1.0/spp;
    // gamma = 2
    r = sqrt(r*scale);
    g = sqrt(g*scale);
    b = sqrt(b*scale);

    // Write the translated [0,255] value of each color component.
    PIXEL p(static_cast<int>(255.999 * clamp<T>(r, 0, 0.999)),
            static_cast<int>(255.999 * clamp<T>(g, 0, 0.999)),
            static_cast<int>(255.999 * clamp<T>(b, 0, 0.999)));
    img.set_pixel(h, w, p);
}
