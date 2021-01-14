#pragma once
#include "Vector3.h"
#include "HitPoint.h"

//hittable class: base class for primitives to inherti from.
class Hittables
{
public:
	virtual bool Intersect(double tMin, double tMax,const Ray& ray, hit_point& hitpoint) = 0;
};