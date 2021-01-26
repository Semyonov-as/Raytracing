#pragma once

#include "Vector3.hpp"

template<typename T>
class Ray {
public:
    Vector3<T> orig;
    Vector3<T> dir;
    T time;

    Ray() noexcept {}
    Ray(const Vector3<T> &origin, const Vector3<T> &direction, T _time = 0.0) noexcept : orig(origin), dir(direction), time(_time) {}

    Vector3<T> direction() const noexcept { return dir;}

    Vector3<T> at(T t) const noexcept { return orig + t*dir;}
};

using RayD = Ray<double>;
using RayF = Ray<float>;
