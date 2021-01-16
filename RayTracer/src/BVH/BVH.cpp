#include "BVH.h"


BVH::BVH(std::vector<Hittables>& objects)
{

	BVH_node* root = new BVH_node();
	construct(root, objects);
}

void BVH::construct(BVH_node* node, std::vector<Hittables>& objects)		//copy the vector, we do not ant to sort the real vector.
{
	int N = objects.size();

	node->box = computeBoundingVolume(objects);
	
	if (N == 1)
	{
		node->obj.push_back(objects[0]);	//makes a copy. objects will get destroyed in the stack frame. do not se a pointer to it.
		return;
	}


	else
	{
		//set up an axis to sort with.
		std::sort(objects.begin(), objects.end());		//sort accordingly to the axis.
		
		int midpoint;
		midpoint = N / 2;
		if (N / 2 != 0)
		{
			midpoint = N + 1 / 2;
		}
	

		std::vector<Hittables> firstpartition;
		for (int i = 0; i < midpoint; ++i)
		{
			firstpartition.emplace_back(objects[i]);
		}

		BVH_node* new_left = new BVH_node();
		node->left = new_left;
		construct(node->left, firstpartition);

		std::vector<Hittables> secondpartition;
		for (int i = midpoint; i < objects.size(); ++i)
		{
			secondpartition.emplace_back(objects[i]);
		}

		BVH_node* new_right = new BVH_node();
		node->right = new_right;
		construct(node->right, secondpartition);
	}
}

AABB BVH::computeBoundingVolume(std::vector<Hittables>& objects)
{
	bool first = true;

	auto surrounding_box = [](AABB& first, AABB& second) {
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
			object.bounding_box(0, 0, output_box);	//get the bunding box of an invididual object
			first_object = false;
		}
		else {
			object.bounding_box(0, 0, single_object_box);
			output_box = surrounding_box(output_box, single_object_box);
		}
	}
	return output_box;
}
