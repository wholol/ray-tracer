#include "BVH.h"
#include "RNG.h"

BVH::BVH(std::vector<std::shared_ptr<Hittables>>& SceneObjects)
	:SceneObjects_(SceneObjects)
{
	root = new BVH_node();
	root->box = computeBoundingVolume(SceneObjects_);
	construct(root, SceneObjects_);
}

void BVH::construct(BVH_node* node, std::vector<std::shared_ptr<Hittables>>& objects)		//copy the vector, we do not ant to sort the real vector.
{
	int N = objects.size();

	node->box = computeBoundingVolume(objects);
	
	if (N == 1)
	{
		node->isLeaf = true;
		node->leaf_object = objects[0];	
		return;
	}

	else
	{
		int random_axis = RNG::rng_int(0, 2);		

		switch (random_axis)
		{
		case 0:	//sort by x axis
			std::sort(objects.begin(), objects.end(), [](std::shared_ptr<Hittables>& one, std::shared_ptr<Hittables>& two) {
														AABB box_a, box_b;
														one->bounding_box(0, 0, box_a);
														two->bounding_box(0, 0, box_b);
														return box_a.min().x < box_b.min().x;});
			break;
		
		case 1:	//sort by y axis
			std::sort(objects.begin(), objects.end(), [](std::shared_ptr<Hittables>& one, std::shared_ptr<Hittables>& two) {
														AABB box_a, box_b;
														one->bounding_box(0, 0, box_a);
														two->bounding_box(0, 0, box_b);
														return box_a.min().y < box_b.min().y; });
			break;
		case 2:	//sort by z axis
			std::sort(objects.begin(), objects.end(), [](std::shared_ptr<Hittables>& one, std::shared_ptr<Hittables>& two) {
														AABB box_a, box_b;
														one->bounding_box(0, 0, box_a);
														two->bounding_box(0, 0, box_b);
														return box_a.min().z < box_b.min().z; });
			break;
		}
		
		int midpoint;
		midpoint = (N + 1) / 2;
		
		//first half
		std::vector<std::shared_ptr<Hittables>> firstpartition;
		for (int i = 0; i < midpoint; ++i)
		{
			firstpartition.emplace_back(objects[i]);
		}
		BVH_node* new_left = new BVH_node();
		node->left = new_left;
		construct(node->left, firstpartition);

		//second half
		std::vector<std::shared_ptr<Hittables>> secondpartition;
		for (int i = midpoint; i < objects.size(); ++i)
		{
			secondpartition.emplace_back(objects[i]);	
		}

		BVH_node* new_right = new BVH_node();
		node->right = new_right;
		construct(node->right, secondpartition);
	}
}

AABB BVH::computeBoundingVolume(std::vector<std::shared_ptr<Hittables>>& objects)
{
	if (objects.size() > 0) {
		bool first = true;
		
		auto merge_boxes = [](AABB& first, AABB& second) {
			Point3d min(fmin(first.min().x, second.min().x),
				fmin(first.min().y, second.min().y),
				fmin(first.min().z, second.min().z));
	
			Point3d max(fmax(first.max().x, second.max().x),
				fmax(first.max().y, second.max().y),
				fmax(first.max().z, second.max().z));
	
			return AABB(min, max);
		};
	
		AABB single_object_box;
		AABB output_box;		//output box = the total bonding box for all objects.
		bool first_object = true;
	
		for (auto& object : objects)
		{
			if (first_object)
			{
				object->bounding_box(0, 0, output_box);	//get the bunding box of an invididual object
				first_object = false;
			}
			else {
				object->bounding_box(0, 0, single_object_box);
				output_box = merge_boxes(output_box, single_object_box);
			}
		}
		return output_box;
	}
}

bool BVH::traverseBVH(const Ray& r, double tMin, double tMax,hit_point& hitpoint)
{
	//return traverseBVH_parallel(r, tMin, tMax, hitpoint, root);
	double t_closest = tMax;
	bool hit_object = false;
	//std::priority_queue<BVH_node*, std::vector<BVH_node*>, comparator> pq;	//local priority queue for multithreading. TODO: REMOVE THIS IF POSSIBLE, PERFORMING ALOT OF REALLOCS.
	hit_point temp;	
	pq.push(root);	//initalize with the root node.
	
	while (!pq.empty())	
	{
		BVH_node* candidate = pq.top();		//get the node with the box distance that is nearest
		pq.pop();
		
		if (candidate->isLeaf)
		{
			const auto& object = candidate->leaf_object;
			if (object->Intersect(tMin, t_closest, r, temp) && temp.t < t_closest)
			{
				hitpoint = temp;		//update hit point
				t_closest = temp.t;		//update the closest distance to the object.
				hit_object = true;
			}
			
		}
	
		else {
			//use t_closest,no point testing the rays behind the closest place.
			ComputeRayAABB(r, tMin, t_closest, candidate->left,pq);
			ComputeRayAABB(r, tMin, t_closest, candidate->right,pq);
		}
	}
	return hit_object;
}

bool BVH::traverseBVH_parallel(const Ray& r, double tMin, double tMax, hit_point& hitpoint, BVH_node* node)
{
	hit_point temp;
	static double t_closest = tMax;

	if (node && node->box.Intersect(r, tMin, t_closest) && node->box.tmin().getMagnitude() < t_closest)	
	{
		if (node->isLeaf)
		{
			const auto& object = node->leaf_object;
		
			if (object->Intersect(tMin, t_closest, r, temp) && temp.t < t_closest)
			{
				hitpoint = temp;		//update hit point
				t_closest = temp.t;		//update the closest distance to the object.
				return true;
			}
			
		}
		else {
			return traverseBVH_parallel(r, tMin, t_closest, hitpoint, node->left) || traverseBVH_parallel(r, tMin, t_closest, hitpoint, node->right);
			
		}
	}
	else
	{
		return false;
	}
}

void BVH::ComputeRayAABB(const Ray& r, double tMin,double tMax, BVH_node* node ,std::priority_queue<BVH_node*, std::vector<BVH_node*>, comparator>& pq)
{
	if (!node->box.Intersect(r, tMin, tMax))
	{
		return;
	}
	else
	{
		pq.push(node);
	}

}

