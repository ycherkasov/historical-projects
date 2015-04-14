// Реализация обобщенных алгоритмов методами шаблонов

// ------------------- Реализация через адаптеры и биндеры -------------------
namespace alg_adapter{

    // Биндеры - это шаблоны, куда передается бинарный предикат и один из параметров
    // В общем случае предикат может принимать произвольное число элементов
    // Но здесь для примера реализованы только двухэлементные

    // Т.е. из алгоритма вызывается не предикат напрямую,
    // а предикат косвенно через биндер, в котором он сохранен вместе с привязанным значением

    // Реализуем 2 биндера:

    // Для первого аргумента
    template <typename Predicate, typename T>
    class bind_first{
    public:
        bind_first(const Predicate& p, const T& t) : pred(p), val(t){}
        bool operator()(const T& right) {
            return pred(val, right);
        }
    private:
        Predicate pred;
        const T val;
    };

    // Для второго аргумента
    template <typename Predicate, typename T>
    class bind_second{
    public:
        bind_second(const Predicate& p, const T& t) : pred(p), val(t){}
        
		// нельзя сделать метод const, т.к. неизвестно, константны ли все связанные инстанциации
		bool operator()(const T& left) /*const*/ {	
            return pred(left, val);
        }
    private:
        Predicate pred;
        const T val;
    };

    // Предикаты для тестирования
    template<typename T>
    struct less_f{
        bool operator()(const T& a,const T& b){return a < b;}
    };

    template<typename T>
    struct greater_f{
        bool operator()(const T& a,const T& b){return a > b;}
    };

    // Реализуем функции-оболочки для биндеров,
    // чтобы не указывать при использовании параметры по многу раз
    template<typename Predicate, typename T>
    bind_first<Predicate, T> bind_left(const Predicate& p, const T& left){
        T value(left);
        return bind_first<Predicate, T>(p, value);
    }

    template<typename Predicate, typename T>
    bind_second<Predicate, T> bind_right(const Predicate& p, const T& right){
        T value(right);
        return bind_second<Predicate, T>(p, value);
    }
}


// ------------------- Адаптер указателя на функцию и его использование в биндере
namespace alg_adapter_func {

    // Унарная функция
    bool is_odd_func( int a ){
        return  !(a%2);
    }

    // Бинарная функция
    bool greater_func( int a, int b ){
        return  a > b;
    }

    // Шаблон класса адаптера и функции-оболочки для указателей на функции:

    // Для унарной функции
    template <typename ResultT, typename T>
    class unary_function_ptr {
    public:
        // Принимает указатель на функцию, инициализирует член класса
        explicit unary_function_ptr( ResultT (*PF)(T) ) : pfun(PF){}

        // Выполняет функцию
        ResultT operator()(T arg) const {
            return pfun(arg);
        }
    private:
        // Указатель, принимающий и возвращающий один параметр
        ResultT (*pfun)(T);
    };

    // Функция для сокращенной записи обертки унарного предиката
    template <typename ResultT, typename T> inline
        unary_function_ptr<ResultT, T> ptrfun( ResultT (*PF)(T) ) {
            return unary_function_ptr<ResultT, T>(PF);
    }

    // Для бинарной функции
    template <typename ResultT, typename T1, typename T2>
    class binary_function_ptr{
    public:
        // Принимает указатель на функцию, инициализирует член класса
        explicit binary_function_ptr( ResultT (*PF)(T1,T2) ) : pfun(PF){}

        // Выполняет функцию
        ResultT operator()(T1 left, T2 right) const{
            return pfun(left, right);
        }
    private:
        // Указатель, принимающий два параметра и возвращающий один
        ResultT (*pfun)(T1, T2);
    };

    // Функция для сокращенной записи  обертки бинарного предиката
    template <typename ResultT, typename T1, typename T2> inline
        binary_function_ptr<ResultT, T1, T2> ptrfun( ResultT (*PF)(T1, T2) ){
            return binary_function_ptr<ResultT, T1, T2>(PF);
    }
    // В книге Джосаттиса (и в соответствующем тестовом проекте)
    // приведена более общая реализация обертки функции
}

// ------------------- Адаптер указателя на метод -------------------
namespace alg_adapter_method{

    // Класс с методом, который будем передавать
    class constant{
    public:
        constant() : _val(){}
        constant(int v) : _val(v){}
        bool greater(int t){
            return _val > t;
        }
        bool is_odd(){
            return !(_val%2);
        };

    private:
        int _val;
    };

    // От этого шаблона можно наследовать
    template <typename Arg>
    struct unary_predicate{
        typedef Arg	argument_type;
    };

    // ...от этого тоже
    template <typename Arg1, typename Arg2>
    struct binary_predicate{
        typedef Arg1	first_argument_type;
        typedef Arg2	second_argument_type;
    };

    // Наследуем класс-предикат
    template <typename T>
    struct greater : public binary_predicate<T,T>{
        bool operator()(const T& left,const T& right){
            return left > right;
        }
    };

    // Наследуем класс-биндер
    template<class Predicate>
    class bind_second : public unary_predicate< typename Predicate::first_argument_type >{
    public:

        // Полезно использовать typedef с шаблонами
        typedef unary_predicate<typename Predicate::first_argument_type> base;
        typedef typename base::argument_type argument_type;

        // Инициализация адаптера
        bind_second(const Predicate& p, const typename Predicate::second_argument_type& right) :
            predicate(p), value(right){}

        // Выполнение предиката
        void operator()(argument_type left){
            return predicate(left, value);
        }

    private:
        Predicate predicate;
        typename Predicate::second_argument_type value;
    };

    // Наследуем класс-адаптер для унарной функции
    // (принимает тип объекта, содержащего метод и тип аргумента)
    template <typename Obj>
    class unary_method_ptr : public unary_predicate<Obj>{
    public:
        explicit unary_method_ptr( bool (Obj::*p)() ) : ptrmethod(p){}

        // Выполнение метода класса Obj
        bool operator()(Obj obj){
            return (obj.*ptrmethod)();
        }
    private:
        // Указатель на метод класса Obj, принимающего T
        bool (Obj::*ptrmethod)();
    };

    // Наследуем класс-адаптер для бинарной функции
    // (принимает тип объекта, содержащего метод и тип аргумента)
    template <typename Obj, typename T>
    class binary_method_ptr : public binary_predicate<Obj,T>{
    public:
        explicit binary_method_ptr( bool (Obj::*p)(T val) ) : ptrmethod(p){}

        // Выполнение метода класса Obj
        bool operator()(Obj obj, T val){
            return (obj.*ptrmethod)(val);
        }
    private:
        // Указатель на метод класса Obj, принимающего T
        bool (Obj::*ptrmethod)(T val);
    };

    // Функция-обертка для адаптера, принимает указатель на унарный метод класса Obj
    template <typename Obj> inline
    unary_method_ptr<Obj> memfun( bool (Obj::*pm)() ){
            return unary_method_ptr<Obj>(pm);
    }

    // Функция-обертка для адаптера, принимает указатель на бинарный метод класса Obj
    template <typename Obj, typename T> inline
    binary_method_ptr<Obj,T> memfun( bool (Obj::*pm)(T val) ){
        return binary_method_ptr<Obj,T>(pm);
    }
}


