#pragma once

// »сходный шаблон дл€ возведени€ числа N в степень POW
template <int N, int POW>
struct Pow{
	enum {result = N * Pow<N, POW-1>::result };
};

// „астична€ специализаци€ дл€ завершени€ рекурсии
template <int N>
struct Pow<N, 0>{
	enum {result = 1 };
};
