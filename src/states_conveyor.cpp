#include "../include/states_conveyor.h"
#include "../include/operations_with_text.h"
#include "../include/operations_with_files.h"
#include "boost/filesystem.hpp"

void merge(safeQueue<std::map<std::string, int>>& que) {
	while (true) {

		std::map<std::string, int> map1 = que.popFront();
		if (map1.empty()) { // poison pill
			que.pushBack(map1);
			break;
		}
		std::map<std::string, int> map2 = que.popFront();
		if (map2.empty()) {
			que.pushBack(map1);
			que.pushBack(map2);
			break;
		}
		for (const auto &element : map2) {
			map1[element.first] += element.second;
		}

		que.pushBack(map1);
	}

}


//void indexer(safeQueue<std::string>& raw_files, safeQueue<std::map<std::string, int>>& maps, std::mutex& m, int& num_current_threads) {
//	while (true) {
//		std::string raw_file = raw_files.popFront();
//		if (raw_file == "POISON_PILL") {
//			raw_files.pushBack(raw_file);
//			break;
//		}
//
//		std::string data = read_archive_from_memory(std::ref(raw_file));
//
//		std::vector<std::string> words = slice_text(std::ref(data));
//
//		std::map<std::string, int> resMap;
//		count_words(std::ref(words), std::ref(resMap), 0, words.size());
//		maps.pushBack(resMap);
//
//	}
//
//	m.lock();
//	num_current_threads --;
//	if (num_current_threads == 0) {
//		std::map<std::string, int> poison_pill;
////		poison_pill["poison_pill"] = 1;
//		maps.pushBack(poison_pill);
//	}
//	m.unlock();
//}

void indexer(safeQueue<std::pair<std::string, std::string>>& raw_files, safeQueue<std::map<std::string, int>>& maps, std::mutex& m, int& num_current_threads) {
	while (true) {
		std::pair<std::string, std::string> raw_file = raw_files.popFront();
		if (raw_file.second == "POISON_PILL") {
			raw_files.pushBack(raw_file);
			break;
		}
		std::string data;
		if (raw_file.second == ".txt") {
			data = raw_file.first;
		}
		else {
			data = read_archive_from_memory(std::ref(raw_file.first));
		}

		std::vector<std::string> words = slice_text(std::ref(data));

		std::map<std::string, int> resMap;
		count_words(std::ref(words), std::ref(resMap), 0, words.size());
		maps.pushBack(resMap);

	}

	m.lock();
	num_current_threads --;
	if (num_current_threads == 0) {
		std::map<std::string, int> poison_pill;
//		poison_pill["poison_pill"] = 1;
		maps.pushBack(poison_pill);
	}
	m.unlock();
}


void fill_queue( const std::string& directory, safeQueue<std::pair<std::string, std::string>> &sq ){

	using recursive_directory_iterator = boost::filesystem::recursive_directory_iterator;

	for (const auto& dirEntry : recursive_directory_iterator( directory )){
		if ( check_file(dirEntry.path().string())){
			auto file = std::pair<std::string, std::string>(read_as_raw(dirEntry.path().string().c_str()), boost::filesystem::extension(dirEntry.path().string()));
			sq.pushBack(std::ref(file));
		}
	}

	std::pair<std::string, std::string> poison_pill = std::make_pair("POISON_PILL", "POISON_PILL");
	sq.pushBack(std::ref(poison_pill));

}

//void fill_queue_consecutive( std::string directory, safeQueue<std::pair<std::string, std::string>> &sq ){
//	using recursive_directory_iterator = boost::filesystem::recursive_directory_iterator;
//
//	for (const auto& dirEntry : recursive_directory_iterator( directory )){
//		if ( check_file(dirEntry.path().string())){
//			sq.pushBack( std::make_pair(read_as_raw(dirEntry.path().string().c_str()), boost::filesystem::extension(dirEntry.path().string())));
//		}
//	}
//}