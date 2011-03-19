#include <vector>
#include <string>
#include <iostream>
#include <iterator>
#include <fstream>
#include <algorithm>
#include <complex>
#include <valarray>

using namespace std;

// --------------- ИТЕРАТОРЫ --------------- 
// Функция демонстрирует итератор,
// ссылающийся на поток вывода
void show_out_iter()
{
	// (строки, присвоенные итератору, выводятся на экран)
	// Поток в данном случае представляется как контейнер
	ostream_iterator<string> oi(cout);

	*oi = "aaa";
	++oi;
	*oi = " bbb\n";
}

// Функция демонстрирует итератор, ссылающийся на поток ввода
// (строки, присвоенные итератору, передаются в поток ввода с клавиатуры)
void show_istr_iter()
{
	// Инициализируем итератор потоком ввода
	istream_iterator<string> iinput_iter(cin);

	// запрашиваем первую строку
	string s1 = *iinput_iter;

	// запрашиваем вторую строку
	++iinput_iter;
	string s2 = *iinput_iter;

	// Теперь выведем результаты
	cout << "First string: " << s1 << '\n' << "Second string: " << s2 << endl;
}

// Функция читает файл (ввести test.txt), сортирует прочитанные слова
// удаляет дубликаты и записывает результат в другой файл (ввести test1.txt)
// Демонстрирует оба вида итераторов (ввода и вывода) и основные алгоритмы
int show_iterators()
{
	// Прочитать имена файлов
	ostream_iterator<string> out_iter(cout);

	// вывели первую строчку
	*out_iter = "Input file:";

	// Инициализируем итератор потоком ввода
	istream_iterator<string> iinput_iter(cin);
	string from = *iinput_iter;

	// вывели вторую строчку
	++out_iter;
	*out_iter = "Output file:";

	++iinput_iter;
	string to = *iinput_iter;

	// поток ввода связать с файлом источника
	ifstream from_file( from.c_str() );

	// Итератор, связанный с данным потоком
	// (поток как контейнер)
	istream_iterator<string> from_it_begin(from_file);

	// Конструктор итератора ввода по умолчанию создает указатель на конец ввода
	istream_iterator<string> from_it_end;

	// Инициализируем вектор начальным и конечным итератором
	vector<string> b(from_it_begin, from_it_end);

	// Отсортируем вектор
	sort(b.begin(), b.end());

	// поток вывода связать с файлом приемника
	ofstream to_file( to.c_str() );

	// Итератор, связанный с данным потоком
	ostream_iterator<string> to_it_begin(to_file, "\n");

	// Скопируем только уникальные элементы
	unique_copy( b.begin(), b.end(), to_it_begin );

	// Возвратить состояние ошибки
	return !from_file.eof() && !to_file;
}

// --------------- for_each --------------- 
// Функция для алгоритма 'foreach'
// принимает элемент соответствующего контейнера
void print_v(const string& s)
{
	cout << s << endl;
}

void show_foreach()
{
	vector<string> st;
	st.push_back("atatat");
	st.push_back("komatoza");
	for_each(st.begin(), st.end(),print_v);
}

// --------------- find_if --------------- 
// Поиск с предикатом
bool is_atatat(const string& s)
{
	return (s == "atatat");
}

void show_findif()
{
	vector<string> st;
	st.push_back("atatat");
	st.push_back("komatoza");
	vector<string>::const_iterator i = 
		find_if(st.begin(), st.end(), is_atatat);
}

// --------------- mem_fun --------------- 
// Вариант for_each - 
// Позволяет работать с контейнерами
// содеражащими полиморфные объекты


// --------------- math --------------- 
// Функция демонстрирует математические возможности STL
void show_math() {

	// Комплексные числа
	complex<double> c1(1. , 2. );
	complex<double> c2(1.5, 0.2);
	complex<double> c3 = c1 + c2;

	// Математический вектор
	valarray<double> vect1(10,2);
	valarray<double> vect2(5,2);
	valarray<double> vect3 = vect1 + vect2;

	// Основные математические функции
	// Возведение комплексного числа в степень
	c3 = pow(c3, 2);

	// Произведение векторов
	vect3 = vect1*vect2;

	// Натуральный логарифм комплексного числа
	c2 = log(c1);
}


// ----------------------------------
// Минимальная программа (функция main может не возвращать значение)
/*
int main(){}
*/


int main()
{
	// Операции в функции main 
	// (особенно включающие алгоритмы и опрерации с контейнерами)
	// всегда заключаются в try/catch
	try
	{
		// 1. Демонстрация итераторов ввода/вывода
		show_out_iter();
		show_istr_iter();
		show_iterators();

		// 2. Демонстрация foreach
		show_foreach();

		// 3. Демонстрация find_if с предикатом
		show_findif();

		// 4. Демонстрация mem_fun с предикатом
		// Вызывает функцию draw для каждого элемента полиморфного списка 'sh'
		// list<Shape*> sh(...);
		// for_each( sh.begin(), sh.end(), mem_fun(&Shape::draw) );

		// 5. Демонстрация математических возможностей
		show_math();
	}
	catch (const std::exception& e)
	{
		cout << "exception: " << e.what();
		return 1;
	}

	// это значение будет возвращено системе
	return 0;
}
