#pragma once

// Финальный класс - это класс, от которого запрещено наследование
// специальных средств для этого нет, но есть паттерн "запирающий класс"
//template <typename T> // Почему не работает с шаблоном?
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
