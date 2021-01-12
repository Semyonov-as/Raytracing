#include <iostream>
#include <cmath>
#include <memory>

#include "src/Point3.hpp"
#include "src/Color.hpp"
#include "src/Ray.hpp"
#include "src/Vector3DF.hpp"
#include "src/HittableObject.hpp"
#include "src/HittableList.hpp"
#include "src/Sphere.hpp"
#include "src/General.hpp"
#include "src/Camera.hpp"


//ColorF ray_color(const Ray<float>& r, const HittableList<float>& world, int depth) {
//    if(depth < 1)
//        return ColorF(0, 0, 0);
//    HitRecord<float> rec;
//    if(world.hit(r, 0, infinity, rec))
//        return 0.5*ray_color(Ray<float>(rec.p, rec.normal + Vector3<float>::random_unit_vector()), world, depth - 1);
//    auto t = 0.5*(r.direction().unit().y()+1.0);
//    return (1.0 - t)*ColorF(1.0, 1.0, 1.0)+t*ColorF(0.5, 0.7, 1);

//}

//int main() {
//    //Image settingis
//    const float aspect_ratio = 16.0 / 9.0;
//    const int image_width = 400;
//    const int image_height = static_cast<int>(image_width/aspect_ratio);
//    const int samples_per_pixel = 100;
//    const int max_depth = 50;

//    //World setup
//    HittableList<float> world;
//    world.add(std::make_shared<Sphere<float>>(Point3F(0, 0, -1), 0.5));
//    world.add(std::make_shared<Sphere<float>>(Point3F(0, -100.5, -1), 100));

//    //Camera settingis
//    Camera<float> cam;

//    //Render Image
//    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    
//    for (int j = image_height-1; j >= 0; --j) {
//        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
//        for (int i = 0; i < image_width; ++i) {
//            ColorF pixel_color;
//            for(int s = 0; s < samples_per_pixel; ++s) {
//                float u = (i + random<float>(-1, 1))/(image_width-1);
//                float v = (j + random<float>(-1, 1))/(image_height-1);
//                Ray<float> r = cam.get_ray(u, v);
//                pixel_color += ray_color(r, world, max_depth);
//            }
//            write_color(std::cout, pixel_color, samples_per_pixel);
//        }
//    }

//    std::cerr << "\nDone.\n";

//    return 0;
//}

ColorD ray_color(const Ray<double>& r, const HittableList<double>& world, int depth) {
    if(depth < 1)
        return ColorD(0, 0, 0);
    HitRecord<double> rec;
    if(world.hit(r, 0.0001, infinity, rec))
        return 0.5*ray_color(Ray<double>(rec.p, rec.normal + Vector3<double>::random_unit_vector()), world, depth - 1);
    auto t = 0.5*(r.direction().unit().y()+1.0);
    return (1.0 - t)*ColorD(1.0, 1.0, 1.0)+t*ColorD(0.5, 0.7, 1);

}

int main() {
    //Image settingis
    const double aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width/aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 50;

    //World setup
    HittableList<double> world;
    world.add(std::make_shared<Sphere<double>>(Point3D(0, 0, -1), 0.5));
    world.add(std::make_shared<Sphere<double>>(Point3D(0, -100.5, -1), 100));

    //Camera settingis
    Camera<double> cam;

    //Render Image
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            ColorD pixel_color;
            for(int s = 0; s < samples_per_pixel; ++s) {
                double u = (i + random<double>(-1, 1))/(image_width-1);
                double v = (j + random<double>(-1, 1))/(image_height-1);
                Ray<double> r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";

    return 0;
}
