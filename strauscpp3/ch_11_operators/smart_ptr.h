#pragma once

// � ���������� smart ptrs ����� ��������� ��� ������ ��������� � ������� � ������� 
// ���������� ��������� ���������� ���� (������ 2-28)
template <typename T>
class simple_ptr{
public:

	// �����������, ����������� ���������� �������
	explicit simple_ptr(T* mem = nullptr) :_ptr(mem){}

	// �������� �������������� simple_ptr �
	// simple_ptr ������ ����! (������� ���� ��������� ���������� � ����)
	// ���������� ������� ������, ����� �������� ��� �������
	template <typename NewType>
	operator simple_ptr<NewType>(){
		return simple_ptr<NewType>(_ptr);
	}

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



