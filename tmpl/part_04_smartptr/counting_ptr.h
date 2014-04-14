#pragma once
#include "ref_counter.h"

// ����� ������ ��������� �� ��������� ������� ��������� �� ������� ���������
// ������������ ����� �������� �������� � ��������� �������� �������
template <typename T,
	typename ref_count_policy = simple_ref_count,
	typename deletion_policy = standard_object_policy
>
class counting_ptr : ref_count_policy, deletion_policy{
	typedef ref_count_policy RCP;
	typedef deletion_policy DP;
public:
	// ����������� �� ���������
	counting_ptr():_ptr(){}

	// �����������, ����������� ���������� ������� � ��������� �������
	explicit counting_ptr(T* mem):_ptr(mem){
		this->init(mem);
	}

	// ���������� �����������, ������������� �������� ��������
	counting_ptr(const counting_ptr& rhs)
		: ref_count_policy(rhs)
		, deletion_policy(rhs){
		
			this->attach(rhs);
	}

	// ���������� ��������� �������� ��������
	~counting_ptr(){
		this->detach();
	}

	// �������� ���������� � ��������� �� this
	// ����������� �������� �������� ��� �������������� �������
	// ��������� ��� ����
	counting_ptr& operator=(const counting_ptr& rhs){
		if(this->_ptr != rhs._ptr){
			// ���������� �������� ��� ����
			this->detach();

			// ����������� ������ � ���������
			ref_count_policy::operator=(rhs);
			deletion_policy::operator=(rhs);
			this->attach(rhs);
		}
		return (*this);
	}

	// �������� ��������� (�����������)
	inline const T* operator->() const {
		return _ptr;
	};

	inline const T& operator*() const {
		return *_ptr;
	};

	// �������� ��������� (�������������)
	inline T* operator->() {
		// ������������� ��������! ����������� ������, ������� �����
		this->detach();
		this->init(this->clone());
		return _ptr;
	};

	inline T& operator*() {
		// ������������� ��������! ����������� ������, ������� �����
		this->detach();
		this->init(this->clone());
		return *_ptr;
	};

private:
	// ���������� �������-�������

	// ������������� ����������
	void init(T* p){
		if(p){
			// ��� ������ ������ �������, 
			// � ����� ������ ����� �� ����� ����� �������
			ref_count_policy::init(p);
		}
		this->_ptr = p;
	}

	// ����������� ��������� � ��������� ��������
	void attach( counting_ptr<T, RCP, DP> const& rhs ){
		this->_ptr = rhs._ptr;
		if(rhs._ptr){
			ref_count_policy::increment(rhs._ptr);
		}
	}

	// ���������� �������� � ������������ ����  == 0
	void detach(){
		if(this->_ptr){
			ref_count_policy::decrement(this->_ptr);

			// �������� �� 0 - ������� ������� � ������� ��� ������
			if(ref_count_policy::is_zero(this->_ptr)){
				ref_count_policy::dispose(this->_ptr);
				deletion_policy::dispose(this->_ptr);
			}
		}
	}

	// �������� ������ ��� ������������
	T* clone(){
		return new T(*_ptr);
	}

private:
	T* _ptr;
};