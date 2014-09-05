#include <algorithm>
#include <vector>

using namespace std;

// Task 1.4-1.5 From Satter 1
// Template constructor

// fixed vector with trmplate copy constructor
template <typename T, size_t size>
class fixed_vector{
public:

    // iterator stuff
    typedef T* iterator;
    typedef const T* const_iterator;

    iterator begin(){
        return _v;
    }

    iterator end(){
        return _v + size;
    }


    const_iterator begin() const {
        return _v;
    }

    const_iterator end() const {
        return _v + size;
    }

    // constructors

    // default (because we defined some constructors with parameters)
    fixed_vector(){}

    // template copy constructor
    // Will never be called for the same type!
    template<typename OtherT, size_t otherSize>
    fixed_vector(const fixed_vector<OtherT, otherSize>& rhs){
        copy(rhs.begin(), rhs.begin() + min(size, otherSize), begin());
    }

    // template assignment
    // Will never be called for the same type!
    template<typename OtherT, size_t otherSize>
    fixed_vector& operator=(const fixed_vector<OtherT, otherSize>& rhs){
        // *this chack is not neccessary - different types!!!
        copy(rhs.begin(), rhs.begin() + min(size, otherSize), begin());
        return *this;
    }

    // copy-constructor and operator= for fixed_vector<T, size> generated automatically!
    // template copy-constructor and assignmant operator will never replace them!

private:
    T _v[size];
};

int main(){

    vector<int> v(1, 20);
    fixed_vector<int, 20> intv1;
    copy(v.begin(), v.end(), intv1.begin());

    // Default-generated copy constructor is called!
    fixed_vector<int, 20> intv2(intv1);

    // Temaplte copy constructor is called!
    fixed_vector<int, 40> intv3(intv1);

    // Temaplte copy constructor is called!
    fixed_vector<double, 40> dblv1(intv1);

    // Temaplte assignment is called!
    intv3 = dblv1;

    return 0;
}
