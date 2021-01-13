#pragma once
#include "Vector3.h"
#include "HitPoint.h"

class Hittables
{
public:
	virtual bool Intersect(double tMin, double tMax,const Ray& ray, hit_point& hitpoint) = 0;
};

class Sphere : public Hittables
{
public:
	Sphere(Vector3d CircleCenter, double radius, std::shared_ptr<Material> mat);
	bool Intersect(double tMin, double tMax, const Ray& ray, hit_point& hitpoint) override;

private:
	Vector3d CircleCenter;
	double radius;
	std::shared_ptr<Material> mat;
};