#ifndef _RVALUE_H_
#define _RVALUE_H_

/////////////////////////////////////////////////////////////////
/// Rvalue references
struct my_vector {

    int i_;
    int x_;

    // default constructor
    explicit my_vector(int i, int x):i_(i),x_(x){}

    // slow copy constructor (lvalue ref)
    my_vector (const my_vector& m):i_(m.i_),x_(m.x_){
        std::cout << "lvalue reference\n";
    }
    
    // quick copy constructor (rvalue ref)
//    my_vector (my_vector && m):i_(m.i_){
//        std::cout << "rvalue reference\n";
//    }

    // slow operator=(lvalue ref)
    my_vector & operator = (const my_vector & m){
        i_ = m.i_;
        x_ = m.x_;
        std::cout << "lvalue operator=\n";
    }

    // quick operator=(rvalue ref)
    my_vector & operator = (my_vector && m){
        i_ = m.i_;
        x_ = m.x_;
        std::cout << "rvalue operator=\n";
    }

    void print(){
        std::cout << i_ << ' ' << x_ << '\n';
    }
};

void show_rvalue1(){
    my_vector mv(0,0);

    // use lval ref
    my_vector mv1(mv);
    mv1.print();

    // use reference to temporary object
    my_vector mv2(my_vector(1, 1));
    mv2.print();
}

void show_rvalue(){
    
}
#endif//_RVALUE_H_

