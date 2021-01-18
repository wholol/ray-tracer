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
	
	bool hit = false;
	traverseBVH_parallel(r, tMin, tMax, hitpoint, root , hit);	
	
	return hit;	//hit is passed by ref,same wih hitpoint
	
	//double t_closest = tMax;
	//bool hit_object = false;
	//
	//hit_point temp;
	//pq.push(root);	//initalize with the root node.
	//while (!pq.empty())	//is queue isnt empty AND the minimum t value of the bounding box is smaller than t closest
	//{
	//	BVH_node* candidate = pq.top();		//get the node with the box distance that is nearest
	//	pq.pop();
	//
	//	if (candidate->isLeaf)
	//	{
	//		if (candidate->leaf_object->Intersect(tMin, t_closest, r, temp) && temp.t < t_closest)
	//		{
	//			hitpoint = temp;		//update hit point
	//			t_closest = temp.t;		//update the closest distance to the object.
	//			hit_object = true;
	//		}
	//	}
	//
	//	else {
	//		//use t_closest,no point testing the rays behind the closest place.
	//		ComputeRayAABB(r, tMin, t_closest, candidate->left);
	//		ComputeRayAABB(r, tMin, t_closest, candidate->right);
	//	}
	//}
	//return hit_object;
}

void BVH::traverseBVH_parallel(const Ray& r, double tMin, double& tMax, hit_point& hitpoint, BVH_node* node,bool& hit)
{
	static double t_closest = tMax;
	
	if (!node->box.Intersect(r, tMin, tMax))	//one thing to note here: tMax is already upated to be hitpoint.t (passed by ref)
	{
		return;
	}

	if (node->isLeaf)	//also prevents taversing null pointers.
	{
		if (node->leaf_object->Intersect(tMin, tMax, r, hitpoint) && hitpoint.t < tMax)
		{
			hit = true;
			tMax = hitpoint.t;
		}

		return;
	}
	else {

		//double left_node = INFINITY;
		//double right_node = INFINITY;
		//
		//node->left->box.Intersect(r, tMin, tMax);
		//left_node = node->left->box.tmin_magnitude();
		//
		//node->right->box.Intersect(r, tMin, tMax);
		//right_node = node->right->box.tmin_magnitude();
		//
		//BVH_node* closer_node = (left_node <= right_node) ? node->left : node->right;
		//BVH_node* further_node = (left_node <= right_node) ? node->right : node->left;
		//
		//
		//traverseBVH_parallel(r, tMin, tMax, hitpoint, closer_node, hit);
		//if (further_node->box.tmin_magnitude() < tMax) {
		//	traverseBVH_parallel(r, tMin, tMax, hitpoint, further_node, hit);
		//}


		traverseBVH_parallel(r, tMin, tMax, hitpoint, node->left, hit);
		traverseBVH_parallel(r, tMin, tMax, hitpoint, node->right,hit);
		
	}
}

void BVH::ComputeRayAABB(const Ray& r, double tMin,double tMax, BVH_node* node)
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

