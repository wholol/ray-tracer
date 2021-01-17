#pragma once
#include "Hittables.h"
#include "HitPoint.h"
#include "BVH.h"
#include <vector>

class Scene {
public:
	void addObject(std::shared_ptr<Hittables> object);
	void buildSceneBVH();
	void clearScene();
	bool Intersect(const Ray& r, double tMin, double tMax, hit_point& hitpoint);
private:
	std::vector<std::shared_ptr<Hittables>> objects;
	BVH bvh;
	bool BVH_built = false;

	
};