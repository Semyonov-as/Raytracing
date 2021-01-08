#pragma once

#include "Vector3.hpp"

template<typename T>
class Ray {
public:
    Vector3<T> orig;
    Vector3<T> dir;

    Ray() {}
    Ray(const Vector3<T> &origin, const Vector3<T> &direction) : orig(origin), dir(direction) {}

    Vector3<T> origin() const noexcept { return orig;}
    Vector3<T> direction() const noexcept { return dir;}

    Vector3<T> at(T t) const noexcept { return orig + t*dir;}
};


using RayD = Ray<double>;
using RayF = Ray<float>;
