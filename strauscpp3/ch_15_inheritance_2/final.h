#pragma once

// Финальный класс - это класс, от которого запрещено наследование
// специальных средств для этого нет, но есть паттерн "запирающий класс"
//template <typename T> // Почему не работает с шаблоном?
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
