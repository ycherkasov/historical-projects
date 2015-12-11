#include <iostream>
#include <map>

/*
Chapter 08 Jeff Eldger

Sparse arrays and iterators

* sparse array could be saved as a collection of indexes and values
* index could be overloaded with operator> for map or != for unordered map

*/

// Class of array index so that pass to sparse_array::operator[]
class array_index{
public:

    array_index(size_t x, size_t y) : x_(x), y_(y){}

    bool operator ==(const array_index& other) const {
        return (x_ == other.x_) && (y_ == other.y_);
    }

    bool operator <(const array_index& other) const {
        if (x_ != other.x_) {
            return (x_ < other.x_);
        }
        else if (y_ != other.y_){
            return (y_ < other.y_);
        }
        // equal
        return false;
    }

    bool operator>(const array_index& other) const {
        if (x_ != other.x_) {
            return (x_ > other.x_);
        }
        else if (y_ != other.y_){
            return (y_ > other.y_);
        }
        // equal
        return false;
    }

    size_t get_x() const { return x_; }
    void get_x(size_t val) { x_ = val; }
    size_t get_y() const { return y_; }
    void get_y(size_t val) { y_ = val; }

private:
    size_t x_;
    size_t y_;
};


// Sparse array based on map
template <typename T>
class sparse_array{
public:

    // set array size
    sparse_array(size_t x, size_t y) : x_size_(x), y_size_(y){}

    T& operator[](const array_index& index){
        if ((index.get_x() >= x_size_) || (index.get_y() >= y_size_)){
            throw std::range_error("out of array bounds");
        }
        return array_[index];
    }

    const T& operator[](const array_index& index) const {
        if ((index.get_x() >= x_size_) || (index.get_y() >= y_size_)){
            throw std::range_error("out of array bounds");
        }
        return array_[index];
    }

private:
    size_t x_size_;
    size_t y_size_;
    std::map<array_index, T> array_;
};


void show_sparse_array(){

    sparse_array<int> arr(1000, 1000);
    arr[array_index(100, 100)] = 100;
    arr[array_index(200, 200)] = 200;
    std::cout << arr[array_index(300, 300)] << std::endl;
}

int main(){
    show_sparse_array();
    return 0;
}
