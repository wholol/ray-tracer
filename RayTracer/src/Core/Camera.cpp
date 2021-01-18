#include "Camera.h"

Camera::Camera(Vector3d lookfrom, Vector3d lookat,const double& Aspect_Ratio, double fov, double aperture,double focus_distance)
{
	const double deg2rad = (3.14159 / 180.0);
	const double AspectRatio = Aspect_Ratio;
	double h = tan(fov * deg2rad * 0.5);
	double viewport_height = 2.0 * h;
	double viewport_width = AspectRatio * viewport_height;

	forward = (lookfrom - lookat).getNormalized();
	right = tmp.getCrossProduct(forward).getNormalized();
	up = forward.getCrossProduct(right);

	camerapos = lookfrom;
	
	//focus distance: specify where should the image be focused/clear.
	horizontal = right * viewport_width * focus_distance;		
	vertical = up * viewport_height * focus_distance;
	//to determine corner positions of a camera: http://www.lighthouse3d.com/tutorials/view-frustum-culling/geometric-approach-extracting-the-planes/
	top_right_corner = camerapos - (horizontal * 0.5) + (vertical * 0.5) - forward * focus_distance;
	lens_radius = aperture * 0.5;
}

Ray Camera::GetRayDir(double u, double v) const
{
	auto rng_lens_point = []() {

		Vector3d p = RNG::rng_vec(-1.0, 1.0);	//generate random point in lens

		while (p.getMagnitude_squared() >= 1.0)
		{
			p = RNG::rng_vec(-1.0, 1.0);
		}
		return p;
	};

	Vector3d p = rng_lens_point();
	Vector3d rd = p * lens_radius;				//unit vec * distance

	Vector3d offset = right * rd.x + up * rd.y;

	return Ray(camerapos + offset, top_right_corner + horizontal * v - vertical * u - camerapos - offset);  //direction ray should subtract offset to "balance the ray out"
}



