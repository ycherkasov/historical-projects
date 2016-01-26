#pragma once
#include <memory>
#include <queue>
#include "at_exit.h"

// This header contains policy that allows some determined destruction order of singletons
// Singleton with definite lifetime must support strategy and derive from lifetime_interface


// Derive any singleton with lifetime support from that class
struct lifetime_interface{
    virtual size_t get_longevity() = 0;
};


/// Base class for any lifetime-controlled Singleton
/// These objects are saved in the destruction order queue
struct lifetime_tracker{
    friend struct lifetime_wrapper;
    lifetime_tracker(size_t x) : longevity_(x){}
    virtual ~lifetime_tracker() = 0;
    virtual void destroy() = 0;
private:
    size_t longevity_;
};

/// Concrete lifetime control class
template <typename T>
class lifetime_tracker_impl : public lifetime_tracker
{
public:

    lifetime_tracker_impl(T* obj, size_t longevity, atexit_pfn_t destroy_func)
        : lifetime_tracker(longevity),
        tracked_object_(obj),
        destroy_func_(destroy_func){}

    ~lifetime_tracker_impl(){
        destroy();
    }

    virtual void destroy(){
        if (tracked_object_){
            destroy_func_();
            tracked_object_ = nullptr;
        }
    }
private:
    T* tracked_object_;
    atexit_pfn_t destroy_func_;
};

struct lifetime_wrapper{

    lifetime_wrapper(lifetime_tracker* tracker_impl) : tracker(tracker_impl){}

    bool operator<(const lifetime_wrapper& rhs) const {
        return tracker->longevity_ < rhs.tracker->longevity_;
    }

    bool operator>(const lifetime_wrapper& rhs) const {
        return tracker->longevity_ > rhs.tracker->longevity_;
    }

    void destroy(){
        tracker->destroy();
    }

private:

    // We must support copy-assignment for our implementation
    // so that save it in the queue
    // That's why we need shared_ptr (it supports copy-assign semantic as we need it)
    std::shared_ptr<lifetime_tracker> tracker;
};

// destroy in predefined order
void C_CALLING_CONVENTION_QUALIFIER at_exit_fn(); // declaration needed below

// Lifetime manager class. It should be static singleton itself
class lifetime_tracker_manager{
public:

    // At-exit routine should have a exclusive access to private cleanup() method
    friend void C_CALLING_CONVENTION_QUALIFIER at_exit_fn();

    template <typename T>
    void push_to_destroy(T* obj, atexit_pfn_t destroy_func){

        // Our Singleton should support this interface
        size_t longevity = obj->get_longevity();

        // Push Singleton to the queue with priorities
        destroy_queue_.emplace(lifetime_wrapper(new lifetime_tracker_impl<T>(obj, longevity, destroy_func)));
    }

private:

    // this is called by at_exit routine at application exit
    static void cleanup(){
        while (!destroy_queue_.empty()){
            lifetime_wrapper& w = destroy_queue_.top();
            w.destroy();
            destroy_queue_.pop();
        }
    }

    static std::priority_queue<lifetime_wrapper> destroy_queue_;
};


// Finally, policy class for the singleton
// Creates at-exit routine on the first usage, after that just add singletons to the destruction
// priority queue
template <typename T>
struct longevity_control{
    static void shedule_destruction(T* obj, atexit_pfn_t destroy_func){

        manager_.push_to_destroy(obj, destroy_func);
        shedule_destruction_queue_cleanup();
    }

    static void on_dead_reference(){
        throw std::runtime_error("Dead reference detected");
    }

    static void cleanup(){
        manager_::cleanup();
    }

private:

    static void shedule_destruction_queue_cleanup(){
        if (!sheduled_){
            sheduled_ = true;
            std::atexit(at_exit_fn);
        }
    }

    // First object with known lifetime has been added
    // At-exit function has been set
    static bool sheduled_;

    // Priority cleanup queue saved here
    static lifetime_tracker_manager manager_;
};

template <typename T>
bool longevity_control<T>::sheduled_ = false;

template <typename T>
lifetime_tracker_manager longevity_control<T>::manager_;
