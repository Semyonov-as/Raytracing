#pragma once

#include "General.hpp"
#include "Vector3.hpp"

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
