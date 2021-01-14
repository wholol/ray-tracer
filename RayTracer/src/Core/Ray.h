#pragma once
#include "Vector3.h"

class Ray
{
public:
	
	Ray() {};
	Ray(const Vector3d& camerapos, const Vector3d& Dir);
	Vector3d at(double t) const;
	Vector3d GetOrgin() const;
	Vector3d GetDir() const;

private:
	Vector3d Origin;
	Vector3d Dir;
};