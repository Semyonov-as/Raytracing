#pragma once

#include "General.hpp"
#include "Vector3.hpp"
#include "Perlin.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

#include <memory>
#include <cmath>


template<class T>
class Texture {
public:
    virtual Vector3<T> value(T, T, const Vector3<T>&) const = 0;
};

template<class T>
class SolidColor : public Texture<T> {
public:
    Vector3<T> color_value;

    SolidColor() {}
    SolidColor(Vector3<T> _color) : color_value(_color) {}
    SolidColor(T r, T g, T b) : SolidColor<T>(Vector3<T>(r, g, b)) {}

    Vector3<T> value(T, T, const Vector3<T>&) const override { return color_value;}
};

template<class T>
class CheckerTexture : public Texture<T> {
public:
    std::shared_ptr<Texture<T>> odd;
    std::shared_ptr<Texture<T>> even;

    CheckerTexture() {}
    CheckerTexture(std::shared_ptr<Texture<T>> _even, std::shared_ptr<Texture<T>> _odd) : even(_even), odd(_odd) {}
    CheckerTexture(Vector3<T> c1, Vector3<T> c2) : even(std::make_shared<SolidColor<T>>(c1)), odd(std::make_shared<SolidColor<T>>(c2)) {}

    Vector3<T> value(T u, T v, const Vector3<T>& p) const override {
        if(sin(10*p.x())*sin(10*p.y())*sin(10*p.z()) < 0)
            return odd->value(u, v, p);
        return even->value(u, v, p);
    }
};

template<class T>
class NoiseTexture : public Texture<T> {
public:
    Perlin<T> noise;
    T scale;

    NoiseTexture(T _scale) : scale(_scale) {}

    Vector3<T> value(T, T, const Vector3<T>& p) const override {return Vector3<T>(1, 1, 1)*0.5*(1+noise.noise(scale*p));}
};


template<class T>
class TurbulenceTexture : public Texture<T> {
public:
    Perlin<T> noise;
    T scale;

    TurbulenceTexture(T _scale) : scale(_scale) {}

    Vector3<T> value(T, T, const Vector3<T>& p) const override {return Vector3<T>(1, 1, 1)* 0.5 * (1 + sin(scale*p.z() + 10*noise.turb(p)));}
};

template<class T>
class ImageTexture : public Texture<T> {
private:
    unsigned char* data;
    int width, height;
    int bytes_per_scanline;

public:
    const static int bytes_per_pixel = 3;

    ImageTexture() : data(nullptr), width(0), height(0), bytes_per_scanline(0) {}
    ImageTexture(const char* filename) {
        auto components_per_pixel = bytes_per_pixel;

        data = stbi_load(filename, &width, &height, &components_per_pixel, components_per_pixel);

        if(!data) {
            std::cerr << "--------------------\n ERROR: Could not load texture image file '" << filename << "'.\n--------------";
            width = height = 0;
        }

        bytes_per_scanline = bytes_per_pixel * width;
    }

    ~ImageTexture() {
        delete data;
    }

    Vector3<T> value(T u, T v, const Vector3<T>&) const override {
        if(data == nullptr)
            return Vector3<T>(0, 1, 1);

        u = clamp<T>(u, 0.0, 1.0);
        v = 1.0 - clamp<T>(v, 0.0, 1.0);

        auto i = static_cast<int>(u*width);
        auto j = static_cast<int>(v*height);

        if(i >= width)
            i = width - 1;
        if(j >= height)
            j = height - 1;

        const auto color_scale = 1.0/255.0;
        auto pixel = data + j*bytes_per_scanline + i*bytes_per_pixel;

        return Vector3<T>(color_scale*pixel[0], color_scale*pixel[1], color_scale*pixel[2]);
    }
};


