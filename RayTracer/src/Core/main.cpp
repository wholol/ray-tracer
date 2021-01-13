#include <iostream>
#include <fstream>
#include <string>
#include "Vector3.h"
#include "WriteColor.h"
#include "Ray.h"
#include "Scene.h"
#include "Materials.h"
#include "Hittables.h"
#include "RNG.h"
#include "Camera.h"
#include <future>
#include "Timer.h"

#define PARALLEL 1

ColorVec RayTrace(const Ray& r, Scene& scene, int max_depth)
{
	hit_point hitpoint;	//obtains paarameters at interection point.

	if (max_depth <= 0) {
		return ColorVec(0, 0, 0);
	}

	if (scene.Intersect(r, 0.0001, INFINITY, hitpoint))	//if the ray hits any object in scene
	{
		Ray newray;		//a new ray to be generated based on the material intersection
		ColorVec atten;		//depedant of material struck by ray.
		if (hitpoint.mat_ptr->scatter(r, hitpoint, atten, newray))
		{
			return RayTrace(newray, scene, max_depth - 1) * atten;	//return light color * object color
		}
		return ColorVec(0, 0, 0);
	}

	else//if no intersection
	{
		Vector3d n = r.GetDir().getNormalized();
		double t = (n.y * 0.5) + 1.0;
		ColorVec col = ColorVec(0.8, 1.0, 1.0) * (1.0 - t) + ColorVec(0.5, 0.7, 1.0) * t;	//background is the light source.
		return col;
	}
}

void ColorPixels(int xStart, int xEnd, int yStart, int yEnd, const int image_width, const int image_height, const int samples, const int max_depth, const int max_rgb, Scene& world, Camera& cam, std::vector<ColorVec>& pixels)
{
	for (int y = yStart; y < yEnd; y++)		//row
	{
		for (int x = xStart; x < xEnd; x++)		//col
		{
			ColorVec pixel(0.0, 0.0, 0.0);
			for (int k = 0; k < samples; ++k) {
				double u = (double)(y + RNG::rng()) / (double)(image_height - 1);		//image height ratio
				double v = (double)(x + RNG::rng()) / (double)(image_width - 1);		//image width ratio (for unit vector scale)
				pixel += RayTrace(cam.GetRayDir(u, v), world, max_depth);
			}
			double inv_samples = 1.0 / (double)samples;
			pixels[x + image_width*y].x = std::sqrt(std::min(pixel.x * inv_samples, 0.99)) * (double)max_rgb;
			pixels[x + image_width*y].y = std::sqrt(std::min(pixel.y * inv_samples, 0.99)) * (double)max_rgb;
			pixels[x + image_width*y].z = std::sqrt(std::min(pixel.z * inv_samples, 0.99)) * (double)max_rgb;

		}
	}
}


int main()
{
	/*image dimension setup*/
	const double tMin = 0.0, tMax = INFINITY;
	const double AspectRatio = 1.0 / 1.0;
	const int image_width = 1200;
	const int image_height = (int)((double)image_width / AspectRatio);
	const int rgb_max = 256;

	/*viewport*/
	Camera cam(Vector3d(13, 2, 3), Vector3d(0.0, 0.0, 0.0), Vector3d(0.0, 1.0, 0.0), AspectRatio, 20.0, 0.1);
	auto R = cos(3.14159 / 4);

	//material setup
	auto material_ground = std::make_shared<Lambertian>(ColorVec(0.8, 0.8, 0.0));
	auto material_center = std::make_shared<Lambertian>(ColorVec(1.0, 0.6, 0.3));
	auto material_left = std::make_shared<Dielectric>(1.5);
	auto material_right = std::make_shared<Metal>(ColorVec(0.8, 0.6, 0.2), 0.96);

	//scene objects setup
	Scene scene;
	scene.addObject(std::make_unique<Sphere>(Vector3d(0, 0, -1.0), 0.5, material_center));
	scene.addObject(std::make_unique<Sphere>(Vector3d(1.0, 0.0, -2.0), 0.5, material_right));
	scene.addObject(std::make_unique<Sphere>(Vector3d(0, -100.5, -1.0), 100, material_ground));
	scene.addObject(std::make_unique<Sphere>(Vector3d(-12.0, 0.0, -1.0), -0.45, material_left));

	//anti aliasing samples
	const int samples = 50;
	const int max_depth = 50;

	//image setup
	std::ofstream fout("RayTracer.ppm");
	fout << "P3" << std::endl;
	fout << image_width << " " << image_height << std::endl;
	fout << std::to_string(rgb_max) << std::endl;

	//initailize pixel vector
	std::vector<ColorVec> pixels;
	pixels.reserve(image_height * image_width);
	for (int i = image_height * image_width - 1; i >= 0; --i)
	{
		pixels.emplace_back();
	}

	//parallel impl
#if PARALLEL	
	int num_chunks = 600;
	int section_height = image_height / num_chunks;
	std::vector<std::future<void>> fus;
	fus.reserve(num_chunks);
	int chunksize = 30;
	Timer t;
	t.start_timer();
	
	//for (int yStart = 0; yStart < image_height; yStart += chunksize)
	//{
	//	for (int xStart = 0; xStart < image_width; xStart += chunksize)
	//	{
	//		int xEnd = (xStart + chunksize);
	//		int yEnd = (yStart + chunksize);
	//		fus.push_back(std::async(std::launch::async, ColorPixels, xStart, xEnd, yStart, yEnd, image_width, image_height, samples, max_depth, rgb_max, std::ref(world), std::ref(cam), std::ref(pixels)));
	//	}
	//}
	
	for (int i = 0; i < num_chunks; ++i)
	{
		int xStart = 0;
		const int& xEnd = image_width;
		int yStart = (i * section_height);
		int yEnd = ((i + 1) * section_height);
		fus.push_back( std::async(std::launch::async, ColorPixels, xStart, xEnd, yStart, yEnd , image_width,image_height,samples,max_depth,rgb_max,std::ref(scene),std::ref(cam),std::ref(pixels) ));
	}

	for (auto& f : fus)
	{
		f.wait();
	}
	t.end_timer();
	t.print_dt();

#else
	Timer serial;
	serial.start_timer();
	ColorPicture(0, image_width, 0, image_height, image_width, image_height, samples, max_depth, rgb_max, std::ref(world), std::ref(cam), std::ref(pixels));
	serial.end_timer();
	serial.print_dt();
#endif

	//pass to io
	for (int y = 0; y < image_height; ++y) {
		for (int x = 0; x < image_width; ++x) {
			ColorToFile(fout, rgb_max, samples,x, y, image_width,pixels);
		}
	}
}