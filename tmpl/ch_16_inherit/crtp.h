#pragma once
// ����� 16.3 ����������
// ��������� ������������ ������ (Curiously recurrint template pattern)

// ���� ������� ������� � �������� ������-���������� ������ �� ������� � �������� ���������
// ����������� � �������� ����������� ������������� ����
template <typename T>
class object_counter {
private:
	static size_t count;
public:
	object_counter(){ ++object_counter<T>::count; }
	~object_counter(){ --object_counter<T>::count; }
	static size_t live(){return count;}
};
template <typename T>
size_t object_counter<T>::count = 0;

// count1 � count2 - ��������� ������ ����, � ��������� �� ������ ������� 
// (������ ��������� ������ �������)
class count1 : public object_counter< count1 >{};
class count2 : public object_counter< count2 >{};

// � ������� CRTP ����� ����������� ����������� �����������

template <typename Derived>
class BasicWorker{
public:

	void work() {
		// ����� ����� ��� ��� ����������� �������
		get_derived().work(t);
	}

	const Derived& get_derived() const {
		static_cast<const Derived&>(this);
	}

	Derived& get_derived() {
		return static_cast<Derived&>(this);
	}

	// ��� ������
};

class SpecificWorker1 : public BasicWorker<SpecificWorker1> {
public:

    int work() const {
		// �������������
		return 1;
	}
};

class SpecificWorker2 : public BasicWorker<SpecificWorker2> {
public:

    int work() const {
		// �������������
		return 2;
	}
};

/*
����� ���� ������������ CRTP? ��������, ����� ���� ��� ������, ������� �� ������� ���� ���������� 
� ���� �������� ����������� ������ �������� ������, �� ������� ����� ����� ���. 
����������� ����������� ��������� ���� ��� ��������������, ������� ����� ���� ���. 
�����: ��� ��������� �������� �� ����� ����� VMT, ��� ����������� ��� inlining'�, 
��� ���������� ������ ������� ��-�� ��������� �� VMT. 
������: ��������� ���� ���������� ������ � �����, ���������� ����. 
�� �� ������ ������� ��� ���� ���������� � ���������� ��������� �������, 
������� ��� ��������� ������� ������������ �������� ������� �������������.
*/