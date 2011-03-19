#pragma once

// ������������ �������� (��������������) �������������� ������������

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

	// ���������������� �����, ����� ����������� ������������
	/*virtual*/ void draw();

	// ���� �����
	void push();

private:
	int _state;
};

class slider_t : public control_t{
public:
	slider_t();
	~slider_t();

	// ���� �����, ��������� ���������� � ��������
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

	// ��� ������ ����� ����������� ��������������,
	// �.�. window_draw ��������� 3 ����� ������ control_t
	// � ������ �� ������� ���� draw(), hide() � show()
	/*virtual*/ void draw();
	void hide();
	void show();

	// ��������������� ����� ��������� � ������� �������� ::
	// slider_t::draw(), �� ����� ���������� ������������� (���� ��� ����)

private:
	int _gdi;
};