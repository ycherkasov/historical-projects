#pragma once
#include <vector>

class NoiseGenerator
{
public:
	static float FRand(float ran);
	static void GenerateWhiteNoise(std::vector<double>& out_serie);
	static void GenerateJumpNoise(std::vector<double>& out_serie);
	static void GenerateShiftNoise(std::vector<double>& shift_serie);
};
