#pragma once
#include "Vector3.h"
#include "Ray.h"
#include <memory>

class Material;

struct hit_point
{
	Vector3d point;
	Vector3d normal;
	double t;
	bool front_face;
	std::shared_ptr<Material> mat_ptr;

	//this function is used to flip the normal, in the event that there is a ray in the object (caused by refraction)
	void set_face_normal(const Ray& r, const Vector3d& outward_normal)
	{

		if (r.GetDir().getDotProduct(outward_normal) > 0.0)
		{
			front_face = false;
			normal = outward_normal * -1.0;
		}

		else
		{
			normal = outward_normal;
			front_face = true;
		}
	}
};