#pragma once
#include "Vector3.h"
#include "Ray.h"

class AABB
{
public:
	AABB() {}
	AABB(const Point3d& min, Point3d& max);
	Point3d max() const;
	Point3d min() const;
	Point3d tmin() const;
	double tmin_magnitude() const;
	bool Intersect(const Ray& r, double t_min, double t_max);

private:
	Point3d maximum;		//contains the max points for the box in term os x,y and z
	Point3d minimum;		//contains the max points for the box in term of x,y and z
	Point3d t_minimum;
	double t_minimum_magnitude;

};