#pragma once
#include "Ray.h"


struct hit_record
{
	Vector3d point;
	Vector3d normal;
	double t;
	bool front_face;

	void set_face_normal(const Ray& r , const Vector3d& outward_normal)
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

/*all types of geometries that can get hit (e.g. sphere, cube etc)*/
/*each geometry has diffeerent ray interesect function.*/
class Geometries
{
public:
	virtual bool HitRay(double tMin, double tMax,const Ray& ray, hit_record& hit) = 0;
};