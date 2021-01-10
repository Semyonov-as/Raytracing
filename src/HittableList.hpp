#pragma once

#include <memory>
#include <vector>

#include "HittableObject.hpp"

template<typename T>
class HittableList : public HittableObject<T> {
public:
    std::vector<std::shared_ptr<HittableObject<T>>> objects;

    HittableList() {}
    HittableList(std::shared_ptr<HittableList<T>> object) { add(object);}

    void clear() { objects.clear();}
    void add(std::shared_ptr<HittableList<T>> object) { objects.push_back(object);}

    virtual bool hit(const Ray<T>&, T, T, HitRecord<T>&) const override;
};

template<typename T>
bool HittableList<T>::hit(const Ray<T>& r, T t_min, T t_max, HitRecord<T>& rec) const {
    HitRecord<T> tmp_rec;
    bool hit_any = false;
    T closest = t_max;

    for(const std::shared_ptr<HittableObject<T>>& object : objects) {
        if(object->hit(r, t_min, closest, tmp_rec)) {
            hit_any = true;
            closest = tmp_rec.t;
            rec = tmp_rec;
        }
    }

    return hit_any;
}
