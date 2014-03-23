#include <vector>
#include <string>
#include <iostream>
#include <iterator>
#include <fstream>
#include <algorithm>
#include <complex>
#include <valarray>

using namespace std;

// --------------- ��������� --------------- 
// ������� ������������� ��������,
// ����������� �� ����� ������
void show_out_iter()
{
	// (������, ����������� ���������, ��������� �� �����)
	// ����� � ������ ������ �������������� ��� ���������
	ostream_iterator<string> oi(cout);

	*oi = "aaa";
	++oi;
	*oi = " bbb\n";
}

// ������� ������������� ��������, ����������� �� ����� �����
// (������, ����������� ���������, ���������� � ����� ����� � ����������)
void show_istr_iter()
{
	// �������������� �������� ������� �����
	istream_iterator<string> iinput_iter(cin);

	// ����������� ������ ������
	string s1 = *iinput_iter;

	// ����������� ������ ������
	++iinput_iter;
	string s2 = *iinput_iter;

	// ������ ������� ����������
	cout << "First string: " << s1 << '\n' << "Second string: " << s2 << endl;
}

// ������� ������ ���� (������ test.txt), ��������� ����������� �����
// ������� ��������� � ���������� ��������� � ������ ���� (������ test1.txt)
// ������������� ��� ���� ���������� (����� � ������) � �������� ���������
int show_iterators()
{
	// �������� ��� ������ �����������
	ostream_iterator<string> out_iter(cout);

	// ������ ������ �������
	*out_iter = "Input file:";

	// ��������� ����� ������
	// �������������� �������� ������� �����
	istream_iterator<string> iinput_iter(cin);
	string from = *iinput_iter;

	// ������ ������ �������
	++out_iter;
	*out_iter = "Output file:";

	++iinput_iter;
	string to = *iinput_iter;

	// ����� ����� ������� � ������ ���������
	ifstream from_file( from.c_str() );

	// ��������, ��������� � ������ �������
	// (����� ��� ���������)
	istream_iterator<string> from_it_begin(from_file);

	// ����������� ��������� ����� �� ��������� ������� ��������� �� ����� �����
	istream_iterator<string> from_it_end;

	// �������������� ������ ��������� � �������� ����������
	vector<string> b(from_it_begin, from_it_end);

	// ����������� ������
	sort(b.begin(), b.end());

	// ����� ������ ������� � ������ ���������
	ofstream to_file( to.c_str() );

	// ��������, ��������� � ������ �������
	ostream_iterator<string> to_it_begin(to_file, "\n");

	// ��������� ������ ���������� ��������
	unique_copy( b.begin(), b.end(), to_it_begin );

	// ���������� ��������� ������
	return !from_file.eof() && !to_file;
}

// --------------- for_each --------------- 
// ������� ��� ��������� 'foreach'
// ��������� ������� ���������������� ����������
void print_v(const string& s)
{
	cout << s << endl;
}

void show_foreach()
{
	vector<string> st;
	st.push_back("atatat");
	st.push_back("komatoza");
	// for_each() ���������� ��������� �� �������������� ������,
	// �.�. � ��� ����� ����������� �������� ���������� �� ������
	// ������ ���������
	for_each(st.begin(), st.end(),print_v);
}

// --------------- find_if --------------- 
// ����� � ����������
bool is_atatat(const string& s)
{
	return (s == "atatat");
}

void show_findif()
{
	vector<string> st;
	st.push_back("atatat");
	st.push_back("komatoza");
	vector<string>::const_iterator i = 
		find_if(st.begin(), st.end(), is_atatat);
}

// --------------- mem_fun --------------- 
// ������� for_each - 
// ��������� �������� � ������������
// ������������ ����������� �������


// --------------- math --------------- 
// ������� ������������� �������������� ����������� STL
void show_math() {

	// ����������� �����
	complex<double> c1(1. , 2. );
	complex<double> c2(1.5, 0.2);
	complex<double> c3 = c1 + c2;

	// �������������� ������
	valarray<double> vect1(10,2);
	valarray<double> vect2(5,2);
	valarray<double> vect3 = vect1 + vect2;

	// �������� �������������� �������
	// ���������� ������������ ����� � �������
	c3 = pow(c3, 2);

	// ������������ ��������
	vect3 = vect1*vect2;

	// ����������� �������� ������������ �����
	c2 = log(c1);
}


// ----------------------------------
// ����������� ��������� (������� main ����� �� ���������� ��������)
/*
int main(){}
*/

#include <deque>
#include <numeric> 

/** @brief 
Amazon test
Object keeps track of the average of last N numbers in the stream
*/
class RunningAverage{
public:
	/** @param buffer_size - stream size
	*/
	explicit RunningAverage(size_t buffer_size)
		: _buffer_size(buffer_size), _use_prev_calc(false), _current_total(){} 

	/** @param number - add this number to the stream
	Pop the first number in the stream in case of overflow
	*/
	void newNumber(long number){
		if( (_buffer.size() + 1) > _buffer_size){
			_buffer.pop_front();
			_buffer.push_back(number);
			_use_prev_calc = false;
		}
		else{
			_buffer.push_back(number);
		}
		

		if(false == _use_prev_calc){
			_current_total = std::accumulate(_buffer.begin(), _buffer.end(), 0);
			_use_prev_calc = true;
		}
		else{
			_current_total = _current_total + number;
		}
	}

	/** @return Average of all numbers kept in the stream
	In case of empty stream throws an exception
	*/
	long getAverage() const {
		if(_buffer.empty()){
			throw std::runtime_error("There are no numbers in the stream yet");
		}

		return _current_total/_buffer.size();
	}
private:
	std::deque<long> _buffer;
	const size_t _buffer_size;
	mutable bool _use_prev_calc;
	mutable long _current_total;
};

void test_amazon(){
	RunningAverage r(3); // object should keep track of the average of the last three numbers in the stream

	long retval = 0;
	//r.getAverage(); // should throw an exception, because there are no numbers in the stream yet
	r.newNumber(10); // number 10 has arrived in the stream
	retval = r.getAverage(); // should return 10
	r.newNumber(20);
	retval = r.getAverage(); // should return 15
	r.newNumber(30);
	retval = r.getAverage(); // should return 20
	r.newNumber(40);
	retval = r.getAverage();
}

int main()
{
	// �������� � ������� main 
	// (�������� ���������� ��������� � ��������� � ������������)
	// ������ ����������� � try/catch
	try
	{
		// 1. ������������ ���������� �����/������
		show_out_iter();
		show_istr_iter();
		show_iterators();

		// 2. ������������ foreach
		show_foreach();

		// 3. ������������ find_if � ����������
		show_findif();

		// 4. ������������ mem_fun � ����������
		// �������� ������� draw ��� ������� �������� ������������ ������ 'sh'
		// list<Shape*> sh(...);
		// for_each( sh.begin(), sh.end(), mem_fun(&Shape::draw) );

		// 5. ������������ �������������� ������������
		show_math();
	}
	catch (const std::exception& e)
	{
		cout << "exception: " << e.what();
		return 1;
	}

	// ��� �������� ����� ���������� �������
	return 0;
}
