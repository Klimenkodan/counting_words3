#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <functional>
#include "../include/config_parser.h"

std::string parseConf(const std::string& line) {
	std::string res;
	bool flag = false;
	for (auto c = line.begin(); c != line.end(); c++) {
		if (flag && *c != '"') {
			res += *c;
		}
		if (*c == '=') {
			flag = true;
		}
	}
	return res;
}

configuration_t read_conf(std::istream& cf)
{
	std::ios::fmtflags flags(cf.flags()); // Save stream state
	cf.exceptions(std::ifstream::failbit); // Enable exception on fail

	configuration_t res;
	std::string temp;

	try {
		std::getline(cf, temp);
		res.name_dir = parseConf(std::ref(temp));
		std::getline(cf, temp);
		res.a_out_a = parseConf(std::ref(temp));
		std::getline(cf, temp);
		res.a_out_n = parseConf(std::ref(temp));
		std::getline(cf, temp);
		res.num_indexers = stoi(parseConf(std::ref(temp)));
		std::getline(cf, temp);
		res.num_mergers = stoi(parseConf(std::ref(temp)));
		std::getline(cf, temp);
		res.files_size = static_cast<size_t>(stoi(parseConf(std::ref(temp))));
		std::getline(cf, temp);
		res.maps_size = static_cast<size_t>(stoi(parseConf(std::ref(temp))));

	}catch(std::ios_base::failure &fail) // Life without RAII is hard...
	{
		cf.flags(flags);
		throw; // re-throw exception
	}
	cf.flags(flags);
	if( res.num_mergers < 1) {
		throw std::runtime_error("number of mergers should be more than 0");
	}

	if (res.num_indexers < 1) {
		throw std::runtime_error("number of indexers should be more than 0");
	}

	if (res.maps_size < 1) {
		throw std::runtime_error("number of threads should be more than 0");
	}

	if (res.files_size < 1) {
		throw std::runtime_error("number of threads should be more than 0");
	}
	return res;
}