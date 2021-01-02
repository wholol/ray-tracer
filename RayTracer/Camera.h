#pragma once
#include "Vector3.h"
#include "Ray.h"
#include "RNG.h"
class Camera
{
public:
	Camera(Vector3d lookfrom , Vector3d lookat, Vector3d up, const double& Aspect_Ratio,double fov,double aperture)
	{
		
		const double AspectRatio = Aspect_Ratio;
		double d = 1.0;		//distance from origin to viewport center
		double h = tan(fov * (3.14159 / 180) * 0.5);
		double viewport_height = 2.0 * h;
		double viewport_width = AspectRatio * viewport_height;

		 w = (lookfrom - lookat).getNormalized();
		 uv = up.getCrossProduct(w).getNormalized();
		 vv = w.getCrossProduct(uv);

		origin = lookfrom;
		
		double focus_dist = (lookfrom - lookat).getMagnitude();

		horizontal =  uv * viewport_width * focus_dist;
		vertical = vv * viewport_height * focus_dist;
		//to determine corner positions: http://www.lighthouse3d.com/tutorials/view-frustum-culling/geometric-approach-extracting-the-planes/
		bottom_left_corner = origin + (horizontal / 2) + (vertical / 2) - w * focus_dist;

		lens_radius = aperture * 0.5;
	}

	Ray GetRayDir(double u , double v) const
	{
		auto rng_lens_point = []() {
			
			Vector3d p = RNG::rng_vec(-1.0, 1.0);	//generate random point in lens

			while (p.getMagnitude() *p.getMagnitude() >= 1.0)
			{
				p = RNG::rng_vec(-1.0, 1.0);
			}
			return p;
		};

		Vector3d p = rng_lens_point();
		Vector3d rd = p * lens_radius;				//unit vec * distance

		Vector3d offset = uv * rd.x + vv * rd.y;

		return Ray(origin + offset, bottom_left_corner - horizontal * v - vertical * u - origin - offset);
	}

private:
	Vector3d origin;
	Vector3d horizontal;
	Vector3d vertical;
	Vector3d bottom_left_corner;
	Vector3d uv, w, vv;
	double lens_radius;
};