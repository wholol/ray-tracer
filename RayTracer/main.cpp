#include <iostream>
#include <fstream>
#include <string>
#include "Vector3.h"
#include "WriteColor.h"
#include "Ray.h"
#include "WorldObjects.h"
#include "Geometries.h"
#include "Sphere.h"
#include "RNG.h"

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
		Vector3d target = rec.point + rec.normal + point_unit_circle;
		return (Color(Ray(rec.point , target - rec.point), world , depth -1)) * 0.5;
	}

	else//if no intersection
	{
		Vector3d n = r.GetDir().getNormalized();
		double t = (n.y * 0.5) + 1.0;
		ColorVec col = ColorVec(1.0, 1.0, 1.0) * (1.0 - t) + ColorVec(0.5, 0.7, 1.0) * t;
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

	/*ray origin*/
	Vector3d RayOrigin(0.0, 0.0, 0.0);

	/*viewport*/
	double viewport_height = 2.0;
	double viewport_width = AspectRatio * viewport_height;
	double d = 1.0;		//distance from origin to viewport center

	Vector3d origin(0, 0, 0);
	Vector3d focal(0, 0, -1.0);	//unit vector from origin to viewport center
	Vector3d horizontal(viewport_width, 0.0, 0.0);
	Vector3d vertical(0.0, viewport_height, 0.0);
	//to determine corner positions: http://www.lighthouse3d.com/tutorials/view-frustum-culling/geometric-approach-extracting-the-planes/
	Vector3d bottom_left_corner = origin - (horizontal / 2) - (vertical / 2) + (focal * d);

	//world objects setup
	WorldObjects world;
	world.addObject(std::make_shared<Sphere>(Vector3d(0, 0, -1), 0.5));
	world.addObject(std::make_shared<Sphere>(Vector3d(0, -100.5, -1), 100));

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
				Ray r(RayOrigin, bottom_left_corner + horizontal * v + vertical * u - origin);
				pixel += Color(r, world,max_depth);

			}

			ColorToFile(fout,pixel,rgb_max,samples);

		}
	}
}