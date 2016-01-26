#pragma once

// ������� ����� ��������� ��� ����������� � �������� ��������
// ������ scoped_ptr
template <typename T>
class simple_ptr{
public:

	// ����������� �� ���������
	simple_ptr():_ptr(){}

	// �����������, ����������� ���������� �������
	explicit simple_ptr(T* mem):_ptr(mem){}

	// ������������ ������
	~simple_ptr(){
		delete _ptr;
	}

	// �������� ���������
	T* operator->() const {
		return _ptr;
	};

	T& operator*() const {
		return *_ptr;
	};

	// �������������� ������ ��������� ->
	T* get() const {
		return _ptr;
	};

	// ������������ �� ��������
	T* release() {
		T* ptr = _ptr;
		_ptr = 0;
		return ptr;
	};

private:
	T* _ptr;
};