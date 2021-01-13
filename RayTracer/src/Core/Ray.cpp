#include "Ray.h"

Ray::Ray(const Vector3d & origin, const Vector3d & Dir)
	:Origin(origin), Dir(Dir) {}

Vector3d Ray::at(double t) const
{
	Vector3d P;
	//direction = unit vector for the ray
	P = Origin + (Dir * t);
	return P;
}

Vector3d Ray::GetOrgin() const
{
	return Origin;
}

Vector3d Ray::GetDir() const
{
	return Dir;
}


