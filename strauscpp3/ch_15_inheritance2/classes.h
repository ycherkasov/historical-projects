/** @brief 
����� 12 �����������
����������� ������, �����������, ����������� �������
����������� ������
*/
#include <iostream>
#include <string>
#include <set>

using namespace std;

// ������� ������������

// ������� �����,����������
class Employee{

public:
	Employee(const char* n);
	~Employee();

	void show_name();
protected:
	// ����� �������� ������ �� ������������ ������
	int _salary;
private:
	string _name;
	short _departament;
};

// ����������� �����, �������� (������� � ����� ������� �.�. ���������)
// ������������ �������� - ����������� ������ �� �������
// (�.�. ��������� � ������������ �� ���������)
class Manager : public Employee {
	
public:

	// ����������� �� ����������� - �� ��������� �� ���������
	// ��� ����������� ����
	Manager(const char* n, short l, int s);

	// ����� �� ����������� ��������� ������������ � ������������� �������
	
	// ����������� ���������� � �������, �������� ������������, � �� �����������
	~Manager();
	
	// Manager::show_name() �������� ����� �������� ������
	void show_name();
	
	void show_level();
protected:
	// ����������� ���� ����� ��������� �� ����� � ������ ��������
	// ���� ��� ������ �� �������������
	int _salary;
private:
	/** @brief ��������� ����������� */
	set<Employee*>	_group;
	short			_level;
};

