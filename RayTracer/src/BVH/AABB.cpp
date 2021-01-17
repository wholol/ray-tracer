#include "AABB.h"
#include <algorithm>

AABB::AABB(const Point3d& min, Point3d & max)
	: minimum(min), maximum(max)
{
}

Vector3d AABB::max() const
{

	return maximum;
}

Vector3d AABB::min() const
{
	return minimum;
}

Point3d AABB::tmin() const
{
	return t_minimum;
}


bool AABB::Intersect(const Ray& r, double t_min, double t_max)
{
	for (int i = 0; i < 3; ++i)		//for each exis
	{
		double inv_dir = 1 / r.GetDir()[i];
		double t0 = (min()[i] - r.GetOrgin()[i]) * inv_dir;
		double t1 = (max()[i] - r.GetOrgin()[i]) * inv_dir;
		if (inv_dir < 0.0f)		//if ray direction is negative, swap values.
			std::swap(t0, t1);
		t_min = t0 > t_min ? t0 : t_min;
		t_max = t1 < t_max ? t1 : t_max;
		if (t_max <= t_min)
			return false;
		if (i == 0)	t_minimum.x = t_min;
		if (i == 1)	t_minimum.y = t_min;
		if (i == 2)	t_minimum.z = t_min;
	
	}
	return true;
}
