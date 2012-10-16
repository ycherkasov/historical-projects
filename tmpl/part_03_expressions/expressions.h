#pragma once

// �������� ������� �������� ���������� - �������� �� �������� ��� �� ������

template <typename T> class A_Scalar;

// ��������� ������ - �� ��������� �������� �� ������ (��� ��������)
template <typename T>
struct A_Traits{
	typedef T const& ExprRef;
};


// ������������� ��� �������, �������� �� ��������
template <typename T>
struct A_Traits< A_Scalar<T> >{
	typedef A_Scalar<T> ExprRef;
};

// ������ ������� - � �������� "��������" ��� ��������� ���������
// ����� ������� ������ ��� ����� ��� ������
template <typename T>
class A_Scalar{
private:
	T const& val;
public:
	A_Scalar(T const& v):val(v){}

	T operator[](size_t) const {return val;}
	T const& rep() const {return val;}
	size_t size() const {return 0;}
};

// ������� ��� ���������� ��������� ���������

// +
// �������� �������� � ���������� �����������
template <typename T, typename OP1, typename OP2>
class A_Add{
private:
	// ������ �� ��������
	typename A_Traits<OP1>::ExprRef op1;
	typename A_Traits<OP2>::ExprRef op2;
public:

	// ����������� - ������������� ������
	A_Add(OP1 const& a, OP2 const& b)
		:op1(a),op2(b) {}

	// "����������" ���������� ��������
	T operator[](size_t i) const {
		return op1[i] + op2[i];
	}

	size_t size() const {
		//assert(op2.size() == 0);
		return op2.size();
	}

};

// *
// �������� �������� � ���������� �����������
template <typename T, typename OP1, typename OP2>
class A_Mult{
private:
	// ������ �� ��������
	typename A_Traits<OP1>::ExprRef op1;
	typename A_Traits<OP2>::ExprRef op2;
public:

	// ����������� - ������������� ������
	A_Mult(OP1 const& a, OP2 const& b)
		:op1(a),op2(b) {}

	// "����������" ���������� ��������
	T operator[](size_t i) const {
		return op1[i] * op2[i];
	}

	size_t size() const {
		return op2.size();
	}

};