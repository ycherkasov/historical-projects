#pragma once
#include <deque>

using std::deque;
// � ������ ��������� ������������������ ���������� ��������� ������������
// ��� ���������� �������� "�������"


class deque_t
{
public:
	deque_t();
	deque_t( const deque_t& c );
	deque_t& operator=( const deque_t& c );
	~deque_t();

	void push_back(int val);
	void push_front(int val);
	int pop_back();
	int pop_front();
	bool is_empty();
	void clear();

protected:
private:
	deque<int> _deque;
};

// ���� �� ����� ������������ _����������_ ��� _����������_ ,
// �.�. �� �� �����, ����� � ���������� ������ stack_t ���� ��������
// �������� ������ deque_t (�.�. � ���� ����� ��������� ������ � �����)
// �� ����� ������������ _��������_ ������������ - ��� �������� ����� ������ ������ private
// ���� �� �� ����� ������ ����������� �� stack_t - ����� ������������ protected
// �� ������, ��� � deque_t ���� protected � public, ����� private � stack_t.

// � ������ ��������� ������������ ����� �� �������� ���������� �� ������-��������� � �������� ������.

class stack_t : /*public*/ protected /*private*/ deque_t{
public:
	stack_t();
	~stack_t();

	void push(int val);
	int pop();
	bool is_empty();

protected:
	void test_pr();
private:	

	// ���������� ����������� � �������������
	stack_t(const deque_t& c);
	stack_t(const stack_t& c);
	stack_t& operator=( const stack_t& c );
	stack_t& operator=( const deque_t& c );

};

// �� �������, ��� � deque_t ���� protected � public ����� � write_only_stack_t protected.
class write_only_stack_t : public stack_t{
public:
	void push(int val);
};

// ���� �� ����� ������� ������ ����� ����������
// ����� �� �������������� ������ � ������ �������
// � ������� �� � ������� using
class another_stack_t : private deque_t{
public:
	// using ����� ���������� � ����� ����� ������, � ��� ��������� �� ���� �����
	using deque_t::push_back;
	using deque_t::pop_back;
};

// ����� ������� "�������" ����������� �����������. �� �������� 
// ��������� ��� ��� ������� �������� �������� ������������ �������� �����
class composite_t{
	deque_t d;
public:
	void push(int val){d.push_back(val);}
	int pop(){return d.pop_back();}
	bool is_empty(){ return d.is_empty(); }
};