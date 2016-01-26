#include <iostream>
#include "static_init.h"

using namespace std;

/*
Chapters 02-04 Jeff Eldger

* accessibility of virtual methods could be changed
* there's a pattern of static resourses load and free (in is used for std::iostream)
  - create static counter
  - initialize static resourses at first usage
  - deinitialize when counter == 0
* ANSI C unsafe void* data could be wrapped to safe C++ templates
* enums could be thrown as exceptions
*/

//////////////////////////////////////////////////////////////////////////
// Change accessibility for virtual methods


class base{
public:
    virtual ~base(){}

    virtual void hide_me(){
        cout << "base::hide_me()" << endl;
    }
protected:
    virtual void open_me(){
        cout << "base::open_me()" << endl;
    }
};

class derived : public base{

protected:
    // we hide that function in derived
    virtual void hide_me(){
        cout << "derived::hide_me()" << endl;
    }
public:
    // we open that function in derived
    virtual void open_me(){
        cout << "derived::open_me()" << endl;
    }
};

void show_change_accessibility(){
    base b;
    derived d;
    base* bb = new base;
    base* bd = new derived;

    // Use statically
    b.hide_me();

    //d.hide_me(); cannot access protected member
    d.open_me();

    // Use dynamically (use only methods available in base class)
    bb->hide_me();
    bd->hide_me();  // call protected function of derived instead!

    // for non-virtual and members this is not available!
    delete bb;
    delete bd;
}

void show_static_init_counter(){
    static_initializer i1;
    static_initializer i2;
    static_initializer* i3 = new static_initializer;

    delete i3;
}

//////////////////////////////////////////////////////////////////////////
// Wrap unsafe ANSI C-style data to type-safe templates
struct unsafe_type
{
    void* data;
    unsigned int size;
};

template <typename T>
class safe_type{
public:
    safe_type(){
        t.data = reinterpret_cast<void*>(new T);
        t.size = sizeof(T);
    }

    virtual ~safe_type(){
        delete reinterpret_cast<T*>(t.data);
    }

    T& data(){
        return *reinterpret_cast<T*>(t.data);
    }

    const T& data() const {
        T* tmp = reinterpret_cast<T*>(t.data);
        return const_cast<const T&>(*tmp);
    }

private:
    unsafe_type t;
};


void show_safe_type_wrapper(){
    safe_type<derived> s;
    derived& d = s.data();
    d.open_me();
}

int main(){
    show_change_accessibility();
    show_static_init_counter();
    show_safe_type_wrapper();

    return 0;
}
