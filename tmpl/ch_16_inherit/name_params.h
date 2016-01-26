#pragma once
#include <iostream>
using std::cout;
using std::endl;
// Глава 16.1 Джосаттиса
// Именованные аргументы шаблона

// Если класс имеет большое количество параметров по умолчанию
// при их переопределении удобнее указывать один из них
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

// Класс bread_slicer принимает набор стратегий по умолчанию
// Цель: указывать только стратегии, отличающиеся от дефолтных
template<	typename Policy1 = DefaultPolicy1,
			typename Policy2 = DefaultPolicy2,
			typename Policy3 = DefaultPolicy3,
			typename Policy4 = DefaultPolicy4 >
class bread_slicer{
public:
	bread_slicer(){}
};

// ------------------- решение -------------------
// Разместим типы по умолчанию в базовом классе

// Класс discriminator нужен только для того, чтобы передавать несколько одинаковых
// классов в качестве базовых (отличаются только номером)
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

// Используем тот факт, что typedef перекрывает параметр шаблона и наследуется
// (см. ch_09)

// Дефолтные стратегии
class default_policy_args : virtual public default_policies{};

// Переопределяем тип P1 как Policy (параметр шаблона)
template <typename Policy>
struct policy1_is : virtual public default_policies{
	typedef Policy P1;
};

// Переопределяем тип P2 как Policy
template <typename Policy>
struct policy2_is : virtual public default_policies{
	typedef Policy P2;
};

// Переопределяем тип P3 как Policy 
template <typename Policy>
struct policy3_is : virtual public default_policies{
	typedef Policy P3;
};

// Переопределяем тип P4 как Policy 
template <typename Policy>
struct policy4_is : virtual public default_policies{
	typedef Policy P4;
};

// policyN_is и default_policy_args наследуют от default_policies
// policyN_is переопределяет одну из стратегий 
// согласно Правилу доминирования (Гл. 10.2/6 Стандарта)


template <	typename PolicySetter1 = default_policy_args,
			typename PolicySetter2 = default_policy_args,
			typename PolicySetter3 = default_policy_args,
			typename PolicySetter4 = default_policy_args >
class bread_slicer2{
public:

	typedef policy_selector<PolicySetter1,PolicySetter2,PolicySetter3,PolicySetter4> policies;

	bread_slicer2(){}

	void doprint(){
		// Обращение к различным стратегиям
		policies::P1::doprint();
		policies::P2::doprint();
		policies::P3::doprint();
		policies::P4::doprint();
		// Т.к. классы стратегий виртуальные, память используется по минимуму
	}
};

// теперь при инстанциовании будет примерно так:
void show_name_params(){
	// Использование с явным набором парамтеров
	bread_slicer< DefaultPolicy1, DefaultPolicy2, CustomPolicy > bs1;

	// Использование "паттерна" Policy Selector - переопределим стретегию №3
	bread_slicer2< policy3_is<CustomPolicy> > bs2;

	// Проверка стратегий
	bs2.doprint();
}