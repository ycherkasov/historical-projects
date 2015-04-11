#pragma once

/**@brief  
�������� ����� ��� �������� � ��������� �������� � ����������
��������� �������� � ����������� ������������� ���������
*/
class tracer
{
public:

	tracer(int v = 0);
	~tracer();

	// ������� �������:
	static long creations() {return s_created;}
	static long copies() {return s_copied;}
	static long assigmennts() {return s_assigned;}
	static long comparasions() {return s_compared;}
	static long max_objects() {return s_max_objects;}

	// ������� �����������
	tracer(const tracer& t);
	tracer& operator=(const tracer& t);
	bool operator<(const tracer& t) const;
	bool operator>(const tracer& t) const;
	void trace_msg_create() const;
	void trace_msg_destroy() const;
	void trace_msg_assign() const;
	void trace_msg_compare() const;

protected:

	// �������� ������������ ���������� ��������
	static inline void update_max_objects();
private:
	// �������� ��� ����������
	int _val;

	// ��������� ������������� (���������� �����������)
	int _generation;

	// ������� ��������
	static long s_objects;

	// ������� ��������
	static long s_created;

	// ������� �����������
	static long s_copied;

	// ������� ����������
	static long s_assigned;

	// ������� ���������
	static long s_compared;

	// ������������ ���������� ��������
	static long s_max_objects;
};
