#include <boost/locale/boundary.hpp>
#include <boost/locale/conversion.hpp>
#include "../include/operations_with_text.h"

void count_words(const std::vector<std::string> &words_list, std::map<std::string, int> &dictionary, int start, int end) {

	std::string word;
	for (int i=start; i < end; i++) {
		word = words_list[i];
		dictionary[word] += 1;
	}
}

std::vector<std::string> slice_text(std::string &text) {
	text = boost::locale::normalize(text, boost::locale::norm_nfd);
	text = boost::locale::fold_case(text);

	boost::locale::boundary::ssegment_index words(boost::locale::boundary::word, text.begin(), text.end());
	words.rule(boost::locale::boundary::word_any);

	std::vector<std::string> vec_words;

	std::string current_string;
	for (auto it = words.begin(), e = words.end(); it != e; ++it) {
		vec_words.emplace_back((*it));
	}

	return vec_words;
}

bool compare_strings(const std::pair<std::string, int>& pair1, const std::pair<std::string, int>& pair2) {
	return pair1.first < pair2.first;
}

bool compare_pairs_results(const std::pair<std::string, int>& pair1, const std::pair<std::string, int>& pair2) {
	return pair1.second > pair2.second;
}

