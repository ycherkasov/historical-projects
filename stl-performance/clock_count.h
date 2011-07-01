/** @file clock_count.h
����� ����������� �������� �������� ������� � �������������
� ������������ �����.
*/

#ifndef _PERF_TEST_CLOCKCOUNT_
#define _PERF_TEST_CLOCKCOUNT_
#include <ctime>
#include <boost/timer.hpp>

/**@brief 
������������������ ������� �������� ������������ �����. */
double clock_ticks() {
	// ��������� ����������� �����
	int res[2];                              

#if defined (__GNUC__) && !defined(__INTEL_COMPILER)
	// ������-��������� AT&T

// x64
#if defined (_LP64)
	
	// ������������ (��������� rbx)
	__asm__ __volatile__  (              
		"xorl %%eax,%%eax \n push %%rbx \n cpuid \n"
		::: "%rax", "%rcx", "%rdx");
	
	// ��������� TSC, ��������� edx:eax � ������� res[2]
	__asm__ __volatile__  (               
		"rdtsc\n"
		: "=a" (res[0]), "=d" (res[1]) );
	
	// ��� ���� ������������
	__asm__ __volatile__  (               
		"xorl %%eax,%%eax \n cpuid \n pop %%rbx \n"
		::: "%rax", "%rcx", "%rdx");
// 32 bit (x86)
#else                      
	// ������������ ebx
	__asm__ __volatile__  (
		"xorl %%eax,%%eax \n pushl %%ebx \n cpuid \n"
		::: "%eax", "%ecx", "%edx");
	
	// ��������� TSC, ��������� edx:eax � ������� res[2]
	__asm__ __volatile__  (               
		"rdtsc\n"
		: "=a" (res[0]), "=d" (res[1]) );
	
	// ��� ���� ������������
	__asm__ __volatile__  (
		"xorl %%eax,%%eax \n cpuid \n popl %%ebx \n"
		::: "%eax", "%ecx", "%edx");
#endif //(_LP64) 

#else //(__GNUC__)
	// ������-��������� MASM 
	__asm {
		xor eax, eax
			// ������������
			cpuid                              
			
			// ������� TSC
			rdtsc                              
			
			// ��������� ������� ������� ����� � res[0]
			mov dword ptr res, eax             

			// ��������� ������� ������� ����� � res[1]
			mov dword ptr res+4, edx
			
			// ��������� ������������
			xor eax, eax
			cpuid
	};
#endif   // __GNUC__

	return *(double*)res;
}

/**@brief 
�����, ��������������� ��� �������� ������������ �����
�� ����� ������ ������������� */
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

		// ������� ��������� ������� (�� ������� ����� ������)
		for (int i=0; i < numtests; i++) {
			before = clock_ticks();
			clocks[i] = clock_ticks() - before;
		}

		// ������ ������� ��������� �������
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
����������� �� boost ������ - ������������� ����� */
class raiitimer : public boost::timer{
public:
	double restart_count() { 
		double e = elapsed();
		restart();
		return e;
	} 
};

/**@brief 
�����, ��������������� ��� �������� ����������� 
�� ����� ������ ������������� */
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