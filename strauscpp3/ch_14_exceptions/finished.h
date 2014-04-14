#pragma once

// ��������������� ������� ����������� ������� 
// terminate() � unexpected()
// � �� ��������������
typedef void (*handler_func_t)();

class finished
{
public:
	finished(void);
	~finished(void);

	// ������� ������������ unexpected()
	void unexpected();

	// ������� ������������ terminate()
	void terminate();

	// ��������� ������ ������������ � ������������ ����������
	void test_custom_unexpected();

	// ��������� ��������������� ����������
	void test_custom_terminate();
private:
	handler_func_t _old_unexpected;
	handler_func_t _old_terminate;
};
