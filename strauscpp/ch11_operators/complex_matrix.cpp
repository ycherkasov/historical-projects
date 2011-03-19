#include "complex.h"

// �������� - � ������ ������������� ����� ���������� ����� ���������
// � ������ ������ new complex_t*()...
complex_matrix::complex_matrix(const size_t count_x) 
		: _count_x(count_x)
		, _count_y()
		, _matrix( new complex_t*() ) {

	_matrix[0] = new complex_t[count_x];
}

// ... � ����� -  new complex_t*[count_y]
complex_matrix::complex_matrix(const size_t count_x, const size_t count_y)
		: _count_x(count_x)
		, _count_y(count_y)
		, _matrix(new complex_t*[count_y]) {

	for( unsigned i = 0 ; i < count_y ; i++ ){
		_matrix[i] = new complex_t[count_x];
	}
}

complex_matrix::complex_matrix( const complex_matrix& c )
		: _count_x(c._count_x)
		, _count_y(c._count_y)
		, _matrix(new complex_t*[c._count_y]) {

	for( unsigned i = 0 ; i < c._count_y ; i++ ){
		_matrix[i] = new complex_t[c._count_x];
		
		for(unsigned j = 0 ; j < c._count_x ; j++ ){
			_matrix[i][j] = c(i,j);
		}
	}
}

// ��� ������������ ������� ������������ ������������
// �� ������ �������� ��� ����, �� ����� ���������� ���������
// (���� �� ������������� ��� ����������� ������� _NO_SWAP)
#ifdef _NO_SWAP
complex_matrix& complex_matrix::operator =( const complex_matrix& c ){

	// ��������� �� ����������� �� ������ ����
	if(this != &c){

		// ���������� ����� �������
		size_t new_size_x = c._count_x;
		size_t new_size_y = c._count_y;

		// �������� ������ ��� ����� �������
		complex_t** new_matrix = new complex_t*[c._count_y];
		for( unsigned i = 0 ; i < c._count_y ; i++ ){
			new_matrix[i] = new complex_t[c._count_x];

			for(unsigned j = 0 ; j < c._count_x ; j++ ){
				new_matrix[i][j] = c(i,j);
			}
		}
		// �������� ������ ������
		clear();

		// �������� �� �������� ����� �������
		_matrix = new_matrix;

	}
	return *this;
}
#else
// ���� ������� ����� ����������, �.�. ��� ������ �� ��������� ������ � ������� 
// ��������� ����������� ����������� � �-��� clear()
complex_matrix& complex_matrix::operator =( const complex_matrix& c ){
	complex_matrix* tmp = new complex_matrix(c);
	clear();
	_matrix = tmp->_matrix;
	return *this;
}
#endif // _NO_SWAP



complex_matrix::~complex_matrix(){
	clear();
}

void complex_matrix::clear(){
	for( unsigned i = 0 ; i < _count_y ; i++ ){
		delete[] _matrix[i];
	}
	delete[] _matrix;
	_matrix = 0; // �� �������� ��������!
}

// �� 2 ������ ���������� ��������������
complex_t* complex_matrix::operator[](unsigned index){
	return _matrix[index];
}

complex_t& complex_matrix::operator()(unsigned index_x, unsigned index_y){
	return _matrix[index_x][index_y];
}

complex_t* complex_matrix::operator[](unsigned index) const{
	return _matrix[index];
}

complex_t& complex_matrix::operator()(unsigned index_x, unsigned index_y) const{
	return _matrix[index_x][index_y];
}
