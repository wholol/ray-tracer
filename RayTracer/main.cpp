#include <iostream>
#include <fstream>
#include <string>
#include "Vector3.h"
#include "WriteColor.h"
#include "Ray.h"
#include "WorldObjects.h"
#include "Geometries.h"
#include "Sphere.h"


ColorVec Color(const Ray& ray)
{
	Vector3d n = ray.GetDir().getNormalized();
	double t = (n.y * 0.5) + 1.0;
	ColorVec col = ColorVec(1.0, 1.0, 1.0) * (1.0 - t) + ColorVec(0.5, 0.7, 1.0) * t;
	return col;
}

bool hitSphere(const Ray& r, Vector3d center, double radius)
{
	Vector3d n_unit = r.GetDir().getNormalized();
	Vector3d A = r.GetOrgin() - center;		//A - C

	double c = A.getDotProduct(A) - radius * radius;
	double a =  n_unit.getDotProduct(n_unit);
	double b = 2.0 * A.getDotProduct(n_unit);

	double discriminant = b * b -( 4 * a * c);
	if (discriminant < 0)
	{
		return false;
	}
	return true;
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
	Vector3d horizontal(viewport_width, 0.0, 0.0);dwdw
	Vector3d vertical(0.0, viewport_height, 0.0);
	//to determine corner positions: http://www.lighthouse3d.com/tutorials/view-frustum-culling/geometric-approach-extracting-the-planes/
	Vector3d bottom_left_corner = origin - (horizontal / 2) - (vertical / 2) + (focal * d);

	std::ofstream fout("RayTracer.ppm");

	fout << "P3" << std::endl;
	fout << image_width << " " << image_height << std::endl;
	fout << std::to_string(rgb_max) << std::endl;


	/*ray trace algoruthm here*/
	for (int j = image_height - 1; j >= 0; --j) {
		for (int i = 0; i < image_width; ++i) {
			double u = (double)(j) / (double)(image_height - 1);		//image height ratio
			double v = (double)(i) / (double)(image_width - 1);		//image width ratio (for unit vector scale)
			//to determine the ray vector : http://www.lighthouse3d.com/tutorials/view-frustum-culling/geometric-approach-extracting-the-planes/
			Ray r(RayOrigin, bottom_left_corner + horizontal * v + vertical * u - origin);
	
			Vector3d ColorVec = Color(r);
			Vector3d center(0, 0, -1);
			if (hitSphere(r,center , 0.5))
			{
				ColorVec(255, 0, 0);
			}

			ColorToFile(fout,ColorVec,rgb_max);

		}
	}
}