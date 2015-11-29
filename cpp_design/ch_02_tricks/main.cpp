#include <iostream>
#include <memory>

using namespace std;

// Andrei Alexandresku. Chapter 2. Tricks 

// Local class trick - interface adapter

class sample_interface{
public:
    virtual void fun() = 0;
    virtual ~sample_interface(){}
};

template <typename T, typename U>
sample_interface* make_adapter(const T& obj, const U& arg)
{
    class local_interface : public sample_interface{

    public:
        local_interface(const T& obj, const U& arg) :
            obj_(obj), arg_(arg){}

        virtual ~local_interface(){}

        virtual void fun(){
            cout << "Fun" << endl;
        }
    private:
        T obj_;
        U arg_;
    };

    // Local class also finite!
    return new local_interface(obj, arg);
}


// Integer to type
// Good for compile-time dispatching or metaprogramming
template <int I>
struct int2type
{
    enum 
    {
        value = I;
    };
};

void show_adapter(){
    std::unique_ptr<sample_interface> p(make_adapter(1, 1.0));
    p->fun();
}

void show_int2type(){
    int2type<0> i0;
    int2type<1> i1;
    std::cout << i0.value << std::endl;
    std::cout << i1.value << std::endl;
}

int main(){
	
    show_adapter();
    show_int2type();
	return 0;
}