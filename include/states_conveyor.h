#ifndef COUNTING_WORDS2_STATES_CONVEYOR_H
#define COUNTING_WORDS2_STATES_CONVEYOR_H
#include "safeQueue.h"
#include <map>

void merge(safeQueue<std::map<std::string, int>>& que);

void indexer(safeQueue<std::pair<std::string, std::string>>& raw_files, safeQueue<std::map<std::string, int>>& maps, std::mutex& m, int& num_current_threads);
void fill_queue(const std::string& directory, safeQueue<std::pair<std::string, std::string>> &sq);

#endif //COUNTING_WORDS2_STATES_CONVEYOR_H
