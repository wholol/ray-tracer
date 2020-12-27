#include <iostream>
#include <fstream>
#include <string>
#include "Vector3.h"
#include "WriteColor.h"
#include "Ray.h"
#include "WorldObjects.h"
#include "Materials.h"
#include "Geometries.h"
#include "Sphere.h"
#include "RNG.h"
#include "Camera.h"

ColorVec Color(const Ray& r, WorldObjects& world, int depth)
{
	hit_record rec;	//obtains paarameters at interection point.

	if (depth <= 0) {
		return ColorVec(0, 0, 0);
	}

	Vector3d point_unit_circle = RNG::rng_vec(-1.0, 1.0);
	while (point_unit_circle.getMagnitude() >= 1.0)
	{
		point_unit_circle = RNG::rng_vec(-1.0, 1.0);
	}

	point_unit_circle.Normalize();

	if (world.HitRay(r, 0.0001, INFINITY, rec))	//if the ray hits any object in world
	{
		Ray scatter;		//dependant on material struck by ray
		ColorVec atten;		//depedant of material struck by ray.
		if (rec.mat_ptr->scatter(r, rec, atten, scatter))
		{
			return Color(scatter, world, depth - 1) * atten;
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

int main()
{
	/*image dimension setup*/
	const double tMin = 0.0, tMax = INFINITY;
	const double AspectRatio = 16.0 / 9.0;
	const int image_width = 400;
	const int image_height = (double)image_width / AspectRatio;	
	const int rgb_max = 256;

	/*viewport*/
	Camera cam(Vector3d(-2 , 2 , 1) , Vector3d(0.0 ,0.0 , -1.0) ,Vector3d(0.0 ,1.0,0.0),AspectRatio, 90.0);
	auto R = cos(3.14159 / 4);

	//material setup
	auto material_ground = std::make_shared<lambertian>(ColorVec(0.8, 0.8, 0.0));
	auto material_center = std::make_shared<lambertian>(ColorVec(1.0, 0.6, 0.3));
	auto material_left = std::make_shared<dielectric>(1.5);
	auto material_right = std::make_shared<metal>(ColorVec(0.8, 0.6, 0.2),0.96);

	//world objects setup
	WorldObjects world;
	world.addObject(std::make_shared<Sphere>(Vector3d(0, 0, -1.0), 0.5,material_center));
	world.addObject(std::make_shared<Sphere>(Vector3d(0, -100.5, -1.0), 100,material_ground));
	world.addObject(std::make_shared<Sphere>(Vector3d(-1.0, 0.0, -1.0), -0.45, material_left));
	world.addObject(std::make_shared<Sphere>(Vector3d(1.0, 0.0, -1.0),0.5, material_right));

	//anti aliasing samples
	const int samples = 100;
	const int max_depth = 50;

	//image setup
	std::ofstream fout("RayTracer.ppm");
	fout << "P3" << std::endl;
	fout << image_width << " " << image_height << std::endl;
	fout << std::to_string(rgb_max) << std::endl;


	/*ray trace algoruthm here*/
	for (int j = image_height - 1; j >= 0; --j) {
		for (int i = 0; i < image_width; ++i) {
			ColorVec pixel(0.0, 0.0, 0.0);

			for (int k = 0; k < samples; ++k) {
				double u = (double)(j + RNG::rng()) / (double)(image_height - 1);		//image height ratio
				double v = (double)(i + RNG::rng()) / (double)(image_width - 1);		//image width ratio (for unit vector scale)
				//to determine the ray vector : http://www.lighthouse3d.com/tutorials/view-frustum-culling/geometric-approach-extracting-the-planes/
				pixel += Color(cam.GetRayDir(u , v ), world,max_depth);
			}

			ColorToFile(fout,pixel,rgb_max,samples);

		}
	}
}