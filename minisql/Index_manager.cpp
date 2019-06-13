#include "Index_manager.h"
#include"Const.h"
#include"Basic.h"
#include"buffer_manager.h"
#include"Catalog_manager.h"
#include<string>
#include<vector>

Index_manager::index_manager(std::string name) {
	Catalog_manager Catalog;
	Attribute attr = Catalog.Get_attr(name);
	for (int i = 0; i < attr.num; i++) {
		if (attr.has_index[i])
			create_index("INDEX_FILE_" + attr.name[i] + "_" + name, attr.type[i]);
	}
}

Index_manager::~index_manager() {
	for (auto temp : _int_map) {
		if (temp.second) {
			temp.second->write_all_disk();
			delete temp.second;
		}
	}

	for (auto temp : _string_map) {
		if (temp.second) {
			temp.second->write_all_disk();
			delete temp.second;
		}
	}

	for (auto temp : _float_map) {
		if (temp.second)
		{
			temp.second->write_all_disk();
			delete temp.second;
		}
	}
}

void index_manager::create_index(std::string file_path, int type) {
	auto key_size = calc_key_size(type);
	const auto degree = calc_degree(type);

	// int
	if (type == int_type)
	{
		auto ptr_to_tree = new b_plus_tree<int>(file_path, degree);
		_int_map.insert(int_map::value_type(file_path, ptr_to_tree));
	}
	// float
	else if (type == float_type)
	{
		auto ptr_to_tree = new b_plus_tree<float>(file_path, degree);
		_float_map.insert(float_map::value_type(file_path, ptr_to_tree));
	}
	// string
	else if (type >= 1 && type <= 255)
	{
		auto ptr_to_tree = new b_plus_tree<std::string>(file_path, degree);
		_string_map.insert(string_map::value_type(file_path, ptr_to_tree));
	}
	// valid type ÎÞÐ§ÀàÐÍ
	else
	{
		std::cout << "Error in create_index ! valid type." << std::endl;
		exit(-1);
	}
}

bool index_manager::drop_index(int& index, std::string file_path, int type)
{
	if (type == int_type)
	{
		auto iterator = _int_map.find(file_path);
		if (iterator == _int_map.end())
		{
			std::cout << "Error!" << std::endl;
			return false;
		}
		delete iterator->second;
		_int_map.erase(iterator);
		return true;
	}
	if (type == float_type)
	{
		auto iterator = _float_map.find(file_path);
		if (iterator == _float_map.end())
		{
			std::cout << "Error!" << std::endl;
			return false;
		}
		delete iterator->second;
		_float_map.erase(iterator);
		return true;
	}
	if (type >= 1 && type <= 255)
	{
		auto iterator = _string_map.find(file_path);
		if (iterator == _string_map.end())
		{
			std::cout << "Error!" << std::endl;
			return false;
		}
		delete iterator->second;
		_string_map.erase(iterator);
		return true;
	}
	std::cout << "Error! invalid type " << std::endl;
	return false;
}

bool index_manager::find_index(int&index, std::string file_path, Data data)
{
	if (data.type == int_type)
	{
		auto iterator = _int_map.find(file_path);
		if (iterator == _int_map.end())
		{
			std::cout << "Error!" << std::endl;
			return false;
		}
		index = iterator->second->search_key(data.datai);
		return true;
	}
	if (data.type == float_type)
	{
		auto iterator = _float_map.find(file_path);
		if (iterator == _float_map.end())
		{
			std::cout << "Error!" << std::endl;
			return false;
		}
		index = iterator->second->search_key(data.dataf);
		return true;
	}
	if (data.type >= 1 && data.type <= 255)
	{
		auto iterator = _string_map.find(file_path);
		if (iterator == _string_map.end())
		{
			std::cout << "Error!" << std::endl;
			return false;
		}
		iterator->second->search_key(data.datas);
		return true;
	}
	std::cout << "Error! invalid type " << std::endl;
	return false;
}

void index_manager::insert_index(std::string file_path, Data data, int block_id)
{
	if (data.type == int_type)
	{
		auto iterator = _int_map.find(file_path);
		if (iterator == _int_map.end())
		{
			std::cout << "Error!" << std::endl;
			return;
		}
		iterator->second->insert_key(data.datai, block_id);
		return;
	}
	if (data.type == float_type)
	{
		auto iterator = _float_map.find(file_path);
		if (iterator == _float_map.end())
		{
			std::cout << "Error!" << std::endl;
		}
		iterator->second->insert_key(data.dataf, block_id);
		return;
	}
	if (data.type >= 1 && data.type <= 255)
	{
		auto iterator = _string_map.find(file_path);
		if (iterator == _string_map.end())
		{
			std::cout << "Error!" << std::endl;
		}
		iterator->second->insert_key(data.datas, block_id);
		return;
	}
	std::cout << "Error! invalid type " << std::endl;

}

void index_manager::delete_index(std::string file_path, Data data)
{

	if (data.type == int_type)
	{
		auto iterator = _int_map.find(file_path);
		if (iterator == _int_map.end())
		{
			std::cout << "Error!" << std::endl;
			return;
		}
		iterator->second->remove_key(data.datai);
		return;
	}
	if (data.type == float_type)
	{
		auto iterator = _float_map.find(file_path);
		if (iterator == _float_map.end())
		{
			std::cout << "Error!" << std::endl;
		}
		iterator->second->remove_key(data.dataf);
		return;
	}
	if (data.type >= 1 && data.type <= 255)
	{
		auto iterator = _string_map.find(file_path);
		if (iterator == _string_map.end())
		{
			std::cout << "Error!" << std::endl;
		}
		iterator->second->remove_key(data.datas);
	}
	std::cout << "Error! invalid type " << std::endl;
}
void index_manager::search_range(std::string file_path, Data data1, Data data2, std::vector<int>& values_out)
{
	int flag = 0;
	if (data1.type == -2)
	{
		flag = 1;
	}
	else if (data1.type == -1)
	{
		flag = 2;
	}

	if (data1.type != data2.type)
	{
		std::cout << "Error in search_range index_manager.cpp! data type not same" << std::endl;
		exit(1);
	}

	if (data1.type == int_type)
	{
		auto iterator = _int_map.find(file_path);
		if (iterator == _int_map.end())
		{
			std::cout << "Error!" << std::endl;
			return;
		}
		iterator->second->search_range(data1.datai, data2.datai, values_out, flag);
		return;
	}
	if (data1.type == float_type)
	{
		auto iterator = _float_map.find(file_path);
		if (iterator == _float_map.end())
		{
			std::cout << "Error!" << std::endl;
		}
		iterator->second->search_range(data1.dataf, data2.dataf, values_out, flag);
		return;
	}
	if (data1.type >= 1 && data1.type <= 255)
	{
		auto iterator = _string_map.find(file_path);
		if (iterator == _string_map.end())
		{
			std::cout << "Error!" << std::endl;
		}
		iterator->second->search_range(data1.datas, data2.datas, values_out, flag);
	}
	std::cout << "Error! invalid type " << std::endl;
}


int index_manager::calc_degree(int type)
{
	int degree = (Pagesize - sizeof(int)) / (calc_key_size((type)+sizeof(int)));
	return degree;


}


int index_manager::calc_key_size(int type)
{
	if (type == int_type)
		return sizeof(int);
	if (type == float_type)
		return sizeof(float);
	if (type >= 1 && type <= 255)
		return type;
	return -1;
}





