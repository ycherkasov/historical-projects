#include <cfenv>
#include <cmath>
#include <iostream>
#include "bit_operations.h"

using std::cout;
using std::endl;

// Switch on access to floating-point environment
//http://www.cplusplus.com/reference/cfenv/FENV_ACCESS/
#pragma STDC FENV_ACCESS ON

void show_fp_coltrol_noexcept(){

	double d1 = 0., d2 = 0., d3 = 0., s = 0.;
	int res = 0;

	printf("Floating point state flags:\n");
	printf(" FE_DIVBYZERO: %x\n", FE_DIVBYZERO);
	printf(" FE_INEXACT: %x\n", FE_INEXACT);
	printf(" FE_INVALID: %x\n", FE_INVALID);
	printf(" FE_OVERFLOW: %x\n", FE_OVERFLOW);
	printf(" FE_UNDERFLOW: %x\n", FE_UNDERFLOW);

	printf("Floating point rounding flags\n");
	printf(" FE_DOWNWARD: %x\n", FE_DOWNWARD);
	printf(" FE_TONEAREST: %x\n", FE_TONEAREST);
	printf(" FE_TOWARDZERO: %x\n", FE_TOWARDZERO);
	printf(" FE_UPWARD: %x\n", FE_UPWARD);

	printf("Current exceptions flags: %x\n", fetestexcept(FE_ALL_EXCEPT));

	printf("Current rounding mode: %x\n", fegetround());

	// флаги исключений очищены
	// далее они будут устанавливаться по мере накопления ошибок
	feclearexcept(FE_ALL_EXCEPT);

	/* Вызовем ситуацию исчезновения порядка */
	d1 = 1.0;
	do {
		d1 /= 2.0;
	} while ((res = fetestexcept(FE_ALL_EXCEPT)) == 0);
	printf("Inexact/Underflow Exceptions: %x\n", res);
	printf("2^-inf: %g\n", d1);

	feclearexcept(res);

	/* Вызовем ситуацию переполнения */
	d2 = 1.0;
	do {
		d2 *= 2.0;
	} while ((res = fetestexcept(FE_ALL_EXCEPT)) == 0);
	printf("Inexact/Overflow Exceptions: %x\n", res);
	printf("2^+inf: %g\n", d2);

	feclearexcept(res);

	/* Вызовем ситуацию деления на нуль */
	d3 = 1 / d1;
	res = fetestexcept(FE_ALL_EXCEPT);
	printf("Zero Div Exceptions: %x\n", res);
	printf("1/0: %g\n", d3);

	feclearexcept(res);

	/* Пример того, как может возникать потеря точности */
	s = 1;
	do {
		s = (s + 2 / s) * 0.5;
	} while ((s * s - 2) > 0);
	printf("Inexact Exceptions: %x\n", fetestexcept(FE_ALL_EXCEPT));
	printf("sqrt (2): %g\n", s);
}

void show_fp_coltrol(){

	/*
	The floating-point environment maintains a series of status flags and specific control modes.
	Specific about the contents of the floating-point environment depend on the implementation,
	but the status flags generally include the floating-point exceptions and their associated information,
	and the control modes include at least the rounding direction.
	*/

	// FE_* macro - floating point exceptions and rounding rules

	// 1. State (exception) flags
	//FE_DIVBYZERO Pole error exception(macro)
	//FE_INEXACT Inexact result exception(macro)
	//FE_INVALID Invalid argument exception(macro)
	//FE_OVERFLOW Overflow range error exception(macro)
	//FE_UNDERFLOW Underflow range error exception(macro)
	//FE_ALL_EXCEPT All exceptions(macro)

	// 2. Rounding rules
	//FE_DOWNWARD Downward rounding direction mode
	//FE_TONEAREST To - nearest rounding direction mode(macro)
	//FE_TOWARDZERO Toward - zero rounding direction mode(macro)
	//FE_UPWARD Upward rounding direction mode(macro)

	int ret = 0;

	// http://www.intuit.ru/studies/courses/53/53/lecture/1585?page=9
	// http://ru.wikipedia.org/wiki/Fenv.h
	// http://www.cplusplus.com/reference/cfenv/

	// Флаг состояния вещественной арифметики – это системная переменная, 
	// значение которой устанавливается(но никогда не очищается) при возбуждении 
	// исключительной ситуации и содержит дополнительную информацию о случившемся исключении

	// default FP environment (system-dependent!)
	fenv_t fenv = *FE_DFL_ENV;

	// получить текущее окружение FP
	ret = fegetenv(&fenv);
	cout << "fegetenv() returned " << ret << endl; // 0 is no errors

	// pair function setting fenv_t
	ret = fesetenv(&fenv);
	cout << "fesetenv() returned " << ret << endl; // 0 is no errors

	// Attempts to clear the floating-point exceptions specified
	feclearexcept(FE_ALL_EXCEPT);
	ret = fegetenv(&fenv);

	// feholdexcept() saves exception flags clears 
	double x = sin(0.0); // = 0
	fenv_t fe;
	feholdexcept(&fe);
	x = log(x); // should raise exception, but fenv is clear

	// Функция feupdateenv() выполняет еще более сложные действия. 
	// Она сохраняет в своей локальной памяти информацию о текущей исключительной ситуации, 
	// устанавливает новую среду по аргументу fenvp и затем пытается возбудить в ней сохраненное исключение.
	// Подобные манипуляции полезны, когда массовые вычисления производятся в безостановочном режиме, 
	// а затем режим меняется и обрабатывается все то нехорошее, что накопилось за это время.
	feupdateenv(&fe);
	cout << "log 0 = " << x << endl;
	if (!fetestexcept(FE_ALL_EXCEPT))
		cout << "no exceptions raised" << endl;
}
