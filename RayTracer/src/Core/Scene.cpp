#include "Scene.h"

void Scene::addObject(std::shared_ptr<Hittables> object)
{
	objects.emplace_back(std::move(object));
}

void Scene::buildSceneBVH()
{
	bvh = BVH(objects);
	BVH_built = true;
}

void Scene::clearScene()
{
	objects.clear();
}

bool Scene::Intersect(const Ray& r, double tMin, double tMax, hit_point& hitpoint)
{

	//bool hit_anything = false;
	//hit_point temp;
	//double closest = tMax;
	//
	//for (auto& object : objects)		//O(n)
	//{
	//	if (object->Intersect(tMin, closest, r, temp))
	//	{
	//		hit_anything = true;
	//		closest = temp.t;	//acts as a z buffer
	//		hitpoint = temp;
	//	}
	//}
	//
	//return hit_anything;

	if (!BVH_built)
	{
		throw std::runtime_error("bvh has not been built.");
	}
	else {
		return (bvh.traverseBVH(r, tMin, tMax, hitpoint));
	}

	
	
}