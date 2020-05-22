#include <fstream>
#include <sstream>
#include <archive_entry.h>
#include <archive.h>
#include <boost/filesystem.hpp>
#include "../include/operations_with_files.h"


bool check_file(std::string filename){

	auto result = false;
	char extensions[][8] = {"txt", "zip", "tar", "sbx", "iso"};

	for( auto &x: extensions ){
		result |= filename.substr(filename.find_last_of('.') + 1) == x;
	}
	return result;
}

std::string read_as_raw(const char* filename){
	std::ifstream raw_file(filename, std::ios::binary);
	std::ostringstream buffer_ss;
	buffer_ss << raw_file.rdbuf();
	std::string buffer{buffer_ss.str()};
	return buffer;
}


std::string read_archive_from_memory(const std::string& buffer){
	struct archive *a;
	struct archive_entry *entry;

	a = archive_read_new();
	archive_read_support_filter_all(a);
	std::string content, total;

	archive_read_support_format_all(a);
	archive_read_open_memory(a, buffer.c_str(), buffer.size());

	while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {

		if ( check_file(archive_entry_pathname(entry) ) && archive_entry_size(entry) < 10000000 ){
			auto size = archive_entry_size(entry);
			content = std::string(size, 0);

			archive_read_data(a, &content[0], content.size());
			total += content + "\n";
		}
		archive_read_data_skip(a);
	}
	archive_read_free(a);
	return total;
}

void fill_container_with_archives_names(const std::string& directory, std::vector<std::string> &container ){
	using recursive_directory_iterator = boost::filesystem::recursive_directory_iterator;

	for (const auto& dirEntry : recursive_directory_iterator( directory )){
		if ( check_file( dirEntry.path().string() ) ) {
			container.push_back(dirEntry.path().string());
		}
	}
}
