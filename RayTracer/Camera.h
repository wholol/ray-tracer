#pragma once
#include "Vector3.h"
#include "Ray.h"
class Camera
{
public:
	Camera(const double& Aspect_Ratio,double fov)
	{
		double viewport_height = 2.0;
		const double AspectRatio = Aspect_Ratio;
		double viewport_width = AspectRatio * viewport_height;
		double d = 1.0;		//distance from origin to viewport center

		Vector3d origin = Vector3d(0,0,0);
		Vector3d focal = Vector3d(0,0,-1);	//unit vector from origin to viewport center
		horizontal = Vector3d(viewport_width, 0.0, 0.0);
		vertical = Vector3d(0.0, viewport_height, 0.0);
		//to determine corner positions: http://www.lighthouse3d.com/tutorials/view-frustum-culling/geometric-approach-extracting-the-planes/
		bottom_left_corner = origin - (horizontal / 2) - (vertical / 2) + (focal * d);
	}

	Ray GetRayDir(double u , double v) const
	{

		return Ray(origin, bottom_left_corner + horizontal * v + vertical * u - origin);
	}

private:
	Vector3d origin;
	Vector3d focal;
	Vector3d horizontal;
	Vector3d vertical;
	Vector3d bottom_left_corner;
};