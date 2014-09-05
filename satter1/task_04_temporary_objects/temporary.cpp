
#include <algorithm>
#include <iostream>
#include <string>
#include <list>

using namespace std;

// Task 1.6-1.7 From Satter 1
// Temporary objects

struct Employee{

    Employee(const string& name_, const string& addr_):name(name_),addr(addr_){}
    Employee(const string& name_):name(name_),addr(){}

    // comparasion
    bool operator==(const Employee& rhs) const {
        return (name == rhs.name) && (addr == rhs.addr);
    }
    bool operator!=(const Employee& rhs) const {
        return !(*this == rhs);
    }

    string name;
    string addr;
};

// inefficient function
// ERROR: emps by value
// ERROR: name by value
string FindAddr(list<Employee> emps, string name){

    // ERROR: return emps.end() every iteration (could be temporary object!)
    // ERROR: it++ instead of ++it
    for(list<Employee>::iterator it = emps.begin(); it != emps.end() ; it++)
    {
        // ERROR: name turns to Employee temporary object
        if(*it == name){
            return it->addr;
        }
    }
    return "";
}

// efficient version
string FindAddr2(const list<Employee>& emps, const string& name){
    list<Employee>::const_iterator it(find(emps.begin(), emps.end(), name));
    if(it != emps.end()){
        return it->addr;
    }
    return "";
}


int main(){
    list<Employee> e;
    string s1("Skotobaza");
    e.push_back(Employee(s1, "Atatako"));
    e.push_back(Employee("Komatoza", "Upyachka"));
    string addr = FindAddr(e, s1);
    cout << addr << endl;

    addr = FindAddr(e, s1);
    cout << addr << endl;

    return 0;
}
