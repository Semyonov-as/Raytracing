#pragma once

#include "General.hpp"
#include "Vector3.hpp"
#include "Ray.hpp"


template<class T>
class AABB {
public:
    Vector3<T> minimum;
    Vector3<T> maximum;

    AABB() {}
    AABB(const Vector3<T>& a, const Vector3<T>& b) { minimum = a; maximum = b;}

    bool hit(const Ray<T>& r, T t_min, T t_max) const {
        for(int i = 0; i < 3; i++) {
            auto invD = 1.0f/r.dir[i];
            auto t0 = invD*(minimum[i] - r.orig[i]);
            auto t1 = invD*(maximum[i] - r.orig[i]);

            if(invD < 0.0f)
                std::swap(t0, t1);

            t_min = fmax(t0, t_min);
            t_max = fmin(t1, t_max);

            if(t_min >= t_max)
                return false;
        }

        return true;
    }

    static AABB surrounding_box(AABB<T> b1, AABB<T> b2) {
        Vector3<T> small(fmin(b1.minimum.x(), b2.minimum.x()),
                         fmin(b1.minimum.y(), b2.minimum.y()),
                         fmin(b1.minimum.z(), b2.minimum.z()));
        Vector3<T> big(fmax(b1.maximum.x(), b2.maximum.x()),
                       fmax(b1.maximum.y(), b2.maximum.y()),
                       fmax(b1.maximum.z(), b2.maximum.z()));

        return AABB(small, big);
    }
};

