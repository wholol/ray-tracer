#pragma once
#include "Hittables.h"
#include "HitPoint.h"
#include <vector>

class Scene {
public:
	void addObject(std::unique_ptr<Hittables> object);
	void clearScene();
	bool Intersect(const Ray& r, double tMin, double tMax, hit_point& hitpoint);
private:
	std::vector<std::unique_ptr<Hittables>> objects;
};