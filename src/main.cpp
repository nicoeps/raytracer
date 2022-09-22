#include "color.h"
#include "vector3.h"
#include "ray.h"
#include "sphere.h"
#include "scene.h"
#include "material.h"
#include "camera.h"
#include "bvh.h"
#include "texture.h"

#include <iostream>
#include <vector>
#include <memory>

#include <thread>
#include <future>

Scene scene;
Color background(0, 0, 0);
// Color background(0.70, 0.80, 1.00);

Color trace(const Ray& ray, int depth) {
    if(depth <= 0) {
        return Color(0, 0, 0);
    }

    Shape* found_shape = nullptr;
    double t = std::numeric_limits<double>::infinity();;

    if (!scene.raycast(ray, found_shape, t)) {
        return background;
    }

    Vector3 hit = ray.origin + ray.direction*t;
    Vector3 normal;
    Vector2 uv;
    Color attenuation;
    Ray scattered;
    found_shape->getSurfaceProperties(hit, normal, uv);
    Color emitted = found_shape->material->emitted(uv);


    if (found_shape->material == nullptr) {
        return 0.5*trace(Ray(hit, normal), depth-1);
    }

    if (!found_shape->material->scatter(ray, hit, normal, uv, attenuation, scattered)) {
        return emitted;
    }

    return emitted + attenuation.cwiseProduct(trace(scattered, depth-1));
    // return attenuation;
    // Vector3 target = hit + random_in_hemisphere(normal);
    // return 0.5*trace(Ray(hit, target - hit), depth-1);

        // return Color(0, 0, 0);
        // return 0.5*trace(Ray(hit, target - hit), depth-1);
        // return 0.5*Color(normal.x()+1, normal.y()+1, normal.z()+1);

    // Vector3 dir = ray.direction.normalized();
    // t = 0.5 * (dir.y() + 1.0);
    // return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

std::shared_ptr<Shape> earth() {
    auto texture = std::make_shared<ImageTexture>("images/earthmap.jpg");
    auto surface = std::make_shared<Lambertian>(texture);
    auto shape = std::make_shared<Sphere>(Vector3(0, 0, 0), 2, surface);

    return shape;
}

int main() {

    // Image
    const double aspect_ratio = 16.0 / 9.0;
    const int image_width = 720;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples = 100;
    const int max_depth = 50;

    // Final
    // const auto aspect_ratio = 3.0 / 2.0;
    // const int image_width = 400;
    // const int image_height = static_cast<int>(image_width / aspect_ratio);
    // const int samples = 100;
    // const int max_depth = 50;

    Vector3 lookfrom(13,2,3);
    Vector3 lookat(0,0,0);
    // Camera camera(lookfrom, lookat, 20, aspect_ratio);

    // scene.add(earth());

    Scene test;

    // auto ground_material = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    // test.add(std::make_shared<Sphere>(Vector3(0,-1000,0), 1000, ground_material));

    // for (int a = -11; a < 11; a++) {
    //     for (int b = -11; b < 11; b++) {
    //         auto choose_mat = random_double();
    //         Vector3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

    //         if ((center - Vector3(4, 0.2, 0)).norm() > 0.9) {
    //             std::shared_ptr<Material> sphere_material;

    //             if (choose_mat < 0.8) {
    //                 // diffuse
    //                 auto albedo = Vector3(random_double(0.0, 1.0), random_double(0.0, 1.0), random_double(0.0, 1.0)).cwiseProduct(Vector3(random_double(0.0, 1.0), random_double(0.0, 1.0), random_double(0.0, 1.0)));
    //                 sphere_material = std::make_shared<Lambertian>(albedo);
    //                 test.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
    //             } else if (choose_mat < 0.95) {
    //                 // metal
    //                 auto albedo = Vector3(random_double(0.5, 1.0), random_double(0.5, 1.0), random_double(0.5, 1.0));
    //                 auto fuzz = random_double(0, 0.5);
    //                 sphere_material = std::make_shared<Metal>(albedo, fuzz);
    //                 test.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
    //             } else {
    //                 // glass
    //                 sphere_material = std::make_shared<Dielectric>(Color(1.0, 1.0, 1.0), 1.5);
    //                 scene.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
    //             }
    //         }
    //     }
    // }

    // auto material1 = std::make_shared<Dielectric>(Color(1.0, 1.0, 1.0),1.5);
    // test.add(std::make_shared<Sphere>(Vector3(0, 1, 0), 1.0, material1));

    // auto material2 = std::make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    // test.add(std::make_shared<Sphere>(Vector3(-4, 1, 0), 1.0, material2));

    // auto material3 = std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    // test.add(std::make_shared<Sphere>(Vector3(4, 1, 0), 1.0, material3));


    // auto bvh = std::make_shared<BVH>(test);

    // scene.add(bvh);
    // scene = test;

    // Scene

    auto normal_map = std::make_shared<ImageTexture>("images/normal.png");

    auto material_ground = std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    // auto material_center = std::make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
    auto material_left   = std::make_shared<Dielectric>(Color(1.0, 1.0, 1.0), 1.5);
    auto material_right  = std::make_shared<Metal>(Color(0.8, 0.6, 0.2), 0.0);

    auto texture = std::make_shared<ImageTexture>("images/earthmap3.jpg");
    auto material_center = std::make_shared<Lambertian>(texture, normal_map);
    // auto material_center = std::make_shared<Lambertian>(Color(0.1, 0.2, 0.5), normal_map);
    // auto material_center = std::make_shared<Metal>(Color(0.1, 0.2, 0.5));

    auto material_light = std::make_shared<Emission>(Color(10, 10, 10));


    test.add(std::make_shared<Sphere>(Vector3(0, -100.5, -1), 100, material_ground));
    test.add(std::make_shared<Sphere>(Vector3(0, 0, -1), 0.5, material_center));
    // test.add(std::make_shared<Sphere>(Vector3(-1, 0, -1), 0.5, material_left));
    // test.add(std::make_shared<Sphere>(Vector3(-1, 0, -1), -0.4, material_left));
    // test.add(std::make_shared<Sphere>(Vector3(1, 0, -1), 0.5, material_right));
    test.add(std::make_shared<Sphere>(Vector3(1.5, 1, -0.8), 0.5, material_light));

    // auto bvh = std::make_shared<BVH>(test);

    // scene.add(bvh);
    scene = test;

    Camera camera(Vector3(0, 0, 0), Vector3(0, 0, -1), 126.86989764, aspect_ratio);

    // double R = cos(M_PI / 4);
    // auto material_left = std::make_shared<Lambertian>(Color(0, 0, 1));
    // auto material_right = std::make_shared<Lambertian>(Color(1, 0, 0));
    // scene.add(std::make_shared<Sphere>(Vector3(-R, 0, -1), R, material_left));
    // scene.add(std::make_shared<Sphere>(Vector3(R, 0, -1), R, material_right));

    // auto material_ground = std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    // auto material_center = std::make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
    // auto material_left   = std::make_shared<Dielectric>(Color(1.0, 1.0, 1.0), 1.5);
    // auto material_right  = std::make_shared<Metal>(Color(0.8, 0.6, 0.2), 0.0);

    // scene.add(std::make_shared<Sphere>(Vector3( 0.0, -100.5, -1.0), 100.0, material_ground));
    // scene.add(std::make_shared<Sphere>(Vector3( 0.0,    0.0, -1.0),   0.5, material_center));
    // scene.add(std::make_shared<Sphere>(Vector3(-1.0,    0.0, -1.0),   0.5, material_left));
    // scene.add(std::make_shared<Sphere>(Vector3(-1.0,    0.0, -1.0), -0.45, material_left));
    // scene.add(std::make_shared<Sphere>(Vector3( 1.0,    0.0, -1.0),   0.5, material_right));

    // Camera camera(Vector3(-2, 2, 1), Vector3(0, 0, -1), 20.0, aspect_ratio);

    // Camera
    // double viewport_height = 2.0;
    // double viewport_width = aspect_ratio * viewport_height;
    // double focal_length = 1.0;

    // Vector3 origin = Vector3(0, 0, 0);
    // Vector3 horizontal = Vector3(viewport_width, 0, 0);
    // Vector3 vertical = Vector3(0, viewport_height, 0);
    // Vector3 lower_left_corner = origin - horizontal / 2 - vertical / 2 - Vector3(0, 0, focal_length);

    // Render

    // std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    // for (int j = image_height-1; j >= 0; --j) {
    //     std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
    //     for (int i = 0; i < image_width; ++i) {
    //         Color pixel_color(0, 0, 0);
    //         for (int k = 0; k < samples; ++k) {
    //             double u = (i + random_double()) / (image_width-1);
    //             double v = (j + random_double()) / (image_height-1);
    //             // double u = double(i) / (image_width-1);
    //             // double v = double(j) / (image_height-1);
    //             Ray ray(origin, lower_left_corner + u*horizontal + v*vertical - origin);
    //             pixel_color += trace(ray, max_depth);
    //         }
    //         write_color(std::cout, pixel_color, samples);
    //     }
    // }

    std::size_t max = image_height;
    std::size_t cores = std::thread::hardware_concurrency();
    std::vector<std::future<void>> future_vector;
    // int framebuffer[image_width * image_height * 3 * sizeof(int)] = {};
    int* framebuffer = (int*) malloc(image_width * image_height * 3 * sizeof(int));
    for (std::size_t x(0); x < cores; ++x) {
        future_vector.emplace_back(std::async([=, &camera]() {
                for (std::size_t index(x); index < image_height; index += cores) {
                    if (index < image_height) {
                        int j = image_height - index;
                        for (int i = 0; i < image_width; ++i) {
                            Color pixel_color(0, 0, 0);
                            for (int k = 0; k < samples; ++k) {
                                double u = (i + random_double()) / (image_width-1);
                                double v = (j + random_double()) / (image_height-1);
                                // double u = double(i) / (image_width-1);
                                // double v = double(j) / (image_height-1);
                                Ray ray = camera.get_ray(u, v);
                                pixel_color += trace(ray, max_depth);
                            }
                            write_color2(framebuffer, pixel_color, samples, index * image_width + i);
                        }
                        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
                    }
                }
            }
        ));
    }

    for (auto& future : future_vector) {
        future.wait();
    }

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for (int i = 0; i < image_width*image_height; ++i) {
        std::cout << framebuffer[i*3] << ' ' << framebuffer[i*3 + 1] << ' ' << framebuffer[i*3 + 2] << '\n';
    }

    std::cerr << "\nDone.\n";
    free(framebuffer);
}
