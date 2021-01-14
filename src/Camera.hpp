#pragma once

#include <cmath>

#include "General.hpp"
#include "Vector3.hpp"
#include "Ray.hpp"


template<typename T>
class Camera {
public:
    Vector3<T> origin;
    Vector3<T> horizontal;
    Vector3<T> vertical;
    Vector3<T> lower_left_corner;
    Vector3<T> u, v, w;
    T lens_radius;

    Camera(Vector3<T> lookfrom, Vector3<T> lookat, Vector3<T> vup, T vfov, T aspect_ratio, T aperture, T focus_dist) {
        T viewport_height = 2.0*tan(degrees_to_radians(vfov)/2);
        T viewport_width = viewport_height*aspect_ratio;

        w = (lookfrom - lookat).unit();
        u = cross(vup, w).unit();
        v = cross(w, u);

        origin = lookfrom;
        horizontal = focus_dist*viewport_width*u;
        vertical = focus_dist*viewport_height*v;
        lower_left_corner = origin - horizontal/2 - vertical/2 - focus_dist*w;

        lens_radius = aperture/2;
    }

    Ray<T> get_ray(T s, T t) const noexcept {
        Vector3<T> rd = lens_radius*Vector3<T>::randon_unit_vector_xy();
        Vector3<T> offset = u*rd.x() + v*rd.y();
        return Ray<T>(origin + offset, lower_left_corner - origin - offset + s*horizontal + t*vertical);
    }
};
