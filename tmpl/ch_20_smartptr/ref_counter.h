#pragma once
#include <iostream>

using std::cout;
using std::endl;

// ����� �������� ����� �������� ������ 
// � ��������� �������-��������� ��� ����������� ������� ��������� ������ ���������

size_t* alloc_counter(){
	return new size_t;
}

void free_counter(size_t* c){
	delete c;
}

// ����� �������� ������ �������� ������ ��� �������� ��� ������ (����� ����� ����� ���� ����������)
// ����� ����������� ������ ��������, �������� ���������������� � ������������ � ��  
class simple_ref_count{
public:

	// ����������� �����������
	simple_ref_count():_counter(){}

	// ����������� ����������� �� ��������� ������ ��������

	// ������������� �������� (���-�� ������ ������)
	template <typename T>
	void init(T*){
		_counter = alloc_counter();
		(*_counter) = 1;
		cout << _counter << ", counter = 1" << endl;
	}

	// ������������ �������� (���-�� ������� ��������� ����� �������)
	template <typename T>
	void dispose(T*){
		free_counter(_counter);
		cout << _counter << ", dispose counter" << endl;
	}

	// ��������� (���-�� ���������� ������)
	template <typename T>
	void increment(T*){
		(*_counter)++;
		cout << _counter << ", counter = " << (*_counter) << endl;
	}

	// ��������� (���-�� ������ ������)
	template <typename T>
	void decrement(T*){
		(*_counter)--;
		cout << _counter << ", counter = " << (*_counter) << endl;
	}

	// �������� �� 0
	template <typename T>
	bool is_zero(T*){
		return ((*_counter) == 0);
	}


private:
	size_t* _counter;
};

// ����� ����������� ��������� �������� �������
struct standard_object_policy{
	template <typename T>
	void dispose(T* ptr){
		delete ptr;
	}
};


// ����� ����������� ��������� �������� ������� ��������
struct standard_array_policy{
	template <typename T>
	void dispose(T* ptr){
		delete[] ptr;
	}
};



