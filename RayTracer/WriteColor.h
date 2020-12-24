#pragma once
/*write rgb values to the ppm file.*/
#include <fstream>
#include "Vector3.h"
#include <algorithm>

void ColorToFile(std::ofstream& fout, const Vector3d& ColorVec,const int& rgb_max,const int& samples)
{
	//std::sqrt for gamma corerction
	double s = samples;
	double r = std::sqrt(std::min( ColorVec.x / s , 0.99));
	double g = std::sqrt(std::min(ColorVec.y / s, 0.99));
	double b = std::sqrt(std::min(ColorVec.z / s, 0.99));



	//std::cout << ColorVec.x << " " << ColorVec.y << " " << ColorVec.z << '\n';
	fout << (int)(r * (double)rgb_max) << " " << (int)(g * (double)rgb_max) << " " << (int)(b *(double)rgb_max) << '\n';

}