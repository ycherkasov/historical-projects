#pragma once
#include <string>
#include <map>

// ������ �� ����������� (10.19) � � ������������� � �������� 
// http://blog.gamedeff.com/?p=64

class word_counter
{
	static std::map<std::string, int> _word_counter;
public:
	static std::map<std::string, int> count_from_file(const std::string& filename);
};
