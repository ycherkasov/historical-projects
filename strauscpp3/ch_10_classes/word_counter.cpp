#include "word_counter.h"
#include <fstream>
#include <iterator>
#include <vector>
#include <algorithm>

using namespace std;

std::map<std::string, int> word_counter::_word_counter;

std::map<std::string, int> word_counter::count_from_file(const string& filename){

	// ������� ����� ����������� �������������
	_word_counter.clear();

	// ��������� ����
	ifstream f( filename.c_str() );

	// ���� ���������� ��� ������ (������ � �����)
	istream_iterator<string> ii(f);
	istream_iterator<string> eos;

	// ������� ����� (������ ��� ��� �������� ������ ��� ����� ��� ������ ����������)
	vector<string> b(ii, eos);

	// ���������� ��������� ��� �������� ����
	struct counter{

		// ������� (��������� ����������, ������� �������� ������� �����)
		std::map<std::string, int>& _word_counter;
		counter(std::map<std::string, int>& wcount) : _word_counter(wcount){}

		// ���������� ������� ����
		void operator()(const std::string& s)
		{
			std::map<std::string, int>::iterator it = _word_counter.begin();
			if( _word_counter.end() != (it = _word_counter.find(s)) )
				(*it).second++;
			else
				_word_counter.insert( make_pair(s, 1) );
		}
	} c(_word_counter);

	for_each( b.begin(), b.end(), c );
	return _word_counter;
}
