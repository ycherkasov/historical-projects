#pragma once

// ����� 15.1 ����������
// ��������� � ��������

// ------------------- ������� ����������� -------------------
// ��� �����, �������������� ����������� ���������������
template <typename T>
inline T simple_accumulate(const T* begin, const T* end){
	
	// ���������� � ��������������
	T total = T();

	// ������������
	while( begin != end ){
		total += *begin;
		++begin;
	}
	return total;
}
// ���������� � ���, ��� ������������ ��� �� ������� �� ������������
// �������� � ������ char (byte)

// ------------------- ������������� ������ ������� -------------------
// ����� � ����� ������� ������������� ���� � ����������� �� ������������
// ������ ������ ������� (traits), ������� ���������� ������������ ��� � ����������� �� �����������
template <typename T>
class simple_accumulator_traits{};

// ���� ���� ��������� � ������� ��������� �������������
template <>
class simple_accumulator_traits<char>{
public:
	typedef int AccType;
};

template <>
class simple_accumulator_traits<short>{
public:
	typedef int AccType;
};


template <>
class simple_accumulator_traits<int>{
public:
	typedef long AccType;
};

template <>
class simple_accumulator_traits<unsigned int>{
public:
	typedef unsigned long AccType;
};

template <>
class simple_accumulator_traits<float>{
public:
	typedef double AccType;
};

// ����� ������� ������ ���
template <typename T> inline typename 
simple_accumulator_traits<T>::AccType traits_accumulate(const T* begin, const T* end){

	// ���������� � ��������������
	typedef typename simple_accumulator_traits<T>::AccType AccT;
	AccT total = AccT();

	// ������������
	while( begin != end ){
		total += *begin;
		++begin;
	}
	return total;
}


// ------------------- ��������-�������� -------------------
// ����� �����, traits ����� ���������� ����� ��������
// ��������, ������� ��������, ������� ����������� ������������� �� ���������
// ����� ���� �������� � �������� � ������� traits
template <typename T>
class value_accumulator_traits;

template <>
class value_accumulator_traits<char>{
public:
	typedef int AccType;
	static AccType zero(){ return 0; }
};

template <>
class value_accumulator_traits<short>{
public:
	typedef int AccType;
	static AccType zero(){ return 0; }
};


template <>
class value_accumulator_traits<int>{
public:
	typedef long AccType;
	static AccType zero(){ return 0l; }
};

template <>
class value_accumulator_traits<unsigned int>{
public:
	typedef unsigned long AccType;
	static AccType zero(){ return 0u; }
};

template <>
class value_accumulator_traits<float>{
public:
	typedef double AccType;
	static AccType zero(){ return 0.; }
};

// ����� ������� ������ ���
template <typename T> inline typename 
value_accumulator_traits<T>::AccType traits_vals_accumulate(const T* begin, const T* end){

	// ���������� � ��������������
	typedef typename value_accumulator_traits<T>::AccType AccT;
	AccT total = value_accumulator_traits<T>::zero();

	// ������������
	while( begin != end ){
		total += *begin;
		++begin;
	}
	return total;
}

// ------------------- ����������������� ��������  -------------------
// ����������� ���� �������� ���������� ��������������, �.�. �� ������ �������� � ���������
// �������� ����������� � �������� ��������� ������� �� ��������� �� ���������
// (�.�. � ����������� ������� ����������� �������� ������������� ���)
// ����� ������� �������� �������� �� �����, �.�. ������� �� ������������ �������� 
// ���������� ��������� �� ���������
template <typename T, typename AccTraits = value_accumulator_traits<T> >
class custom_accumulator_traits{
public:
	static typename AccTraits::AccType accum(const T* begin, const T* end){
		// ���������� � ��������������
		typename AccTraits::AccType total = AccTraits::zero();

		// ������������
		while( begin != end ){
			total += *begin;
			++begin;
		}
		return total;
	}
};

// ------------------- ��������� -------------------
// ��������, ��������������� ����� ���� �� ������ �������������
// �����, ��������, �������������� ����������. ��� ����� ������ ������� 
// ��������� ��������������� (accumulation policy)
class SumPolicy{
public:
	template <typename T1, typename T2>
	static void accumulate(T1& total, const T2& value){
		total += value;
	}
};

class MultPolicy{
public:
	template <typename T1, typename T2>
	static void accumulate(T1& total, const T2& value){
		total *= value;
	}
};

template <typename T
, typename Policy = SumPolicy
, typename AccTraits = value_accumulator_traits<T> >
class strategy_accumulator_traits{
public:
	static typename AccTraits::AccType accum(const T* begin, const T* end){
		// ���������� � ��������������
		typename AccTraits::AccType total = AccTraits::zero();

		// ������������
		while( begin != end ){
			Policy::accumulate(total, *begin);
			++begin;
		}
		return total;
	}
};

// ------------------- ��������� � ���������� ����������� -------------------
// ����� �������� ������ ��������� �� �������
// � ������� ������ �������� � ����� ��� ���������� ��������
template <typename T1, typename T2>
class SumPolicyT{
public:
	static void accumulate(T1& total, const T2& value){
		total += value;
	}
};

template <typename T1, typename T2>
class MultPolicyT{
public:
	static void accumulate(T1& total, const T2& value){
		total *= value;
	}
};

template <typename T
, template<typename,typename> class Policy = SumPolicyT
, typename AccTraits = value_accumulator_traits<T> >
class strategy_tmpl_accumulator_traits{
public:
	static typename AccTraits::AccType accum(const T* begin, const T* end
		,typename AccTraits::AccType total_1st = AccTraits::zero() ){
		
			// ���������� � ��������������
			typedef AccTraits::AccType AT;
			AT total = total_1st;

			// ������������
			while( begin != end ){
				Policy<AT,T>::accumulate(total, *begin);
				++begin;
			}
			return total;
	}
};


