#pragma once

// ��������� ����� - ��� �����, �� �������� ��������� ������������
// ����������� ������� ��� ����� ���, �� ���� ������� "���������� �����"
//template <typename T> // ������ �� �������� � ��������?
class finalizer {
	friend class final;
	finalizer(){}
	finalizer(const finalizer& f){}
	
};

class final : public virtual finalizer/*<final>*/ {
public:
	final(void);
	~final(void);
};
