/** @brief 
Глава 12 Страуструпа
Производные классы, копирование, виртуальные функции
Абстрактные классы
*/
#include <iostream>
#include <string>
#include <set>

using namespace std;

// Простое наследование

// Базовый класс,суперкласс
class Employee{

public:
	Employee(const char* n);
	~Employee();

	void show_name();
protected:
	// Можно получить доступ из производного класса
	int _salary;
private:
	string _name;
	short _departament;
};

// Производный класс, подкласс (базовый к этому моменту д.б. определен)
// Общепринятая практика - наследовать классы от классов
// (т.е. структуры в наследовании не участвуют)
class Manager : public Employee {
	
public:

	// Конструктор не наследуется - он создается по умолчанию
	// или реализуется явно
	Manager(const char* n, short l, int s);

	// Также не наследуются операторы присваивания и дружественные функции
	
	// Деструкторы вызываются в порядке, обратном конструктору, и не наследуются
	~Manager();
	
	// Manager::show_name() замещает метод базового класса
	void show_name();
	
	void show_level();
protected:
	// Добавляемые поля могут совпадать по имени с полями базового
	// Хотя так делать не рекомендуется
	int _salary;
private:
	/** @brief Множество подчиненных */
	set<Employee*>	_group;
	short			_level;
};

