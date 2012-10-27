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
	// ������ �� ���������
	vector<int> vi;						

	// ��������������� 100 ��������� �� 0 (����������� �� ���������)
	vector<short> vs(100);	
	
	// ����� ��������������� ����, �� ��������� ���������
	vi.reserve(100);

	// 10 �������� �� 1
	vector<int> vi1(10, 1);		
	
	// ���������������� 3 ����������  ������� vi1
	vector<int> vi2(vi1.begin(), vi1.begin() + 3);	
	
	// ���������������� 5 ����������  ������� arr
	vector<int> vi3(arr, arr + 5);

	// assign() ���������������� operator()=, �.�. �������������
	// �������� vi1 � vi2 � ��������� ����� ��������
	vi1.assign(5, 2);
	vi2.assign(arr, arr + 10);

	// �������� 5 �� 2 � ������
	vi.insert(vi.begin(), 5, 2);

	// �������� ���� vi1 � �����
	vi.insert(vi.end(), vi1.begin(), vi1.end());

	// --- ������ ��� �������� 6 �� vi2 ---
	sort(vi2.begin(),vi2.end());
	// ������ ������ 6 � ������
	vector<int>::iterator i1 = find_if( vi2.begin(), vi2.end(), is6 );
	// � � �����
	vector<int>::reverse_iterator ri2 = find_if( vi2.rbegin(), vi2.rend(), is6 );
	// �������� ��������� iterator �� reverse_iterator 
	vector<int>::iterator i2 = ri2.base();
	// ������ ��������
	vi2.erase(i1, i2);
	// �� �� ����� ����� ������� � ������� lower_bound, upper_bound, equal_range

	// �������� 2 �������
	vi1.swap(vi2);

	// ������ bool �������� 1 ��� ��� ������ �������
	// ������������� ��� �������� - �� ���������!
	// vector<bool> ������� �������� � ������������ bitset
	vector<bool> bv(5, true);

	// get_allocator() ��������� ������������ � ������� ���������
	// �������������� ���� ������ ���������� �� vi1 � ����������� �� vi2
	vector<int> vi4(vi1.begin(),vi1.begin() + 2, vi2.get_allocator() );

	// ������� ���������� ������
	size_t sz1 = vi1.capacity();
	size_t sz2 = vi.capacity();

	// ����������� ��������� ������
	size_t m = vi1.max_size();

	// ������ ����� ������������ �����
	vector<int> vi5(2,1);
	vi5.pop_back();
	vi5.pop_back();
	//vi5.pop_back(); pop_back() �� ������� ������� - UB
}