#pragma once
#include "Geometries.h"
#include <vector>


class WorldObjects {
public:

	void addObject(std::shared_ptr<Geometries> geo)
	{
		
		objects.emplace_back(std::move(geo));
	}

	
	void clearWorld()
	{
		objects.clear();
	}

	/*checks for all geometries ray intersction.*/
	bool HitRay(const Ray& r, double tMin, double tMax,hit_record& rec)
	{
		bool hit_anything = false;
		hit_record temp;
		double closest = tMax;

		for (auto& object : objects)
		{
			if (object->HitRay(tMin, closest, r, temp))
			{
				hit_anything = true;
				closest = temp.t;	//acts as a z buffer
				rec = temp;
			}
		}

		return hit_anything;
	}

private:
	std::vector<std::shared_ptr<Geometries>> objects;	
	bool hit;

};