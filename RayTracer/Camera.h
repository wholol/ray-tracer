#pragma once
#include "Vector3.h"
#include "Ray.h"
class Camera
{
public:
	Camera(Vector3d lookfrom , Vector3d lookat, Vector3d up, const double& Aspect_Ratio,double fov)
	{
		
		const double AspectRatio = Aspect_Ratio;
		double d = 1.0;		//distance from origin to viewport center
		double h = tan(fov * (3.14159 / 180) * 0.5);
		double viewport_height = 2.0 * h;
		double viewport_width = AspectRatio * viewport_height;

		Vector3d w = (lookfrom - lookat).getNormalized();
		Vector3d u = up.getCrossProduct(w).getNormalized();
		Vector3d v = w.getCrossProduct(u);

		origin = lookfrom;
		Vector3d focal = Vector3d(0,0,-1);	//unit vector from origin to viewport center

		horizontal =  u * viewport_width;
		vertical = v * viewport_height;
		//to determine corner positions: http://www.lighthouse3d.com/tutorials/view-frustum-culling/geometric-approach-extracting-the-planes/
		bottom_left_corner = origin - (horizontal / 2) - (vertical / 2) - w;
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