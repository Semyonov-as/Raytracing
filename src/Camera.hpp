#pragma once

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

    Camera() {
        T aspect_ratio = 16.0/9.0;
        T viewport_height = 2.0;
        T viewport_width = viewport_height*aspect_ratio;
        T focal_length = 1.0;

        origin = Vector3<T>(0, 0, 0);
        horizontal = Vector3<T>(viewport_width, 0, 0);
        vertical = Vector3<T>(0, viewport_height, 0);
        lower_left_corner = origin - horizontal/2 - vertical/2 - Vector3<T>(0, 0, focal_length);
    }

    Ray<T> get_ray(T u, T v) const noexcept { return Ray<T>(origin, lower_left_corner - origin + u*horizontal + v*vertical);}
};
