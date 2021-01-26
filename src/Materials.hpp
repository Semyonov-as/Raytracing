#pragma once

#include "General.hpp"
#include "HittableObject.hpp"
#include "Ray.hpp"
#include "Vector3.hpp"

#include<cmath>
#include<stdexcept>

template<typename>
class HitRecord;

template<typename T>
class Material {
public:
    virtual bool scatter(const Ray<T>&, const HitRecord<T>&, Vector3<T>&, Ray<T>&) const = 0;
};

template<typename T>
class Lambertian : public Material<T> {
public:
    Vector3<T> albedo;

    Lambertian(const Vector3<T> _albedo) noexcept : albedo(_albedo) {}

    bool scatter(const Ray<T>& r_in, const HitRecord<T>& rec, Vector3<T>& att, Ray<T>& r_out) const override {
        auto direction = rec.normal + Vector3<T>::random_unit_vector();
        if (direction.near_zero())
            direction = rec.normal;
        r_out = Ray<T>(rec.p, direction, r_in.time);
        att = albedo;
        return true;
    }
};

template<typename T>
class Metal : public Material<T> {
public:
    Vector3<T> albedo;
    T fuzz;

    Metal(const Vector3<T>& _albedo, T _fuzz) noexcept : albedo(_albedo), fuzz(_fuzz < 1 ? _fuzz : 1) {}

    bool scatter(const Ray<T>& r_in, const HitRecord<T>& rec, Vector3<T>& att, Ray<T>& r_out) const override {
        r_out = Ray<T>(rec.p, r_in.dir.reflect(rec.normal) + fuzz*Vector3<T>::random_unit_vector(), r_in.time);
        att = albedo;
        return dot(r_out.dir, rec.normal) > 0;
    }
};

template<typename T>
class Dielectric : public Material<T> {
public:
    T ir;

    Dielectric(T _ir) : ir(_ir) { if (_ir < 0) throw std::invalid_argument("wrong index of refraction");}

    bool scatter(const Ray<T>& r_in, const HitRecord<T>& rec, Vector3<T>& att, Ray<T>& r_out) const override {
        att = Vector3<T>(1.0, 1.0, 1.0);
        T k = rec.front_face ? 1/ir : ir;
        T cos = fmin(dot(-r_in.dir.unit(), rec.normal), 1.0);
        T sin = sqrt(1.0 - cos*cos);

        if (sin*k > 1.0 || reflectance(cos, k) > random<T>(0.0, 1.0))
            r_out = Ray<T>(rec.p, r_in.dir.reflect(rec.normal), r_in.time);
        else
            r_out = Ray<T>(rec.p, r_in.dir.refract(rec.normal, k), r_in.time);

        return true;
    }

private:
    static T reflectance(T cos, T k) {
        T r0 = (1-k)/(1+k);
        r0 *= r0;
        return r0 + (1-r0)*pow((1-cos), 5);
    }
};
