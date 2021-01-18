#pragma once
#include "AABB.h"
#include "Hittables.h"
#include <memory>
#include <vector>
#include <queue>
#include <algorithm>
#include <functional>


struct BVH_node
{
	BVH_node* left = nullptr;
	BVH_node* right = nullptr;
	bool isLeaf = false;
	std::shared_ptr<Hittables> leaf_object = nullptr;
	AABB box;
};

struct comparator		//used to determine priority queue order.
{
	//smallest distance t = highest priority. (objects are liekly in this region).
	bool operator()(BVH_node* left, BVH_node* right) {
		return (left->box.tmin().getMagnitude() > right->box.tmin().getMagnitude());	
	}
};

class BVH
{
public:
	BVH() {};
	BVH(std::vector<std::shared_ptr<Hittables>>& SceneObjects);
	bool traverseBVH(const Ray& r, double tMin, double tMax, hit_point& hitpoint);	//traverse = same meaning as intersect with an object.
	bool traverseBVH_parallel(const Ray& r, double tMin, double tMax, hit_point& hitpoint , BVH_node* node);

private:
	//helper functions
	void construct(BVH_node* node, std::vector<std::shared_ptr<Hittables>>& objects);
	AABB computeBoundingVolume(std::vector<std::shared_ptr<Hittables>>& objects);
	void ComputeRayAABB(const Ray& r, double tMin, double tMax, BVH_node* node , std::priority_queue<BVH_node*, std::vector<BVH_node*>, comparator>& pq);

private:
	std::priority_queue<BVH_node*, std::vector<BVH_node*>, comparator> pq;	
	BVH_node* root;
	std::vector<std::shared_ptr<Hittables>> SceneObjects_;
	int max_depth = 50;
};