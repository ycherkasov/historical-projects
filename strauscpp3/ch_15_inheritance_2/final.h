#pragma once

// ��������� ����� - ��� �����, �� �������� ��������� ������������
// ����������� ������� ��� ����� ���, �� ���� ������� "���������� �����"
//template <typename T> // ������ �� �������� � ��������?
class inheritance_locker {
	friend class finalizer;
	inheritance_locker(){}
	inheritance_locker(const inheritance_locker& f){}
	
};

class finalizer : public virtual inheritance_locker/*<final>*/ {
public:
	finalizer(void);
	~finalizer(void);
};
