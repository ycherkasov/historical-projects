#include <iostream>
#include <memory>
#include <thread>
#include <atomic>
#include "hazard_pointers.h"

///////////////////////////////////////////////////////////
// Middle-loaded lock-free stack based on delayed deletion nodes chain
// See Concurrency in Action, Chapter 7.2.1-7.2.2
template <typename T>
class lock_free_stack_lazy{
private:

    // stack node
    struct node{
        std::shared_ptr<T> data;
        node* next;
        node(const T& d):data( std::make_shared<T>(d) ){}
    };
    
public:
    lock_free_stack_lazy():head(), threads_in_pop(), to_be_deleted(){}
    
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
        
        // method access counter (for the correct nodes deallocation)
        ++threads_in_pop;
                
        // current top of the stack
        node* old_head = head.load();
        
        // move top to the next element
        // check also for the nullptr (e.g. stack is empty)
        while( old_head && !head.compare_exchange_weak(old_head, old_head->next) ){}
        
        // if some data exist return it, wrapping to shared_ptr
        std::shared_ptr<T> res;
        if(old_head){
            // at least one element exists
            // extract data from the node
            res.swap(old_head->data);
        }
        // try to deallocate if possible
        try_reclaim(old_head);
        return res;
    }
    
    static void delete_nodes(node* nodes){
        while(nodes){
            node* next = nodes->next;
            delete nodes;
            nodes = next;
        }
    }
    
    // add nodes those we were unable to delete
    // back to the pending list
    void chain_pending_nodes(node* nodes){
        // reach the end of chain
        node* last = nodes;
        while( node * const next = last->next ){
            last = next;
        }
        // append to the existing chain
        chain_pending_nodes(nodes, last);
    }
    
    // case with chain size=1
    void chain_pending_node(node* node){
        chain_pending_nodes(node, node);
    }
    
    void chain_pending_nodes(node* first, node* last){
        // append a chain to existing
        // existing chain was created 
        // while we tried to deallocate the first chain
        last->next = to_be_deleted;
        while( !to_be_deleted.compare_exchange_weak(last->next, first) );
    }
    
    void try_reclaim(node* old_head){
        
        // This is the only thread in pop(), therefore
        // node that just deleted could be deallocated
        if(threads_in_pop == 1){
            
            // aquire pending nodes list
            node* nodes_to_delete = to_be_deleted.exchange(nullptr);
            
            // if nobody comes to pop(), deallocate also those nodes
            // which waiting for delete
            if(!--threads_in_pop){
                // nobody called pop(), delete pending nodes
                delete_nodes(nodes_to_delete);
            }
            else if(nodes_to_delete){
                // somebody called pop(), add nodes we were going to delete
                // to the head of pending list and keep waiting for deallocation
                chain_pending_nodes(nodes_to_delete);
            }
            
            // deallocate just deleted node in any case
            delete old_head;
        }
        else{
            // pop() called more than one time, add pending node for deletion
            chain_pending_node(old_head);
            --threads_in_pop;
        }
    }
private:
    
   
    // pointer to the stack top
    std::atomic<node*> head;
    
    // counter of thread accessed pop() method
    std::atomic<unsigned> threads_in_pop;
    
    // 'lazy' nodes deleting (when pop() is not loaded)
    std::atomic<node*> to_be_deleted;
};

///////////////////////////////////////////////////////////
// test stack
void add_stack_values(lock_free_stack_lazy<int>& s){
    s.push(4);
    s.push(5);
    s.push(6);
}

void read_stack_values(lock_free_stack_lazy<int>& s){
    std::shared_ptr<int> zero;
    std::shared_ptr<int> spi = s.pop();
    while (spi){
        std::cout << *spi << std::endl;
        spi = s.pop();
    }
}

int main(int argc, char* argv[]) {
    
    try{
        lock_free_stack_lazy<int> s;
        std::thread t1(add_stack_values, std::ref(s));
        s.push(1);
        s.push(2);
        s.push(3);
        t1.join();
        std::thread t2(read_stack_values, std::ref(s));
        t2.join();
    }
    catch(const std::exception& e){
    }
    return 0;
}
