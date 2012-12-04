#include <vector>
#include <algorithm>
#include <functional>

using namespace std;

bool is6(int v){
	return v == 6;
}

template <class T> struct is_odd : public std::unary_function<T, bool> {
	bool operator()( T& val ) 
	{
		return ( val % 2 ) == 1;
	}
};

void test_std_vector()
{
	int arr[10] = {1,6,3,5,7,6,4,2,4};
	// --------------- vector ---------------
	// пустой по умолчанию
	vector<int> vi;						

	// зарезервировано 100 элементов по 0 (конструктор по умолчанию)
	vector<short> vs(100);	
	
	// ћожно зарезервировать €вно, во избежание аллокаций
	vi.reserve(100);

	// 10 значений по 1
	vector<int> vi1(10, 1);		
	
	// инициализировать 3 элементами  вектора vi1
	vector<int> vi2(vi1.begin(), vi1.begin() + 3);	
	
	// инициализировать 5 элементами  массива arr
	vector<int> vi3(arr, arr + 5);

	// assign() предпочтительнее operator()=, т.к. универсальнее
	// затереть vi1 и vi2 и назначить новые значени€
	vi1.assign(5, 2);
	vi2.assign(arr, arr + 10);

	// вставить 5 по 2 в начало
	vi.insert(vi.begin(), 5, 2);

	// добавить весь vi1 в конец
	vi.insert(vi.end(), vi1.begin(), vi1.end());

	// --- ”далим все значени€ 6 из vi2 ---
	sort(vi2.begin(),vi2.end());
	// найдем первую 6 с начала
	vector<int>::iterator i1 = find_if( vi2.begin(), vi2.end(), is6 );
	// и с конца
	vector<int>::reverse_iterator ri2 = find_if( vi2.rbegin(), vi2.rend(), is6 );
	// извлечем правильно iterator из reverse_iterator 
	vector<int>::iterator i2 = ri2.base();
	// удалим элементы
	vi2.erase(i1, i2);
	// то же самое можно сделать с помощью lower_bound, upper_bound, equal_range

	// обмен€ть 2 вектора
	vi1.swap(vi2);

	// ¬ектор bool занимает 1 бит под каждый элемент
	// —ледовательно его итератор - не указатель!
	// vector<bool> следует избегать и использовать bitset
	vector<bool> bv(5, true);

	// get_allocator() возвращет используемый в векторе аллокатор
	// инициализируем этот вектор значени€ми из vi1 и аллокатором из vi2
	vector<int> vi4(vi1.begin(),vi1.begin() + 2, vi2.get_allocator() );

	// ≈мкость выделенной пам€ти
	size_t sz1 = vi1.capacity();
	size_t sz2 = vi.capacity();

	// ћаксимально возможный размер
	size_t m = vi1.max_size();

	// ¬ектор может переполнитс€ снизу
	vector<int> vi5(2,1);
	vi5.pop_back();
	vi5.pop_back();
	//vi5.pop_back(); pop_back() из пустого вектора - UB
}