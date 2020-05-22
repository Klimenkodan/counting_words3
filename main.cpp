///------------------------------------------------------------------------------------------------------------------///
/// INCLUDES
#include <iostream>
#include <ostream>
#include <map>
#include <string>
#include <vector>
#include <filesystem>
#include "include/config_parser.h"
#include "include/safeQueue.h"
#include "include/states_conveyor.h"
#include "include/operations_with_text.h"
#include "include/operations_with_files.h"
#include <boost/locale.hpp>
#include <thread>
#include <condition_variable>
#include <boost/filesystem.hpp>


inline std::chrono::high_resolution_clock::time_point get_current_time_fenced(){
	std::atomic_thread_fence(std::memory_order_seq_cst);
	auto res_time = std::chrono::high_resolution_clock::now();
	std::atomic_thread_fence(std::memory_order_seq_cst);
	return res_time;
}

void analyze_queue(safeQueue<std::pair<std::string, std::string>> &sq, std::map<std::string, int> &dictionary ){
	size_t queue_size = sq.size();
	for(size_t i = 0; i < queue_size; i++){
		auto element = sq.popBack();
		std::string txts;
		if (element.second == "txt"){
			txts = element.first;
		}
		else{
			txts = read_archive_from_memory(element.first);
		}
		auto vect = slice_text(txts);
		count_words(vect, dictionary, 0, vect.size());
	}
}


auto consecutive(const std::string& directoryname, size_t que_max_size, safeQueue<std::pair<std::string, std::string>> &sq){

	std::vector<std::string> container;

	fill_container_with_archives_names(directoryname, container);
	std::map<std::string, int> dictionary;
	int k = 0;

	sq.setMaxSize(que_max_size);

	for(const auto& current_archive: container){


		k++;
		sq.pushBack( std::make_pair(read_as_raw( current_archive.c_str()), boost::filesystem::extension(boost::filesystem::path(current_archive)))) ;

		if (sq.size() == sq.max_size - 1){
			analyze_queue(std::ref(sq), std::ref(dictionary));
		}

		std::cout << k << "\n";
	}
	analyze_queue(std::ref(sq), std::ref(dictionary));

	sort_given_comparator(std::ref(dictionary), "results.txt", compare_strings);
	sort_given_comparator(std::ref(dictionary), "results_3.txt", compare_pairs_results);
}



template<class D>
inline long long to_us(const D& d){
	return std::chrono::duration_cast<std::chrono::microseconds>(d).count();
}


int main(int argc, char *argv[]){

	boost::locale::generator gen;
	static std::locale loc = gen("en_US.UTF-8");
	std::locale::global(loc);


	//define config file
	std::string conf;
	if (argc > 2) {
		return 2;
	}
	else if (argc == 1) {

		if (std::filesystem::exists(std::filesystem::path("config.dat"))) {
			conf = "config.dat";
		}
	}
	else {
		std::cout << "now, I'm here " << std::endl;
		conf = argv[1];
	}


	std::ifstream conf_stream(conf);
	if (!conf_stream.is_open()) {
		throw;
	}

	configuration_t confs = read_conf(conf_stream);

	safeQueue<std::pair<std::string, std::string>> files;
	safeQueue<std::map<std::string, int>> maps;

	//set queues max_size
	files.setMaxSize(confs.files_size);
	maps.setMaxSize(confs.maps_size);

	//init reader thread with fill_queue
	auto start = get_current_time_fenced();


	std::thread reader(fill_queue, std::ref(confs.name_dir), std::ref(files));

	//init all indexer threads with count_words
	std::vector<std::thread> indexers;
	std::mutex mx;
	int currThreads = confs.num_indexers;
	for (int i = 0; i < confs.num_indexers; i++) {
		indexers.emplace_back(indexer, std::ref(files), std::ref(maps), std::ref(mx), std::ref(currThreads));
	}

	std::vector<std::thread> mergers;
	for (int i = 0; i < confs.num_mergers; i++) {
		mergers.emplace_back(std::thread(merge, std::ref(maps)));
	}

	reader.join();

	for (auto &c : indexers) {
		c.join();
	}
	auto end = get_current_time_fenced();
	auto diff = end-start;


	for (auto &c: mergers) {
		c.join();
	}


	std::map<std::string, int> res = maps.popFront();

	sort_given_comparator(std::ref(res), std::ref(confs.a_out_n), compare_pairs_results);
	sort_given_comparator(std::ref(res), std::ref(confs.a_out_a), compare_strings);

	for (const auto &c: res) {
		std::cout << c.first + " " << c.second << std::endl;
	}

	std::cout << "Total: " << to_us(diff);

	return 0;

}///------------------------------------------------------------------------------------------------------------------///


