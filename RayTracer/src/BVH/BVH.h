#pragma once
#include "AABB.h"
#include "Hittables.h"
#include <memory>
#include <vector>
#include <algorithm>

struct BVH_node
{
	BVH_node* left;
	BVH_node* right;
	std::vector<Hittables> obj;
	AABB box;
};

class BVH
{
public:
	BVH(std::vector<Hittables>& objects);
private:
	void construct(BVH_node* node, std::vector<Hittables>& objects);
	AABB computeBoundingVolume(std::vector<Hittables>& objects);
};