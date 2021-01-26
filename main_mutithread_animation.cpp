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

#include <iostream>
#include <cmath>
#include <memory>
#include <thread>
#include <chrono>
#include <atomic>
#include <sstream>


//Atomic counter
std::atomic<int> counter{ 0 };

HittableList<double> random_scene() {
    HittableList<double> world;

    auto ground_mat = std::make_shared<Lambertian<double>>(ColorD(0.3, 0.5, 0.2));
    world.add(std::make_shared<Sphere<double>>(Point3D(0, -1000, 0), 1000, ground_mat));

    for (int a = -5; a < 5; a++) {
        for (int b = -5; b < 5; b++) {
            double choose = random<double>(0, 1);
            Point3D center(a + 0.8*random<double>(0, 1), 0.2, b + 0.9*random<double>(0, 1));

            if ((center - Point3D(4, 0.2, 0)).length() > 1.0 && (center - Point3D(0, 0.2, 0)).length() > 1.0 && (center - Point3D(-4, 0.2, 0)).length() > 1.0) {
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

ColorD ray_color(const Ray<double>& r, const HittableList<double>& world, int depth) {
    if (depth < 1)
        return ColorD(0, 0, 0);
    HitRecord<double> rec;
    if (world.hit(r, 0.0001, infinity, rec)) {
        Ray<double> scattered;
        ColorD attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation*ray_color(scattered, world, depth-1);
        return ColorD(0, 0, 0);
    }
    auto t = 0.5*(r.direction().unit().y()+1.0);
    return (1.0 - t)*ColorD(1.0, 1.0, 1.0)+t*ColorD(0.5, 0.7, 1);

}

//Threading
constexpr int MAX_THREADS = 4;

void COMPUTE(PPM_IMAGE& image, int begin, int end, int spp, int depth, Camera<double>& cam, HittableList<double>& world) {
    for (int j = end-1; j >= begin; --j) {
        for (int i = 0; i < image.width; ++i) {
            ColorD pixel_color;
            for (int s = 0; s < spp; ++s) {
                double u = (i + random<double>(-1, 1))/(image.width - 1);
                double v = (j + random<double>(-1, 1))/(image.height-1);
                Ray<double> r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, depth);
            }
            write_color(image, j, i, pixel_color, spp);
            counter.fetch_add(1, std::memory_order_relaxed);;
        }
    }

}


int main() {   
    //Image settingis
    const double aspect_ratio = 3.0 / 2.0;
    const double vfov = 25.0; //vertical field of view in degrees
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width/aspect_ratio);
    const int samples_per_pixel = 10;
    const int max_depth = 50;

    //World setup
    HittableList<double> world = random_scene();

    int T_MAX = 69;
    for(int t = 0; t <= T_MAX; t++){
        auto start = std::chrono::high_resolution_clock::now();
        //Camera settingis
        Point3D lookfrom(12-t/3.0, 4 - t/25.0, (t-20)/5.0);
        Point3D lookat(0, 1, 0);
        const Vector3<double> vup(0, 1, 0);
        double dist_to_focus = 9.0;//std::abs(12-t)*9/12 + 1;
        double aperture = 0.1;//*(std::abs(T_MAX-2*t)/T_MAX + 1);

        Camera<double> cam(lookfrom,  lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus);

        //Render Animation

        PPM_IMAGE image(image_width, image_height);

        std::vector<std::thread> threads;
        int part = static_cast<int>(image_height/MAX_THREADS);

        std::cerr << "Setting threads\n" << std::flush;
        for(int i = 0; i < MAX_THREADS - 1; i++)
            threads.emplace_back(std::thread(COMPUTE, std::ref(image), i*part, (i+1)*part,
                                             samples_per_pixel, max_depth, std::ref(cam),
                                             std::ref(world)));
        threads.emplace_back(std::thread(COMPUTE, std::ref(image), (MAX_THREADS-1)*part, image_height,
                                         samples_per_pixel, max_depth, std::ref(cam),
                                         std::ref(world)));

        int total_pixels = image_height*image_width;
        while(counter.load() < total_pixels - 1){
            std::cerr << "\rComputing frame " << t+1 <<  ": " << static_cast<int>(counter.load()*100/total_pixels) << '%' << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        std::cerr << "\rComputing frame " << t+1 <<  ": " << static_cast<int>(counter.load()*100/total_pixels) << '%' << std::flush;
        for(auto& th : threads)
            th.join();


        std::cerr << "\nWriting in file.\n" << std::flush;

        //Printing in png file
        std::stringstream path;
        path << "png/frame" << t << ".png";
        image.print_to_png(path.str());
        counter.fetch_sub(counter.load());

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end-start;
        std::cerr << "It took " << elapsed.count()/1000 << "s\n";
        std::cerr << "Remaining: " << elapsed.count()*(T_MAX - t)/1000 << "s\n";
        std::cerr << "---------------------------\n" << std::flush;
    }

    return 0;
}
