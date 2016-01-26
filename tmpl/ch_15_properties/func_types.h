#pragma once
// ����� 15.2 ����������
// ������� ����

// ������� ���� - ��� �������, ����������� ���� � ������������� ��� ��� ���������
// ��������, ������ ����
template <typename T>
size_t type_size(const T& t) {
	return sizeof(T);
}

// �����������, �������� �� ������ ��� ������� (������� �� SFINAE)
template <typename T>
class is_class{
private:
	typedef char One;
	typedef struct{char a[2];} Two;
	template <typename C> static One test(int C::*);
	template <typename C> static Two test(...);
public:
	enum{ Yes = sizeof(is_class<T>::test<T>(0)) == 1 };
	enum{ No = !Yes };
};

// ���� ��� ������������
class MyClass{
};

struct MyStruct{
};

union MyUnion{
};

void MyFunc(){

}

enum E {e1} e;

