#pragma once

#include "utility.hpp"

#include "color.hpp"
#include "hittable.hpp"
#include "material.hpp"

class camera{
public:
    double aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    int samples_per_pixel = 10;
    int max_depth = 10;

    double fov = 90;
    vec3 lookfrom = vec3(0,0,0);
    vec3 lookat = vec3(0,0,-1);
    vec3 vup = vec3(0,1,0);

    double defocus_angle = 0;
    double focus_dist = 10;


    //Main Render Function
    void render(hittable_list world){
        std::cout << "P3\n" << image_width << ' ' << image_height << "\n256\n";

        for(int i = 0; i< image_height; i++){
            std::clog << "\rScanlines remaining: " << (image_height - i) << ' ' << std::flush;
            for(int j = 0; j < image_width; j++){
                vec3 pixel_color(0,0,0);
                for(int sample = 0; sample < samples_per_pixel; sample++){
                    ray r = get_ray(i,j);
                    pixel_color += ray_color(r, max_depth, world);
                }
                write_color(std::cout, pixel_color, samples_per_pixel);
            }
        }
        std::clog << "\rDone                    \n";

    }

    //Call Before Calling render()
    void initialize() {
        image_height = static_cast<int>(image_width/aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        center = lookfrom;


        //Camera & Viewport
        auto focal_length = (lookfrom - lookat).length();
        auto theta = degrees_to_radians(fov);
        auto h = tan(theta/2);
        auto viewport_height = 2*h*focus_dist;
        auto viewport_width = viewport_height * (static_cast<double>(image_width)/image_height);

        //Basis unit vectors
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup,w));
        v = cross(w,u);

        //Viewport vectors
        auto viewport_horizontal = viewport_width * u;
        auto viewport_vertical = viewport_height * -v;


        //Pixel Delta Vectors
        pixel_delta_horizontal = viewport_horizontal / image_width;
        pixel_delta_vertical = viewport_vertical / image_height;

        //Upper Left pixel
        auto viewport_upper_left = center - (focus_dist * w) - viewport_horizontal/2 - viewport_vertical/2;
        pixel_dst = viewport_upper_left + 0.5 * (pixel_delta_horizontal + pixel_delta_vertical);

        //Camera focus basis vectors
        auto defocus_radius = focus_dist * tan(degrees_to_radians(defocus_angle/2));
        defocus_disk_horizontal = u * defocus_radius;
        defocus_disk_vertical = v * defocus_radius;
    }

private:
    int image_height;
    vec3 center;
    vec3 pixel_dst;
    vec3 pixel_delta_horizontal;
    vec3 pixel_delta_vertical;
    interval width_interval;
    vec3 u;
    vec3 v;
    vec3 w;
    vec3 defocus_disk_horizontal;
    vec3 defocus_disk_vertical;

    vec3 ray_color(const ray& r, int depth, const hittable& world) {
        if(depth <= 0){
            return vec3(0,0,0);
        }
        hit_record rec;
        if(world.hit(r,interval(0.001, infinity), rec)) {
            ray scattered;
            vec3 attenuation;
            if(rec.mat->scatter(r, rec, attenuation, scattered)){
                return attenuation * ray_color(scattered, depth-1, world);
            }
            return vec3(0,0,0);
        }

        vec3 unit_direction = unit_vector(r.direction);
        double a = 0.5*(unit_direction.y + 1.0);
        return (1.0-a)*vec3(1.0,1.0,1.0) + a*vec3(0.5,0.7,1.0);
    }

    ray get_ray(int i, int j) const {
        auto pixel_center = pixel_dst + (j * pixel_delta_horizontal) + (i * pixel_delta_vertical);
        auto pixel_sample = pixel_center + pixel_sample_square();

        auto ray_origin = defocus_angle <=0 ? center : defocus_disk_sample();
        auto ray_direction = pixel_sample - ray_origin;
        auto ray_time = random_double();

        return ray(ray_origin, ray_direction, ray_time);
    }

    vec3 defocus_disk_sample() const {
        auto p = random_in_unit_disk();
        return center+(p.x * defocus_disk_horizontal) + (p.y * defocus_disk_vertical);
    }

    vec3 pixel_sample_square() const {
        double px = -0.5 + random_double();
        double py = -0.5 + random_double();

        return (px * pixel_delta_horizontal) + (py * pixel_delta_vertical);
    }
};
