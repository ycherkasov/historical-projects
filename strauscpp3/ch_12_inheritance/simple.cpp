#include "simple.h"
#include <iostream>
using std::cout;
using std::endl;

//--------------------------------------
control_t::control_t() : _x(), _y(){
	cout << "control_t() construction" << endl;
}

control_t::~control_t(){
	cout << "~control_t() destruction" << endl << endl;
}

void control_t::draw(){
	cout << "control_t::draw()" << endl;
}

void control_t::hide(){
	cout << "control_t::hide()" << endl;
}

void control_t::show(){
	cout << "control_t::show()" << endl;
}

//--------------------------------------
button_t::button_t() : control_t(), _state(){
	cout << "button_t() construction" << endl;
}

button_t::~button_t(){
	cout << "~button_t() destruction" << endl << endl;
}

void button_t::push(){
	cout << "button_t::push()" << endl;
}

void button_t::draw(){
	cout << "button_t::draw()" << endl;
}

//--------------------------------------
slider_t::slider_t() : control_t(), _position(), _max(), _min(){
	cout << "slider_t() construction " << endl;
}

slider_t::~slider_t(){
	cout << "~slider_t() destruction " << endl << endl;
}

void slider_t::move(int pos){
	cout << "slider_t::move(int pos) " << pos << endl;
}

//--------------------------------------
window_draw::window_draw() : button_t(), slider_t(), _gdi(){
	cout << "window_draw() construction" << endl;
}

window_draw::~window_draw(){
	cout << "~window_draw() destruction" << endl << endl;
}

void window_draw::draw(){
	cout << "window_draw::draw()" << endl;
}

void window_draw::hide(){
	cout << "window_draw::hide()" << endl;
}

void window_draw::show(){
	cout << "window_draw::show()" << endl;
}
