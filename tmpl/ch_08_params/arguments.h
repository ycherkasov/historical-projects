#pragma once

// --- ��������� �������� ������� ---
// ��������:
//- ���� <int, int>
//- ������ ������ (type_t a; X<a>)
//- �� ���������
//- ����� ���������� (������� ����������� ���������� �������, <>)

// ���������� ��������� �������� ������� ���������� �������� ����� ������, ��������:
template <typename DestType, typename SrcType>
inline DestType implicit_cast(SrcType t){
	return static_cast<DestType>(t);
}

// SrcType ����� �������, � DestType - ���
// ����� ���������� ������� ��������� ����, ������� �������,
// ����� �� ���� �� ����� ������ �������
template <typename SrcType, typename DestType>
inline DestType implicit_cast2(SrcType t){
	return static_cast<DestType>(t);
}

// ��� ���������� ��������� ������� �� ����� ����� ���������� ���������� ����� ����������� ����������
// ��������, ��������� �������, ����������� ��������� �� ������� � �� ��������
template <typename Func, typename T>
inline void apply(Func fptr, T t){
	fptr(T);
}

// ��������� ������������� �������
template <typename T>
T multi(T t){
	return t;
}

template <typename T>
T multi(T* t){
	return *t;
}

void test_functions_family(){
	// ����������������� ��� ������
	// error C2784: 'void apply(Func,T)' : could not deduce template argument for 'overloaded function type' from 'overloaded function type'
	//apply(multi<int>, 3);
	//apply(multi<int>, 5);
	// ��� ������������� ������� ��������, ���� � ������� ������ ����������� �����
}

// ------------------- ��������� ����� -------------------
// ����������� ����� �� ����� ����:
// 1. ��������� ������ � ������������ (���������� � �������� �������)
// 2. ������������� ������ � ������������ (����� ����������� ����� typedef)
// ��������:
template <typename T>
class List_t{
public:
	List_t() : _t(){}
	List_t(const T& t) : _t(t){}
	T _t;
};

// ������������� ��������� � typedef 
// � ��� ���
struct  {
	double x;
} str1;

typedef struct  {
	double x;
} str2;

// ������������� ������������ � typedef 
// � ��� ���
enum {red, green, blue} color1;
typedef enum {red1, green1, blue1} color_t;

void test_argument_types(){
	struct association{
		int* p;
	};
	// �� ������ ���������������, �� �������������
	List_t<association*> err1;

	// ������������� ��������� ���������� ������
	// � ����������� ����� typedef �����
	//List_t<str1> s1;
	List_t<str2> s1;

	// �� �� ��� ������������
	//List_t<color1> c1;
	List_t<color_t> c2;
}

// ������ ���������� �������� ������������ ���� ����� ���� 
// ��� ������� ��������� �� ����� ����������
