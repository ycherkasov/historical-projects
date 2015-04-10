#include "multimethod.h"
#include <iostream>
using std::cout;
using std::endl;

//--------------------------------------
base::base()
{
}

base::~base()
{
}

//--------------------------------------
void base::multiop( const base& b ){ 
	cout << "base::multiop(const base&)" << endl; 
}
void base::multiop( const derived_a& b ){ 
	cout << "base::multiop(const derived_a&)" << endl; 
}
void base::multiop( const derived_b& b ){ 
	cout << "base::multiop(const derived_b&)" << endl; 
}
//--------------------------------------
void derived_a::multiop( const base& b ){ 
	cout << "derived_a::multiop(const base&)" << endl; 
}
void derived_a::multiop( const derived_a& b ){ 
	cout << "derived_a::multiop(const derived_a&)" << endl; 
}
void derived_a::multiop( const derived_b& b ){ 
	cout << "derived_a::multiop(const derived_b&)" << endl; 
}
//--------------------------------------
void derived_b::multiop( const base& b ){
	cout << "derived_b::multiop(const base&)" << endl; 
}
void derived_b::multiop( const derived_a& b ){
	cout << "derived_b::multiop(const derived_a&)" << endl; 
}
void derived_b::multiop( const derived_b& b ){
	cout << "derived_b::multiop(const derived_b&)" << endl; 
}