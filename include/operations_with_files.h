#ifndef COUNTING_WORDS2_OPERATIONS_WITH_FILES_H
#define COUNTING_WORDS2_OPERATIONS_WITH_FILES_H
#include <string>
std::string read_as_raw(const char* filename);

bool check_file(std::string filename);

std::string read_archive_from_memory(const std::string& buffer);

void fill_container_with_archives_names(const std::string& directory, std::vector<std::string> &container );

#endif //COUNTING_WORDS2_OPERATIONS_WITH_FILES_H
