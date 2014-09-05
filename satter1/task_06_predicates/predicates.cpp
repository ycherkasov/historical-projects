#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Task 1.9-1.10 From Satter 1
// Predicates

// Helper - array size
template <typename T, std::size_t N>
std::size_t array_size(const T(&) [N]) {
    return N;
}

// Helper - print container
template<typename T, template <typename ELEM, typename = std::allocator<ELEM> > class CONT >
void print_container(const CONT<T>& c){
    CONT<T>::const_iterator it = c.begin();
    while (it != c.end()){
        cout << (*it) << ' ';
        ++it;
    }
    cout << endl;
}


// remove() algirithm does not actually remove objects
// it looks like that
template <class ForwardIterator, class T>
ForwardIterator my_remove(ForwardIterator first, ForwardIterator last, const T& val)
{
    ForwardIterator result = first;
    while (first!=last) {
        if (!(*first == val)) {
            *result = *first;
            ++result;
        }
        ++first;
    }
    return result;
}

// so its usage
void show_remove(){
    int arr[] = {0,1,1,2,2,7,8,5,3};
    vector<int> v(arr, arr + array_size(arr));
    print_container(v);
    vector<int>::iterator it1 = my_remove(v.begin(), v.end(), 1);
    print_container(v);

    v.erase( it1, v.end() );
    print_container(v);
    // just move to the end

    // standard remove
    vector<int>::iterator it2 = remove(v.begin(), v.end(), 2);
    v.erase( it2, v.end() );
    print_container(v);
}


// remove n-th algorithm
template <class ForwardIterator, class T>
ForwardIterator remove_nth_incorrect(ForwardIterator first, ForwardIterator last, size_t n)
{
    // move to n-th
    // ERROR: do not check ( [first..last] > n )
    // ERROR: for random iterators could be just (begin + n)
    // use advance
    for(;n > 0; --n, ++first);

    if(first != last){
        ForwardIterator dest = first;
        // just move all following elements left, erasing current element
        return copy(++first, last, dest);
    }
    return last;
}


// remove n-th algorithm, correct version
template <class ForwardIterator>
ForwardIterator remove_nth(ForwardIterator first, ForwardIterator last, size_t n)
{
    // check size
    if(distance(first, last) < n){
        return last;
    }
    // move to n-th
    advance(first, n);

    if(first != last){
        ForwardIterator dest = first;
        // just move all following elements left, erasing current element
        return copy(++first, last, dest);
    }
    return last;
}

void show_remove_nth(){
    int arr[] = {0,1,1,2,2,7,8,5,3};
    vector<int> v(arr, arr + array_size(arr));
    print_container(v);
    //(!)remove_nth_incorrect(v.begin(), v.end(), 3);
    vector<int>::iterator it = remove_nth(v.begin(), v.end(), 3);
    print_container(v);
    v.erase( it, v.end() );
    print_container(v);
}

void show_predicates(){
    // try to solve previous task using predicate
    int arr[] = {0,1,1,2,2,7,8,5,3};
    vector<int> v(arr, arr + array_size(arr));
    print_container(v);

    // functor that returns true for n-th element
    struct flag_nth {

        flag_nth(size_t n):_n(n),_current(){}
        bool operator()(const int){
            return ++_current == _n;
        }
    private:
        const size_t _n;
        size_t _current;
    };
    // ERROR: predicate could be copied inside the algorithm!
    remove_if(v.begin(), v.end(), flag_nth(3));
    // NOTE: for_each() able to return modified functor with state
}

int main(){
    show_remove();
    show_remove_nth();
    return 0;
}

