#pragma once
/*write rgb values to the ppm file.*/
#include <fstream>
#include "Vector3.h"
#include <algorithm>
#include <vector>
#include "Camera.h"
#include "Scene.h"



void ColorToFile(std::ofstream& fout,const int& rgb_max,const int& samples,int i , int j, int image_width, const std::vector<ColorVec>& pixels)
{
	//r << //g  << // b
	fout << (int)pixels[i + image_width * j].x << " " << (int)pixels[i + image_width * j].y << " " << (int)pixels[i + image_width * j].z << '\n';

}

void ColorToFile(std::ofstream& fout,ColorVec& c, const int& max_rgb, const int& samples)
{
	int r = int(std::sqrt(std::min(c.x / samples, 0.99)) * (double)max_rgb);
	int g = int(std::sqrt(std::min(c.y / samples, 0.99)) * (double)max_rgb);
	int b = int(std::sqrt(std::min(c.z / samples, 0.99)) * (double)max_rgb);
	fout << r << " " << g << " " << b << '\n';

}