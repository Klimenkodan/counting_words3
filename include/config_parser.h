#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H
struct configuration_t {
	std::string name_dir;
	std::string a_out_n, a_out_a;
	int num_mergers;
	int num_indexers;
	size_t files_size, maps_size;
};

configuration_t read_conf(std::istream& cf);

#endif //CONFIG_PARSER_H