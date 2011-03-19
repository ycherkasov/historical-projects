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
