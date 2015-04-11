#pragma once

// ������� ������ ��� ���� ����� - �� ��������� �� ���������������
template <typename T>
struct is_fundamental{
	static bool result(){return false;}
};

// ������ ��� ����������� �������������� �����
#define MAKE_FUNDAMENTAL(T) \
template <> \
struct is_fundamental<T>{ \
	static bool result(){return true;} \
}; \

// ������������� ��� ���������������� �����
MAKE_FUNDAMENTAL(void);

MAKE_FUNDAMENTAL(char);
MAKE_FUNDAMENTAL(unsigned char);

MAKE_FUNDAMENTAL(short);
MAKE_FUNDAMENTAL(unsigned short);

MAKE_FUNDAMENTAL(int);
MAKE_FUNDAMENTAL(unsigned int);

MAKE_FUNDAMENTAL(long);
MAKE_FUNDAMENTAL(unsigned long);

MAKE_FUNDAMENTAL(float);
MAKE_FUNDAMENTAL(double);
MAKE_FUNDAMENTAL(long double);

