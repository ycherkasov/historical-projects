#include "construct.h"
#include "class.h"
#include "user_allooc.h"
#include "word_counter.h"
#include "new_forms.h"
#include "memory_pool.h"
#include "ref_count.h"
#include "proxy.h"

#include <iostream>

using namespace std;

construct g_construct;


// Разрешается определять структуру и не структуры одного имени
// (то же для class, enum, union)
void struct_one_name(){
    struct A{
        int a;
    };

    A aa;
    aa.a = 2;
    int A = 1;
}

void f1(A a){}
void f2(A& a){}

// Функция демонстрирует инициализацию и работу
// с константными объектами и mutable-членами
void show_const(){

    A a;

    bool b = false;
    size_t s = sizeof(b); // 1

    const A aa;

    f1(aa); // non-const copy
    // f2(aa); compile error

    //aa.set_int1(5); - так нельзя, вызов неконстантного метода в константном классе

    // а так можно, член константного класса mutable может быть изменен
    aa.set_a6_mutable(5);

    // static class member could be changed in const method
    aa.set_static(6);
}

// Функция демонстрирует инициализацию и работу
// с константными указателями и указателями на константу
void show_const_pointers(){

    // Указатель на константу
    const int* pp1 = new const int(3);

    // Явная ошибка - константа не проинициализирована,
    // но компилятор молчит
    // Clang & GCC detect it!

#if defined(_WIN32) || defined(_WIN64)
    const int* pp2 = new const int;

    // Проинициализировать ее легальным способом теперь не удастся
    // Не сработает даже const_cast
    //const_cast<int>(*pp2) = 1;

    // Можно создать непроинициализированный константный динамический массив
    // Он даже не будет обнулен, и больше с ним ничего сделать нельзя.
    const int* pp3 = new const int[100];
#endif

    // Логичнее создавать динамический массив по константному указателю
    int* const pp4 = new int[100];

    // Внимание - Стандарт разрешает динамический массив нулевого размера!
    int* pp5 = new int[0];
    // а статический - не разрешает (хотя есть расширения gcc, где это можно)
    //int p6[0];

    // Функция _msize позволяет узнать реальный размер выделенного массива
    // (Windows-specific)
#if defined(_WIN32) || defined(_WIN64)
    size_t sz = _msize(pp4);
    sz = _msize(pp5);
#endif

    delete pp1;
#if defined(_WIN32) || defined(_WIN64)
    delete pp2;
    delete[] pp3;
#endif
    delete[] pp4;
    delete[] pp5;

    // осторожно прменять typedef к массивам
    typedef good_weekday_t week_array[7];
    good_weekday_t* s = new week_array;
    //delete s; - неправильно! нужен delete[], но мы об этом не знаем!
    delete[] s;

    //X* x = new Y[2];
    // delete[] x;
    //It invokes undefined behavior.
    //This is correct.
    //When deleting an array, the dynamic and the static type of the object must be the same,
    //or the behavior is undefined (C++ Standard 5.3.5/3).
}

// Способы вызова конструктора объекта:
// 1. Именованный автоматический объект(по умолчанию, копия).
// 2. Объект в свободной памяти (new/delete).
// 3. Нестатический объект-член класса.
// 4. Объект - элемент массива.
// 5. Локальный статический объект.
// 6. Глобальный объект.
// 7. Временный объект.
// 8. Объект, сконструированный пользовательским аллокатором.
// 9. Член объединения (union)
void show_construct_destruct(){

    std::cout << std::endl << "Create automatic objects" << std::endl;
    // 1. Именованный автоматический объект(по умолчанию, копия).
    // Если в объекте есть константы или ссылки, он не может быть создан
    // конструктором по умолчанию
    {
        construct c;
        construct d;
        {
            construct c1(c);
        }
        noncopyble nc;
        // А скопировать этот объект нельзя!
        // noncopyble nc1(nc);
    }
    // Создаются в порядке c-d-c1-nc, уничтожаются c1-nc-d-c


    // 2. Объект в свободной памяти (new/delete).
    std::cout << std::endl << "Create dynamic objects" << std::endl;

    // Если в классе только автоматически сгенерированный конструктор,
    // то эта форма вызова, с (), обнулит память...
    construct* pc1 = new construct();
    // ... а эта - нет
    construct* pc2 = new construct;

    // Обнуляйте неиспользуемые указатели!
    delete pc1; pc1 = 0;
    delete pc2; pc2 = 0;

    // 3. Нестатический объект-член класса.
    std::cout << std::endl << "Create non-static member" << std::endl;
    // Вызовется конструктор по умолчанию intern()
    {
        construct c1;
    }


    // 4. Объект - элемент массива.
    std::cout << std::endl << "Create objects array " << std::endl;
    // ! Элементы массива могут быть сконструированы по умолчанию
    construct arr_c[5];
    // Можно использовать список инициализации в виде конструкторов
    construct arr_1[] = { construct(2), construct(g_construct) };
    // Можно использовать даже неявные преобразования при конструировании
    construct arr_2[] = { 1, 2 };
    // прим.-такие массивы лучше объявлять без указания количества элементов - []

    // 5. Локальный статический объект.
    std::cout << std::endl << "Create local static objects " << std::endl;
    create_static(0);
    create_static(1);
    create_static(2);

    // В случае простых объектов можно пользоваться представлением в виде функций
    int st = return_static();

    // 6. Глобальный объект.
    // Конструкторы глобальных объектов вызываются в одном модуле - в порядке определения
    // в разных модулях - порядок не определен. То же для разрушения объектов (см. g_construct, g_intern)
    // Полезно использовать синглтон, а также обертку-инициализатор (см. construct.h)

    // 7. Временный объект.
    // Убьется сразу
    std::cout << std::endl << "Create temporary object " << std::endl;
    construct(5);
    // Также неявное копирование происходит при передаче и возвращении по значению
    // и бросании исключения

    // 8. Объект, сконструированный пользовательским аллокатором.
    {
        // см. show_user_alloc()
    }


    // 9. Член объединения (union)
    // union не может иметь членов с конструкторами-деструкторами
    // потому что непонятно какой из них вызывать
}

// Task from B.Batkin
void show_word_counter(){
    string s("test.txt");
    word_counter::count_from_file(s);
}



void show_placement_delete(){

    struct X {
        X() {
            throw std::runtime_error("X object never be created");
        }
        // custom placement new
        static void* operator new(std::size_t sz, bool b){
            std::cout << "custom placement new called, b = " << b << '\n';
            return ::operator new(sz);
        }
        // custom placement delete
        // could be called in functional form only
        // however, pair delete could be found in case of exception
        static void operator delete(void* ptr, bool b)
        {
            std::cout << "custom placement delete called, b = " << b << '\n';
            ::operator delete(ptr);
        }
    };

    try {
        X* p1 = new (true) X;
    }
    catch (const std::exception& e) {
        cout << e.what();
    }
}

void test_placement_new_handler(){
    printf("No more memory!\n");
    throw bad_alloc();
}

void show_user_alloc(){

    // разные версии перегруженных new() и delete()

    // simple new overload
    {
        cout << "Test simple new overload" << endl;
        user_alloc* x = new user_alloc();
        x->test();
        delete x;
    }

    // new for array overload
    {
    cout << "Test array new overload" << endl;
    user_alloc* x = new user_alloc[10];
    x[1].test();
    delete[] x;
}

    // placement new overload
    {
        cout << "Test placement new overload" << endl;
        user_alloc* x = new user_alloc();
        x->test();
        x->~user_alloc();

        user_alloc* y = new(x)user_alloc();
        // could be directly called in functional form only!
        // will be implicitly called in case of exception
        user_alloc::operator delete(y, x);
        delete x;
    }

    // new_handler replacement new overload
    {
        cout << "Test placement new overload" << endl;
        user_alloc* x = new (test_placement_new_handler)user_alloc();
        x->test();
        delete x;
    }
}

void show_new_handler(){

    // Продемонстируем, как пользоваться mixture-классом из Мейерс 1-7,
    // замещающим глобальный new_handler своим

    // попробуем с очень большим объектом
    try{
        LargeObject::set_new_handler(NewHandlerSupport<LargeObject>::no_more_memory);
        LargeObject* x = new LargeObject();
        delete x;
    }
    catch (const std::bad_alloc& e){
        cerr << "Lack of memory: " << e.what() << '\n';
    }

    // попробуем с обычным объектом
    try{
        SmallObject::set_new_handler(NewHandlerSupport<SmallObject>::no_more_memory);
        SmallObject* x = new SmallObject();
        delete x;
    }
    catch (const std::bad_alloc& e){
        cerr << "Lack of memory: " << e.what() << '\n';
    }
}

void show_memory_pool(){

    // allocated from pool, addr = 0x003789d0
    memory_pool_item* mpi1 = new memory_pool_item();
    mpi1->test();

    // allocated from pool, addr = 0x003789d8 (prev + sizeof memory_pool_item)
    memory_pool_item* mpi2 = new memory_pool_item();
    mpi2->test();

    // deallocated both from pool
    delete mpi1;
    delete mpi2;

    // allocated from pool again, should again addr = 0x003789d0
    // (block 0 free again)
    memory_pool_item* mpi3 = new memory_pool_item();
    mpi3->test();
    delete mpi3;
}

//delete вместо delete[] приводит к падению, если у типа есть нетривиальный деструктор(почему ? ).
class class_with_non_trivial_destructor
{
private:
    size_t s;
    int * data;
public:
    class_with_non_trivial_destructor(size_t new_s = 10)
        :
        s(new_s)
    {
        printf("class_with_non_trivial_destructor\n");
        data = new int[s];
    }

    ~class_with_non_trivial_destructor()
    {
        printf("~class_with_non_trivial_destructor\n");
        if (data) delete[] data;
    }
private:
    class_with_non_trivial_destructor(const class_with_non_trivial_destructor&);
    class_with_non_trivial_destructor &
        operator = (const class_with_non_trivial_destructor&);
};

//=============================================================================
void crash()
{
    class_with_non_trivial_destructor * bad_array =
        new class_with_non_trivial_destructor[10];

    // в дебаге срабатывает ассерт _ASSERTE(_BLOCK_TYPE_IS_VALID(pHead->nBlockUse));
    // в релизе падает
    delete bad_array;
}
//=============================================================================
// По поводу new[] - в случае, если деструктор нетривиальный, то надо в delete[] знать,
// у скольких объектов позвать деструктор.Где же хранить эту информацию,
// если delete получает на вход просто указатель, по которому лежит хз сколько объектов ?
// Компиляторы обычно хранят эту информацию в 4 - х байтах перед массивом
void array_size_info()
{
    class_with_non_trivial_destructor *pa = new class_with_non_trivial_destructor[100];
    int *pi = reinterpret_cast<int*>(pa);
    std::cout << pi[-1];
    delete[] pa;
}
// delete, в отличие от delete[], не знает об этом 4-байтовом смещении,
//
// Да, и new / delete, и низкоуровневый менеджер памяти(по умолчанию - malloc / free) -
// это два слоя, которые никак не зависят друго от друга.
// + 4 байта добавляет компилятор, а не malloc / free, которые библиотека
// а не язык(компилятор)и в memory manager при освобождении засовывается не тот указатель,
// который он выдал, а смещённый на 4 байта

// Втроая задачка: когда и почему delete на объект приводит к падению из-за забытого виртуального деструктора?

struct base{

    int data;

    base(){
        cout << "ctor of base\n";
    }
    ~base(){
        cout << "dtor of base\n";
    }
};

struct derived : base{

    derived(){
        cout << "ctor of derived\n";
    }
    virtual ~derived(){
        cout << "dtor of derived\n";
    }
};

void crach_2(){

    base *p = new derived;

    std::cout << p << '\n';
    base *b = p;
    std::cout << b << '\n';

    // trying to delete data, but in fact this is vtable pointer
    delete p;

    // When base is empty, the size of{ vtable, base, derived } and{ base } happen to be the same
    // because allocating an object of empty class occupies nonzero number of bytes, which happens to be equal in both cases.
    // When derived has no virtual functions, vtable is not present,
    // the addresses are again the same and delete succeeds.
}

template <typename RCString>
void show_ref_count(){
    using namespace meyers_refcount;
    RCString s1("Skotobaza");
    RCString s2(s1);
    const RCString s3 = s1;
    s2 = s3;

    // in that point ref count == 3
    const char c1 = s1[0];
    // detached s1, s1.ref_count == 1
    const char c2 = s3[0];
    // previous operation is non-modifying
    char c3 = s2[0];
    // detached s2 as well

    // We 'open' internal representation of the object
    // so refcount usage could be unsafe
    // restrict it setting shareable=false
    // (do not use refcount anymore)
    RCString ss1("Komatoza");
    char* p = &ss1[0];
    RCString ss2 = ss1;
    // without 'shared' flag both version have changed
    *p = 'C';
}

void show_proxy(){
    using namespace meyers_proxy;
    rc_string2 s1("Skotobaza");
    rc_string2 s2(s1);
    const rc_string2 s3 = s1;
    s2 = s3;

    // all these operations do not detach value now
    const char c1 = s1[0];
    const char c2 = s3[0];
    char c3 = s2[0];

    // detach s2
    s2[0] = 'C';

    // detach s1
    s1[1] = s2[0];
}

int main(){

    // delete/delete[] crash
    //crash();

    // non-virtual crash
    //crach_2();

    //return 0;

    array_size_info();

    struct_one_name();

    // should be reproduced in release
    //show_bad_alloc();
    //return 0;

    show_const_pointers();

    A a(1, 1, 1);
    int aa = a.get_int6();
    show_const();
    show_construct_destruct();

    show_user_alloc();
    show_new_handler();
    show_placement_delete();
    show_memory_pool();

    // Статический метод можно вызывать как для класса, так и для объекта
    construct::out_static_array();
    construct c;
    c.out_static_array();

    // Демонстрация стандартных перегруженных операторов new
    show_new1();
    show_new2();
    show_new_delete();

    show_ref_count<meyers_refcount::rc_string>();
    show_ref_count<meyers_refcount::rc_string2>();
    show_proxy();

    return 0;
}
