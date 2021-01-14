#pragma once
#include "Vector3.h"
#include "Ray.h"
#include "RNG.h"
class Camera
{
public:
	Camera(Vector3d lookfrom, Vector3d lookat, const double& Aspect_Ratio, double fov, double aperture, double focus_distance);
	Ray GetRayDir(double u, double v) const;

private:
	Vector3d camerapos;
	Vector3d horizontal;
	Vector3d vertical;
	Vector3d top_right_corner;
	Vector3d right, forward, up;
	Vector3d tmp = Vector3d(0.0, 1.0, 0.0);	
	double lens_radius;
};