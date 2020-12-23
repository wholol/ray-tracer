#pragma once
/*write rgb values to the ppm file.*/
#include <fstream>
#include "Vector3.h"

void ColorToFile(std::ofstream& fout, const Vector3d& ColorVec,const int& rgb_max)
{
	//std::cout << ColorVec.x << " " << ColorVec.y << " " << ColorVec.z << '\n';
	fout << ColorVec.x * (double)rgb_max << " " << ColorVec.y * (double)rgb_max << " " << ColorVec.z *(double)rgb_max << '\n';

}