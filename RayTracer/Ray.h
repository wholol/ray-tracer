#pragma once
#include "Vector3.h"

class Ray
{
public:
	Ray(const Vector3d& origin, const Vector3d& Dir) 
		:Origin(origin) , Dir(Dir)  {}

	Vector3d at(double t) const
	{
		Vector3d P;
		//direction = unit vector for the ray
		P = Origin + (Dir * t);
		return P;
	}

	Vector3d GetOrgin() const
	{
		return Origin;
	}

	Vector3d GetDir() const
	{
		return Dir;
	}


private:
	Vector3d Origin;
	Vector3d Dir;
	
};