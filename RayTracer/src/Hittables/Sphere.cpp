#include "Sphere.h"

Sphere::Sphere(Point3d CircleCenter, double radius, std::shared_ptr<Material> mat)
	:CircleCenter(CircleCenter), radius(radius), mat(mat)
{}

bool Sphere::Intersect(double tMin, double tMax, const Ray& ray, hit_point& hitpoint)
{
	double a = ray.GetDir().getDotProduct(ray.GetDir());	//get the dot product of the ray direction.
	Vector3d temp = ray.GetOrgin() - CircleCenter;
	double c = -(radius * radius) + temp.getDotProduct(temp);
	double b = 2 * ray.GetDir().getDotProduct(temp);
	double det = b * b - 4 * a*c;

	//if a solution for t exists on the sphere
	//if a solution for t exists on the sphere
	if (det > 0.0)
	{
		double get_t_val = (-b - (sqrt(det))) / (2.0 * a);	//get the closer root value


		if (get_t_val > tMax || get_t_val < tMin)	//if t value within bounds
		{
			get_t_val = (-b + (sqrt(det))) / (2.0 * a);

			if (get_t_val > tMax || get_t_val < tMin)
			{
				return false;
			}
		}
		hitpoint.t = get_t_val;
		hitpoint.point = ray.at(get_t_val);
		Vector3d outward_normal = (hitpoint.point - CircleCenter) / radius;
		hitpoint.set_face_normal(ray, outward_normal);
		hitpoint.mat_ptr = mat;
		return true;
	}

	return false;
}

void Sphere::bounding_box(double time0, double time1, AABB& output_box) const
{
	Point3d min(CircleCenter.x - radius, CircleCenter.y - radius, CircleCenter.z - radius);
	Point3d max(CircleCenter.x + radius, CircleCenter.y + radius, CircleCenter.z + radius);

	output_box = AABB(min,max);
}

