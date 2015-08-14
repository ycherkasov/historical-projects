#pragma once
#include <iostream>
using std::cout;
using std::endl;
// ����� 16.1 ����������
// ����������� ��������� �������

// ���� ����� ����� ������� ���������� ���������� �� ���������
// ��� �� ��������������� ������� ��������� ���� �� ���
struct DefaultPolicy1{ 
	static void doprint(){
		cout << "DefaultPolicy1::doprint" << endl;
	} 
};

struct DefaultPolicy2{ 
	static void doprint(){
		cout << "DefaultPolicy2::doprint" << endl;
	} 
};

struct DefaultPolicy3{ 
	static void doprint(){
		cout << "DefaultPolicy3::doprint" << endl;
	}
};

struct DefaultPolicy4{ 
	static void doprint(){
		cout << "DefaultPolicy4::doprint" << endl;
	} 
};

struct CustomPolicy{ 
	static void doprint(){
		cout << "CustomPolicy::doprint" << endl;
	} 
};

// ����� bread_slicer ��������� ����� ��������� �� ���������
// ����: ��������� ������ ���������, ������������ �� ���������
template<	typename Policy1 = DefaultPolicy1,
			typename Policy2 = DefaultPolicy2,
			typename Policy3 = DefaultPolicy3,
			typename Policy4 = DefaultPolicy4 >
class bread_slicer{
public:
	bread_slicer(){}
};

// ------------------- ������� -------------------
// ��������� ���� �� ��������� � ������� ������

// ����� discriminator ����� ������ ��� ����, ����� ���������� ��������� ����������
// ������� � �������� ������� (���������� ������ �������)
template <typename Base, int D>
class discriminator : public Base{};

template <typename Setter1, typename Setter2, typename Setter3, typename Setter4 >
class policy_selector : public discriminator<Setter1, 1>,
						public discriminator<Setter2, 2>,
						public discriminator<Setter3, 3>,
						public discriminator<Setter4, 4>{};

class default_policies{
public:
	typedef DefaultPolicy1 P1;
	typedef DefaultPolicy2 P2;
	typedef DefaultPolicy3 P3;
	typedef DefaultPolicy4 P4;
};

// ���������� ��� ����, ��� typedef ����������� �������� ������� � �����������
// (��. ch_09)

// ��������� ���������
class default_policy_args : virtual public default_policies{};

// �������������� ��� P1 ��� Policy (�������� �������)
template <typename Policy>
struct policy1_is : virtual public default_policies{
	typedef Policy P1;
};

// �������������� ��� P2 ��� Policy
template <typename Policy>
struct policy2_is : virtual public default_policies{
	typedef Policy P2;
};

// �������������� ��� P3 ��� Policy 
template <typename Policy>
struct policy3_is : virtual public default_policies{
	typedef Policy P3;
};

// �������������� ��� P4 ��� Policy 
template <typename Policy>
struct policy4_is : virtual public default_policies{
	typedef Policy P4;
};

// policyN_is � default_policy_args ��������� �� default_policies
// policyN_is �������������� ���� �� ��������� 
// �������� ������� ������������� (��. 10.2/6 ���������)


template <	typename PolicySetter1 = default_policy_args,
			typename PolicySetter2 = default_policy_args,
			typename PolicySetter3 = default_policy_args,
			typename PolicySetter4 = default_policy_args >
class bread_slicer2{
public:

	typedef policy_selector<PolicySetter1,PolicySetter2,PolicySetter3,PolicySetter4> policies;

	bread_slicer2(){}

	void doprint(){
		// ��������� � ��������� ����������
		policies::P1::doprint();
		policies::P2::doprint();
		policies::P3::doprint();
		policies::P4::doprint();
		// �.�. ������ ��������� �����������, ������ ������������ �� ��������
	}
};

// ������ ��� �������������� ����� �������� ���:
void show_name_params(){
	// ������������� � ����� ������� ����������
	bread_slicer< DefaultPolicy1, DefaultPolicy2, CustomPolicy > bs1;

	// ������������� "��������" Policy Selector - ������������� ��������� �3
	bread_slicer2< policy3_is<CustomPolicy> > bs2;

	// �������� ���������
	bs2.doprint();
}