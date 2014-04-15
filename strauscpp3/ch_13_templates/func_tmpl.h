#pragma once

// ------------------- ������� �� ������� ������ -------------------
// - �������� ���������� � �������� ���������
// - �������� ����� ����������
// - ���������� ��������� �� ���������
// - ���������� ��������� �������������

// ------------------- ����������, ������������� -------------------
// ������� ������������� ������� (����� ��� �������, 
// ������ ���� ��������� ����� �������� ��� ���������� ����������)
inline const int max(const int a, const int b){
	return (a > b) ? a : b;
}

// ��� ��������� ������������ �������� ��������

// typename ����� �������� �� class, �� ��� ����� �� ������
// ���� �������� ������� - ���, �� ����������� typename 
// typename T - ������������ �����
// T - �������� �������, a, b - ��������� ������

template <typename T>
inline const T& max(const T& a, const T& b){
	return (a > b) ? a : b;
}

// ������������� ��� ����� (���������������� ��������)
template <>
inline const char* const& max(const char* const& a, const char* const& b ){
	return ( std::strcmp(a, b) < 0 ) ? a : b;
}


// ��������� ������� �.�. ������������� (inline)

// ���� ������������� ��������� ����� ������������������ ��� ������,
// ���� ������ ������ ���������� ������������� ������ �������������
template <typename T> class Vector;
template <typename T> class Vector<T*>;
template <> class Vector<void*>;


// ------------------- ��������� �� ������� � ������ -------------------

void first_func(int i){
	cout << "First func: " << i << endl;
}

void second_func(int i){
	cout << "Second func: " << i << endl;
}

// ������ ��������� �� ������� void void_func_p(int);
typedef void (*void_func_p)(int);

// � ��������� �� ������, ����������� � �������
struct holder{
	void method(int i){
		cout << "Method: " << i << endl;
	};
	static void stat_method(int i){
		cout << "Static method: " << i << endl;
	}
};

typedef void (holder::* method_p)(int);

void show_func_pointers(){

	// ��������� �� ������� �������
	void_func_p f = &first_func;
	f(5);
	f = &second_func;
	
	// ����� ������� �� ���������
	f(6);
	(*f)(7);

	cout << "sizeof f: "<< sizeof(f) << endl;

	// ��������� �� �����
	method_p m = &holder::method;
	holder h;

	cout << "sizeof m: "<< sizeof(m) << endl;
	
	// ����� ������ �� ��������� - �������� .* ��� ->*
	(h.*m)(8);

	// �������� ������ ��� ������� ������� holder!

	// ����� ������ �� ��������� ��� ������������� �������
	holder* ph = new holder();
	(ph->*m)(9);
	delete ph;

	// ��������� �� ������� ����� ������ ��������� ��������� �� �������
	//m = f; (����������������� ��� ������)
	//f = &holder::method;

	// � �� ����������� - �����

	// ��������� �� ����������� �����
	void_func_p ms = &holder::stat_method;
	ms(10);

	// ����� ������ ��������� �� ����������� ��������� ��������
	//method_p m1 = ms;  (����������������� ��� ������)
}