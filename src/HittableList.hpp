#pragma once

#include "HittableObject.hpp"

#include <memory>
#include <vector>

template<typename T>
class HittableList : public HittableObject<T> {
public:
    std::vector<std::shared_ptr<HittableObject<T>>> objects;

    HittableList() noexcept {}
    HittableList(std::shared_ptr<HittableList<T>> object) noexcept { add(object);}

    void clear() { objects.clear();}
    void add(std::shared_ptr<HittableList<T>> object) noexcept { objects.push_back(object);}

    virtual bool hit(const Ray<T>& r, T t_min, T t_max, HitRecord<T>& rec) const noexcept override;
};

template<typename T>
bool HittableList<T>::hit(const Ray<T>& r, T t_min, T t_max, HitRecord<T>& rec) const noexcept {
    HitRecord<T> tmp_rec;
    bool hit_any = false;
    auto closest = t_max;

    for (const auto& object : objects) {
        if (object->hit(r, t_min, closest, tmp_rec)) {
            hit_any = true;
            closest = tmp_rec.t;
            rec = tmp_rec;
        }
    }

    return hit_any;
}
