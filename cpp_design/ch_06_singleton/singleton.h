#pragma once
#include <stdexcept>
#include <cassert>
#include <cstdlib>
#include <new>
#include "at_exit.h"
#include "lifetime_manager.h"

//////////////////////////////////////////////////////////////////////////
// Creation policies:
// * new
// * malloc
// * static
template <typename T>
struct new_creation_policy{

    static T* create(){
        return new T();
    }

    static void destroy(T* p){
        delete p;
    }
};

//////////////////////////////////////////////////////////////////////////
// Lifetime policies
// * default - exists all application lifetime, destroyed using atexit()
// * phoenix - could be re-created one time after destruction
// * longevity - support several singletons destruction order
// * no_destroy - lives forever, destroyed at process exit

// Default
template <typename T>
struct default_lifetime{
    static void shedule_destruction(T*, atexit_pfn_t destroy_func){
        std::atexit(destroy_func);
    }

    static void on_dead_reference(){
        throw std::runtime_error("Dead reference detected");
    }
};


// Phoenix - could be re-created one time after destruction
template <typename T>
struct phoenix_lifetime{
    static void shedule_destruction(T*, atexit_pfn_t destroy_func){

        // re-create only once
        if (destroyed_once_){
            std::atexit(destroy_func);
        }
    }

    static void on_dead_reference(){
        destroyed_once_ = true;
    }

private:
    static bool destroyed_once_;
};
template <typename T>
bool phoenix_lifetime<T>::destroyed_once_ = false;


// See Lifetime Manager in a separate header

// No destroy at all (rely on OS)
template <typename T>
struct no_destroy{
    static void shedule_destruction(T*, atexit_pfn_t destroy_func){}
    static void on_dead_reference(){}
};


//////////////////////////////////////////////////////////////////////////
// Threading policies
// * single_thread
// * multy_thread
template <typename T>
struct single_thread{
    struct lock{};
};



template <typename T,
    template <typename> class CreationPolicy = new_creation_policy,
    template <typename> class LifetimePolicy = default_lifetime,
    template <typename> class ThreadingPolicy = single_thread>
class singleton_holder{
public:

    static T& instance(){

        if (nullptr == instance_){

            typename ThreadingPolicy<T>::lock l;
            (l); // make compiler happy

            if (nullptr == instance_){
                if (destroyed_){
                    LifetimePolicy<T>::on_dead_reference();
                    destroyed_ = false;
                }

                instance_ = CreationPolicy<T>::create();

                // destruction is delayed
                LifetimePolicy<T>::shedule_destruction(instance_, &destroy);
            }
        }
        return *instance_;
    }


private:

    // Could not be destroysed directly
    // Destruction relies on LifetimePolicy strategy
    // that schedules it according to its internal policies
    static void destroy(){
        assert(!destroyed_);
        CreationPolicy<T>::destroy(instance_);
        instance_ = nullptr;
        destroyed_ = true;
    }

    singleton_holder(){}
    ~singleton_holder(){}

    singleton_holder(const singleton_holder&) = delete;
    singleton_holder& operator=(const singleton_holder&) = delete;

private:

    // should be:
    //static ThreadingPolicy<T>::VolatileType instance_;

    // for simplicity:
    typedef T* instance_type;
    static instance_type instance_;
    static bool destroyed_;
};

// Define statics
template<typename T,
    template <typename> class CreationPolicy,
    template <typename> class LifetimePolicy,
    template <typename> class ThreadingPolicy>
// should be:
// typename singleton_holder<T, CreationPolicy, LifetimePolicy, ThreadingPolicy>::ThreadingPolicy<T>::VolatileType
// but for simplicity:
typename singleton_holder<T, CreationPolicy, LifetimePolicy, ThreadingPolicy>::instance_type
singleton_holder<T, CreationPolicy, LifetimePolicy, ThreadingPolicy>::instance_;

template<typename T,
    template <typename> class CreationPolicy,
    template <typename> class LifetimePolicy,
    template <typename> class ThreadingPolicy>
bool singleton_holder<T, CreationPolicy, LifetimePolicy, ThreadingPolicy>::destroyed_;
