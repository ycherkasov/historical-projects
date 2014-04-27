#pragma once
#include <new>

// � ���� ������ ��������������� �������� ������� 
// ��� ������ ����������������� ����������

// ������ 1 ������� 7
// ����� �������� ����������� ���������� �� �������� ������
// ���������� set_new_handler() ����������, 
// ���� ������ �� ����� ����������� ��� ��������� �� ���������
// ���������������� ���������� �����:
// * ������� ��������� ����������� ���������� ������
// * ���������� ������ ���������� new
// * ���������� ������� ���������� (�.�. ���������� bad_alloc) ��� ������� bad_alloc �������
// * ��������� ��������� �� abort()/exit()

// ����� ��� ������� ����������� ������ ����������� ���� ���������� set_new_handler()
// ����� ������� �����-������� (mixture) ����� ����������� �� ���� �������� � ���������� new
// ��� ��������������� ����� ��
// ���������� ������� � CRTP, ����� �������� ��������, ��� ������ ������� ����� ��������
template <typename T>
class NewHandlerSupport{
public:
	static new_handler set_new_handler(new_handler p);
	static void* operator new(size_t size);
	static void operator delete(void* p);
	static void no_more_memory();
private:
	static new_handler _current_handler;
};

// Mixtury class defenition that replaces standard
// new_handler
template <typename T>
new_handler NewHandlerSupport<T>::_current_handler = &NewHandlerSupport<T>::no_more_memory;


template <typename T>
new_handler NewHandlerSupport<T>::set_new_handler(new_handler p)
{
	// ����� ���� ��� ����������� set_new_handler
	new_handler old_handler = _current_handler;
	_current_handler = p;
	return old_handler;
}


template <typename T>
void NewHandlerSupport<T>::no_more_memory(){
	// � ����� ��������� ����������� ������ ������� ���������
	// � ������ �������� ��� ����������� new_handler
	printf("No more memory!\n");
	throw bad_alloc();
}

// ������ 2 ������� 8
// ���������� �������� new - �� �������� ������ � �������� �����������. ��� �������������� ������
// � ������� operator new() - ��� �������� ������ ��������� ������, � �������������� �� �����
// ���������� � ���������� delete � �������� operator delete()


template <typename T>
void* NewHandlerSupport<T>::operator new(size_t sz)
{
	// �������� ����������� ���������� (0 ���� throw bad_alloc)
	new_handler global_handler = std::set_new_handler(_current_handler);
	void* mem = nullptr;

	try{
		// �������� ������ ���������� ����������
		mem = ::operator new(sz);
		// ���� �������� ������ - ��������� _current_handler()
		// ���� �� = 0, �������� bad_alloc
	}
	catch (const std::bad_alloc&){
		// ������ �� ����� ������ ���������� new_handler
		std::set_new_handler(global_handler);
		throw;
	}
	set_new_handler(global_handler);
	return mem;
}


template <typename T>
void NewHandlerSupport<T>::operator delete(void* p){
	::operator delete(p);
}


// ��������� mixture-class
class LargeObject : public NewHandlerSupport<LargeObject> {
public:
	LargeObject(){}
	~LargeObject(){}
private:
	// very large array, will throw out of memory
	char large_array[2147483646];
};

// ��� ��� ��������� mixture-class
class SmallObject : public NewHandlerSupport<SmallObject> {
public:
	SmallObject(){}
	~SmallObject(){}
private:
	// everything ok should be here
	char large_array[42];
};


// ������ 1 ������� 7
// ��������������� ��� ������ operator new() �����������,
// ��� ��� ��� ����������� ����� ������ ������ ��
// ������ 1 ������� 8
// operator new() ����������� ������������ ��������
// ������� operator new() � ������� ������ ����� ������������� ��� �����������,
// ���� ������ �������� ��� ��� ����������� operator new()
// ����� ���������� ����������� operator new[]()
// ������ 1 ������� 9
// � �������� new ����� ���� ��������� ������� ������ ����������
// ���� ����������� �����-���� ������ ����� new, ����� ����������
// (����������, � ���������� �� ����������),
// �� ���������� ����������� ������ ���� �����������
// ����� ������������� ��������� ������� ���������� new()
// ����������� �������� operator new � ����������� 
// �������� operator delete ������ ���������� ������ ������
// ��� ������ ����������� ������� ������������� ������ ��� ������
// void operator new(size_t, P2, P3, ..., Pn);
// ������ ������������ ����������� ������� ������������ ������
// void *operator delete(void *, P2, P3, ..., Pn);
// ��� ���� ����������� �������� delete ���������� ������ ��� 
// �������� �������� ���������� ������� (throw �� ������������). 
// ��������� ����������������� ������ ��������� � ������� ��������� delete � 
// � ��� ��������� �������� ������������� operator delete

class user_alloc
{
public:
	user_alloc();

	// ��� ����������� ������������ ������� operator delete() ����� �������� ������������ �������
	virtual ~user_alloc();
	
	// 1. Simple new/delete overload
	static void* operator new(size_t s);
	static void operator delete(void* p);

	// 2. Array new/delete overload

	static void* operator new[](size_t s);
	static void operator delete[](void* p);


	// 3. Placement new/delete overload
	
	static void* operator new(size_t s, void* place);
	static void operator delete(void *p, void *place);

	// operator delete is unique in being a non-member or static member function that is dynamically dispatched
	// A type with a virtual destructor performs the call to its own delete from the most derived destructor

	// 4. New replacing new_handler
	static void* operator new(size_t s, new_handler p);
	static void operator delete(void *p, new_handler h);

	void test();
protected:

	// ����� ��� �������� ��������� ���� ���������� �������
	// ��� new � new[]. ��� ������������� ��������, �.�. � �������������
	// operator new (��� new[]) �� ������ �������� ������, � ������ �� ������ 
	// �� ������������������ ���������.
	// �� �������� ����� ���� ��������, ��������, � new() �� ����� ���������
	// s == sizeof(Type), � new[] - ��� ��� (������ ������� ����������)
	static void* universal_allocate(size_t s, new_handler handler = 0);
	static void universal_free(void* p);
private:
	int _test;
};

// ������ 1 ������� 8
// ���������� ������� � �������� new
// ������� ������ �������� ������, �������� �������� �����������
// ������� delete() � �������� delete() ����������� �������� ��� ��

// ������ 1 ������� 10
// operator new() � operator delete() ������������� ������ ��� ��������� �������������
// ��� �������� ����� ��� ����������, ����������� ����� ��������� ��������




