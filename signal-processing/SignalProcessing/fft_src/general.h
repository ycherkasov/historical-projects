#if 0

	������� � ���������� ������ ����������.

#endif



#ifndef GENERAL_H_INCLUDED_
#define GENERAL_H_INCLUDED_

#include <ctime>

using namespace std;

#define CACHE_HALF 65536

#define CONST_PI      3.1415926535897932384626433832
#define CONST_SQRT_2  0.7071067811865475244008443621
#define CONST_SQRT2   1.4142135623730950488016887242

#define max(x,y) ((x) > (y) ? (x) : (y))
#define min(x,y) ((x) < (y) ? (x) : (y))

typedef double real;
typedef unsigned long ulong;
typedef unsigned short ushort;
class Complex;

void RealFFT(real* ddata, ulong Len, int Dir = 1);
void FFT_T(Complex* Data, ulong Len, int Dir = 1);

#endif
