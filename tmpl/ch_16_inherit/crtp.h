#pragma once
// Глава 16.3 Джосаттиса
// Необычный рекуррентный шаблон (Curiously recurrint template pattern)

// Этот паттерн состоит в передаче класса-наследника одному из базовых в качестве аргумента
// Применяется в подсчете экземпляров определенного типа
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

// count1 и count2 - абсолютно разные типы, и наследуют от разных классов 
// (разных инстансов одного шаблона)
class count1 : public object_counter< count1 >{};
class count2 : public object_counter< count2 >{};

// С помощью CRTP можно реализовать статический полиморфизм

template <typename Derived>
class BasicWorker{
public:

	void work() {
		// здесь общий код для наследуемых классов
		get_derived().work(t);
	}

	const Derived& get_derived() const {
		static_cast<const Derived&>(this);
	}

	Derived& get_derived() {
		return static_cast<Derived&>(this);
	}

	// еще методы
};

class SpecificWorker1 : public BasicWorker<SpecificWorker1> {
public:

    int work() const {
		// имплементация
		return 1;
	}
};

class SpecificWorker2 : public BasicWorker<SpecificWorker2> {
public:

    int work() const {
		// имплементация
		return 2;
	}
};

/*
Когда надо использовать CRTP? Например, когда есть два класса, которые не обязаны быть связанными 
в одну иерархию посредством общего базового класса, но которые имеют общий код. 
Статический полиморфизм позволяет этот код продублировать, написав всего лишь раз. 
Плюсы: нет накладных расходов на вызов через VMT, нет препятствий для inlining'а, 
нет увеличения объёма объекта из-за указателя на VMT. 
Минусы: вынесение кода шаблонного класса в хэдер, раздувание кода. 
Но во многих случаях это даже желательно — встраивать маленькие функции, 
поэтому для небольших классов дублирование „базовых классов“ несущественно.
*/