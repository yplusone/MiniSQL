
#ifndef INDEX_MANAGER_H
#define INDEX_MANAGER_H
#include <string>
#include <map>
#include"Basic.h"
#include "Bplus_tree.h"


class Index_manager {
public:
	Index_manager(std::string table_name);//constructio F
	~Index_manager();//destruction F
	void create_index(std::string file_path, int type);
	bool drop_index(int&index, std::string file_path, int type);
	bool find_index(int&index, std::string file_path, Data data);
	void insert_index(std::string file_path, Data data, int block_id);
	void delete_index_key(std::string file_path, Data data);
	void search_range(std::string file_path, Data data1, Data data2, std::vector<int>& values_out);
	//int find_index(std::string file_path,)

private:
	typedef std::map<std::string, b_plus_tree<int>* > int_map;
	typedef std::map<std::string, b_plus_tree<float>* > float_map;
	typedef std::map<std::string, b_plus_tree<std::string>* > string_map;

	int_map _int_map;
	float_map _float_map;
	string_map _string_map;

	const static int int_type = -1;
	const static int float_type = 0;


	int calc_degree(int type);
	int calc_key_size(int type);

};

#endif

