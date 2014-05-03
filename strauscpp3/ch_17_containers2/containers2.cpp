#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <deque>
#include <algorithm>
#include <functional>
#include <cctype>
#include <string>
#include <string>

using namespace std;

template<typename T, template <typename ELEM, typename = std::allocator<ELEM> > class CONT >
void print_container(const CONT<T>& c){
	CONT<T>::const_iterator it = c.begin();
	while( it != c.end() ){
		cout << (*it) << ' ';
		++it;
	}
	cout << endl;
}

template<typename TKey, typename TVal>
void print_map(const map<TKey, TVal>& m	) {
	map<TKey, TVal>::const_iterator it = m.begin();
	while( it != m.end() ) {
		cout << (*it).first << '=/t' << (*it).second;
		++it;
	}
	cout << endl;
}

bool is6(int v){
	return v == 6;
}

// ��������, ����������� ������ ����� � ������
class initial{
public:
	initial(const char c) : ic(c){}
	bool operator()(const string& cs){
		return cs[0] == ic;
	}
private:
	const char ic;
};


const std::string string_to_lower(const std::string& s) {
	std::string res(s);
	std::transform(res.begin(), res.end(), res.begin(),std::ptr_fun(std::tolower));
	return res;
}

template<typename T>
struct no_case: public binary_function<T, T, bool>{};

template<>
struct no_case<const char*>
	: public binary_function<const char*, const char*, bool>{
	bool operator()(const char* ls, const char* rs) const
	{
		do{
			if (tolower(*ls) != tolower(*rs))
				return false;
		} while (*(++ls) && *(++rs));
		return true;
	}
};

template<>
struct no_case<string>
	: public binary_function<string, string, bool> {	
	bool operator()(const string& ls, const string& rs) const
	{	
		string nocase_left = string_to_lower(ls);
		string nocase_right = string_to_lower(rs);
		return nocase_left < nocase_right;
	}
};
 

//
struct initial2{
	initial2(const char c) : _c(c){}
	bool operator()(const string& _Left, const string& _Right) const{
		return (_Left == _Right) && (_Left[0] == _c);
	}
private:
	const char _c;
};

template <typename T> struct is_odd : public unary_function<T, bool> {
	bool operator()( T& val ) {
		return ( val % 2 ) == 1;
	}
};

// ------------ list ------------ 
// list �� �������� ���������� �����������, � ��������� reserve() � capacity()
void show_list(){
	int arr[10] = {1,6,3,5,7,6,4,2,4};
	vector<int> vi1(10, 1);	

	list<int> l;
	list<int> l1;
	l.assign(arr, arr + sizeof(arr)/sizeof(int));
	l1.assign(vi1.begin(), vi1.begin() + vi1.size());
	print_container(l);
	print_container(l1);

	// ����������� ���������� ���������� ������ ������ ���������
	// �.�. �������� ������� ��������� ���������
	l.sort();
	print_container(l);

	list<string> ls;
	list<string> ug;
	ls.push_back("onotole");
	ls.push_back("chake");
	ls.push_back("leonide");
	ug.push_back("ermakove");
	print_container(ls);

	// �������� "������� � ��������"
	// ����� ��������� ������ �� �������
	// ������� ��� ���������� ���������, �� ����������� ����������� ���������

	// ���� ��������, ����� ������ ��������
	ls.splice(ls.begin(), ug);
	print_container(ls);

	// ���� ��������, �� ������ ������, ����� �������
	ug.splice(ug.begin(), ls, ls.begin() );
	print_container(ug);

	// ���� ��������, �� ������ ������, � ������ �� ����� ������� 
	// (������ ������ �������, � �������� ������ ����� 'c' � ������� � ������ �� ���� �������)
	list<string>::iterator p = find_if(ls.begin(), ls.end(), initial('c'));
	ug.splice(ug.begin(), ls, ls.begin(), p );
	print_container(ug);

	// ��� - ������, ������ ��� �� ��������� ��������
	//ug.splice(ug.begin(), ls, ls.begin(), ls.begin()+2 );

	// ������, � �������� splice ���� ���� ����������
	// ��� �������� ���������� - ��� ������ ��������� �� ���� ������ �
	// ���� �������

	// �������� � ��������� ���������
	ug.push_front("gotyachke"); 
	// ���� ����������� ������������ �����������,
	// �� ����� ������������ �������� � ����� - 
	// ����� ������ ����� ��������, ��������, �� ������

	// ����� ������� - ����� ������� ��������, �� ���������� �����������
	ug.swap(ls);

	// ������� ��������������� ������� � ����������� �������:
	l.merge(l1);

	// ����� ������� ����������������� ������, �� ����� ������� �� ������������
	l1.assign(10,2);
	list<int> l3(arr, arr + sizeof(arr)/sizeof(int));
	// l1.merge(l3);  � VC ������ ������ ������

	// ����������� ���������� ��������
	l.remove(1);
	
	// � ��� ����� � ���������� - �������� ������� ��������
	l.remove_if(is_odd<int>());

	ls.push_back("onotole");
	ls.push_back("Onotole");
	ls.push_back("ermakove");
	
	// ���������� � ���������� no_case
	ls.sort( no_case<string>() );

	// ������� ���������������� �������,
	// ��������������� ���������
	ls.unique( initial2('o') );

	// ������� ��� ���������������� �������
	l.unique();

	// ����������� ������
	ls.reverse();

	// ������ � ���������
	string first = ls.front();
	string last = ls.back();
	
	// ������ �������� ������� � ����� ����������
	// ������� �������� ������������� ������ sort()
	// ��� ��������� �������� ������������� �������
	// ��� ���� ��������� �� ������ ������ ����������
	
	// ��������� ������ ������ �������
}

// ------------ deque ------------ 
void show_deque(){
	int arr[10] = {1,6,3,5,7,6,4,2,4};

	// ���������� �������� ������� � ������
	deque<int> d;
	d.insert(d.begin(),arr,arr + sizeof(arr)/sizeof(int));
	int a = d.at(1);
	// �������� ���������� ����������� �������
}

// ------------ �������� ������������������� ------------ 
void show_adapters(){
	// --- ���� - ������� �� ������ deque
	stack<int> s1;
	s1.push(1);
	s1.push(2);

	// � ����� ����� �������� ������������ �����
	s1.pop();s1.pop();
	//s1.pop(); - UB

	// ����� ���������� ������ ��� ����� �� ������� ����������
	stack<int, vector<int> > s2;
	s2.push(1);

	// --- ������� - ������� �� ������ deque
	queue<int> q1;
	q1.push(1);
	q1.push(2);
	q1.push(3);

	q1.pop();
	q1.pop();
	q1.pop();
	
	// --- ������� � �����������
	// �� ��������� �������� �� heap
	priority_queue<int> pq1;
	pq1.push(1);
	pq1.push(9);
	pq1.push(2);
	pq1.push(8);
	pq1.pop();	// ������������� ����������!
	pq1.push(5);
	pq1.pop();
	pq1.pop();

	// ������� � ��������� �����������
	priority_queue<string, deque<string> > pq2;
	pq2.push("aaaa");
	pq2.push("Aaaa");
	pq2.push("bbbb");
	pq2.push("Bbbb");
	pq2.push("krabe");
	pq2.pop();
	pq2.push("shmele");
	pq2.pop();

	// ������� � ��������� ����������� � ���������� ���������
	priority_queue<string, deque<string>, no_case<string> > pq3;
	pq3.push("AaAa");
	pq3.push("aaaa");
	pq3.push("abab");
	pq3.push("baba");
}

// ------------ map ------------ 
// ������ ���������� typedef ��� ���������� map
typedef map<int, string> is_map;
typedef map<int, string>::iterator is_map_it;

typedef map<string, int, no_case<string> > si_nocase_map;
typedef map<string, int, no_case<string> >::iterator si_nocase_map_it;

template <typename TMap, typename TKey, typename TVal>
void _insert_to_map(TMap& m1, const TKey& k, const TVal& s){

	// ������������� ������ �������, ����� ��� ����� 
	// ������������ �������� "������"
	
	// ������������ ���� �������� �������� �� ����������� ������
	// � ���������� ����, ���������� �������� �������
	pair< typename TMap::iterator, bool > p = m1.insert( make_pair(k,s) );
	if(p.second == true){
		cout << "Insert success" << endl;
	}
	else{
		cout << "Such key is already defined" << endl;
	}
}

void show_map()
{
	string s1("onotole");
	string s2("chake");
	is_map m1;

	// map ������� ������� � �������� �������� <
	// unordered(hash) map ������� �������  !=

	// ����� map (� �������� set) �������� ������ ������
	// ����� ������� ������ � �������� �����

	_insert_to_map<is_map, int, string>(m1,1,s1);
	_insert_to_map<is_map, int, string>(m1,1,s1);

	// ��� � ���������� no_case
	si_nocase_map ncm1;
	_insert_to_map<si_nocase_map, string, int>(ncm1,s1,1);
	_insert_to_map<si_nocase_map, string, int>(ncm1,"Onotole",2);
	_insert_to_map<si_nocase_map, string, int>(ncm1,"chake",3);
	_insert_to_map<si_nocase_map, string, int>(ncm1,"Chake",4);

	pair<is_map_it, bool> p = m1.insert( make_pair(2,"chake") );
	p = m1.insert( make_pair(2,"chake2!") );

	// �������� ���������� ����� ���� ������� ���������� ������������
	// ��� ��������� �������� �������� � runtime
	si_nocase_map pred;
	map<string, int, si_nocase_map> mm(pred);

	// ��� ������� �������� ����� �������, ����� ������ �������� ��� ����� ��������
	// ��� ������� ������� ������ ��� ����� ���� �������
	m1.insert( p.first, make_pair(3,"leonide") );

	// �������� ������������ �� ������� ����������
	print_map(m1);

	// ��� map'� - ������ pair<>
	// � �������������� pair �������� ��������������
	pair<char, int> p1('a', 1);
	pair<char, double> p2(p1);

	// ��������� �� �������
	string s = m1[2];

	// ����� ���� �� �������� [] �� ���������,
	// ��������� �������� �� ���������
	// ������� ����� ������ � ������ ���������
	s = m1[3];
	
	// ������� ����� ������ �� ���������
	m1[4] = "leonide";

	// ���� � �������� ���� ��� ������������ �� ���������,
	// ��� ������ ����� ���������������� ����

	// ����� ��������� �� �������� ����
	map<int, string>::iterator it = m1.find(2);
	
	// ������ ������� � ������ ������ 1/������= 2
	it = m1.upper_bound(1);
	it = m1.lower_bound(2);
	
	// �������� ���� ������ �� �������
	if(it == m1.end()){
		m1[2] = string("onotole");
	}

	// �������� ����� �������
	// - �� ������
	m1.erase( m1.find(1) );

	// - ����������� [)
	m1.erase( m1.find(2), m1.find(3) );
	// ����� ������ erase() ���������, ����������� �� ���,
	// ��������������

	size_t sz = m1.size();
	size_t msz = m1.max_size();
	// map � set ������������� ������� �������� ����������,
	// ����� ������� ���������� ��������� (�����-�� ���������� ����� ����������)
}

// ------------ set ------------ 
void show_set(){
	// ��������� � ����������� ����������
	// ����� ����������� ��� map ������ � �������
	// ����� ������������ ��� ������ unique()
	
	// ������� �� ������� � �������������� ����������
	
	set<string> st;

}

// ------------ multiset, multimap ------------ 
void show_multimap(){
	typedef multimap<int, string> is_mmap;
	typedef multimap<int, string>::iterator is_mmap_it;

	is_mmap m2;

	m2.insert( make_pair(1,"onotole") );
	m2.insert( make_pair(1,"chake") );
	m2.insert( make_pair(2,"") );
	m2.insert( make_pair(2,"leonide") );

	// ������ ������� � ������ 2
	is_mmap_it p1 = m2.lower_bound(2);

	// ��������� �� ��������� ��������� � ������ 1
	p1 = m2.upper_bound(1);

	// ���� ���������� �������������� ��� �������� � ������ 2
	pair<is_mmap_it, is_mmap_it> pm = m2.equal_range(2);

	// ���������� ��������� � ������ 2
	size_t sz = m2.count(2);
}

void show_multiset(){
	// ��������� ������������� ���������. ����� ����.

	// todo : ������� � ����������!
	multiset<string> ms;

}

