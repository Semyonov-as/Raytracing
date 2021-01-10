#include <iostream>
#include <cmath>

#include "src/Point3.hpp"
#include "src/Color.hpp"
#include "src/Ray.hpp"
#include "src/Vector3DF.hpp"
#include "src/HittableObject.hpp"
#include "src/HittableList.hpp"
#include "src/Sphere.hpp"

float hit_sphere(const Point3F& center, float radius, const Ray<float>& r) {
    auto oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(r.direction(), oc);
    auto c = oc.length_squared() - radius*radius;
    auto D = half_b*half_b - a*c;

    if (D < 0)
        return -1.0;
    else
        return (-half_b - sqrt(D))/a;
}

ColorF ray_color(const Ray<float>& r) {
    auto t = hit_sphere(Point3F(0, 0, -1), 0.5, r);
    if (t > 0.0) {
        Vector3F N = (r.at(t) - Vector3F(0, 0, -1)).unit();
        return 0.5*ColorF(N.x()+1, N.y()+1, N.z()+1 );
    } else {
        t = 0.5*(r.direction().unit().y()+1.0);
        return (1.0 - t)*ColorF(1.0, 1.0, 1.0)+t*ColorF(0.5, 0.7, 1);
    }
}

int main() {
    //Image settingis
    const float aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width/aspect_ratio);

    //Camera settingis
    float viewport_height = 2.0;
    float viewport_width = viewport_height*aspect_ratio;
    float focal_length = 1.0;
    
    Point3F origin(0, 0, 0);
    Vector3F horizontal(viewport_width, 0, 0);
    Vector3F vertical(0, viewport_height, 0);
    Point3F lower_left_corner = origin - horizontal/2 - vertical/2 - Vector3F(0, 0, focal_length);

    //Render Image
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    
    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            float u = float(i)/(image_width-1);
            float v = float(j)/(image_height-1);
            Ray<float> r(origin, lower_left_corner - origin + u*horizontal + v*vertical);
            write_color(std::cout, ray_color(r));  
        }
    }

    std::cerr << "\nDone.\n";

    return 0;
}
