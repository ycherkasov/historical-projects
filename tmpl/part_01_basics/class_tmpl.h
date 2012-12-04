#include <vector>
#include <list>
#include <string>
#include <iostream>
using namespace std;

// ����� 3 ����������
// ������� ������� �������

// ������ �������� ������

// ����������
template <typename T>
class stack{
public:
	stack();
	void push(const T& p);
	void pop();
	const T& top() const;
	bool empty() const;
private:
	std::vector<T> _stack;
};

// �����������

template <typename T>
stack<T>::stack(){}

template <typename T>
void stack<T>::push(const T &p){
	_stack.push_back(p);
}

template <typename T>
void stack<T>::pop(){
	_stack.pop_back();
}

template <typename T>
const T& stack<T>::top() const {
	return _stack.back();
}

template <typename T>
bool stack<T>::empty() const{
	return _stack.empty();
}

// ------------------- ������������� �������� ------------------- 

// ���������� ����������������� ��� ����� �������
// typename ����� �� ���������
template <>
class stack<std::string>{
public:
	stack();
	void push(const std::string& p);
	void pop();
	const std::string& top() const;
	bool empty() const;
private:
	// � ���� ������������� �������� ������ �� ������
	std::list<std::string> _stack;
};


//template <> � ����������� ������� �� �����
//����������, ��� ������� �����
stack<std::string>::stack(){}

void stack<std::string>::push(const std::string& p){
	_stack.push_back(p);
}

void stack<std::string>::pop(){
	_stack.pop_back();
}

const std::string& stack<std::string>::top() const {
	return _stack.back();
}

bool stack<std::string>::empty() const{
	return _stack.empty();
}

// ------------------- ��������� ������������� ------------------- 
// ����� ���������� ���������, �� ���������������� ���� �� ���
// �������� ������ ��� �������!
template <typename T1, typename T2>
class double_stack{
public:
	double_stack(){
		cout << "double_stack" << endl;
	}
	// ����� ������
private:
	std::vector<T1> _stack1;
	std::vector<T2> _stack2;
};

// �������� ��������� �������������
// 1. ������ �������� - int
template <typename T1>
class double_stack<T1, int>{
public:
	double_stack(){
		cout << "double_stack<T1, int>" << endl;
	}
	// ����� ������
private:
	std::vector<T1> _stack1;
	std::vector<int> _stack2;
};

// 2. ��������� ���������
template <typename T>
class double_stack<T, T>{
public:
	double_stack(){
		cout << "double_stack<T, T>" << endl;
	}
	// ����� ������
private:
	std::vector<T> _stack1;
	std::vector<T> _stack2;
};

// 2. ��������� - ���������
template <typename T1,typename T2>
class double_stack<T1*, T2*>{
public:
	double_stack(){
		cout << "double_stack<T1*, T2*>" << endl;
	}
	// ����� ������
private:
	std::vector<T1*> _stack1;
	std::vector<T2*> _stack2;
};

// ��� ��������� ������������� � ���������� ������� �� ����� ������ 
// ������ �����  � ��������� ClassType<T>::
// ������� ������� �� ����������������� ��� ��������� ��������������!

// ------------------- ��������� �� ��������� ------------------- 
// ��� �� ����, �� � ������������ �������� ���������� ����������
// �� ��������� - ������
// ��������� �� ��������� �.�. ���������� � ������ ����������
template <typename T, typename CONTAINER = std::vector<T> >
class custom_stack{
public:
	custom_stack();
	void push(const T& p);
	void pop();
	const T& top() const;
	bool empty() const;
private:
	CONTAINER _custom_stack;
};

// �����������
template <typename T, typename CONTAINER>
custom_stack<T,CONTAINER>::custom_stack(){}

template <typename T, typename CONTAINER>
void custom_stack<T,CONTAINER>::push(const T &p){
	_custom_stack.push_back(p);
}

template <typename T, typename CONTAINER>
void custom_stack<T,CONTAINER>::pop(){
	_custom_stack.pop_back();
}

template <typename T, typename CONTAINER>
const T& custom_stack<T,CONTAINER>::top() const {
	return _custom_stack.back();
}

template <typename T, typename CONTAINER>
bool custom_stack<T,CONTAINER>::empty() const{
	return _custom_stack.empty();
}
