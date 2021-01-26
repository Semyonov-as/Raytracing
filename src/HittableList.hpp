#pragma once

#include "HittableObject.hpp"
#include "AABB.hpp"

#include <memory>
#include <vector>

template<typename T>
class HittableList : public HittableObject<T> {
public:
    std::vector<std::shared_ptr<HittableObject<T>>> objects;

    HittableList() noexcept {}
    HittableList(std::shared_ptr<HittableObject<T>> object) noexcept { add(object);}

    void clear() { objects.clear();}
    void add(std::shared_ptr<HittableObject<T>> object) noexcept { objects.push_back(object);}

    bool hit(const Ray<T>& r, T t_min, T t_max, HitRecord<T>& rec) const noexcept override;
    bool bounding_box(T, T, AABB<T>&) const override;
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

template<class T>
bool HittableList<T>::bounding_box(T t_min, T t_max, AABB<T>& out) const {
    if(objects.empty())
        return false;

    AABB<T> tmp_box;
    bool first = true;

    for(const auto& obj: objects) {
        if(!obj->bounding_box(t_min, t_max, tmp_box))
            return false;
        out = first ? tmp_box : AABB<T>::surrounding_box(tmp_box, out);
        first = false;
    }

    return true;
}
