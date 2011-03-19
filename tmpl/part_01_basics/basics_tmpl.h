#include <iostream>
#include <bitset>
using std::cout;
using std::endl;
// ����� 5 ����������
// �������� ����� typename. ����������� X.template.
// ������������� this-> 
// ������� - ����� �������. ��������� ���������.
// ������������� �����. ������������� ���������.

// ------------------- �������� ����� typename -------------------
// �������� ����� typename ������������ �� ������ ��� �������� ����������-�����
// �� � ��� ��������, ��� ������ �����������, ����������� �� ���������, �������� �����

// ������� ������� �������� ������������� ���������� ���� T
template <typename T>
void print_container(const T& coll){

	// ������, ��� ��������� - ���� ����
	// ���������� ��������� ����� typename � ��������� - ����������� �����
	typename T::const_iterator pos;
	typename T::const_iterator end(coll.end());

	// ������������
	pos = coll.begin();
	while( pos != end ){
		std::cout << *pos << endl;
		++pos;
	}
}


// ------------------- �������� ����� template -------------------
template<int N>
void print_bitset(const std::bitset<N>& b ){

	// ����� ��������, ��� �������� b ������� �� ������� (�� ��������� ����������,
	cout << b.template to_string<char, char_traits<char>, allocator<char> >() << endl;	

	// ...�� � ��������� ������������ �������������)
	cout << b.to_string<char, char_traits<char>, allocator<char> >() << endl;	
}

void test_bitset(){
	bitset<10> b;
	print_bitset(b);
}

// ------------------- ������������� this -------------------
template <typename T>
class base{
public:
	void exit(){
		cout << "base::exit" << endl;
	}
};

template <typename T>
class derived : public base<T>{
public:
	void foo(){
		
		// �� ����:
		// exit();

		// ������� �� �� ������������� exit() �� ������ base()

/*
������ this->exit() ���������� ����������� ������, ��� "� ������� ������ ������ ���� ���� exit". 
��������� ��� ��������������� �� ���������� �� ������ ������� ���������� (���� ��, ��� ����� ��������������� 
Base ��� ���������� ����������), ���� ��������� ��������� ������ ������������� �� �����.
����������� ����� � ��� Derived::exit() � Base::exit().

� ��������� ������ gcc �������� � ������ ������ ����� ��� exit � ������� ���������. � ������� �������� � ���
� ���� ������� Derived<T>::g()
� ���������� ������ Derived<T> �� ����������� ������ ����� � ���� ���� Base<T>
� ������������ ��� ::, � ������� �������� Derived
����� ��� ��� ����. ��� extern "C" void exit(int exitcode) �� <stdlib.h>
*/
		// ����
		this->exit();
		// ��� ���
		//Base<T>::exit();
		
		// ��� ���
		//using Base<T>::exit;    
		//exit();

		// ������ � ������ ������������ �������� ����� ������ this->member ����
		// ��� ���������� � ��� ����� ��� ���������� �������������!
	}
};


// ------------------- �������-����� ������� -------------------
// ��������� ����� ��� ����� �.�. ������ ������ (��� ����������, ��� � ��������)

// ����������
template <typename T>
class copyable_stack{
public:
	copyable_stack();
	void push(const T& p);
	void pop();
	const T& top() const;
	bool empty() const;

	// ��������� �����
	template<typename TCOPY>
	copyable_stack<T>& operator=(const copyable_stack<TCOPY>& s);

private:
	std::vector<T> _copyable_stack;
};

template <typename T>
copyable_stack<T>::copyable_stack(){}

template <typename T>
void copyable_stack<T>::push(const T &p){
	_copyable_stack.push_back(p);
}

template <typename T>
void copyable_stack<T>::pop(){
	_copyable_stack.pop_back();
}

template <typename T>
const T& copyable_stack<T>::top() const {
	return _copyable_stack.back();
}

template <typename T>
bool copyable_stack<T>::empty() const{
	return _copyable_stack.empty();
}

// ������� ���������� ������ ����� �� ����� ���� ���������� 
// �������� ������� ���������� ����
// �������� stack<int> a; stack<double> b; a=b

// ��� ����� ����� �������������� � ������ copyable_stack ��������� operator=
// ����������� ����������� � �������� ������� ����������� ���������
template <typename T>
template <typename TCOPY>
copyable_stack<T>& copyable_stack<T>::operator=( const copyable_stack<TCOPY>& s ) {
	
	copyable_stack<TCOPY>  tmp(s);
	_copyable_stack.clear();

	// �������� �������� � ����������� ���� (����� ����������� � ������ ����������)
	while( !tmp.empty() ){
		_copyable_stack.push_back( static_cast<T>( tmp.top() ) );
		tmp.pop();
	}
	return *this;
}

// ------------------- ��������� ��������� �������� -------------------
// ���������� ������� ����� ���� ������
// ��������, ���� � ��������� ����������� ����� ��������� ��������� ����������� 
// ����������, �� ������� ������� ����, �������� ���:
template < 
	typename T, 
	// �.�. CONT ���������� ��� ������, � �� ���, ������������ �������� ����� class
	// ����� �������������� ���������� ����� �������� (... typename = std::allocator)
	template <typename ELEM, typename = std::allocator<ELEM> > class CONT = std::deque >
class tmpl_stack {
public:
	tmpl_stack();
	void push(const T& p);
	void pop();
	const T& top() const;
	bool empty() const;

	// ��������� �����
	template<typename TCOPY,template <typename, typename> class CONT_COPY>
	tmpl_stack<T,CONT>& operator=(const tmpl_stack<TCOPY,CONT_COPY>& s);

private:
	CONT<T> _copyable_stack;
};

// template <typename ELEM, typename = std::allocator<ELEM> > - ����������� ���������������� ����������� STL
// ������ �������� ���������� ����� ���������� � ���� std::deque, � �� std::deque<T>

// ����������� ������� ������ � ��������� ����������
// (����� �������������� ���������� ����� ��������)
template <typename T, template <typename, typename> class CONT>
tmpl_stack<T,CONT>::tmpl_stack(){}

template <typename T, template <typename, typename> class CONT>
void tmpl_stack<T,CONT>::push(const T &p){
	_copyable_stack.push_back(p);
}

template <typename T, template <typename, typename> class CONT>
void tmpl_stack<T,CONT>::pop(){
	_copyable_stack.pop_back();
}

template <typename T, template <typename, typename> class CONT>
const T& tmpl_stack<T,CONT>::top() const {
	return _copyable_stack.back();
}

template <typename T, template <typename, typename> class CONT>
bool tmpl_stack<T,CONT>::empty() const{
	return _copyable_stack.empty();
}


// ����� ������� ����� - ��������� ����� � ��������� ������, 
// ���������� ������� ��������� ��������
template <typename T, template <typename, typename> class CONT>
template <typename TCOPY, template <typename, typename> class CONT_COPY>
tmpl_stack<T, CONT>& tmpl_stack<T, CONT>::operator=( const tmpl_stack<TCOPY, CONT_COPY>& s ) {

	tmpl_stack<TCOPY, CONT_COPY>  tmp(s);
	_copyable_stack.clear();

	// �������� �������� � ����������� ���� (����� ����������� � ������ ����������)
	while( !tmp.empty() ){
		_copyable_stack.push_back( static_cast<T>( tmp.top() ) );
		tmp.pop();
	}
	return *this;
}

// ��� �������� ������� ��������� ��������� �� �����������!

// ------------------- ������������� ����� -------------------
// ����� T() ������������ ������������� �� ��������� ��� �������
// � ������������� ����� ��� ������� �����
template <typename T>
class init_zero{
public:
	// ������������� ���������� ��������� �����
	init_zero() : _x() {}
private:
	T _x;
};

// ------------------- �������� �������� � �������� ���������� -------------------
// �������������� ������� � ��������� ��������� ������ � ������, ���� ��� ������ ���������� �� ��������

// 1. ������ � ��������� �� ������
template <typename T>
inline const T& str_max_ref( const T& a,const T& b ){
	return (a > b) ? a : b;
}

// 1. ������ � ��������� �� ��������
template <typename T>
inline const T str_max_val( const T a,const T b ){
	return (a > b) ? a : b;
}

// ������������ ��������������
template <typename T>
inline void show_ref( const T& a){
	// ������� �������� ���������
	cout << "str_max_ref() param type is: " << typeid(a).name() << endl;
}

template <typename T>
inline void show_val( const T a ){
	// ������� �������� ��������� (��� ������������)
	cout << "str_max_val() param type is: " << typeid(a).name() << endl;
}
