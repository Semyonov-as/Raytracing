#pragma once

#include "General.hpp"
#include "HittableObject.hpp"
#include "Ray.hpp"
#include "Vector3.hpp"
#include "Textures.hpp"

#include<cmath>
#include<stdexcept>

template<typename>
class HitRecord;

template<typename T>
class Material {
public:
    virtual bool scatter(const Ray<T>&, const HitRecord<T>&, Vector3<T>&, Ray<T>&) const = 0;
    virtual Vector3<T> emitted(T, T, const Vector3<T>&) const { return Vector3<T>(0, 0, 0);}
};

template<typename T>
class Lambertian : public Material<T> {
public:
    std::shared_ptr<Texture<T>> albedo;

    Lambertian(const Vector3<T>& _albedo) : albedo(std::make_shared<SolidColor<T>>(_albedo)) {}
    Lambertian(std::shared_ptr<Texture<T>> _albedo) : albedo(_albedo) {}

    bool scatter(const Ray<T>& r_in, const HitRecord<T>& rec, Vector3<T>& att, Ray<T>& r_out) const override {
        auto direction = rec.normal + Vector3<T>::random_unit_vector();
        if (direction.near_zero())
            direction = rec.normal;
        r_out = Ray<T>(rec.p, direction, r_in.time);
        att = albedo->value(rec.u, rec.v, rec.p);
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

        if (sin*k > 1.0 || reflectance(cos, k) > fast_random<T>())
            r_out = Ray<T>(rec.p, r_in.dir.reflect(rec.normal), r_in.time);
        else
            r_out = Ray<T>(rec.p, r_in.dir.refract(rec.normal, k), r_in.time);

        return true;
    }

private:
    static T reflectance(T cos, T k) {
        T r0 = (1-k)/(1+k);
        r0 = r0*r0;
        return r0 + (1-r0)*pow((1-cos), 5);
    }
};

template<class T>
class DiffuseLight : public Material<T> {
public:
    std::shared_ptr<Texture<T>> emit;

    DiffuseLight(std::shared_ptr<Texture<T>> a) : emit(a) {}
    DiffuseLight(Vector3<T> c) : emit(std::make_shared<SolidColor<T>>(c)) {}

    bool scatter(const Ray<T>&, const HitRecord<T>&, Vector3<T>&, Ray<T>&) const override { return false;}
    Vector3<T> emitted(T u, T v, const Vector3<T>& p) const override { return emit->value(u, v, p);}
};

template<class T>
class Isotropic : public Material<T> {
public:
    std::shared_ptr<Texture<T>> albedo;

    Isotropic(Vector3<T> color) : albedo(std::make_shared<SolidColor<T>>(color)) {}
    Isotropic(std::shared_ptr<Texture<T>> _al) : albedo(_al) {}

    bool scatter(const Ray<T>& r_in, const HitRecord<T>& rec, Vector3<T>& att, Ray<T>& r_out) const override {
        r_out = Ray<T>(rec.p, Vector3<T>::random_unit_vector(), r_in.time);
        att = albedo->value(rec.u, rec.v, rec.p);
        return true;
    }
};
