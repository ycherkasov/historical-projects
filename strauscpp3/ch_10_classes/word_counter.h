#pragma once
#include <string>
#include <map>

// Задача из Страуструпа (10.19) и с собеседования в геймдеве 
// http://blog.gamedeff.com/?p=64

class word_counter
{
	static std::map<std::string, int> _word_counter;
public:
	static std::map<std::string, int> count_from_file(const std::string& filename);
};
