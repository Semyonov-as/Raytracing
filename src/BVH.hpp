#pragma once

#include "General.hpp"
#include "HittableObject.hpp"
#include "HittableList.hpp"
#include "Ray.hpp"

#include <memory>
#include <cassert>
#include <algorithm>


template<class T>
class BvhNode : public HittableObject<T> {
public:
    std::shared_ptr<HittableObject<T>> left;
    std::shared_ptr<HittableObject<T>> right;
    AABB<T> box;

    BvhNode() {}
    BvhNode(const std::vector<std::shared_ptr<HittableObject<T>>>& , size_t, size_t, T, T);
    BvhNode(const HittableList<T>& list, T time0, T time1)
        : BvhNode(list.objects, 0, list.objects.size(), time0, time1) {}

    bool hit(const Ray<T>&, T, T, HitRecord<T>&) const override;
    bool bounding_box(T, T, AABB<T>&) const override;

    static bool box_compare(std::shared_ptr<HittableObject<T>>, std::shared_ptr<HittableObject<T>>, int);
    static bool box_x_compare(std::shared_ptr<HittableObject<T>> a, std::shared_ptr<HittableObject<T>> b) {return box_compare(a, b, 0);}
    static bool box_y_compare(std::shared_ptr<HittableObject<T>> a, std::shared_ptr<HittableObject<T>> b) {return box_compare(a, b, 1);}
    static bool box_z_compare(std::shared_ptr<HittableObject<T>> a, std::shared_ptr<HittableObject<T>> b) {return box_compare(a, b, 2);}
};

template<class T>
bool BvhNode<T>::bounding_box(T _, T __, AABB<T>& out) const {
    out = box;
    return true;
}

template<class T>
bool BvhNode<T>::hit(const Ray<T>& r, T t_min, T t_max, HitRecord<T>& rec) const {
    if(!box.hit(r, t_min, t_max))
        return false;

    bool hit_left = left->hit(r, t_min, t_max, rec);
    bool hit_right = right->hit(r, t_min, hit_left ? rec.t : t_max, rec);

    return hit_left || hit_right;
}

template<class T>
bool BvhNode<T>::box_compare(std::shared_ptr<HittableObject<T>> a, std::shared_ptr<HittableObject<T>> b, int axis) {
    AABB<T> box_a;
    AABB<T> box_b;

    assert((!a->bounding_box(0, 0, box_a) || !b->bounding_box(0, 0, box_b))
           &&"No bounding box in BvhNode constructor.\n");

    return box_a.minimum[axis] < box_b.minimum[axis];
}

template<class T>
BvhNode<T>::BvhNode(const std::vector<std::shared_ptr<HittableObject<T>>>& src_obj,
                    size_t start, size_t end, T t0, T t1) {
    auto obj = src_obj;

    int axis = random_int(0, 2);
    auto comparator = (axis == 0) ? box_x_compare : (axis == 1) ? box_y_compare : box_z_compare;

    size_t obj_span = end - start;

    if(obj_span == 1)
        left = right = obj[start];
    else if(obj_span == 2) {
        if(comparator(obj[start], obj[start + 1])) {
            left = obj[start];
            right = obj[start + 1];
        } else {
            left = obj[start + 1];
            right = obj[start];
        }
    } else {
        std::sort(obj.begin() + start, obj.begin() + end, comparator);

        auto mid = start + obj_span/2;
        left = std::make_shared<BvhNode<T>>(obj, start, mid, t0, t1);
        right = std::make_shared<BvhNode<T>>(obj, mid, end, t0, t1);
    }

    AABB<T> box_left, box_right;

    assert((!left->bounding_box(t0, t1, box_left) || !right->bounding_box(t0, t1, box_right))
           &&"No bounding box in BvhNode constructor.\n");

    box = AABB<T>::surrounding_box(box_left, box_right);
}


