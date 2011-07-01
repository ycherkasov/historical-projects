/** @file randint.h
Здесь определены генераторы случайных данных.
*/


#ifndef _PERF_TEST_RNDINT_
#define _PERF_TEST_RNDINT_
#include <utility>

/**@brief 
Базовый класс для генератора */
class randint
{
public:
	randint(long s = 0) : _randx(s), _magic_double(2147483638.0){}
	
	long abs(long x){ return x&_magic_mask; }
	double max(){ return _magic_double; }
	long draw(){ return _randx = _randx * _magic_int1 + _magic_int2; }
	double fdraw(){ return abs(draw())/max(); }
	long operator()(){ return abs(draw()); }

private:
	unsigned long _randx;
	static const int _magic_mask = 0x7fffffff;
	static const int _magic_int1 = 1103515245;
	static const int _magic_int2 = 12345;
	const double _magic_double;
	
};

/**@brief 
Равномерное распределение */
class urand : public randint {
public:
	urand(long s = 0) : randint(s) { }
	long operator()(long n) { 
		long r = long(n*fdraw()); 
		return (r==n) ? n-1: r; 
	}
};

/**@brief 
Экспоненциальное распределение */
struct erand : public randint {
	int mean;
	erand(int m) { mean = m; };
	long operator()(long n) { 
		double d = (-mean * log( 1.0 - randint::fdraw() ) + 0.5);
		return static_cast<long>(d); }
};

/**@brief 
Генерация последовательных ключей */
class uniqueint{
public:
	uniqueint(int init) : _inintial(init){}
	int operator()(){return _inintial++;}
private:
	int _inintial;
};

/**@brief 
Генерация уникальных пар */
class randpair{
public:
	randpair(int init) : _inintial(init){}
	std::pair<int, int> operator()(){return std::make_pair(_inintial++,_inintial++);}
private:
	int _inintial;
};
#endif //_PERF_TEST_RNDINT_
