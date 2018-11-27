#pragma once
#include <systemc.h>
#include <iostream>
#include <map>
#include <array>

std::string formatted_time_stamp();


class ConfigLoader
{
public:
	ConfigLoader(std::string path);
	~ConfigLoader();

	const std::vector<std::array<int, 2>>& get_first_part() const;
	const std::vector<std::array<int, 2>>& get_second_part() const;
	const std::map<std::string, int>& get_first_part_la() const;
	const std::map<std::string, int>& get_second_part_la() const;
private:
	ifstream file;
	std::array<std::vector<std::array<int, 2>>, 2> parts;
	std::array<std::map<std::string, int>, 2> la;
};