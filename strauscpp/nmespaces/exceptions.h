#pragma once
#include <exception>
#include <iostream>

// ��������� ����������
struct range_error_t : public std::exception{
	int _range;

	// ����������� ��������� ����� ������� ��� �������
	range_error_t(int r) throw() { // �� ������� ����������
		std::cout << "range_error_t(int r)\n";
		_range = r;
	}

	// ����������� ����������� ���������� ������ ���� ������ �� ��������
	range_error_t(const range_error_t& r)  throw()  { // �� ������� ����������
		std::cout << "range_error_t(const range_error_t& r)\n";
		_range = r._range;
	}
	// ������ ���������� ����� ������������ ������� ������ ���, 
	// � ��� ����������� �� ������ � ���� ������� ������� ���������� 
	// (���� ������ � ��������� ����� ����� �� terminate).
	// �������� ������������ std::string ���������� �������, 
	// ����� ������������� �� ��������� string() throw();
};

/* ����� shared_ptr � ������� �������, �.�. ���������� ����� ������� 
	�� �� ������ �������, � �� ������ ���������. � ��� ���������� ��
struct my_exception
{
	shared_ptr<string> m_pStr;
public:
	explicit my_exception(const string& str) : m_pStr(new string(str)) {}
	const string& what() const { return *m_pStr; }
};
*/

void show_exceptions();
void test_overflow(short a, short b);

