#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

// Task 1.13-1.14 From Satter 1
// Vector and deque

// Helper - array size
template <typename T, std::size_t N>
std::size_t array_size(const T(&) [N]) {
    return N;
}

void shrink_vector_to_fit(){

    int arr[] = {0,1,1,2,2,7,8,5,3};

    vector<int> v;
    v.reserve(1000);
    v.assign(arr, arr + array_size(arr));

    // large amount of memory, shall amount of data
    cout << "vector capacity = " << v.capacity() << endl;

    // try to shrink to fit
    v.reserve(10);

    // nothing happens
    cout << "vector capacity = " << v.capacity() << endl;

    // shrink-to-fit trick (pre-C++11 only)
    vector<int>(v).swap(v);

    // it works?
    cout << "vector capacity = " << v.capacity() << endl;

    // try to clear, does not deallocate anyway
    v.clear();
    cout << "vector capacity = " << v.capacity() << endl;

    // "real" clear
    vector<int>().swap(v);
    cout << "vector capacity = " << v.capacity() << endl;

    // new way
    vector<int> v1;
    v1.reserve(1000);
    cout << "vector1 capacity = " << v1.capacity() << endl;
    v1.assign(arr, arr + array_size(arr));
    v1.shrink_to_fit();
    cout << "vector1 capacity = " << v1.capacity() << endl;

    // try to clear
    v.clear();
    v1.shrink_to_fit();
    cout << "vector1 capacity = " << v1.capacity() << endl;

    // deque does not require that trick!
}

void vector_bool(){
    bool arr[] = {true, false, true, false, true, false};
    vector<bool> v;
    v.assign(arr, arr + array_size(arr));

    // vector<bool> is proxy, it is less efficient on standard algorithms
    for(vector<bool>::iterator it  = v.begin(); it != v.end(); ++it){
        cout << "Unpack from vector<bool>: " << (*it) << endl;
    }
}


int main(){
    shrink_vector_to_fit();
    vector_bool();
    return 0;
}

