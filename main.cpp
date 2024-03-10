#include "utility.hpp"

#include "vec3.hpp"
#include "color.hpp"
#include "ray.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "camera.hpp"
#include "material.hpp"
#include "bvh.hpp"
#include "texture.hpp"

#include <iostream>

void scene1(){
    hittable_list world;

    auto checker = make_shared<checker_texture>(0.32, vec3(0.3,0.2,0.1), vec3(0.9,0.9,0.9));

    auto material_ground = make_shared<lambertian>(checker);
    auto material_center = make_shared<lambertian>(vec3(0.1,0.2,0.5));
    auto material_left = make_shared<dielectric>(1.5);
    auto material_right = make_shared<metal>(vec3(0.8,0.6,0.2), 0.1);

    world.add(make_shared<sphere>(vec3(0,-100.5,-1), 100, material_ground));
    world.add(make_shared<sphere>(vec3(0,0.5,-1), vec3(0,0,-1), 0.5, material_center));
    world.add(make_shared<sphere>(vec3(-1.0,0,-1), 0.5, material_left));
    world.add(make_shared<sphere>(vec3(-1.0,    0.0, -1.0),  -0.4, material_left));
    world.add(make_shared<sphere>(vec3(1.0,0,-1.0), 0.5, material_right));


    world = hittable_list(make_shared<bvh_node>(world));

    camera cam;


    cam.aspect_ratio = 16.0/9.0;
    cam.image_width = 400;

    cam.fov = 50;
    cam.lookfrom = vec3(-2,2,1);
    cam.lookat = vec3(0,0,-1);
    cam.vup = vec3(0,1,0);

    cam.defocus_angle = 0;
    cam.focus_dist = 100.0;

    cam.initialize();
    cam.render(world);
}

void scene2(){
    hittable_list world;

    auto checker = make_shared<checker_texture>(0.32, vec3(0.3,0.2,0.1), vec3(0.9,0.9,0.9));

    world.add(make_shared<sphere>(vec3(0,-10, 0), 10, make_shared<lambertian>(checker)));
    world.add(make_shared<sphere>(vec3(0, 10, 0), 10, make_shared<lambertian>(checker)));

    world = hittable_list(make_shared<bvh_node>(world));

    camera cam;


    cam.aspect_ratio = 16.0/9.0;
    cam.image_width = 400;
    cam.max_depth = 50;

    cam.fov = 20;
    cam.lookfrom = vec3(13,2,3);
    cam.lookat = vec3(0,0,0);
    cam.vup = vec3(0,1,0);


    cam.defocus_angle = 0;
    cam.focus_dist = 100.0;

    cam.initialize();
    cam.render(world);
}

int main(){
    scene2();
    return 0;
}
