#include <iostream>
#include <numeric>
#include <valarray>
#include <complex>
#include <vector>
#include "slice_iter.h"

using std::numeric_limits;
using std::valarray;
using std::slice;
using std::slice_array;
using std::gslice;
using std::complex;
using std::cout;
using std::endl;

void show_limits(){
	
	// Примеры использования
	
	if (numeric_limits<unsigned char>::digits != 8){
		cout << "You have so strange bytes (!= 8 bit)\n";
	}

	int i = 65535;
	if (i < numeric_limits<unsigned char>::min() || numeric_limits<unsigned char>::max() < i ){
		cout << "i too long for short type\n";
	}

	// Проверка double epsilon
	double d = 1.0;
	if (d > 0 && d < numeric_limits<double>::epsilon()){
		d = 0.0;
	}

	if (numeric_limits<long>::is_specialized){
		cout << "long type has numeric_limits specialization\n";
	}

	if (numeric_limits<long>::is_signed && numeric_limits<long>::is_integer){
		cout << "long is integer and signed type\n";
	}
	// также содержит много описателей для double (nas_NaN, has_infinity, ...)
}

double my_square(double d){
	return d*d;
}

void show_valarray(){
	// valarray - это вектор, более оптимизированный для численных операций
	// расчитан на относительно низкоуровневые вычисления

	// Конструирование
	// по умолчанию
	valarray<double> v0;

	// 1000 элементов
	valarray<double> v1(1000);

	// в отличие от вектора, значение элемента идет первым
	valarray<double> v2(1.9, 1000);

	// из массива
	double arr[] = { 1.0, 2.0, 3.0, 1.5, 1.9 };
	valarray<double> v3(arr, 5); // проверок границ нет!

	// Присваивание - проверок границ нет!
	valarray<double> v4(5);
	v4 = v3;

	// присваивание всем элементам
	v2 = 1.0;

	// Доступ (без проверки)
	double d = v2[0];

	// Операции
	v3 *= 2.0; // *= применяется к каждому элементу (как и все ей подобные)

	// аккумулированная сумма и прочие
	double s = v4.sum();
	double m = v4.max();
	m = v4.min();

	// сдвиг элементов влево на 2
	valarray<double> v5 = v4.shift(2);
	
	// циклический сдвиг элементов вправо на 3
	valarray<double> v6 = v4.cshift(-3);

	// в обоих случаях порождается новый вектор

	// изменить размер можно только вручную
	// данные при этом теряются
	v6.resize(10);

	// применение функции к каждому элементу
	// порождается новый вектор
	valarray<double> v7 = v5.apply(my_square);

	// все арифметические операции и функции работают поэлементно
	// и порождают новый вектор
	valarray<double> v8 = v3 * v4;
	valarray<double> v9 = cos(v8);
}

void show_slices(){
	
	// slice - это срез каждых n-ых элементов вектора
	// применяется для моделирования матриц 
	// матрица 3*4
	valarray<int> foo(12);
	for (int i = 0; i < 12; ++i) 
		foo[i] = i;
	// 0 4 8
	// 1 5 9
	// 2 6 10
	// 3 7 11

	// 1 строка матрицы
	slice s = std::slice(0, 3, 4);
	cout << "slice with start = " << s.start()
		<< " stride = " << s.stride()
		<< " size = " << s.size() << endl;
	valarray<int> bar1 = foo[s];

	// выведем ее по срезам (строкам)
	cout << "Matrix: " << endl;
	for (size_t i = 0; i < 4; ++i){
		// начало - i элемент, шаг 4, размер 3
		slice s = std::slice(i, 3, 4);

		// получим срез вектора, или 1-ю строку матрицы
		valarray<int> bar = foo[s];
		for (size_t n = 0; n < bar.size(); n++)
			cout << bar[n] << ' ';
		cout << endl;
	}
	cout << endl;

}

void show_slices_iterator(){
	// на основе срезов можно реализовать итерирование по матрице
	// см. Страуструп 3, 738
	
	// вектор-матрица из предыдущего примера
	valarray<int> foo(12);
	for (int i = 0; i < 12; ++i)
		foo[i] = i;
	
	// срез для итерации
	slice s = std::slice(0, 3, 4);

	slice_iter<int> s_iter(foo, s);
	
	++s_iter;
	double d = (*s_iter);
	
	++s_iter;
	d = (*s_iter);

	// можно также, чтобы возвращал слайсы, а не элементы
}

void show_slice_array(){

	// slice_array позволяет представить некое подмножество элементов массива

	double arr[] = { 1.0, 2.0, 3.0, 1.5, 1.9 };
	valarray<double> v(arr, 5);
	size_t half_size = v.size() / 2;

	// представим четные и нечетные элементы
	slice_array<double>& v_even = v[slice(0, half_size + 1, 2)];
	slice_array<double>& v_odd = v[slice(0, half_size, 2)];

	// На основе slice_array и valarray можно смоделировать полноценную матрицу
	// см. Страус 3 740
	// В матрицах могут оказаться полезными отложенные вычисления
	// см. Страус 3 742, пример с матрицами из книги Джосаттиса
}

void show_gslice(){
	// обобщенный срез может представить часть вектора,
	// которую нельзя представить обычным срезом
	// например, угловая матрица

	// поэтому gslice содержит n шагов и размеров

	// вектор-матрица из предыдущего примера
	valarray<int> foo(12);
	for (int i = 0; i < 12; ++i)
		foo[i] = i;
	// 0 4 8
	// 1 5 9
	// 2 6 10
	// 3 7 11

	size_t len[] = { 2, 3 };
	size_t str[] = { 4, 1 };
	
	// 2-4 описывает строку (описывает одну строку чисел!)
	// 3-1 описывает столбец (описывыает столбец строк!)

	valarray<size_t> lengths(len, 2);
	valarray<size_t> strides(str, 2);

	// gslice с двумя парами описывает двумерный подмассив, с 3 - трехмерный и т.д.
	gslice gs(0, lengths, strides);

	valarray<int> bar = foo[gs];
	// 0 4
	// 1 5
	// 2 6
}

void show_valarray_mask(){

	double arr[] = { 1.0, 2.0, 3.0, 1.5, 1.9 };
	valarray<double> v(arr, 5);

	// mask_array создает битовую маску для вектора
	bool b[] = { true, false, true, false, true };
	valarray<bool> mask(b, 5);

	// выполнение операций над элементами по маске
	// только где true!
	valarray<double> v1 = v[mask];
}

void show_indirect_array(){

	// indirect_array создает произвольные подмножества и переупорядочивает элементы
	double arr[] = { 1.0, 2.0, 3.0, 1.5, 1.9 };
	valarray<double> v(arr, 5);

	// создадим indirect_array для переупорядочивания элементов
	size_t s[] = { 4,3,2,1,0 };
	valarray<size_t> mask(s, 5);

	// взятие логарифма в обратном порядке
	valarray<double> v1 = log(valarray<double>(v[mask]));
}

void show_complex(){

	// комплексные числа
	// содержат несколько специализаций (float, double, long double)
	// для возможных оптимизаций
	
	complex<double> c1(1.0, 2.0);
	complex<double> c2(1.0, 2.0);

	// классические операции
	complex<double> c3 = c1 + c1;
	complex<double> c4 = c1 * c1;

	// вывод
	cout << "c1 = " << c1 << endl;
	cout << "c2 = " << c2 << endl;
	cout << "c3 = " << c3 << endl;
	cout << "c4 = " << c4 << endl;

	// координаты
	double r = c1.real();
	double im = c1.imag();

	// сопряженное
	complex<double> c5 = conj(c1);
	cout << "c5 = " << c4 << endl;

	// полярные координаты (rho, theta)
	// real = rho * cos(theta);
	// imag = rho * sin(theta);
	complex<double> c6 = std::polar(2.0, 0.5);
	cout << "c6 = " << c6 << endl;
	double rho_ = abs(c6);
	double theta_ = arg(c6);
	double norm_ = norm(c6);
	
	// complex overload (does not work?)
	//double sin_ = std::sin(c6);

}

void show_algorithms(){

	// накопление
	std::vector<int> v1 = { 0, 1, 2, 3, 4, 5, 5, 4, 3, 2, 1, 0 };
	int acc_sum = std::accumulate(v1.begin(), v1.end(), 0);
	int acc_mul = std::accumulate(v1.begin(), v1.end(), 1, std::multiplies<int>());
	cout << "Sum accumulated = " << acc_sum << " product accumulated = " << acc_mul << endl;

	// накопление из двух последовательностей
	std::vector<int> v2 = { 5, 4, 3, 2, 1, 1, 2, 3, 4, 5, 0, 0 };
	acc_sum = std::inner_product(v1.begin(), v1.end(), v2.begin(), 0);
	acc_mul = std::inner_product(v1.begin(), v1.end(), v2.begin(), 
		1, std::multiplies<int>(), std::multiplies<int>()); // 1 op1 (v1 op2 v2)
	cout << "Sum accumulated = " << acc_sum << " product accumulated = " << acc_mul << endl;

	// инкрементное изменение
	
	// adjacent_difference : a, b-a, c-b, d-c, ...
	std::vector<int> res1;
	std::adjacent_difference(v1.begin(), v1.end(), std::back_inserter(res1));
	
	// partial_sum : a, a+b, a+b+c, ...
	std::vector<int> res2;
	std::partial_sum(v1.begin(), v1.end(), std::back_inserter(res2));
}

int main(){
	show_limits();
	show_valarray();
	show_slices();
	show_slices_iterator();
	show_gslice();
	show_valarray_mask();
	show_indirect_array();
	show_complex();
	show_algorithms();
	return 0;
}