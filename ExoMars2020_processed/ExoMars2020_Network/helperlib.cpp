#include "helperlib.h"

std::string formatted_time_stamp()
{
	unsigned short fs(0), ps(0), ns(0), us(0), ms(0), s(0), m(0), h(0);
	uint64 val = sc_time_stamp().value()*(1e-12 / sc_get_time_resolution().to_seconds());
	ps = val % 1000; val /= 1000;
	ns = val % 1000; val /= 1000;
	us = val % 1000; val /= 1000;
	ms = val % 1000; val /= 1000;
	s = val % 60; val /= 60;
	h = val % 60; val /= 60;

	std::string tstr;
	if (h) tstr += std::to_string(h) + "h ";
	if (m) tstr += std::to_string(m) + "m ";
	if (s) tstr += std::to_string(s) + "s ";
	if (ms) tstr += std::to_string(ms) + "ms ";
	if (us) tstr += std::to_string(us) + "us ";
	if (ns) tstr += std::to_string(ns) + "ns ";
	if (ps) tstr += std::to_string(ps) + "ps ";

	if (tstr.empty()) tstr += "0s ";

	return tstr;
}

ConfigLoader::ConfigLoader(std::string path) : file(path)
{
	std::string tmp, id, target;
	int n, addr;
	if (file)
	{
		while (file >> tmp)
		{
			if (tmp == "--")
			{
				file >> tmp;
				if (tmp == "Part")
				{
					file >> n;
					file >> tmp >> id;
					while (id != "--" && !file.eof())
					{
						file >> tmp;
						if (tmp == ":")
						{
							file >> addr;
							if (!la[n-1][id])
								la[n-1][id] = addr;
							else
							{
								if (la[n-1][id] != addr)
									std::cerr << "Logical address mismatch for " << id << ", keeping old one: " << la[n-1][id] << std::endl;
							}
						}
						else if (tmp == "->")
						{
							file >> target;
							if (!la[n-1][id])
							{
								std::cerr << "Logical address for " << id << " not set in network part " << n << std::endl;
								exit(-1);
							}
							else if (!la[n-1][target])
							{
								std::cerr << "Logical address for " << target << " not set in network part " << n << std::endl;
								exit(-1);
							}
							else
							{
								parts[n-1].push_back(std::array<int, 2>{la[n-1][id],la[n-1][target]});
							}
						}
						file >> id;
					}
					file.seekg(-2, file.cur);
				}
				else
				{
					std::cerr << "'Part n' expected after '--', found " << tmp << std::endl;
					exit(-1);
				}
			}
			else
			{
				std::cerr << "Config file should begin with '-- Part n --', found " << tmp << std::endl;
				exit(-1);
			}
		}
	}
	else
		std::cerr << "File not found at " << path << std::endl;
}

ConfigLoader::~ConfigLoader()
{
	file.close();
}

const std::vector<std::array<int, 2>>& ConfigLoader::get_first_part() const
{
	return parts[0];
}

const std::vector<std::array<int, 2>>& ConfigLoader::get_second_part() const
{
	return parts[1];
}

const std::map<std::string, int>& ConfigLoader::get_first_part_la() const
{
	return la[0];
}

const std::map<std::string, int>& ConfigLoader::get_second_part_la() const
{
	return la[1];
}
