#pragma once
#include <new>

// � ���� ������ ��������������� �������� ������� 
// ��� ������ ����������������� ����������

// ������ 1 ������� 10
// operator new() � operator delete() ������������� ������ ��� ��������� �������������
// ��� �������� ����� ��� ����������, ����������� ����� ��������� ��������

// ������ 1 ������� 8
// ���������� ������� � �������� new
// ������� ������ �������� ������, �������� �������� �����������
// ������� delete() � �������� delete() ����������� �������� ��� ��

void show_new1();
void show_new2();
void show_new_delete(); 
void show_bad_alloc();

class user_allooc
{
public:
	user_allooc();

	// ����� ����������� ������ ��������� �������� � operator new
	// ���������� ������ ���� �����������
	virtual ~user_allooc();
	// ������ 1 ������� 7
	// ��������������� ��� ������ operator new() �����������,
	// ��� ��� ��� ����������� ����� ������ ������ ��
	// ������ 1 ������� 8
	// operator new() ����������� ������������ ��������
	// ������� operator new() � ������� ������ ����� ������������� ��� �����������,
	// ���� ������ �������� ��� ��� ����������� operator new()
	// ����� ���������� ����������� operator new[]()
	static void* operator new(size_t s);

	// ��� ����������� ������������ ������� operator delete() ����� �������� ������������ �������
	static void operator delete(void* p);

	// ������������� ����� ����������� ��������� new
	// ������ 1 ������� 9
	// � �������� new ����� ���� ��������� ������� ������ ����������
	// ���� ����������� �����-���� ������ ����� new, ����� ����������
	// (����������, � ���������� �� ����������),
	// �� ���������� ����������� ������ ���� �����������
	// ����� ������������� ��������� ������� ���������� new()
	static void* operator new(size_t s, void* p);
	
	// pair delete operator for the placement new
	static void operator delete(void *p, void *) {}
	
	static void operator delete(void* p, size_t s);

	// ����� new � ���������� new_handler
	static void* operator new(size_t s, new_handler p);


};

// ����� ��� ������� ����������� ������ ����������� ���� ���������� set_new_handler()
// ����� ������� �����-������� (mixture) ����� ����������� �� ���� �������� � ���������� new
// ��� ��������������� ����� ��
class NewHandlerSupport{
public:
	static new_handler set_new_handler(new_handler p);
	static void* operator new(size_t size);
	static void operator delete(void* p);
private:
	static new_handler _current_handler;
};



// �����-��� ������
class memory_pool{
public:
	memory_pool(size_t n);
	void* alloc(size_t n);
	void free(void* p, size_t n);
	~memory_pool();
};

