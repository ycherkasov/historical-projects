/** @file clock_count.h
Здесь реализованы средства подсчета времени в микросекундах
и процессорных тиках.
*/

#ifndef _PERF_TEST_CLOCKCOUNT_
#define _PERF_TEST_CLOCKCOUNT_
#include <ctime>
#include <boost/timer.hpp>

/**@brief 
Кроссплатформенная функция подсчета процессорных тиков. */
double clock_ticks() {
	// результат сохраняется здесь
	int res[2];                              

#if defined (__GNUC__) && !defined(__INTEL_COMPILER)
	// Инлайн-ассемблер AT&T

// x64
#if defined (_LP64)
	
	// сериализация (сохранить rbx)
	__asm__ __volatile__  (              
		"xorl %%eax,%%eax \n push %%rbx \n cpuid \n"
		::: "%rax", "%rcx", "%rdx");
	
	// прочитать TSC, сохранить edx:eax в массиве res[2]
	__asm__ __volatile__  (               
		"rdtsc\n"
		: "=a" (res[0]), "=d" (res[1]) );
	
	// еще одна сериализация
	__asm__ __volatile__  (               
		"xorl %%eax,%%eax \n cpuid \n pop %%rbx \n"
		::: "%rax", "%rcx", "%rdx");
// 32 bit (x86)
#else                      
	// сериализация ebx
	__asm__ __volatile__  (
		"xorl %%eax,%%eax \n pushl %%ebx \n cpuid \n"
		::: "%eax", "%ecx", "%edx");
	
	// прочитать TSC, сохранить edx:eax в массиве res[2]
	__asm__ __volatile__  (               
		"rdtsc\n"
		: "=a" (res[0]), "=d" (res[1]) );
	
	// еще одна сериализация
	__asm__ __volatile__  (
		"xorl %%eax,%%eax \n cpuid \n popl %%ebx \n"
		::: "%eax", "%ecx", "%edx");
#endif //(_LP64) 

#else //(__GNUC__)
	// Инлайн-ассемблер MASM 
	__asm {
		xor eax, eax
			// сериализация
			cpuid                              
			
			// считать TSC
			rdtsc                              
			
			// сохранить младшее двойное слово в res[0]
			mov dword ptr res, eax             

			// сохранить старшее двойное слово в res[1]
			mov dword ptr res+4, edx
			
			// повторная сериализация
			xor eax, eax
			cpuid
	};
#endif   // __GNUC__

	return *(double*)res;
}

/**@brief 
Класс, предназначенный для подсчета процессорных тиков
за время своего существования */
class clock_count {
public:
	clock_count(){
		_clock_count = clock_ticks();
	}
	~clock_count(){
		_clock_count = clock_ticks() - _clock_count - _overhead;
	}

	static void calibrate_overhead(){

		double before = 0, overhead = 0;
		const int numtests = 1000000;
		double clocks[numtests] = {};

		// измерим накладные расходы (не измеряя более ничего)
		for (int i=0; i < numtests; i++) {
			before = clock_ticks();
			clocks[i] = clock_ticks() - before;
		}

		// найдем средние накладные расходы
		overhead = clocks[0];
		for (int i=0; i < numtests; i++) {
			overhead = (overhead + clocks[i])/i;
		}
		_overhead = overhead;
	}
	
	static double get_last_count(){
		return _clock_count;
	}

private:
	static double _clock_count;
	static double _overhead;
};

/**@brief 
Наследуемый от boost таймер - переопределен сброс */
class raiitimer : public boost::timer{
public:
	double restart_count() { 
		double e = elapsed();
		restart();
		return e;
	} 
};

/**@brief 
Класс, предназначенный для подсчета миллисекунд 
за время своего существования */
class msec_count {
public:
	msec_count(){
		_clock_count = t.restart_count();
	}
	~msec_count(){
		_clock_count = t.elapsed();
	}

	static double get_last_count(){
		return _clock_count;
	}

private:
	static double _clock_count;
	raiitimer t;
};

double clock_count::_clock_count = 0.;
double clock_count::_overhead = 0.;
double msec_count::_clock_count = 0.;

#endif //_PERF_TEST_CLOCKCOUNT_