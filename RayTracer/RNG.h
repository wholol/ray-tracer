#pragma once
#include <random>

class RNG
{
public:
	RNG(const RNG&) = delete;

	static RNG& GetInstance()
	{
		static RNG inst;
		return inst;
	}

private:
	RNG();

};