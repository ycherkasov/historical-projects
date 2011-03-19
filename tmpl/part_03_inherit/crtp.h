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
