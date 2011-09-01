#include <iterator>
#include <algorithm>
#include <vector>
#include <string>

#include "adapters.h"
#include "predicates.h"

using namespace std;

// --------------- ������������������ --------------- 
// ��� ���������� �������� ������������������ � �������� 
// ����� ���������� ��� ������������������

// ������������������ (���� ����������, ������������ ���������)
template <typename T> struct IterSeq : public pair<T, T>{
	IterSeq(T in1, T in2) : pair<T,T>(in1, in2){}
};

// �������� ������������������ ��� ���� ����������
template <typename In, typename T>
In find_in_seq(IterSeq<In> seq, const T& val){
	return find( seq.first, seq.second, val );
}

// �������-�������� ��� ������ ����������
template <typename T> 
IterSeq<typename T::iterator> iseq(T& t){
	return IterSeq<T::iterator>( t.begin(), t.end() );
}

// -------------------  -------------------
// ������������ ������������� ���� ���������� ��� ������� ������������������
void show_sequences(){
	int arr[] = { 0,4,1,9,3,7,3,7,3,8 };
	vector<int> vi( arr, arr + sizeof(arr)/sizeof(int) );

	// ����� � ������������������ (������� ������� ���������)
	vector<int>::iterator it3 = find_in_seq( iseq(vi), 7 );
}


// --------------- �������� --------------- 
// --- unary_function --- 
// --- binary_function --- 
void show_functors(){
	
	// ����� ����������� ������� �� ��������� ����� mem_fun()
	vector<shape*> vshape;
	vshape.push_back( new rectangle(10) );
	vshape.push_back( new rectangle(20) );
	vshape.push_back( new circle(20) );
	vector<shape*>::iterator shape_it = find_if( vshape.begin(), vshape.end(), shape_eq(20) );
	for_each( vshape.begin(), vshape.end(), mem_fun( &shape::draw ) );
	

	// ����� ����������� ����� ������� �� ��������� �� ������� 
	// (������� ���� ����� �� 7 ��������)
	for_each( vshape.begin(), vshape.end(), bind2nd( mem_fun(&shape::rotate), 7 ) );

	// �������
	for_each( vshape.begin(), vshape.end(), mem_fun( &shape::clear ) );

	// ����� ����������� ������� �������� ����� mem_fun_ref()
	vector<circle> vshape1;
	vshape1.push_back(circle(10));
	vshape1.push_back(circle(20));
	vshape1.push_back(circle(30));
	for_each( vshape1.begin(), vshape1.end(), mem_fun_ref( &circle::draw ) );

	
}

// --------------- ��������� --------------- 
// ��������������: plus, minus, multiplies, divide, modulus, negate
// ����������: equial_to, greater, less, logical_and etc.
void show_predicates(){
	int arr1[] = { 0,4,1,9,3,7,3,7,3,8 };
	int arr2[] = { 4,2,9,4,6,1,0,3,7,4 };
	vector<int> vi1( arr1, arr1 + sizeof(arr1)/sizeof(int) );
	vector<int> vi2( arr2, arr2 + sizeof(arr2)/sizeof(int) );
	vector<int> res;

	// ������������ ��������� ���� �������� � ������ � ������
	// ������������ �������������� �������� multiplies
	transform( vi1.begin(), vi1.end(), vi2.begin(), 
		std::back_inserter(res), multiplies<int>() );

	// ������������ ���������� �������� ������ � ������� � ������ ��� � ��� �� ������
	transform( vi1.begin(), vi1.end(), vi1.begin(), negate<int>() );
}


// -------------------  -------------------
// ������� ��� ������������ �������� ptr_fun
bool numeric_eq(int val, int comparer){
	return val == comparer;
}

bool string_eq(int val, const char* comparer){
	return val == atoi(comparer);
}

bool is_odd_func( int a ){
	return !(a%2);
}
// -------------------  -------------------


// --------------- �����������, �������� --------------- 
// --- bind1st --- 
// --- bind2nd --- 
// --- mem_fun --- 
// --- mem_fun_ref --- 
// --- ptr_fun --- 
void show_binders_adapters(){
	int arr1[] = { 0,4,1,9,3,7,3,7,3,8 };
	int arr2[] = { 4,2,9,4,6,1,0,3,7,4 };
	vector<int> vi1( arr1, arr1 + sizeof(arr1)/sizeof(int) );
	vector<int> vi2( arr2, arr2 + sizeof(arr2)/sizeof(int) );
	vector<int> res;

	typedef vector<int>::iterator v_it;

	// bind1st + bind2nd
	// ���������������� ����������� � ������� ����� � ������ ����������
	cout << "bind1st" << endl;
	transform( vi1.begin(), vi1.end(), vi1.begin(), bind1st( bind_output<int>(), 7 ) );
	cout << "bind2nd" << endl;
	transform( vi2.begin(), vi2.end(), vi2.begin(), bind2nd( bind_output<int>(), 7 ) );

	// --- mem_fun + mem_fun_ref --- 
	// ��. show_functors()

	// --- ptr_fun ---
	// �������� ��������� �� ������� � �������
	long long c = count_if(vi1.begin(), vi1.end(), bind2nd( ptr_fun(numeric_eq), 7 ) );
	c = count_if(vi1.begin(), vi1.end(), bind2nd( ptr_fun(string_eq), "7" ) );
	c = count_if(vi1.begin(), vi1.end(), ptr_fun(is_odd_func) );

	// --- not1, not2 ---
	// ���������� - ���������� �������� ��������
	// ����� �������� ���������� ������ ����, ��� ������� ����������� ������� less<>()
	// ��������� not2 - �.�. ��������� � ��������� 2
	pair<v_it, v_it> p1 = mismatch( vi1.begin(), vi1.end(), vi2.begin(), not2( less<int>() ) );

	// ������� �������� ������ �� �����������
	c = count_if(vi1.begin(), vi1.end(), not1( ptr_fun(is_odd_func) ) );
}

