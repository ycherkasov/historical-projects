#include <iostream>
#include <list>
#include <algorithm>
#include <memory>

/*
Chapter 09 Jeff Eldger

Smart pointers that support transactions
* transactions could be stored in a list or stack
* thread safety should be applied (losk-free?)
*/


// smart pointer that really owns their objects (master pointer)
template <typename T>
class transaction_ptr{
public:

    // construction and destruction - always create by default
    // variable number of params could be created with variadics
    explicit transaction_ptr() : current_(new T){}

    ~transaction_ptr(){
        std::for_each(transactions_.begin(), transactions_.end(), std::default_delete<T>());
    }

    // better not to copy or assign
    transaction_ptr(const transaction_ptr& other) = delete;
    transaction_ptr& operator=(const transaction_ptr& other) = delete;

    // template copy and assignment (do not replace own copy and assignment)

    // pointer semantic (const and non-const)
    const T& operator*() const {
        return *current_;
    }

    const T* operator->() const {
        return current_;
    }

    T& operator*() {
        transactions_.push_back(new T(*current_));
        return *current_;
    }

    T* operator->() {
        transactions_.push_back(new T(*current_));
        return current_;
    }

    void undo(){
        if (transactions_.empty()){
            throw std::runtime_error("No transactions to rollback");
        }
        delete current_;
        current_ = transactions_.back();
        transactions_.pop_back();
    }

private:
    T* current_;
    std::list<T*> transactions_;
};

class test_me_transactions{
public:
    void change_me(){
        ++i;
    }

    int do_not_change_me() const {
        return i;
    }

private:
    int i = 0;
};

void show_transactional_ptrs(){

    transaction_ptr<test_me_transactions> tr;
    tr->change_me();
    // for const method some kinds of proxy could be used
    tr->do_not_change_me();
    tr.undo();
    tr.undo();
}

int main(){
    show_transactional_ptrs();
    return 0;
}
