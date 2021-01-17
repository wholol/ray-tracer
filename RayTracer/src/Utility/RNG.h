#pragma once
#include <random>

//mt19337 is slow. do not use it.
class RNG {
public:
	
	static double rng() {
		 return rand() / (RAND_MAX + 1.0);
		//static std::uniform_real_distribution<double> distribution(0.0, 1.0);
		//static std::mt19937 generator;
		//return distribution(generator);

	}

	static double rng(double min, double max) {
		//static std::uniform_real_distribution<double> distribution(min, max);
		//static std::mt19937 generator;
		//return distribution(generator);
		return min + (max - min)*rng();
	}

	static int rng_int() {
		return rand() / (RAND_MAX + 1);
		//static std::uniform_real_distribution<double> distribution(0.0, 1.0);
		//static std::mt19937 generator;
		//return distribution(generator);

	}

	static int rng_int(int min, int max) {
		//static std::uniform_real_distribution<double> distribution(min, max);
		//static std::mt19937 generator;
		//return distribution(generator);
		return min + (max - min)*rng();
	}

	static Vector3d rng_vec(double min, double max) {
		return Vector3d(rng(min, max), rng(min, max), rng(min, max));
	}

	static Vector3d rng_vec() {
		return Vector3d(rng(), rng(), rng());
	}
};