#include <iostream>
#include <map>
#include <set>
#include <string>

using namespace std;

// Task 1.15 From Satter 1
// Map and set


// want to change key 1 to 99 if exists
void change_map_key(map<int, string>& m){

    map<int, string>::iterator it = m.find(1);
    if(it != m.end()){
        // Wrong way - it will brake RB-tree!!!
        // const_cast<int&>(it->first) = 99;

        // Correct way - re-insert the key-value
        string s = it->second;
        m.erase(it);
        m.insert(make_pair(99, s));
    }
}

void show_map_change(){
    map<int, string> m;
    m[1] = "atatat";
    m[2] = "tatata";
    m[3] = "skotobaza";
    change_map_key(m);
}

//////////////////////////////////////////////////////////////////////////
// Standard does not consequent about key constancy, because key changing (in case of the key is class)
// could not to mean their order has been changed
// One STL implementation could require it, another - does not

// Struct which order does not depend on value
struct Pair
{
    Pair(int k, double v):key(k),value(v){}
    Pair(int k):key(k),value(){}
    int key;
    double value;

    friend bool operator<(const Pair& l, const Pair& r){
        return l.key < r.key;
    }

    friend bool operator>(const Pair& l, const Pair& r){
        return l.key > r.key;
    }

    friend bool operator==(const Pair& l, const Pair& r){
        return l.key == r.key;
    }

    friend bool operator!=(const Pair& l, const Pair& r){
        return !operator==(l, r);
    }

};


// want to change part of the set key if exists
void change_set_key(set<Pair>& s){
    set<Pair>::iterator it = s.find(1);
    if(it != s.end()){
        // Wrong way - key is konstant, even if its order does not changed
        //it->value = 99.0;

        // Correct way
        s.erase(it);
        s.insert(Pair(1, 99.0));
    }
}

void show_set_change(){
    set<Pair> s;
    s.insert(Pair(1, 1.0));
    s.insert(Pair(2, 2.0));
    s.insert(Pair(3, 3.0));
    s.insert(Pair(4, 4.0));
    change_set_key(s);
}

int main(){
    show_map_change();
    show_set_change();
    return 0;
}

