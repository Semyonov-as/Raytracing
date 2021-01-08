#pragma once

#include "Vec3.hpp"


template<typename T>
class Ray {
public:
    Vec3<T> orig;
    Vec3<T> dir;

    Ray() {}
    Ray(const Vec3<T> &origin, const Vec3<T> &direction) : orig(origin), dir(direction) {}

    Vec3<T> origin() const noexcept { return orig;}
    Vec3<T> direction() const noexcept { return dir;}

    Vec3<T> at(T t) const noexcept { return orig + t*dir;}
};


using RayD = Ray<double>;
using RayF = Ray<float>;
