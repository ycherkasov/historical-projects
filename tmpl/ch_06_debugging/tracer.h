#pragma once

/**@brief  
Тестовый класс для передачи в шаблонный алгоритм и проведения
первичных проверок и трассировки эффективности алгоритма
*/
class tracer
{
public:

	tracer(int v = 0);
	~tracer();

	// Функции доступа:
	static long creations() {return s_created;}
	static long copies() {return s_copied;}
	static long assigmennts() {return s_assigned;}
	static long comparasions() {return s_compared;}
	static long max_objects() {return s_max_objects;}

	// Функции трассировки
	tracer(const tracer& t);
	tracer& operator=(const tracer& t);
	bool operator<(const tracer& t) const;
	bool operator>(const tracer& t) const;
	void trace_msg_create() const;
	void trace_msg_destroy() const;
	void trace_msg_assign() const;
	void trace_msg_compare() const;

protected:

	// Обновить максимальное количество объектов
	static inline void update_max_objects();
private:
	// значение для сортировки
	int _val;

	// Поколение трассировщика (количество копирований)
	int _generation;

	// Счетчик созданий
	static long s_objects;

	// Счетчик созданий
	static long s_created;

	// Счетчик копирований
	static long s_copied;

	// Счетчик присвоений
	static long s_assigned;

	// Счетчик сравнений
	static long s_compared;

	// Максимальное количество объектов
	static long s_max_objects;
};
