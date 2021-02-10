#pragma once

#include "General.hpp"
#include "HittableObject.hpp"
#include "Materials.hpp"
#include "Textures.hpp"


template<class T>
class ConstantMedium : public HittableObject<T> {
public:
    std::shared_ptr<HittableObject<T>> boundary;
    std::shared_ptr<Material<T>> phase_function;
    T neg_inv_density;

    ConstantMedium() {}
    ConstantMedium(std::shared_ptr<HittableObject<T>> b, T d, std::shared_ptr<Texture<T>> a)
        : boundary(b), neg_inv_density(-1/d),  phase_function(a) {}
    ConstantMedium(std::shared_ptr<HittableObject<T>> b, T d, Vector3<T> color)
        : boundary(b), neg_inv_density(-1/d),  phase_function(std::make_shared<Isotropic<T>>(color)) {}

    bool bounding_box(T t0, T t1, AABB<T>& out) const override { return boundary->bounding_box(t0, t1, out);}
    bool hit(const Ray<T>& r, T t0, T t1, HitRecord<T>& rec) const noexcept override {
        HitRecord<T> rec_1, rec_2;

        if(!boundary->hit(r, -infinity, infinity, rec_1))
            return false;

        if(!boundary->hit(r, rec_1.t + 0.0001, infinity, rec_2))
            return false;

        if(rec_1.t < t0)
            rec_1.t = t0;

        if(rec_2.t > t1)
            rec_2.t = t1;

        if(rec_1.t > rec_2.t)
            return false;

        if(rec_1.t < 0)
            rec_1.t = 0;

        const auto ray_length = r.dir.length();
        const auto distance_inside_boundary = (rec_2.t - rec_1.t)*ray_length;
        const auto hit_distance = neg_inv_density*log(random<double>(0, 1));

        if(hit_distance > distance_inside_boundary)
            return false;

        rec.t = rec_1.t = hit_distance/ray_length;
        rec.p = r.at(rec.t);

        rec.normal = Vector3<T>(1, 0, 0);
        rec.front_face = true;
        rec.mat_ptr = phase_function;

        return true;
    }
};
