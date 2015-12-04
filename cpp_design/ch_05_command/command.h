#pragma once
#include <memory>

//////////////////////////////////////////////////////////////////////////
// Type for non-used functor params
struct null_type{};

// Command is a pattern that completely split function and its caller
// Call could be passed anywhere and delayed to any time

//////////////////////////////////////////////////////////////////////////
// Abstract bases set for every functor
// It could wrap anything that could be called so that implement Command pattern
// Interface support up to 3 params
// For every number of params created its own specialization
// TODO: implement with variadics!?
template <typename RetT, typename P1, typename P2, typename P3>
struct functor_impl{
    virtual RetT operator()(P1, P2, P3) = 0;
    virtual functor_impl* clone() const = 0;
    virtual ~functor_impl(){}
};

template <typename RetT, typename P1, typename P2>
struct functor_impl<RetT, P1, P2, null_type>{
    virtual RetT operator()(P1, P2) = 0;
    virtual functor_impl* clone() const = 0;
    virtual ~functor_impl(){}
};

template <typename RetT, typename P1>
struct functor_impl<RetT, P1, null_type, null_type>{
    virtual RetT operator()(P1) = 0;
    virtual functor_impl* clone() const = 0;
    virtual ~functor_impl(){}
};

template <typename RetT>
struct functor_impl<RetT, null_type, null_type, null_type>{
    virtual RetT operator()() = 0;
    virtual functor_impl* clone() const = 0;
    virtual ~functor_impl(){}
};

//////////////////////////////////////////////////////////////////////////
// Implementation that accept other functor or function pointer
// (something that supports calling semantic)
template <typename Parent, typename Function>
class functor_handle :
    public functor_impl<typename Parent::ReturnType, typename Parent::Type1, typename Parent::Type2, typename Parent::Type3>{
public:
    typedef typename Parent::ReturnType RetT;
    typedef typename Parent::Type1 P1;
    typedef typename Parent::Type2 P2;
    typedef typename Parent::Type3 P3;

    functor_handle(const Function& f) : fun_(f){}

    functor_handle* clone() const {
        return new functor_handle(*this);
    }

    RetT operator()(){
        return fun_();
    }

    RetT operator()(P1 p1){
        return fun_(p1);
    }

    RetT operator()(P1 p1, P2 p2){
        return fun_(p1, p2);
    }

    RetT operator()(P1 p1, P2 p2, P3 p3){
        return fun_(p1, p2, p3);
    }

private:
    // Functor of function pointer
    // smth. should support ()
    Function fun_;
};

//////////////////////////////////////////////////////////////////////////
// Implementation that accept pointer to member
// and something that behaves like an object pointer (real or smart)
template <typename Parent, typename PointerToType, typename PointerToMethod>
class method_handler :
    public functor_impl<typename Parent::ReturnType, typename Parent::Type1, typename Parent::Type2, typename Parent::Type3>{
public:
    typedef typename Parent::ReturnType RetT;
    typedef typename Parent::Type1 P1;
    typedef typename Parent::Type2 P2;
    typedef typename Parent::Type3 P3;

    method_handler(const PointerToType& t, const PointerToMethod& m) : type_(t), method_(m){}

    method_handler* clone() const {
        return new method_handler(*this);
    }

    RetT operator()(){
        return ((*type_).*method_)();
    }

    RetT operator()(P1 p1){
        return ((*type_).*method_)(p1);
    }

    RetT operator()(P1 p1, P2 p2){
        return ((*type_).*method_)(p1, p2);
    }

    RetT operator()(P1 p1, P2 p2, P3 p3){
        return ((*type_).*method_)(p1, p2, p3);
    }

private:
    // we use pointer type so that process every type who supports
    // unary operator* (dereference).
    // It could be plain or smart pointer or iterator
    PointerToType type_;

    // Class member pointer
    PointerToMethod method_;
};

//////////////////////////////////////////////////////////////////////////
// Generic Command functor with 0-3 params
// Body for wrapped implementation
template <typename RetT, typename P1 = null_type, typename P2 = null_type, typename P3 = null_type>
class functor{
public:

    typedef typename RetT ReturnType;
    typedef typename P1 Type1;
    typedef typename P2 Type2;
    typedef typename P3 Type3;
    // Default
    functor(){}

    // Copy-assign
    functor(const functor&) : impl_(other.impl_->clone()){}

    functor& operator=(const functor&){
        functor<RetT, P1, P2, P3> cp(other);
        impl_ = std::move(other.impl_);
    }

    // From other functor type of function pointer
    // (used functor_handle implementation for type casting)
    template <typename Function>
    functor(const Function& f) : impl_(new functor_handle<functor, Function>(f)){}

    // From pointer to type and pointer to member
    // (used method_handler implementation for type casting)
    template <typename PointerToType, typename PointerToMethod>
    functor(const PointerToType& t, const PointerToMethod& m)
        : impl_(new method_handler<functor, PointerToType, PointerToMethod>(t, m)){}

    RetT operator()();
    RetT operator()(P1 p1);
    RetT operator()(P1 p1, P2 p2);
    RetT operator()(P1 p1, P2 p2, P3 p3);

private:
    // Some Functor (Command) implementation
    typedef functor_impl<typename RetT, typename P1, typename P2, typename P3> impl_t;
    std::unique_ptr<impl_t> impl_;
};


template <typename RetT, typename P1 /*= null_type*/, typename P2 /*= null_type*/, typename P3 /*= null_type*/>
RetT functor<RetT, P1, P2, P3>::operator()(P1 p1, P2 p2, P3 p3){

    return (*impl_)(p1, p2, p3);
}

template <typename RetT, typename P1 /*= null_type*/, typename P2 /*= null_type*/, typename P3 /*= null_type*/>
RetT functor<RetT, P1, P2, P3>::operator()(P1 p1, P2 p2){

    return (*impl_)(p1, p2);
}

template <typename RetT, typename P1 /*= null_type*/, typename P2 /*= null_type*/, typename P3 /*= null_type*/>
RetT functor<RetT, P1, P2, P3>::operator()(P1 p1){

    return (*impl_)(p1);
}

template <typename RetT, typename P1 /*= null_type*/, typename P2 /*= null_type*/, typename P3 /*= null_type*/>
RetT functor<RetT, P1, P2, P3>::operator()(){

    return (*impl_)();
}
