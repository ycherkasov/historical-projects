#pragma once

// Демонстрация простого (невиртуального) множественного наследования

class control_t{
public:
	control_t();
	~control_t();
	/*virtual*/ void draw();
	void hide();
	void show();

private:
	int _x;
	int _y;
};

class button_t : public control_t{
public:
	button_t();
	~button_t();

	// Переопределенные метод, будет перекрывать родительский
	/*virtual*/ void draw();

	// свой метод
	void push();

private:
	int _state;
};

class slider_t : public control_t{
public:
	slider_t();
	~slider_t();

	// свой метод, остальные вызываются у родителя
	void move(int pos);

private:
	int _position;
	int _min;
	int _max;
};

class window_draw :   public slider_t
					, public button_t{
public:
	window_draw();
	~window_draw();

	// эти методы нужно обязательно переопределить,
	// т.к. window_draw наследует 3 копии класса control_t
	// в каждом из которых есть draw(), hide() и show()
	/*virtual*/ void draw();
	void hide();
	void show();

	// неоднозначность можно разрешить с помощью префикса ::
	// slider_t::draw(), но тогда потеряется виртуальность (если она есть)

private:
	int _gdi;
};