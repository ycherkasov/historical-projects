#include <stdio.h>
#include <stdarg.h>
#include <string>
#include <iostream>

#if defined(_WIN32) || defined(_WIN64) 
#define cdecl __cdecl
#define stdcall __stdcall
#define fastcall __fastcall
#else
// TODO: manage under GCC
#define cdecl
#define stdcall
#define fastcall
#endif


// стандартное соглашение по вызову __cdecl
// Аргументы передаются через стек, справа налево. 
// Очистку стека производит вызывающая программа.
int cdecl cdecl_func(int a, long b)
{
	// Помещение в стек значения регистра базы
	//00411440  push        ebp  

	//Сохранение адреса текущей вершины стека
	//00411441  mov         ebp,esp 

	//Установка стекового кадра для локальных переменных 
	//(C0h – избыточность возможно для отладочной информации)
	//00411443  sub         esp,0C0h 

	// Помещение регистров в стек
	//00411449  push        ebx  
	//0041144A  push        esi  
	//0041144B  push        edi  

	// какая-то инициализация
	//0041144C  lea         edi,[ebp-0C0h] 
	//00411452  mov         ecx,30h 
	//00411457  mov         eax,0CCCCCCCCh 
	//0041145C  rep stos    dword ptr es:[edi] 

	// выполнение сложения
	//0041145E  mov         eax,dword ptr [a] 
	//00411461  add         eax,dword ptr [b] 
	return a + static_cast<int>(b);

	// Достаем регистры из стека 
	// в обратном порядке 
	//00411464  pop         edi  
	//00411465  pop         esi  
	//00411466  pop         ebx  

	// возвращаем указатель стека на место,
	// достаем регистр базы и выходим
	//00411467  mov         esp,ebp
	//00411469  pop         ebp  
	
	// стек будет очищен после выхода
	//0041146A  ret   
}


// соглашение принятое в WINAPI __stdcall
// Аргументы передаются через стек, справа налево. 
// Очистку стека производит вызываемая функция.
int stdcall stdcall_func(int a, long* b)
{
	// Помещение в стек значения регистра базы
	//  push        ebp  

	//Сохранение адреса текущей вершины стека
	//  mov         ebp,esp 
	
	//Установка стекового кадра для локальных переменных 
	//(C0h – избыточность возможно для отладочной информации)
	//  sub         esp,0C0h 

	// Помещение регистров в стек
	//  push        ebx  
	//  push        esi  
	//  push        edi  

	// какая-то инициализация
	//  lea         edi,[ebp-0C0h] 
	//  mov         ecx,30h 
	//  mov         eax,0CCCCCCCCh 
	//  rep stos    dword ptr es:[edi] 

	// Присвоение
	//	mov         eax,dword ptr [b] 
	//	mov         ecx,dword ptr [a] 
	//	mov         dword ptr [eax],ecx 
	*b = static_cast<long>(a);

	return 0;
	//  xor         eax,eax 

	// Достаем регистры из стека 
	// в обратном порядке 
	//	pop         edi  
	//	pop         esi  
	//	pop         ebx  
	
	// возвращаем указатель стека на место,
	// достаем регистр базы и выходим
	//	mov         esp,ebp 
	//	pop         ebp  

	// При выходе освобождаем 8 байт все еще занятые под стек
	//	ret         8  
}


// "быстрое" соглашение по вызову - 
// передача параметров через регистры вместо стека,
// слева направо в eax, edx, ecx и, если параметров больше трёх, в стеке.
// Указатель стека на исходное значение возвращает функция.
int fastcall fastcall_func(char a, short b)
{
	return a%b;
}


// Если стек очищает вызывающая функция
// можно передавать переменное число параметров
// 0 - ограничитель списка параметров
// The Microsoft Visual Studio C/C++ compiler resolves this conflict 
// by silently converting the calling convention to __cdecl
int cdecl var_param_func(int a, ...)
{
	va_list argptr;
	va_start(argptr, a);
	int i = a;
	int sum = 0;

	while( i != 0 )
	{
		sum += i;
		i = va_arg( argptr, int);
	}
	va_end( argptr );
	return sum;
}


// Работа напрямую с указателем на стек
// при передаче произвольного числа параметров
double f_var(double n, ...)    //--заголовок с переменным числом параметров
{
	double *p = &n;        //--установились на начало списка параметров
	double sum = 0, count = 0;    
	while (*p)            //--пока аргумент не равен нулю
	{ 
		sum+=(*p);            //--суммируем аргумент
		p++;                  //--«перемещаемся на следующий аргумент
		count++;              //--считаем  количество аргументов
	}
	return ((sum)?sum/count:0);    //--вычисляем среднее
}


// передача переменного числа указателей
double f_var_pointers(int n, ...)
{  
	int *p = &n;        //--«одноразовый» указатель на количество параметров
	p++;                //--«достаем» список параметров-указателей

	union Pointer 
	{
		double **pp;
		double *kp; 
	};    //--«подстава» указателей

	Pointer A = {0};
	A.kp = (double *)p;           //-«обманываем» компилятора
	double sum = 0, count = n;
	for ( ; n-- ; A.pp++ )			//--изменяем двойной указатель!
		sum += (**A.pp);         //--двойной доступ!
	return ((sum)?sum/count:0);
}

// ------------------------------ 
// При передаче аргументов по _неконстантной_ ссылке 
// нельзя неявно преобразовать передаваемый аргумент
// во избежание неопределенного поведения или срезки
// Аргумент тоже д.б. неконстантным!
void update(float& f)
{
	f += 1.0f;
}

void show_unconst_ref()
{
	float f = 1.0f;
	double d = 2.0;
	//update(2.0f); // ошибка - константный аргумент, rvalue
	update(f);
	//update(d); // ошибка - неявное преобразование
}


// ------------------------------ 
// Указатели на функцию
void error_(const std::string& s){ std::cerr << s << std::endl; }

void show_func_pointer()
{
	// нулевой указатель на функцию
	void (*p_func)(const std::string&) = 0;

	p_func = error_;	// присвоении функции указателю - правильно
	p_func = &error_;	// так тоже правильно

	p_func("error");	// правильный вызов
	(*p_func)("error");	// тоже правильный

	// определение нового типа 'err_func_t' для сокращения записи
	typedef void (*err_func_t)(const std::string&);

	err_func_t p = p_func;
	p("more errors");
}

// ------------------------------ 
// Макрос конкатенации
// предназначен для вывода группы функций начинающихся
// с общего префикса, например 'show'
#define CONCAT_FUNC_NAME(pref,name) pref##_##name()
void show_macro()
{
	CONCAT_FUNC_NAME(show, unconst_ref);	// show_unconst_ref()
	CONCAT_FUNC_NAME(show, func_pointer);	// show_func_pointer()
	//CONCAT_FUNC_NAME(show, macro); // - а это бесконечная рекурсия с переполнением стека
}

// ----------------------------------
int main()
{
	int a = 109;
	long b = 1090;
	char aa = 56;
	short bb  = 10;

	int c = cdecl_func(a, b);
	//mov         eax,dword ptr [b] 
	//push        eax						// положили в стек параметр b
	//mov         ecx,dword ptr [a] 
	//push        ecx						// положили в стек параметр a
	//call        cdecl_func (41102Dh)		// вызов функции
	//add         esp,8						// очистили стек (8 = sizeof(int)*2)
	//mov         dword ptr [c],eax			// возвращаемое значение

	int d = stdcall_func(a, &b);
	//lea         eax,[b]					
	//push        eax						// положили в стек параметр по адресу b
	//mov         ecx,dword ptr [a] 
	//push        ecx						// положили в стек параметр а
	//call        stdcall_func (4110B9h)	// вызов функции
	//mov         dword ptr [d],eax			// возвращение результата без очистки стека!


	int e = fastcall_func(aa, bb);
	//mov         dx,word ptr [bb]		// кладем оба парметра прямо в регистры
	//mov         cl,byte ptr [aa] 
	//call        fastcall_func (411019h) 
	//mov         dword ptr [e],eax 

	int sm = var_param_func(1,2,3,4,-1);

	double da = 1.0, db = 2.0, dc = 3.0;
	double avg = f_var_pointers(3, &da, &db, &dc);

	show_macro();

	return 0;
}
