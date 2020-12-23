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
	bool HitRay(Ray& r, double tMin, double tMax,hit_record& rec)
	{
		bool hit = false;
		hit_record temp;
		double closest = tMax;

		for (auto& object : objects)
		{
			if (object->HitRay(tMin, tMax, r, temp))
			{
				hit = true;
				closest = temp.t;
				rec = temp;
			}
		}

		return hit;
	}

private:
	std::vector<std::shared_ptr<Geometries>> objects;	
	bool hit;

};