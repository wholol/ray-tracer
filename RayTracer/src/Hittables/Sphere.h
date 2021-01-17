#pragma once
#include "Hittables.h"

class Sphere : public Hittables
{
public:
	Sphere(Point3d CircleCenter, double radius, std::shared_ptr<Material> mat);
	bool Intersect(double tMin, double tMax, const Ray& ray, hit_point& hitpoint) override;
	void bounding_box(double time0, double time1, AABB& output_box) const override;
private:
	Point3d CircleCenter;
	double radius;
	std::shared_ptr<Material> mat;
};