#pragma once
#include "Vector3.h"
#include "Ray.h"
#include <math.h>
#include "HitPoint.h"
#include "Geometries.h"

/*the point where the sphere hits*/

class Sphere : public Geometries
{

public:

	Sphere(Vector3d CircleCenter, double radius)
		:CircleCenter(CircleCenter) , radius(radius)
	{}

	bool HitRay(double tMin, double tMax,Ray& ray, hit_record& rec) override
	{
		double a = ray.GetDir().getDotProduct(ray.GetDir());	//get the dot product of the ray direction.

		Vector3d temp = ray.GetOrgin() - CircleCenter;
		double c = -(radius * radius) + temp.getDotProduct(temp);
		double b = 2 * ray.GetDir().getDotProduct(temp);
		double det = b * b - 4 * a*c;

		/*if a solution for t exists on the sphere*/
		if (det > 0.0)
		{
			double get_t_val = (-b - (sqrt(det))) / (2.0 * a);	

			if (get_t_val < tMax && get_t_val > tMin)
			{
				rec.t = get_t_val;
				rec.point = ray.at(get_t_val);
				Vector3d outward_normal = (rec.point - CircleCenter) / radius;
				rec.set_face_normal(ray, outward_normal);

				return true;
			}

			get_t_val = (-b - (sqrt(det))) / (2.0 * a);

			if (get_t_val < tMax && get_t_val > tMin)
			{
				rec.t = get_t_val;
				rec.point = ray.at(get_t_val);
				Vector3d outward_normal = (rec.point - CircleCenter) / radius;
				rec.set_face_normal(ray, outward_normal);

				return true;
			}
			
		}
		
		return false;
	}


private:
	Vector3d CircleCenter;
	double radius;
	HitPoint In;
	HitPoint Out;

};