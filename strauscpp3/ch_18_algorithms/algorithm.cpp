#include "algorithm.h"
#include "predicates.h"
#include <set>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <cstdlib>

// ������-������������ �������� ��� �������� � ������������ ����������� ���������� C++ STL

using namespace std;
typedef vector<int> v_int;
typedef vector<int>::iterator v_iit;

template<typename T>
void print_vector(const vector<T>& l){
	vector<T>::const_iterator it = l.begin();
	while( it != l.end() ){
		cout << (*it) << ' ';
		++it;
	}
	cout << endl;
}

/// Pair values extraction functor
/// Extracts 1-st value (key)
template <typename Pair> struct select1st {
    typedef Pair argument_type ;
    typedef typename Pair::first_type result_type ;
    const result_type& operator()(const argument_type& p) const {
        return p.first ;
    }
};

/// Pair values extraction functor
/// Extracts 2-nd value
template <typename Pair> struct select2nd {
    typedef Pair argument_type ;
    typedef typename Pair::second_type result_type ;
    const result_type & operator()(const argument_type& p) const {
        return p.second ;
    }
} ;

// --------------- ���������������� ��������� --------------- 
// for_each 
// find 
// count
// ��������� � ������������ 
// ����� 
void show_non_modif() {
	int arr[] =  { 0,4,1,9,3,7,3,7,3,8 };
	int arr1[] = { 7,3,7,4,5,2,8,8,0,4 };
	vector<int> vi( arr, arr + sizeof(arr)/sizeof(int) );
	vector<int> vi1( arr1, arr1 + sizeof(arr1)/sizeof(int) );

	// --- find, find_if ---
	v_iit it = find(vi.begin(), vi.end(), 7 );
	
	// ���������� ����� ���� ���, � ���� ����������� �������� ()
	it = find_if(vi.begin(), vi.end(), my_less_predivate(7) );

	// ������� _if �������� ��-�� ��������� ���������� ������� ���������
	// ��������, ���� �� ���� ��������� ��������� bool

	// --- for_each ---
	// ������������� for_each() � ��������� ������������ � ��������������� ���������
	// for_each() - ���������������� ��������!
	sum_accum<int> s;
	s = for_each(vi.begin(), vi.end(), s );
	int sum = s.res;

	// --- find_first_of ---
	// ������� � ������������������ ������ �������,
	// ������� ������� �� ������ ������������������
	it = find_first_of(vi.begin(), vi.end(), vi1.begin(), vi1.end() );

	// --- adjacent_find ---
	// ���� ����� � ���������� �������� �� ������ �������
	it = adjacent_find( vi1.begin(), vi1.end() /*, pred*/ );

	// --- count, count_if ---
	long long c = count(vi.begin(), vi.end(), 7 );
	c = count_if(vi.begin(), vi.end(), my_less_predivate(7) );

	// --- mismatch ---
	// ���������� 2 ������������������ � ������� ������ ������������
	// ����� ������ ������������������ �� ���������, ���������, ��� ��� �� ������ ������
	pair<v_iit, v_iit> p1 = mismatch(vi.begin(), vi.end(), vi1.begin());

	// --- equal ---
	// ������ ���������� 2 ������������������, �� �������� ����� ������������
	bool b = equal( vi.begin(), vi.end(), vi1.begin() );

	// --- search, find_end, search_n ---
	int arr2[] = {9,3,7};
	size_t arr2_sz = sizeof(arr2)/sizeof(int);

	// search - ������� ��������������������� � ������������������
	// � ���������� �������� �� ������ ����������� ������� 
	// ��������: ������� � ������� vi ������ arr2
	v_iit it2 = search(vi.begin(), vi.end(), arr2, arr2 + arr2_sz);
	
	// find_end - ������� ��������������������� � ������������������
	// � ���������� �������� �� ��������� ����������� ������� (???)
	// ��������: ������� � ������� vi ������ arr2, �� ���������� �������� �� �����
	it2 = find_end(vi.begin(), vi.end(), arr2, arr2 + arr2_sz);

	// ������� � vi1 ������������������ �� 2 ���������
	it2 = search_n(vi1.begin(), vi1.end(), 2, 8);
}

// --------------- �������������� ��������� --------------- 
// copy, copy_if, copy_backward  
// transform  
// replace, replace_if, replace_copy, replace_copy_if  
// remove,remove_copy,remove_copy_if  
// fill,fill_n  
// generate,generate_n  
// unique,unique_copy  
// reverse,reverse_copy  
// rotate,rotate_copy  
// random_shuffle  
// swap,iter_swap,swap_ranges  
void show_modif(){

	int arr[] =  { 0,4,1,9,3,7,3,7,3,8 };
	int arr1[] = { 7,3,7,4,5,2,8,8,0,4 };
	vector<int> vi( arr, arr + sizeof(arr)/sizeof(int) );
	vector<int> vi1( arr1, arr1 + sizeof(arr1)/sizeof(int) );

	// �������������� ��������� ������������� ����� ��������� 
	// �� ������� ������������������

	// ������ ������� � ���������, �������� �������������� � ������ ����� ����������,
	// � ����� ���������/���������� �������
	
	// ������� ��� ������, ��� �������� ������ �� ����� �� ���������� ����������,
	// � �� ������, ��������, �������� ��� �������� �������

	// --- copy, copy_backward --- 
	// �������� ���� ������, �� ����������� � ��������. ��������, � �����.
	copy(vi.begin(), vi.end(),ostream_iterator<int>(cout));
	cout << endl;

	// ������������� ������� �� ��������� ����� �� ������� ���������� 
	// � ��������� ������ ���������� � �����
	vector<int> v_copy;
	copy(vi.begin(), vi.end(),back_inserter(v_copy));

	// ������� � �������� ������������������ ����� �������������
	// ���� ������ �������� ������������������ ��������� ������ �������
	// ���������� ������������ copy_backward - ����������� � �����
	// ��������, ����������� ��� �������� � ��� �� ��������� �� ������� �� 2
	// (???)
	//copy_backward( v_copy.begin(), v_copy.end(), v_copy.begin() + 2 );

	// copy_if � ����������� �������� STL �����������
	//copy_if(vi.begin(), vi.end(),back_inserter(v_copy), less<int>(7) );

	// --- transform --- 
	// ������� �������� ������������������, �������� �������� �� �������
	// (��� � 2-� �������)

	// � ��� ������� ����� �������� ����������� ���������, ������� case_sens � ��.

	vector<int> vcolors;
	vector<circle> vshape1;
	vshape1.push_back(circle(10));
	vshape1.push_back(circle(20));
	vshape1.push_back(circle(30));

	// ���������� ��������� ������ �� ���������� �����
	transform( vshape1.begin(), vshape1.end(), back_inserter(vcolors), mem_fun_ref( &shape::color ) );

	// ���� ������, ����������� 2 �����������������
	// ������������ ��������� ���� �������� � ������ � ������
	vector<int> res;
	transform( vi.begin(), vi.end(), vi1.begin(), 
		back_inserter(res), multiplies<int>() );
	
	// ����� 2 ������������������� �� ���������

	// --- unique,unique_copy --- 
	// unique - ��������� ��� ���������� �������� � ����� (��� ������???) ������������������,
	// � ���������� �������� �� ����� ������������������ ����������
	vi.clear();
	int arr2[] =  { 0,4,1,1,1,7,3,3,3,8,6,2,9,4,4,7 };
	vector<int> vi2( arr2, arr2 + sizeof(arr2)/sizeof(int) );
	sort(vi2.begin(), vi2.end());
	v_iit vit1 = unique( vi2.begin(), vi2.end() );

	// ������� �� �������� �������
	vi2.assign( arr2, arr2 + sizeof(arr2)/sizeof(int) );
	sort(vi2.begin(), vi2.end());

	// unique_copy - ��������� ����� ������������������ (������ �� �������)
	unique_copy( vi2.begin(), vi2.end(), back_inserter(vi) );

	// --- replace, replace_if, replace_copy, replace_copy_if --- 
	// ��������� ���� ���������� �������� � ������������������ ��������� ��������
	
	// �������� 7 �� 8
	replace( vi.begin(), vi.end(), 7, 8 );

	// �������� � vi 4 �� 6 � �������� � ������ vi2 (������ �� �������)
	vi2.clear();
	replace_copy( vi.begin(), vi.end(), back_inserter(vi2), 4, 6 );

	// �������� ��� <= 2 �� 3
	replace_if( vi.begin(), vi.end(), bind2nd(less_equal<int>(), 2) , 3 );

	// �������� � vi1 ��� <= 2 �� 3 � ��������� � ������ vi  (������ �� �������)
	replace_copy_if( vi1.begin(), vi1.end()
		, back_inserter(vi)
		, bind2nd(less_equal<int>(), 2)
		, 3 );

	// --- remove,remove_copy,remove_copy_if --- 
	// ��������� remove ���������� �������� (� ������������������ ��� ��� ��),
	// ����������� �� �������� ��� ���������
	// ������� remove ������ ���������� � ������ (��� �����???) ������������������ (������� �������)
	print_vector(vi);
	v_iit it = remove( vi.begin(), vi.end(), 5 );
	print_vector(vi);
	// todo : print vector

	// remove_if - �� ���������
	it = remove_if( vi.begin(), vi.end(), bind2nd( less<int>(), 4 ) );
	print_vector(vi);
	// todo : print vector

	// remove_copy_if - ������� ���������� �� �����, 
	// ���� �� �� ������������� ������� �� ���������
	vector<int> vi3;
	remove_copy_if( vi1.begin(), vi1.end(), back_inserter(vi3), bind2nd( less<int>(), 2 ) );
	print_vector(vi3);

	// --- fill,fill_n ---
	// ��������� ������� ��������� ���������� ���������

	// ������ 2 ��������� ��������� 5
	fill(vi3.begin(), vi3.begin() + 2, 5 );

	// �� �� ����� � ����������� fill_n
	fill_n(vi3.begin(), 2, 6 );

	// --- generate,generate_n --- 
	// ��������� �������, ������������ �������� ���������� �� �������
	// ��������� ��������� �����
	generate(vi3.begin(), vi3.end(), rand );

	// ����� 20 ��������� �����
	generate_n( ostream_iterator<int>(cout), 20, rand );

	// --- reverse,reverse_copy --- 
	// reverse ������ ������� ������������������ �� ��������
	reverse( vi.begin(), vi.end() );

	// reverse_copy ���������� ����� ������������������ � �������� �������
	vector<int> vi5;
	reverse_copy( vi.begin(), vi.end(), back_inserter(vi5) );
	
	// --- rotate,rotate_copy --- 
	// rotate ���������� �������� ��������, ���� ������ �������� �������
	// �� �������� ������ ���������� ������������������
	int arr3[] = {7,2,89,3,8,6,2,6};
	size_t sz3 = sizeof(arr3)/sizeof(int);
	
	// �������� ����������� ����� ������� �� 3 �����
	rotate(arr3, arr3 + 3, arr3 + sz3);

	// rotate_copy
	// ����������� ���������� �� 5 ������� � vi5
	vi5.clear();
	rotate_copy(arr3, arr3 + 5, arr3 + sz3,back_inserter(vi5));
	
	// --- random_shuffle --- 
	// ������������� ����������
	random_shuffle(arr3, arr3 + sz3/* , rand_gen()*/ );
	
	// --- swap,iter_swap,swap_ranges --- 
	// swap - ������������ 2 ������� �������
	swap(vi, vi1);

	// iter_swap 
	// swap - ������������ 2 �������� ������� (������������� � ����� ����������) (??? � �����)
	swap(vi.begin(), vi1.begin());

	// swap_ranges 
	// ������ ������� 2 ��������� ������� �������, �������
	// ������ 2 �������� vi1 � vi2
	swap_ranges(vi1.begin(), vi1.begin() + 2, vi2.begin());
}

// --------------- ���������� --------------- 
//  sort,stable_sort  
//  parital_sort,parital_stable_sort  
//  nth_element, binary_search  
//  merge,inplace_merge  
//  lower_bound, upper_bound  
//  partition,stable_partition  
void show_sort(){
	int arr[] =  { 0,4,1,9,3,7,3,7,3,8 };
	int arr1[] = { 7,3,7,4,5,2,8,8,0,4 };
	vector<int> vi( arr, arr + sizeof(arr)/sizeof(int) );
	vector<int> vi1( arr1, arr1 + sizeof(arr1)/sizeof(int) );

	// --- sort,stable_sort --- 
	// ������� ���������� � ������������ ��������
	sort(vi.begin(), vi.end());

	// stable_sort ��������� ������� ���������� ���������
	stable_sort(vi1.begin(), vi1.end());

	vi.assign( arr, arr + sizeof(arr)/sizeof(int) );
	vi1.assign( arr1, arr1 + sizeof(arr1)/sizeof(int) );

	// --- parital_sort,partial_sort_copy --- 
	// ������������������ ����������� �� 2 �����, � ���������� ������������ 
	// �� ��� ���, ���� �� ����� ������������� ������ �����
	partial_sort(vi.begin(), vi.begin() + 3, vi.end());

	// ������������������ ����������� �� 2 �����, ���������� � �������� ������������������
	// � ���������� ������������ �� ��� ���, ���� �� ����� ������������� ������ �����
	partial_sort_copy(vi.begin(), vi.end(),vi1.begin(), vi1.begin() + 3);

	vi.assign( arr, arr + sizeof(arr)/sizeof(int) );
	vi1.assign( arr1, arr1 + sizeof(arr1)/sizeof(int) );
	// --- nth_element --- 
	// ���������� �� ��� ���, ���� n-�� ������� �� ������ ���� �����
	// ������� ��� ���������� ������, ����������� � ��
	// ��������, ������ �������
	nth_element(vi.begin(), vi.begin() + 3, vi.end());

	// --- 	binary_search, lower_bound, upper_bound --- 
	// binary_search - ����� � ��������������� ������������������
	sort(vi.begin(), vi.end());
	bool b = binary_search(vi.begin(), vi.end(),1);

	// lower_bound � upper_bound - ����������
	// ������� � ������ ������� ���������� ��������� 
	// ��������������� ������������������
	v_iit it =lower_bound(vi.begin(), vi.end(),7);
	it = upper_bound(vi.begin(), vi.end(),7);

	// --- merge,inplace_merge --- 
	// ��������� ������� ������� ��������������� �������������������
	
	// merge
	// ������� ���� �������������������
	sort(vi1.begin(), vi1.end());
	vector<int> res;
	merge( vi.begin(), vi.end(),vi1.begin(), vi1.end(),back_inserter(res) );

	// inplace_merge
	// ������� ���� ��������������� ������ ����� ������������������
	res.clear();
	res.reserve(100);	// ���� �� ���������������� ������

	copy(vi.begin(), vi.end(), back_inserter(res));
	copy(vi1.begin(), vi1.end(), back_inserter(res));
	inplace_merge(res.begin(), res.begin()+vi.size() , res.end());	//(??? wtf)

	// --- partition,stable_partition --- 
	// ���������� ���������� - ��������� ���� ���������,
	// ��������������� ���������, ����� ����, ������� ��� �� �������������

}


// --------------- Heap --------------- 
// make_heap, sort_heap
// push_heap, pop_heap
void show_heap(){

}


// --------------- Min|Max --------------- 
// min, max, max_element, min_element
// lexigraphical_compare
void show_min_max(){
	// --- min, max, max_element, min_element ---
	double m = max(1.2, 2.5);
	const char* a = "aaa";
	const char* b = "bbb";
	string s( min(a, b) );
	// max/min �������� �� ���� ��� �� ����������
	// no comments
	
	const char* A = "AAA";

	// lexigraphical_compare - ��������� �����
	bool diff = lexicographical_compare(a , a + strlen(a), b , b + strlen(b) );
	diff = lexicographical_compare(a , a + strlen(a), A , b + strlen(A)/*, no_case()*/ );
}

// ------------------- Set -------------------

// --- includes ---
// --- set_union, set_intersection ---
// --- set_difference, set_symmetric_difference ---
void show_set(){
	// �������� � ����������� ����� ����� ������ ��� ��������������� �������������������
	// �������� ���������� � ������������ set � multiset
	int arr[] =  { 0,4,1,9,3,7,3,7,3,8 };
	int arr1[] = { 7,3,7,4,5,2,8,8,0,4 };
	set<int> s( arr, arr + sizeof(arr)/sizeof(int) );
	multiset<int> ms( arr1, arr1 + sizeof(arr1)/sizeof(int) );

	// --- includes ---
	// ���������, �������� �� ��� ����� ������ ������������������
	// ����� ������� ������
	bool b = includes(s.begin(), s.end(),ms.begin(), ms.end());

	// --- set_union, set_intersection ---
	// ����������� � ����������� �������������� 
	multiset<int> union_out;
	multiset<int> intersection_out;
	//set_union(s.begin(), s.end(),ms.begin(), ms.end(), union_out.begin() ); (??? wtf)
	//set_intersection(s.begin(), s.end(),ms.begin(), ms.end(), intersection_out.begin() );(??? wtf)

	// --- set_difference, set_symmetric_difference ---
	multiset<int> diff_out;
	multiset<int> symm_out;

	// set_difference - ������������������ �������� � ������, �� �� �������� �� ������ ���������
	//set_difference(s.begin(), s.end(),ms.begin(), ms.end(), diff_out.begin() );(??? wtf)

	// set_symmetric_difference - ������������������ �������� � ������ ���� �� ������������������� ���������
	//set_symmetric_difference(s.begin(), s.end(),ms.begin(), ms.end(), symm_out.begin() );(??? wtf)
}

// --------------- Permutations --------------- 
// next_permutation, prev_permutation
void show_permutations(){

	// ��� ��������� ������������ ������������������
	char v[] = "abcd";
	cout << endl << "start permutations:" << endl << v << '\t';
	while ( next_permutation(v, v + 4) ){
		cout << v << '\t';
	}
}

// --------------- C algorithms --------------- 
// qsort, bsearch
int c_compare( const void* arg1, const void* arg2 ){
	/* Compare all of both strings: */
	return (*static_cast<const int*>(arg1)) > (*static_cast<const int*>(arg2));
}

void c_algorithms(){
	int arr[] =  { 0,4,1,9,3,7,3,7,3,8 };
	size_t sz = sizeof(arr)/sizeof(int);
	qsort( arr, sz, sizeof(int), c_compare );
	int key = 1;
	int* i = (int*)bsearch( (const void*)&key, (int*)arr, sz, sizeof(int), c_compare );
}
