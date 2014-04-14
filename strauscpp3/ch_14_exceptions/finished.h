#pragma once

// Демонстрируется подмена стандартных функций 
// terminate() и unexpected()
// и их восстановление
typedef void (*handler_func_t)();

class finished
{
public:
	finished(void);
	~finished(void);

	// Подмена стандартного unexpected()
	void unexpected();

	// Подмена стандартного terminate()
	void terminate();

	// Тестируем бросок неописанного в спецификации исключения
	void test_custom_unexpected();

	// Тестируем неперехваченное исключение
	void test_custom_terminate();
private:
	handler_func_t _old_unexpected;
	handler_func_t _old_terminate;
};
