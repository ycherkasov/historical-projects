#define _USE_MATH_DEFINES
#include <iostream>
#include <iomanip>
#include <bitset>
#include <cmath>

//This header also defines the following macro constants :
//MATH_ERRNO
//MATH_ERREXCEPT	
//Bitmask value with the possible values math_errhandling can take.
//
//FP_FAST_FMA
//FP_FAST_FMAF
//FP_FAST_FMAL	
//Each, if defined, identifies for which type fma is at least as efficient as x*y + z.
//
//FP_INFINITE
//FP_NAN
//FP_NORMAL
//FP_SUBNORMAL
//FP_ZERO	
//The possible values returned by fpclassify.
//
//FP_ILOGB0
//FP_ILOGBNAN	
//Special values the ilogb function may return.

#include <cfenv>

// for atan2
#include <valarray>

#include "bit_operations.h"

using namespace std;



/*
http://en.wikipedia.org/wiki/Floating_point

����� � ��������� ������� ������� ��:
����� ��������(������������ �� ��������������� ��� ��������������� �����)
��������(���������� �������� ����� ��� ����� �������) (Significand)
����� �������
�������(����������� ������� ��������� �����, �� ������� ���������� ��������) (Exponint)

���������� ������ ����� � ��������� ������� ���������� ����� �����, � ������� �������� 
(��� ����� �����) � ���������� ������� ��������� �� ������������� [0; 1). 
����� ����� ������ ����� ����������: ��������� ����� ������������ ������������

����� ��������� �������� (float)
������� 8 ��� (-127 .. 128)
�������� 23 ���

����� ������� ��������
������� 11 ��� (-1024 .. 1024)
�������� 52 ���

�� 64 �����, �������� ���������� 1 ��� ���� + 52 ����, ���������� � 1 ��� ���� + 10 ���.
����� ������� �������� �������� �������� �������� 
�� 4.94 * 10^-324 �� 1.79 * 10^308 (�� 2^-52 * 2^-1024 �� 1*2^1024)

� ������� �� ����� � ������������� �������, ����� �����, 
������� �������� ���������� ���������� � ��������� �������, ������������: 
��� ����� ������ ��� ����� � ������ ��������� � ����� ������ � ��� ����� � �������� ���������.
�.�. ����������� ������ �� ������������ �����, � ������� ����� �����

�������� ������� ���������� ���������� ������������� �����, ��� (1 +/- Epsilon) != 1
DBL_EPSILON ����������� ���������� �������� ����� 10^-16

FP ����� ����� ��������� ����������� �������� +/- INF � NaN (Not a number)
Two kinds of NaN: a quiet NaN (qNaN) and a signaling NaN (sNaN)

������������� �������� ���������:
32 ���: 7 �����	 e +/- 96
64 ���: 16 ����� e +/- 384

������� ������������ �������, ��� Maxima ��� Mathematica, ����� "�����" ������ �������������
�������� ��������������� ����� (e, pi, sqrt(2), sqrt(3),...)
*/

/*
http://habrahabr.ru/post/112953/
s E        M
1 11111111 11111111111111111111111
(-1)^s * M * B^E, ��� s - ����, B - ���������, E - �������, M - ��������
B=2
� ����� ��������� IEE754-2008 ����� ����� � ���������� 2 ������������ ����� � ���������� 10, 
��� ���������� ���������� (decimal) ����� � ��������� �������

�.�. ��������������� ����� ����� ��������� ���:
(-1)^s * 1.M * 2^E
����������� ������� ������������ ��� E-127
*/

void print_double_binary(double d1){
	
	long long* double_hack = reinterpret_cast<long long*>(&d1);
	static_assert(sizeof(d1) == sizeof(*double_hack), "Double and long long should have equal size");

	cout << "Double representation " << d1 << " has size " << sizeof(d1) << '\n';
	cout << "Long long" << std::hex << *double_hack << " has size " << sizeof(*double_hack) << '\n';
	cout << "Binary representation of " << *double_hack
		<< " is " << bitset<sizeof(double)*8>(*double_hack) << '\n';
}

void print_float_binary(float f1){
	long* float_hack = reinterpret_cast<long*>(&f1);
	static_assert(sizeof(f1) == sizeof(*float_hack), "Float and long should have equal size");

	cout << "Float representation " << f1 << " has size " << sizeof(f1) << '\n';
	cout << "Long " << std::hex << *float_hack << " has size " << sizeof(*float_hack) << '\n';
	cout << "Binary representation of " << *float_hack
		<< " is " << bitset<sizeof(float)* 8>(*float_hack) << '\n';
}

// ������� ������� �� ������� - 15. 
// �� ���� ����� �������� ��������� �������� ������� ����� ������� �� ���� �����.
// ����� ����� �������� �� ������� 2^(b-1) - 1, 
// ��� b - ����� ���, ���������� �� �������� ������� 
// (� ������ ����� ��������� �������� b=8, ������� b=11).

//����� �� ���� �� 2048 ���������� ��� ����.
//����� �� 2049 �� 4096 ����������� � ���������� ������� ������.
//����� �� 4097 �� 8192 ����������� �� ���������� ������, ���������� ������ �� 4.
//����� �� 8193 �� 16384 ����������� �� ���������� ������, ���������� �� 8.
//����� �� 16385 �� 32768 ����������� �� ���������� ������, ���������� �� 16.
//����� �� 32769 �� 65535 ����������� �� ���������� ������, ���������� �� 32.


void show_float(){
	// ����� ��������� ������ 1.!
	// ����������� ������� ������������ ��� E-127!

	//       s E        M
	// 1.0 = 0 01111111 00000000000000000000000
	// E = 01111111 = 127 - 127 = 0
	// 1.0 = (-1)^s * 1.M * 2^E = (1-)^0 * 1.000 * 2^0 = 1.0
	print_float_binary(1.0);

	//       s E        M
	// 1.5 = 0 01111111 10000000000000000000000
	// E = 01111111 = 127 - 127 = 0
	// 1.1(2) = 2^0 + 2^(-1) = 1 + 1/2 = 1.5
	// 1.5 = (-1)^s * 1.M * 2^E = (1-)^0 * 1.1(2) * 2^0 = 1.5
	print_float_binary(1.5);

	// 0.15 = 0 01111100 00110011001100110011010
	// E = 01111100 = 124-127 = -3
	// 1.00110011001100110011010 = 2^(-3) + 0^(-4) + 0^(-5) + 2^(-5) + 2^(-6) + ...
	// 0.15 = (-1)^s * 1.M * 2^E = (1-)^0 * 1.00110011001100110011010(2) * 2^(-3) = 1.5
	print_float_binary(0.15);
}


void show_double(){

	// 1.0 = 
	print_double_binary(1.0);

	// changed sign bit
	// -1.0 = 
	print_double_binary(-1.0);

	// 1.5 = 
	print_double_binary(1.5);

	// 2.0 = 
	print_double_binary(2.0);


	// 
	print_double_binary(1000000000.0);

	// 
	print_double_binary(1500000000.0);

	// 
	print_double_binary(2000000000.0);

}


void show_floating_point(){
	show_float();
	//show_double();
}

void print_roundings_header(){
	cout 
		<< "val" << '\t'
		<< "ceil" << '\t'
		<< "floor" << '\t'
		<< "trunc" << '\t'
		<< "round" << '\t'
		<< "lround" << '\t'
		<< "rint" << '\t'
		<< "lrint" << '\t'
		<< "nearbyint" << '\n';
}


void print_roundings(double val){
	cout << fixed;
	cout << setprecision(4);
	cout 
		<< val << '\t'
		<< ceil(val) << '\t'
		<< floor(val) << '\t'
		<< trunc(val) << '\t'
		<< round(val) << '\t'
		<< lround(val) << '\t'
		<< rint(val) << '\t'
		<< lrint(val) << '\t'
		<< nearbyint(val) << '\n';
}

void print_fpclassify(double val){
	int val_type = fpclassify(val);
	cout << val << " is ";
	switch (val_type)
	{
	case FP_INFINITE:  cout << "infinite" << endl;  break;
	case FP_NAN:       cout << "NaN" << endl;       break;
	case FP_ZERO:      cout << "zero" << endl;      break;
	case FP_SUBNORMAL: cout << "subnormal" << endl; break;
	case FP_NORMAL:    cout << "normal";

	if (signbit(val))
	{
		cout << " negative" << endl;
	}
	else
	{
		cout << " positive or unsigned" << endl;
	}
	break;
	default:
		cout << "Error! Should not be here!" << endl;
	}
}

void show_cmath_fpoint_operations(){

	//
	// Trigonometric
	double pi = M_PI;

	double c1 = cos(pi);
	cout << "cos PI = " << c1 << '\n';

	double s1 = sin(pi); // close to 0, but not a 0
	cout << "sin PI = " << s1 << '\n';

	double t1 = tan(pi);
	cout << "tan PI = " << t1 << '\n';

	double ac1 = acos(c1);
	cout << "acos c1 = " << ac1 << '\n';

	double as1 = asin(s1);
	cout << "asin s1 = " << as1 << '\n';

	double at1 = atan(t1);
	cout << "atan t1 = " << at1 << '\n';

	double at2 = atan2( 1.0, 2.0 );
	cout << "atan2 1/2 = " << at2 << '\n';

	// Hyperbolic supported as well

	// Exponential
	double param = 5.0;

	double result = exp(param);
	cout << "exp (" << param << ") = " << result << "\n";

	// Breaks the floating point number x into its binary significand
	// (a floating point value between 0.5(included)and 1.0(excluded)) 
	// and an integral exponent for 2, such that :
	// x = significand * 2^exponent 
	int n = 0;
	param = 8.0;
	result = frexp(param, &n);
	cout << param << " = " << result << "*2^" << n << '\n';

	// Compose value back
	double back = ldexp(result, n);
	cout << back << " = " << result << "*2^" << n << '\n';

	// ln and lg are calculated as well

	// modf() splits value
	double fractpart = 0.0;
	double intpart = 0.0;
	fractpart = modf(pi, &intpart);
	cout << pi << " = " << intpart << " + " << fractpart << '\n';

	// exp2() returns the base-2 exponential function of x
	// which is 2 raised to the power x
	param = 8.0;
	double e2 = exp2(param);
	cout << "2 ^ " << param << " = " << e2 << '\n';

	// expm1() returns e raised to the power x minus one : e^x - 1
	cout << "expm1(1.0) = e^x - 1 = " << expm1(1.0) << '\n';

	// logb() returns the integral part of the logarithm of |x|, using FLT_RADIX as base
	// log1p() returns the natural logarithm of one plus x (log(1+x))
	// log2() returns the binary(base - 2) logarithm of x
	cout << "log(10.0) = " << log(10.0) << '\n';
	cout << "logb(10.0) = " << ilogb(10.0) << '\n';
	cout << "log1p(10.0) = log(1+x) = " << log1p(10.0) << '\n';
	cout << "log2(1024) = " << log2(1024) << '\n';

	// scalbn(x, n) = x * FLT_RADIX^n
	// scalbln(x, n) = ?
	double x = 2.0;
	int n1 = 4;
	cout << "FLT_RADIX = " << FLT_RADIX << '\n';
	cout << "x = " << x << '\n';
	cout << "n1 = " << n1 << '\n';
	cout << "scalbn(x, n) = x * FLT_RADIX^n = " << scalbn(x, n1) << '\n';
	cout << "scalbln(x, n) (?)= " << scalbln(x, n1) << '\n';

	// Roundings
	print_roundings_header();
	print_roundings(0.0);
	print_roundings(0.3);
	print_roundings(0.5);
	print_roundings(0.8);
	print_roundings(1.3);
	print_roundings(2.3);
	print_roundings(2.5);
	print_roundings(2.8);

	// Power functions
	cout << "sqrt(2.0) = " << sqrt(2.0) << '\n';
	cout << "cbrt(2.0) = " << cbrt(2.0) << '\n';

	// hypot() returns the hypotenuse of a right - angled triangle whose legs are x and y
	cout << "Hypotenuse of (3, 4) =" << hypot(3, 4) << '\n';

	// Error function/Gamma function (probability, statistics, PDE)
	// http://en.wikipedia.org/wiki/Error_function
	// http://en.wikipedia.org/wiki/Gamma_function
	cout << "Error function of 1 =" << erf(1.0) << '\n';
	cout << "Complimentary error function of 1 =" << erfc(1.0) << '\n';
	cout << "Gamma function of 1 =" << tgamma(1.0) << '\n';
	cout << "Log of Gamma function of 1 =" << tgammal(1.0) << '\n';

	// copysign copies sign of second value to the first
	cout << "copysign(-10.0, 1.0)" << copysign(-10.0, 1.0) << '\n';
	cout << "copysign(-10.0, -1.0)" << copysign(-10.0, -1.0) << '\n';
	cout << "copysign(10.0, -1.0)" << copysign(10.0, -1.0) << '\n';

	// nextafter() returns the next representable value after x in the direction of y
	cout << "Next representable value after 0 = " << nextafter(0.0, 1.0) << '\n';
	cout << "Next representable value before 0 = " << nextafter(0.0, -1.0) << '\n';
	// nexttoward() the same with long y
	
	// NAN (not a number)
	// he NaN values are used to identify undefined or non-representable values 
	// for floating-point elements, such as the square root of negative numbers or the result of 0/0
	// Generate quiet NAN
	double nn = nan("");
	float nf = nanf("");
	cout << "For " << nn << " isnan(nn) = " << isnan(nn) << '\n';
	cout << "For " << nf << " isnan(nf) = " << isnan(nf) << '\n';
	cout << "For " << "isnan sqrt(-1.0) = " << isnan(sqrt(-1.0)) << '\n';
	// isnormal is opposite to isnan, but also checks for INF and 0
	cout << "For " << "isnormal sqrt(-1.0) = " << isnormal(sqrt(-1.0)) << '\n';

	// isunordered() check at least one of values is NAN
	if (isunordered(sqrt(-1.0), 0.0))
		cout << "sqrt(-1.0) and 0.0 cannot be ordered\n";

	double myinf = INFINITY;
	if ((1 / sin(0.0) == myinf) && (!isnormal(myinf)))
	{
		cout << "1/0 is " << myinf << '\n';
	}

	// Value is too large
	double huge = pow(10.0, 1000000000);
	if (huge == HUGE_VAL)
	{
		cout << huge << " is HUGE_VAL\n";
	}

	// fpclassify() returns a value of type int that matches one of the classification macro constants
	//FP_INFINITE	Positive or negative infinity(overflow)
	//FP_NAN	Not - A - Number
	//FP_ZERO	Value of zero
	//FP_SUBNORMAL	Sub - normal value(underflow)
	//FP_NORMAL	Normal value(none of the above)
	print_fpclassify( 1/sin(0.0) );
	print_fpclassify(sin(0.0) / sin(0.0));
	print_fpclassify( 0.0 );
	print_fpclassify(1.0);
	print_fpclassify(-1.0);

	// subnormal numbers could not be representet a normal double
	// (less than minimal double)
	// requires expanded representation, works slower (10-100 times)
	double subnorm = 1.0;
	while (fpclassify(subnorm) != FP_SUBNORMAL)
	{
		subnorm /= 2;
	}
	print_fpclassify(subnorm);

}

void show_fp_coltrol(){

	/*
	The floating-point environment maintains a series of status flags and specific control modes. 
	Specific about the contents of the floating-point environment depend on the implementation, 
	but the status flags generally include the floating-point exceptions and their associated information, 
	and the control modes include at least the rounding direction.
	*/

	// FE_* macro - floating point exceptions and rounding rules
	//FE_DIVBYZERO Pole error exception(macro)
	//FE_INEXACT Inexact result exception(macro)
	//FE_INVALID Invalid argument exception(macro)
	//FE_OVERFLOW Overflow range error exception(macro)
	//FE_UNDERFLOW Underflow range error exception(macro)
	//FE_ALL_EXCEPT All exceptions(macro)

	//FE_DOWNWARD Downward rounding direction mode
	//FE_TONEAREST To - nearest rounding direction mode(macro)
	//FE_TOWARDZERO Toward - zero rounding direction mode(macro)
	//FE_UPWARD Upward rounding direction mode(macro)

	// FP exception rules

	int ret = 0;

	// TODO: it seems fenv functions does not work
	// Check it on Linux!
	// http://www.intuit.ru/studies/courses/53/53/lecture/1585?page=9
	// http://ru.wikipedia.org/wiki/Fenv.h
	// http://www.cplusplus.com/reference/cfenv/
#if 0
	// env format depends on implementation
	fenv_t env;
	ret = fegetenv(&env);
	cout << "fegetenv() returned " << ret << ", CTL = " << env._Fe_ctl << ", STAT = " << env._Fe_stat << '\n';

	fexcept_t flag = 0;
	ret = fegetexceptflag(&flag, FE_ALL_EXCEPT);
	
	int mask = FE_ALL_EXCEPT;
	cout << "fegetexceptflag() returned " << ret << ", mask = " << mask << ", flag = " << flag << '\n';

#endif
}

/*
���. ��� ������� ��������� � ������� ��� ��������� ��������� ����� �� ���������.
��������� ������.���������� ��������� �����(��� �������� � �������� ���������� ���� float ��� double) 
������ ����� � ���� �����.
�� ���� �������� �������� ���� ��������� �����.
� �������� ����������� � ���, ��� ����� � ������������� ������ (����� � ��� ������� ������ 
����� � ������������� ������) ����� ���������� �������� �����������, � ������� �� ����� � ��������� ������, 
��� �������� ����������� ��������� � ������ ������������������ �� ������ �����.

����� ����� ��������� ������� ��������� � ���� ����� �����, � ������ � ����� ����������, 
�������� �� ��������� ������ ����� �� ���������.
��� ��, ��� � �� ��������� ��������� � ����������.
�� ��� �� ��������� � � ����� ������, ���� ������, �� ����������� �������� �� 0...1 � 0...1000000000.
�� ����, �� ����� ��������, ��������� ��� ����� ��� �����.
� ������������ ��������� ���������� �� �� ������� ��������� ������.
� ���� ��������� ���� ������������ ����� Epsilon.
� ��� ����� - �� � �������� ������ ��� ��������������. ��� ����� DBL_EPSILON ? 
� � ��� ��� ���.��� ����������� ��������, ������� ��� ����������� ��� � �������, 
������ �������� ���� �������.��������� ? � � �������! ������� �������, ����� 1.0 
� �� � ������ �������.������� ���������� ����� � ��������� ������ �� + / -DBL_EPSILON 
���������� ������������. ��� ��������� ������ ��� ������� ���������� � ������������� ������ ������.
����� ������ � ��������� ����� ������ 2.0 ��� DBL_EPSILON ������ �� �������.
�� ��� ��������� ��, ��� ��� ��� � ������ �� ������.
��� ���� ����� DBL_EPSILON �������� ������� ����� � ������ �� ����������. � �� �� �����, 
DBL_EPSILON ����� �������� ������� 1e-16.��� ��� ������ ? � ��� ������, ��� ����� � ��������� 
����������� ���������(���� 1e-34) � ����� ������ ���� DBL_EPSILON ����� ��� �����. �� ����, 
��� 1e-16 ���������� ������ � �������� �����. � ���� ���������� ������ ������ �� ���� �������� ����� 
� ��� ����� ���������� � ���� ��������� ����� � ��������� ������, ����� ���������� ������� ��������� 
�������� � ����� ������������� ����������� ������.
*/

// ��� ��� ���� �� ��� - ���� ����� ��� ����� DBL_EPSILON(�� ��� FLT_EPSILON) ? 
// ����� - �� ? � �����!���� ��������, ����� ������������� ���� ���������� ����� 
// � ����� ���������� ���������. � ����� ? � � ��� ��� ��� ��� � ������� �� 
// ����������� �������� ����� � �������� ����������. 
// ������ ������, ���� ��� Epsilon �������� �� �������� �����.
// � ��������� � ��� ��� �����, �� ��� ����������� � ����� �� ��� �����.
// �� ����, ���������� ��������� �������� ��� :
bool close_enough(double a, double b){
	if (fabs(a - b) <= DBL_EPSILON * fmax(fabs(a), fabs(b)))
	{
		return true;
	}
	return false;
}

/*
������ ? ��, ������, � ��� ��������� �����������, ����� ����. �� � ��� ���� �����������!
���� � ���, ��� ���� DBL_EPSILON ���������� ������� � 1 (����!) �������� ��� ���������� 
� ���������� � ����� 1.0.�� �������� ����� ������� �� ����������� � ����� ���� ������ �����, 
���� ����� ����������� ������ ��� �� ���� �������� ���. ������� ���� ����� ��� - �� ���� 
16 * DBL_EPSILON, ����� ������������� ������� � 4 ������� ���� 
(��� �������� ������� ��������� �������� ���������� ����� �� �������� 16 ���������).

������� ��, ���� ������, ����� �������� ����� ����� - ����� �������� � �����������.
������, 0...1000. � ���� ������, ��� ��������� �� ��������������� ��������� ����� ����� ���������, 
���� 1000 * 16 * DBL_EPSILON. 
�� ���� ����� � ����, ��� ����� ��������� ���������� ���������� ��� ���� ��������� ����� 
� ������������� �����(�����������, ������).

� ������ ������� ������ ���������� � ���� � ������ ��������� ���������� GPC by Alan Murta 
������������ ����� ��������� � ����������� Epsilon.
�� ���������� �������� ��������� ��� ��� �����, ��� ��������� �� ������� ���������, 
� �� 1e-20 �������� ������ ��������� ��������.
*/

/*
��������� ��� ����� ������ ��������. ������ � IEEE ��������� ����� ��� - � ������� ����� �������, 
����� ���������� �� ���������, � ����� ������� - ����. ��� ���� ������ � ���, ��� ����� ������� - 
����� ��� ��� ����� �����, ������� �������� ������ �������. ���� ��� ����� ��������������� ������ 
��� ��������� �������� - �������� �������� � �������� ������� ������ �� ������� ��������, 
������� ����� � ������� ����� ��, ��� � ���������� �, � ������ �������������� �����, 
������ ���������� �� ����. �� ������� ��������, ��, � �������� �������, ����� �������� ������ ��� ���.

��-������ ����� ��� ������, ����� ����� �������� ����� - ������ �������� ��� ����� ����������� 
� ������� ������������ � ������� ������� �������� ����� ������ ���� ���������� � ���� �� ��������� 
�� ���������� �������� �����. �� ���� ������� ������ �� �������, ���� ��������� ���������� � ������� 
����� ������ ����� �������� � ������� �� ��� ��� ����� ����� �� ������� ��������, ������� ����!
������� ������ � ���������� ������� : �������� � ��� ���� 5 �������� ������� � ���� ����� 36.72, 
������� ���������� ��� 3.67200�1. �������� �� ����� ������� ��� � 100000 ��� 1.00000�5. 
��� �������� ��� ��� ���������� 5 ������� ������ �� 5 - 1 = 4 � ��������� 0.0003672�5.
��������� � ��� ����� 5 �������� �� ��� ��� ��� ��������� ������ 0.00036�5 ��� 0.00037�5 
� ����������� �� ����, ����� �� ���������� ����������� �������� ��� ���. 
� ����� �������� ������� 1.00036�5, �����, � ������� ������� � ������� �������� 
����� ����� ����� ������ ��������� ���������.

� ������������� �� ������ ���� ����� �� �������, ������ ��� ����� �������� ���������� 
������� �� �������� ������� � ���������� ���������� � ��� ���������  �����! ��� �� ���� ? 
� ���������� ������ - �����������, ����� ���������� �� � 1�5, � 2�5 - ����� ����������� 
�� ������, � � �������� - ���� �������, ��������� ������ � ��� ��� ����� ������������ 1.1�� 
� ���������� ����� �������� ��������.�� ���� �������������� ��������� ����� ��� float 
� 23 ������ ������� ��� 1.1�1011 (� �������� �����, � ���������� - 1.5 * 2 ^ 23), 
��������� ������� �������� �� 127 ����� ��� ���������� ���(150 << 23) | (1 << 22).

����� ������ ������ �� ��������� ������� ������� �� ���������� � �����.
� ��� ���������, ��� � ������ �����, �������� ���� ��� ����� - ������������ double 
������ float � ������ ����� ������� ������� ����� ����������, � ������� ���� ���� �������.
���������� �� �� ����� � � ���� ����, ��� ���� ������������ �� ��������.
�������� ������� ���� ������ - �� ������� ��� ����� �������� ������ � �������������� �������.
������ ������� ��� ������� ������ ���� �� �������. � ������� ���� ���������� ������ ���������� 
������� ���� ����� ���������!

� ����� ����, ���� ������������� float �� � ��� �������� ������ ��� �������� ���������� :
���� - ������ 0, ���������� - ������ 150, � ��� ����� ������� ��� ������� �������� - 
� ������ �������������� ����� 1, � ������ �������������� 0. ���� ��������� ������� 22 ����, 
�� ����� ��������, ��� ��� ������������� ����� ��������� ����� ����� ������ ���������� �����, 
� ��� ������������� - ������ ��� ����� �� 1! �� ���� ���� ������ �� ���������� ��������� ����� ��� int
�� ������� 22 ���� �� ���������, � ������� ����� �������������� ����� ����� ���� ����, 
� � �������������� - ���� ��������, ��� ��� ��, ��� ��� ����.

���, �������, �� � ��� ��������, � ������� � ����� ������ ������ ������ ����� ������, 
�������� ��� ��� http ://www.d6.com/users/checker/pdfs/gdmfp.pdf 
�� �� �� � ������ ��� ���� �������� ��������� � �� ������ � �����, 
� � ����� ����� ����� � ������� �� ������ ��������� �����.

*/

int fast_float2int(float x)
{
	const int magic = (150 << 23) | (1 << 22);
	x += *(float*)&magic;
	return *(int*)&x - magic;
}

/*
������ ���������� ������� ������� _ftol, ������� ������ ������������� ��������� ���������� ��� ���������
(��� ������ �� �������������� ��� ����� ��������) ��� ����� ��������� ������� ����� ����������, 
������������� ����� ���������� �������������� ��������� �, ����������, ����� �������������� ������.
������ ��� ������� �������� ���� � ������� ��������� ��������� ������.
� ����� ���� ������ ���� � ���� ������ ���������(������� �� ����������� �����������) �� ����� ����������� � ����� ������.
*/
