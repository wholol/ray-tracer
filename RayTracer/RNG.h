#pragma once
#include <random>


class RNG {
public:
	static double rng() {
		static std::uniform_real_distribution<double> distribution(0.0, 1.0);
		static std::mt19937 generator;
		return distribution(generator);
	}

	static double rng(double min, double max) {
		static std::uniform_real_distribution<double> distribution(min, max);
		static std::mt19937 generator;
		return distribution(generator);
	}

	static Vector3d rng_vec(double min, double max) {
		return Vector3d(rng(min, max), rng(min, max), rng(min, max));
	}

	static Vector3d rng_vec() {
		return Vector3d(rng(), rng(), rng());
	}
};