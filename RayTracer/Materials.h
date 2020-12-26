#pragma once
#include "RNG.h"
#include "Geometries.h"

struct hit_record;

class material
{
public:
	virtual bool scatter(const Ray& r,const hit_record& rec, ColorVec& attenuation, Ray& scattered) const = 0;
};

class lambertian : public material
{
public:
	lambertian(const ColorVec& a) : albedo(a)	{}

	bool scatter(const Ray& r, const hit_record& rec, ColorVec& attenuation, Ray& scattered) const override
	{
		Vector3d point_unit_circle = RNG::rng_vec(-1.0, 1.0);
		
		while (point_unit_circle.getMagnitude() >= 1.0)
		{
			point_unit_circle = RNG::rng_vec(-1.0, 1.0);
		}
		
		auto near_zero = [](Vector3d& scat)
		{
			const auto s = 1e-8;
			return fabs(scat.x < s) && fabs(scat.y < s) && fabs(scat.z < s);
		};

		Vector3d scatter_dir = rec.normal + point_unit_circle;

		if (near_zero(scatter_dir))
		{
			scatter_dir = rec.normal;
		}

		scattered = Ray(rec.point, scatter_dir);	//construct the new scattered ray vector
		attenuation = albedo;
		
		return true;
	}

public:
	ColorVec albedo;
};

class metal : public material
{
public:
	metal(const ColorVec& a,double fuzz) : albedo(a) , fuzz(fuzz) {
		if (fuzz > 1) fuzz = 1;
	}

	bool scatter(const Ray& r, const hit_record& rec, ColorVec& attenuation, Ray& scattered) const override
	{
		auto reflect = [](const Vector3d& v, const Vector3d& n)
		{
			return v  - (n * v.getDotProduct(n) * 2);
		};


		Vector3d point_unit_circle = RNG::rng_vec(-1.0, 1.0);

		while (point_unit_circle.getMagnitude() >= 1.0)
		{
			point_unit_circle = RNG::rng_vec(-1.0, 1.0);
		}

		Vector3d reflect_ray = reflect(r.GetDir().getNormalized(), rec.normal);
		scattered = Ray(rec.point, reflect_ray + point_unit_circle * fuzz);
		attenuation = albedo;
		return (scattered.GetDir().getDotProduct(rec.normal) > 0);
	}


public:
	ColorVec albedo;
	double fuzz;
};

class dielectric : public material
{
public:
	dielectric(double index_of_refraction) : ir(index_of_refraction) {}

	bool scatter(const Ray& r, const hit_record& rec, ColorVec& attenuation, Ray& scattered) const override
	{
		attenuation = ColorVec(1.0, 1.0, 1.0);
		double refractio_ratio = rec.front_face ? (1.0 / ir) : ir;

		auto refracted = [](const Vector3d& uv, const Vector3d& n, double etai_over_etat)
		{
			Vector3d t = uv * -1;
			auto g = uv.getDotProduct(n);

			auto cos_theta = fmin(g, 1.0);
			Vector3d r_out_perp = (uv + n * cos_theta) * etai_over_etat;
			Vector3d r_out_parallel = n * -sqrt(fabs(1.0 - r_out_perp.getMagnitude()));
			return r_out_perp + r_out_parallel;
		};

		auto getrefracted = refracted(r.GetDir().getNormalized(), rec.normal, refractio_ratio);
		scattered = Ray(rec.point, getrefracted);
		return true;
	}



public:
	double ir;
};