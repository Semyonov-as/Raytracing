#include "src/Point3.hpp"
#include "src/Color.hpp"
#include "src/Ray.hpp"
#include "src/Vector3DF.hpp"
#include "src/HittableObject.hpp"
#include "src/HittableList.hpp"
#include "src/Sphere.hpp"
#include "src/General.hpp"
#include "src/Camera.hpp"
#include "src/Materials.hpp"
#include "src/MovingSphere.hpp"
#include "src/Textures.hpp"
#include "src/AARect.hpp"
#include "src/Box.hpp"
#include "src/Medium.hpp"
#include "src/BVH.hpp"

#include <iostream>
#include <cmath>
#include <memory>
#include <thread>
#include <chrono>
#include <atomic>


//Atomic counter
std::atomic<int> counter{ 0 };

HittableList<double> random_scene() {
    HittableList<double> world;

    auto ground_mat = std::make_shared<Lambertian<double>>(ColorD(0.3, 0.5, 0.2));
    world.add(std::make_shared<Sphere<double>>(Point3D(0, -1000, 0), 1000, ground_mat));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            double choose = random<double>(0, 1);
            Point3D center(a + 0.8*random<double>(0, 1), 0.2, b + 0.9*random<double>(0, 1));

            if ((center - Point3D(4, 0.2, 0)).length() > 1.0) {
                std::shared_ptr<Material<double>> sphere_material;
                Vector3<double> rnd = Vector3<double>(random<double>(0, 1), random<double>(0, 1), random<double>(0, 1));
                if (choose < 0.7) {
                    ColorD albedo = rnd*rnd;
                    sphere_material = std::make_shared<Lambertian<double>>(albedo);
                    world.add(std::make_shared<Sphere<double>>(center, 0.2, sphere_material));
                } else if ( choose < 0.95) {
                    ColorD albedo = rnd;
                    double fuzz = random<double>(0, 0.5);
                    sphere_material = std::make_shared<Metal<double>>(albedo, fuzz);
                    world.add(std::make_shared<Sphere<double>>(center, 0.18, sphere_material));
                } else {
                    sphere_material = std::make_shared<Dielectric<double>>(1.5);
                    world.add(std::make_shared<Sphere<double>>(center, 0.15, sphere_material));
                }
            }
        }
    }

    world.add(std::make_shared<Sphere<double>>(Point3D(0, 1, 0), 1.0, std::make_shared<Dielectric<double>>(1.5)));
    world.add(std::make_shared<Sphere<double>>(Point3D(-4, 1, 0), 1.0, std::make_shared<Lambertian<double>>(ColorD(0.4, 0.2, 0.1))));
    world.add(std::make_shared<Sphere<double>>(Point3D(4, 1, 0), 1.0, std::make_shared<Metal<double>>(ColorD(0.7, 0.6, 0.5), 0)));

    return world;
}

HittableList<double> box_scene() {
    HittableList<double> boxes1;
    auto ground = std::make_shared<Lambertian<double>>(ColorD(0.48, 0.83, 0.53));

    const int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            auto w = 200.0;
            auto x0 = -1000.0 + i*w;
            auto z0 = -1000.0 + j*w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = 101*fast_random<double>();
            auto z1 = z0 + w;

            boxes1.add(std::make_shared<Box<double>>(Point3D(x0,y0,z0), Point3D(x1,y1,z1), ground));
        }
    }

    HittableList<double> objects;

    objects.add(std::make_shared<BvhNode<double>>(boxes1, 0, 1));

    auto light = std::make_shared<DiffuseLight<double>>(ColorD(15, 15, 10));
    objects.add(std::make_shared<XZRect<double>>(123, 423, 147, 412, 554, light));

    auto center1 = Point3D(400, 400, 200);
    auto center2 = center1 + Vector3D(30,0,0);
    auto moving_sphere_material = std::make_shared<Lambertian<double>>(ColorD(0.7, 0.3, 0.1));
    objects.add(std::make_shared<MovingSphere<double>>(center1, center2, 0, 1, 50, moving_sphere_material));

    objects.add(std::make_shared<Sphere<double>>(Point3D(260, 150, 45), 50, std::make_shared<Dielectric<double>>(1.5)));
    objects.add(std::make_shared<Sphere<double>>(Point3D(0, 150, 145), 50, std::make_shared<Metal<double>>(ColorD(0.8, 0.8, 0.9), 1.0)));

    auto boundary = std::make_shared<Sphere<double>>(Point3D(360,150,145), 70, std::make_shared<Dielectric<double>>(1.5));
    objects.add(boundary);
    objects.add(std::make_shared<ConstantMedium<double>>(boundary, 0.2, ColorD(0.2, 0.4, 0.9)));
    boundary = std::make_shared<Sphere<double>>(Point3D(0, 0, 0), 5000, std::make_shared<Dielectric<double>>(1.5));
    objects.add(std::make_shared<ConstantMedium<double>>(boundary, .0001, ColorD(1,1,1)));

    auto emat = std::make_shared<Lambertian<double>>(std::make_shared<ImageTexture<double>>("earthmap.jpg"));
    objects.add(std::make_shared<Sphere<double>>(Point3D(400,200,400), 100, emat));
    auto pertext = std::make_shared<NoiseTexture<double>>(0.1);
    objects.add(std::make_shared<Sphere<double>>(Point3D(220,280,300), 80, std::make_shared<Lambertian<double>>(pertext)));

    HittableList<double> boxes2;
    auto white = std::make_shared<Lambertian<double>>(ColorD(.73, .73, .73));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxes2.add(std::make_shared<Sphere<double>>(Vector3<double>(165*fast_random<double>(), 165*fast_random<double>(), 165*fast_random<double>()), 10, white));
    }

    objects.add(std::make_shared<Translate<double>>(std::make_shared<RotateY<double>>(
            std::make_shared<BvhNode<double>>(boxes2, 0.0, 1.0), 15),
            Vector3D(100,220,395)));

    return objects;
}

ColorD ray_color(const Ray<double>& r, const ColorD& background, const HittableList<double>& world, int depth) {
    if (depth < 1)
        return ColorD(0, 0, 0);
    HitRecord<double> rec;
    if (!world.hit(r, 0.0001, infinity, rec))
        return background;

    Ray<double> scattered;
    ColorD attenuation;
    ColorD emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

    if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        return emitted;

    return emitted + attenuation*ray_color(scattered, background, world, depth-1);
}

//Threading
constexpr int MAX_THREADS = 4;

void COMPUTE(PPM_IMAGE& image, int begin, int end, int spp, int depth, Camera<double>& cam, HittableList<double>& world, const ColorD& background) {
    for (int j = end-1; j >= begin; --j) {
        for (int i = 0; i < image.width; ++i) {
            ColorD pixel_color;
            for (int s = 0; s < spp; ++s) {
                double u = (i + random<double>(-1, 1))/(image.width - 1);
                double v = (j + random<double>(-1, 1))/(image.height-1);
                Ray<double> r = cam.get_ray(u, v);
                pixel_color += ray_color(r, background, world, depth);
            }
            write_color(image, j, i, pixel_color, spp);
            counter.fetch_add(1, std::memory_order_relaxed);
        }
    }

}


int main() {   
    //Image settingis
    const double aspect_ratio = 1.0;
    const double vfov = 40.0; //vertical field of view in degrees
    const int image_width = 800;
    const int image_height = static_cast<int>(image_width/aspect_ratio);
    const int samples_per_pixel = 1000;
    const int max_depth = 50;

    //World setup
    HittableList<double> world = box_scene();

//    auto red   = std::make_shared<Lambertian<double>>(ColorD(.65, .05, .05));
//    auto white = std::make_shared<Lambertian<double>>(ColorD(.73, .73, .73));
//    auto green = std::make_shared<Lambertian<double>>(ColorD(.12, .45, .15));
//    auto light = std::make_shared<DiffuseLight<double>>(ColorD(7, 7, 7));

//    world.add(std::make_shared<YZRect<double>>(0, 555, 0, 555, 555, green));
//    world.add(std::make_shared<YZRect<double>>(0, 555, 0, 555, 0, red));
//    world.add(std::make_shared<XZRect<double>>(113, 443, 127, 432, 554, light));
//    world.add(std::make_shared<XZRect<double>>(0, 555, 0, 555, 0, white));
//    world.add(std::make_shared<XZRect<double>>(0, 555, 0, 555, 555, white));
//    world.add(std::make_shared<XYRect<double>>(0, 555, 0, 555, 555, white));

//    std::shared_ptr<HittableObject<double>> box1 = std::make_shared<Box<double>>(Point3D(0, 0, 0), Point3D(165, 330, 165), white);
//    box1 = std::make_shared<RotateY<double>>(box1, 15);
//    box1 = std::make_shared<Translate<double>>(box1, Vector3D(265, 0, 295));
//    world.add(std::make_shared<ConstantMedium<double>>(box1, 0.01, Vector3D(0, 0, 0)));

//    std::shared_ptr<HittableObject<double>> box2 = std::make_shared<Box<double>>(Point3D(0, 0, 0), Point3D(165, 165, 165), white);
//    box2 = std::make_shared<RotateY<double>>(box2, -18);
//    box2 = std::make_shared<Translate<double>>(box2, Vector3D(130, 0, 65));
//    world.add(std::make_shared<ConstantMedium<double>>(box2, 0.01, Vector3D(1, 1, 1)));

    //Camera settingis
    Point3D lookfrom(478, 278, -600);
    Point3D lookat(278, 278, 0);
    const Vector3<double> vup(0, 1, 0);
    double dist_to_focus = 10.0;
    double aperture = 0; //defocus blur is off
    ColorD background(0.0, 0.0, 0.0);

    Camera<double> cam(lookfrom,  lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

    //Render Image
    PPM_IMAGE image(image_width, image_height);

    std::vector<std::thread> threads;
    int part = static_cast<int>(image_height/MAX_THREADS);
    std::cerr << "Setting threads.\n" << std::flush;
    for(int i = 0; i < MAX_THREADS - 1; i++)
        threads.emplace_back(std::thread(COMPUTE, std::ref(image), i*part, (i+1)*part,
                                         samples_per_pixel, max_depth, std::ref(cam),
                                         std::ref(world), std::ref(background)));
    threads.emplace_back(std::thread(COMPUTE, std::ref(image), (MAX_THREADS-1)*part, image_height,
                                     samples_per_pixel, max_depth, std::ref(cam),
                                     std::ref(world), std::ref(background)));

    int total_pixels = image_height*image_width;
    while(counter.load() < total_pixels - 1){
        std::cerr << "\rComputing image: " << static_cast<int>(counter.load()*100/total_pixels) << '%' << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    for(auto& t : threads)
        t.join();
    std::cerr << "\rComputing image: " << static_cast<int>(counter.load()*100/total_pixels) << '%' << std::flush;


    std::cerr << "\nWriting in file.\n" << std::flush;

    //Printing in png file
    std::string path = "png/1.png";
    image.print_to_png(path);

    return 0;
}
