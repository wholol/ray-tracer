#pragma once
#include "Vector3.h"
#include "Ray.h"
#include "RNG.h"
class Camera
{
public:
	Camera(Vector3d lookfrom, Vector3d lookat, Vector3d up, const double& Aspect_Ratio, double fov, double aperture);
	Ray GetRayDir(double u, double v) const;

private:
	Vector3d origin;
	Vector3d horizontal;
	Vector3d vertical;
	Vector3d bottom_left_corner;
	Vector3d uv, w, vv;
	double lens_radius;
};