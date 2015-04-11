#pragma once
#include "if_then_else.h"

// »сходный шаблон дл€ вз€ти€ корн€
//  орень целого числа ищетс€ двоичным поиском и пр€мым сравнением
// „исла, не имеющие целого корн€, округл€ютс€ до корн€ меньшего числа
// ƒл€ примера N=16 пор€док вычислений следующий
// Sqrt<16, 1, 16>
// Sqrt<16, 1, 8>
// Sqrt<16, 1, 4>
// Sqrt<16, 3, 4>
// Sqrt<16, 4, 4> => sqrt(16) = 4

template <int N, int L0 = 1, int HI = N>
struct Sqrt{
	enum { mid = (L0+HI+1)/2 };

	typedef typename 
		IfThenElse< 
			N<(mid*mid), 
			Sqrt<N, L0, mid-1>, 
			Sqrt<N, mid, HI> 
		>::Result CalculatedT;

	enum { result = CalculatedT::result };
};

// „астична€ специализаци€ дл€ завершени€ рекурсии
// ¬ерхний и нижний интервалы совпадают - получен результат
template <int N, int M>
struct Sqrt<N, M, M>{
	enum {result = M };
};
