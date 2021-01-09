#pragma once

#include "Ray.hpp"
#include "Vector3DF.hpp"
#include "Point3.hpp"

template<typename T>
class HitRecord {
public:
    Vector3<T> p;
    Vector3<T> normal;
    T t;
};

template<typename T>
class HittableObject {
public:
    virtual bool hit(const Ray<T>&, T, T, HitRecord<T>&) = 0;
};

