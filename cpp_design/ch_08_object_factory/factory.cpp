#include <map>
#include <iostream>
#include <stdexcept>
#include <functional>
#include <memory>
#include <sstream>

using namespace std;

//////////////////////////////////////////////////////////////////////////
// Hierarchy for generating
enum ids{
    e_derived1,
    e_derived2,
    e_derived3
};


struct base{
    virtual void work() = 0;
};

struct derived1 : public base{
    virtual void work(){ cout << "derived1" << endl; }
};

struct derived2 : public base{
    virtual void work(){ cout << "derived2" << endl; }
};

struct derived3 : public base{
    virtual void work(){ cout << "derived3" << endl; }
};

//////////////////////////////////////////////////////////////////////////
// Error policy helper
template <typename IdentifierType>
struct error_policy{
    static void on_unknown_type(IdentifierType id){
        std::stringstream ss;
        ss << "Unknown type by identifier " << id;
        throw std::runtime_error(ss.str());
    }
};

//////////////////////////////////////////////////////////////////////////
// Abstract factory
template <typename IdentifierType, typename AbstractProduct, template <typename IdentifierType> class ErrorPolicy >
class abstract_factory{
public:

    void register_product(IdentifierType id, std::function<AbstractProduct*()> f){
        auto it = associations_.find(id);
        if (it == associations_.end()){
            associations_[id] = f;
        }
    }

    AbstractProduct* create(IdentifierType id){
        auto it = associations_.find(id);
        if (it != associations_.end()){
            return it->second();
        }
        ErrorPolicy<IdentifierType>::on_unknown_type(id);
        // TODO: could be returned by ErrorPolicy
        return nullptr;
    }
private:
    // Every lambda has a different type - even if they have the same signature
    // We must use a run-time encapsulating container such as std::function
    // so that store lambda
    std::map<IdentifierType, std::function<AbstractProduct*()>> associations_;
};

//////////////////////////////////////////////////////////////////////////
// demonstration Object Factory
void show_object_factory(){

    abstract_factory<ids, base, error_policy> f;
    f.register_product(e_derived1, [&]{return new derived1; });
    f.register_product(e_derived2, [&]{return new derived2; });
    f.register_product(e_derived3, [&]{return new derived3; });

    std::unique_ptr<base> b1(f.create(e_derived1));
    std::unique_ptr<base> b2(f.create(e_derived2));
    std::unique_ptr<base> b3(f.create(e_derived3));
    b1->work();
    b2->work();
    b3->work();
}

int main(){

    show_object_factory();
    return 0;
}
