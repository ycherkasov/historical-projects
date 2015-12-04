#pragma once
#include <stdexcept>
#include <cassert>
#include <cstdlib>
#include <new>
#include <queue>
#include <memory>


// atexit pointer type
#ifdef _MSC_VER
#define C_CALLING_CONVENTION_QUALIFIER __cdecl
#else
#define C_CALLING_CONVENTION_QUALIFIER
#endif

typedef void (C_CALLING_CONVENTION_QUALIFIER *atexit_pfn_t)();

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
template <typename T>
struct default_lifetime{
    static void shedule_destruction(T*, atexit_pfn_t exit_func){
        std::atexit(exit_func);
    }

    static void on_dead_reference(){
        throw std::runtime_error("Dead reference detected");
    }
};

template <typename T>
struct phoenix_lifetime{
    static void shedule_destruction(T*, atexit_pfn_t exit_func){

        // re-create only once
        if (destroyed_once_){
            std::atexit(exit_func);
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

//
template <class T>
struct adapter
{
    void operator()(T*) { return pFun_(); }
    atexit_pfn_t pFun_;
};
//////////////////////////////////////////////////////////////////////////

/// Base class
struct lifetime_tracker{
    lifetime_tracker(size_t x) : longevity_(x){}
    virtual ~lifetime_tracker(){} // TODO: pure virtual
    size_t longevity_;
};

/// Helper delete class
template <typename T>
struct deleter {
    static void delete(T* pObj){
        delete pObj;
    }
};


/// Concrete lifetime control class
template <typename T, typename Destroyer>
class lifetime_tracker_impl : public lifetime_tracker
{
public:
    lifetime_tracker_impl(T* obj, size_t longevity, Destroyer d)
        : lifetime_tracker(longevity),
        tracked_object_(obj),
        destroyer_(d)
    {}

    ~lifetime_tracker_impl(){
        destroyer_(tracked_object_);
    }

private:
    T* tracked_object_;
    Destroyer destroyer_;
};

struct lifetime_wrapper{

    lifetime_wrapper(lifetime_tracker* tracker_impl) : tracker(tracker_impl){}

    bool operator<(const lifetime_wrapper& rhs){
        return tracker->longevity_ < rhs.tracker->longevity_;
    }

    bool operator>(const lifetime_wrapper& rhs){
        return tracker->longevity_ > rhs.tracker->longevity_;
    }
private:
    std::unique_ptr<lifetime_tracker> tracker;
};

// destroy in predefined order
void C_CALLING_CONVENTION_QUALIFIER at_exit_fn(); // declaration needed below

class lifetime_tracker_manager{
public:
    template <typename T, typename Destr>
    void push_to_destroy(size_t longevity, T* obj, Destr d){
        destroy_queue_.emplace(lifetime_wrapper(new lifetime_tracker_impl<T, Destr>(obj, longevity, d)));
        std::atexit(at_exit_fn);
    }
private:

    std::priority_queue <lifetime_tracker> destroy_queue_;
};

template <typename T>
struct longevity_control{
    static void shedule_destruction(T* obj, atexit_pfn_t exit_func){

        // TODO: std::function
        adapter<T> a = { exit_func };
        set_longevity(obj, get_longevity(obj), a);
    }

    static void on_dead_reference(){
        throw std::runtime_error("Dead reference detected");
    }
};
//////////////////////////////////////////////////////////////////////////
// No destroy at all (rely on OS)
template <typename T>
struct no_destroy{
    static void shedule_destruction(T*, atexit_pfn_t exit_func){}
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
            if (nullptr == instance_){
                if (destroyed_){
                    LifetimePolicy<T>::on_dead_reference();
                    destroyed_ = false;
                }

                instance_ = CreationPolicy<T>::create();
                LifetimePolicy<T>::shedule_destruction(instance_, &destroy);
            }
        }
        return *instance_;
    }


private:

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
