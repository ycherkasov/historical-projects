/* 
 * File:   hazard_pointers.h
 * Author: straight
 *
 * Created on March 30, 2014, 6:15 PM
 */

#ifndef HAZARD_POINTERS_H
#define	HAZARD_POINTERS_H

#include <memory>
#include <thread>
#include <atomic>
#include <functional>

///////////////////////////////////////////////////////////
// High-loaded lock-free stack based on "hazard pointers"
// See Concurrency in Action, Chapter 7.2.3

// TODO: finish implementation

const unsigned max_hazard_pointers = 128;

struct hazard_pointer{
    std::atomic<std::thread::id> id;
    std::atomic<void*> ptr;
};

hazard_pointer hazard_pointers[max_hazard_pointers];

class hp_owner{
public:
    hp_owner():hp(nullptr){
        for(unsigned i = 0; i < max_hazard_pointers ; ++i){
            std::thread::id old_id;
            if( hazard_pointers[i].id.compare_exchange_strong(old_id, std::this_thread::get_id()) ){
                hp = &hazard_pointers[i];
                break;
            }
        }
        if(!hp){
            throw std::runtime_error("No more hazard pointers available");
        }
    }
    hp_owner(const hp_owner&)=delete;
    hp_owner& operator=(const hp_owner&)=delete;
    
    std::atomic<void*>& get_pointer(){
        return hp->ptr;
    }
    
    ~hp_owner(){
        hp->ptr.store(nullptr);
        hp->id.store(std::thread::id());
    }
private:
    hazard_pointer* hp;
};

template <typename T>
void delete_cast(void* p){
    delete static_cast<T*>(p);
}

struct pending_data{
    
    template <typename T>
    pending_data(T* p):data(p), delete_handle(&delete_cast<T>), next(nullptr){}
    
    ~pending_data(){ delete_handle(data); }
    
    void* data;
    std::function<void(void*)> delete_handle;
    pending_data* next;
};

std::atomic<pending_data*> nodes_to_delete;

std::atomic<void*>& get_hazard_pointer(){
    thread_local static hp_owner hazard;
    return hazard.get_pointer();
}

bool hazard_pointers_exist(void* p){
    for(unsigned i = 0; i < max_hazard_pointers; ++i){
        if(hazard_pointers[i].ptr.load() == p) 
            return true;
    }
    return false;
}

void add_to_pending(pending_data* node){
    node->next = nodes_to_delete.load();
    while( !nodes_to_delete.compare_exchange_weak(node->next, node) );
}


template <typename T>
void delete_later(T* data){
    add_to_pending(new pending_data(data));
}


void cleanup_nodes(){
    pending_data* current = nodes_to_delete.exchange(nullptr);
    while(current){
        pending_data* const next = current->next;
        if(!hazard_pointers_exist(current->data))
        {
            delete current;
        }
        else
        {
            add_to_pending(current);
        }
        current = next;
    }
}

template <typename T>
class lock_free_stack_hazard{
private:

    // stack node
    struct node{
        std::shared_ptr<T> data;
        node* next;
        node(const T& d):data( std::make_shared<T>(d) ){}
    };
public:
    lock_free_stack_hazard():head(){}
    
        void push(const T& data){
        // bad_alloc() exception is possible, but the stack is not modified yet
        // so leaving in consistent state in any case
        node * const new_node = new node(data);
        
        new_node->next = head.load();
        
        while(!head.compare_exchange_weak(new_node->next, new_node)){
            // try CAS until new node is inserted to the stack
        }
    }
    
    // pop data as a shared ptr so that not to call copy constructor
    std::shared_ptr<T> pop(){

        // get hazard pointer
        std::atomic<void*>& hp = get_hazard_pointer();
        
        
        // current top of the stack
        node* old_head = head.load();
        
        // move top to the next element
        // check also for the nullptr (e.g. stack is empty)
        do{
            node* temp;
            do{
                temp = old_head;
                hp.store(old_head);
                old_head = head.load();
            }while(old_head != temp);
        } while( old_head && !head.compare_exchange_strong(old_head, old_head->next) );
        hp.store(nullptr);
        
        // if some data exist return it, wrapping to shared_ptr
        std::shared_ptr<T> res;
        if(old_head){
            // at least one element exists
            // extract data from the node
            res.swap(old_head->data);
            
            // try to deallocate if possible
            if(exist_hazard_pointers(old_head)){
                delete_later(old_head);
            }
            else{
                delete old_head;
            }
            cleanup_nodes();
            
        }
        return res;
    }

private:
    // pointer to the stack top
    std::atomic<node*> head;
};

#endif	/* HAZARD_POINTERS_H */

