#ifndef COUNTING_WORDS2_OPERATIONS_WITH_TEXT_H
#define COUNTING_WORDS2_OPERATIONS_WITH_TEXT_H

#include <vector>
#include <string>
#include <map>
#include <fstream>

void count_words(const std::vector<std::string> &words_list, std::map<std::string, int> &dictionary, int start, int end);

std::vector<std::string> slice_text(std::string &text);

bool compare_strings(const std::pair<std::string, int>& pair1, const std::pair<std::string, int>& pair2);

bool compare_pairs_results(const std::pair<std::string, int>& pair1, const std::pair<std::string, int>& pair2);

template<typename funcT>
int sort_given_comparator(const std::map<std::string, int>& words, const std::string& file_name, funcT comparator) {
	std::vector<std::pair<std::string, int>> words_vector (words.begin(), words.end());
	sort(words_vector.begin(), words_vector.end(), comparator);
	std::ofstream out(file_name);
	if (out.is_open()) {
		for (const auto &word : words_vector) {
			out << word.first << "  " << std::to_string(word.second) << "\n";
		}
		out.close();
		return 0;
	}
	return -1;
}

#endif //COUNTING_WORDS2_OPERATIONS_WITH_TEXT_H
