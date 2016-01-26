#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Task 1.1 From Satter 1
// The most common errors while using iterators

// string date representation
typedef std::string Date;


// Helper - array size
template <typename T, std::size_t N>
std::size_t array_size(const T(&) [N]) {
    return N;
}


int main(){

    Date todayDate("01/08/2014");
    vector<Date> e;

    // copy from some source to vector
    Date source[] = {"01/01/2000", "01/01/2001", "01/01/2010", "01/01/2014", "01/01/2015"};
    size_t sz = array_size(source);
    copy(source, source + sz, back_inserter(e));

    // find begin and end of some range
    // ERROR: end() check is absent
    vector<Date>::iterator first = find(e.begin(), e.end(), "01/01/2000");
    // ERROR: end() check is absent
    vector<Date>::iterator last = find(e.begin(), e.end(), "01/01/2014");

    // Finded range output
    // ERROR: first could be after last
    // ERROR: first or last coudl be end()
    copy(first, last, ostream_iterator<Date>(cout, "\n"));

    // Insert before the end today date
    // ERROR: e.end() could be temporary Date* pointer
    // C++ does not allow changing temporary builtin variables (could be (e.end() - 1))
    // ERROR: vector could be empty
    e.insert(--e.end(), todayDate);

    // Range with today output
    // ERROR: after previous insert() iterators could be invalidated
    copy(first, last, ostream_iterator<Date>(cout, "\n"));

    return 0;
}
