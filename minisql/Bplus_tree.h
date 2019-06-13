#pragma once
#ifndef MY_B_PLUS_TREE_H
#define MY_B_PLUS_TREE_H

#include  "Tree_node.h"
#include"Const.h"
#include  "BufferManager.h"
#include"Basic.h"
#include <ios>
#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include<sstream>


extern BufferManager buffer_manager;
template<typename T>
void copy_string(unsigned char* p, int& offset, T data) {
	std::stringstream stream;
	stream << data;
	std::string s = stream.str();

	for (auto i = 0; i < s.length(); i++, offset++) {
		p[offset] = s[i];
	}
}

template <class T>
class b_plus_tree {
private:
	typedef tree_node<T>* ptr_to_node;

	ptr_to_node root_;
	ptr_to_node left_head_;
	int degree_;
	int key_num_;
	int node_num_;
	int height_;

	std::string file_name_;

	struct info
	{
		ptr_to_node ptr;
		int index;
		bool if_found;

		info();
	};

public:
	/**
	* \brief
	* \param p_file_name ÎÄ¼þÃû
	* \param degree B+Ê÷µÄ¶È
	*/
	b_plus_tree(const std::string& p_file_name, const int degree);

	/**
	* \brief ³õÊ¼»¯Ê÷µÄ¸÷ÖÖÐÅÏ¢
	*/
	void init_tree();

	/**
	* \brief B+Ê÷µÄÎö¹¹º¯Êý£¬ÊÍ·Å¿Õ¼ä
	*/
	~b_plus_tree();


	/**
	* \brief É¾³ýÒÔptrÎª¸ùµÄÊ÷
	* \param ptr ×ÓÊ÷µÄ¸ù
	*/
	void drop_tree(const ptr_to_node ptr);


	/**
	* \brief ÕÒµ½keyÔÚb+Ê÷µÄÎ»ÖÃ£¬²¢·µ»Ø²éÕÒÐÅÏ¢
	* \param ptr µ±Ç°ËÑË÷µÄ½ÚµãµÄÖ¸Õë
	* \param key Òª²éÕÒµÄkey
	* \param info ·µ»ØÖµ ·µ»ØÐÅÏ¢
	*/
	void find_leaf(ptr_to_node ptr, const T& key, info& info);

	/**
	* \brief ²éÕÒkeyÔÚÊ÷ÖÐµÄÎ»ÖÃ
	* \param key Òª²éÕÒµÄkey
	* \return ·µ»ØkeyÔÚÊ÷ÖÐµÄÎ»ÖÃ
	*/
	int search_key(const T key);


	/**
	* \brief ½«keyºÍvalue²åÈëÊ÷ÖÐ
	* \param key Òª²åÈëµÄkey
	* \param value Òª²åÈëµÄkey¶ÑÓ¦µÄvalue
	* \return ÊÇ·ñ²åÈë³É¹¦
	*/
	bool insert_key(const T& key, const int& value);


	/**
	* \brief ²åÈëºó¶Ôb+Ê÷½øÐÐµ÷Õû
	* \param ptr Òªµ÷ÕûµÄ×ÓÊ÷µÄ¸ù
	* \return ·µ»ØÊÇ·ñµ÷Õû³É¹¦
	*/
	bool insert_adjust(ptr_to_node ptr);


	/**
	* \brief °Ñkey´ÓÊ÷É¾³ý
	* \param key ÒªÉ¾³ýµÄkey
	* \return ·µ»ØÊÇ·ñÉ¾³ý³É¹¦
	*/
	bool remove_key(const T& key);


	/**
	* \brief ·µ»ØptrµÄ×ÓÊ÷Ö¸ÏòµÄµÚÒ»¸öÔªËØ
	* \param ptr ×ÓÊ÷¸ù
	* \return ·µ»ØTÀàÐÍµÄdata
	*/
	T first_key(ptr_to_node ptr);

	/**
	* \brief É¾³ýºó½øÐÐµ÷Õû
	* \param ptr Òªµ÷ÕûµÄ×ÓÊ÷µÄ¸ù
	* \return ÊÇ·ñµ÷Õû³É¹¦
	*/
	bool remove_adjust(ptr_to_node ptr);

	/**
	* \brief ·µ»Ø[key_left, key_right]·¶Î§ÄÚµÄkeyÖµ
	* \param key_left ×ó¶ËkeyÖµ
	* \param key_right ÓÒ¶ËkeyÖµ
	* \param values_out ·µ»ØkeyµÄvector
	* \param flag ²éÕÒkeyµÄÀàÐÍ
	*/
	void search_range(const T& key_left, const T& key_right, vector<int>& values_out, const int flag);

	/**
	* \brief debugº¯Êý£¬Êä³öB+Ê÷µÄÐÅÏ¢
	*/
	void debug_b_plus_tree() const;


	void get_file(std::string file_name);
	int get_block_num(std::string table_name);

	void read_all_disk();
	void read_disk(char *p, char* end);
	void write_all_disk();

	/**
	* \brief ´òÓ¡Ò¶×ÓÔªËØ
	*/
	void print_leaf();
};

template <class T>
b_plus_tree<T>::info::info()
{
	ptr = nullptr;
	index = 0;
	if_found = false;
}

template <class T>
b_plus_tree<T>::b_plus_tree(const std::string& p_file_name, const int degree)
{
	file_name_ = p_file_name;
	root_ = nullptr;
	left_head_ = nullptr;
	this->degree_ = degree;
	key_num_ = 0;
	node_num_ = 0;
	height_ = 0;
}

template <class T>
void b_plus_tree<T>::init_tree()
{
	root_ = new tree_node<T>(degree_, true);
	left_head_ = root_;
	key_num_ = 0;
	node_num_ = 0;
	height_ = 0;
}

template <class T>
b_plus_tree<T>::~b_plus_tree()
{
	root_ = nullptr;
	left_head_ = nullptr;
}

template <class T>
void b_plus_tree<T>::drop_tree(const ptr_to_node ptr)
{
	if (ptr == nullptr)
		return;
	if (!ptr->is_leaf_)
		for (auto i = 0; i <= ptr->n_; i++)
		{
			drop_tree(ptr->child_[i]);
			ptr->child_[i] = nullptr;
		}
	delete ptr;
}

template <class T>
void b_plus_tree<T>::find_leaf(ptr_to_node ptr, const T& key, info& info)
{
	auto index = 0;
	if (ptr->find_key((key), index))
	{
		if (ptr->is_leaf_)
		{
			info.index = index;
			info.ptr = ptr;
			info.if_found = true;
		}
		else
		{
			ptr = ptr->child_[index + 1];
			while (!ptr->is_leaf_)
			{
				ptr = ptr->child_[0];
			}
			info.ptr = ptr;
			info.index = 0;
			info.if_found = true;
		}
	}
	else
	{
		if (ptr->is_leaf_)
		{
			info.if_found = false;
			info.ptr = ptr;
			info.index = index;
		}
		else
		{
			//lower_boundµ±²»´æÔÚÔªËØÊ±£¬»á·µ»ØµÚÒ»¸ö´óÓÚ¸ÃÔªËØµÄÎ»ÖÃ¡£È»ºó¾ÍÓ¦¸ÃÑ¡ÔñÇ°Ò»¸öÎ»ÖÃÀ´ËÑË÷ 
			find_leaf(ptr->child_[index], key, info);
		}
	}
}

/**
* \brief
* \param key
* \return
*/
template <class T>
int b_plus_tree<T>::search_key(const T key)
{
	if (root_ == nullptr)
	{
		return -1;
	}
	info info;
	find_leaf(root_, key, info);

	if (info.if_found)
		return info.index;
	return -1;
}

template <class T>
bool b_plus_tree<T>::insert_key(const T& key, const int& value)
{
	info info;
	if (!root_)
	{
		init_tree();
	}
	find_leaf(root_, key, info);
	if (info.if_found)
	{
		std::cout << "Error: insert a key which already existed in the tree" << std::endl;
		return false;
	}
	int temp_index;
	info.ptr->add_key_leaf(key, value, temp_index);
	if (info.ptr->n_ == degree_)
	{
		insert_adjust(info.ptr);
	}
	key_num_++;
	return true;
}

template <class T>
bool b_plus_tree<T>::insert_adjust(ptr_to_node ptr)
{
	// ¼ì²âÊÇ·ñÖ´ÐÐÁËinsert_adjust º¯Êý
	// std::cout << "insert_adjust" << std::endl;
	T key;

	auto ptr_to_new_node = ptr->split_node(key);
	node_num_++;

	if (ptr->is_root())
	{
		auto root = new tree_node<T>(degree_, false);
		if (root == nullptr)
		{
			std::cout << "Error! no space for root" << std::endl;
			exit(1);
		}
		int temp_index;
		height_++;
		node_num_++;
		this->root_ = root;
		ptr->father_ = root;
		ptr_to_new_node->father_ = root;
		root->father_ = nullptr;
		root->add_key(key, temp_index);
		root->child_[0] = ptr;
		root->child_[1] = ptr_to_new_node;

		return true;
	}
	auto father = ptr->father_;
	int index;

	father->add_key(key, index);
	father->child_[index + 1] = ptr_to_new_node;
	ptr_to_new_node->father_ = father;
	if (father->n_ == degree_)
		return insert_adjust(father);
	return true;
}

template <class T>
bool b_plus_tree<T>::remove_key(const T& key)
{
	info info;
	find_leaf(root_, key, info);
	if (info.if_found)
	{
		// Èç¹ûptrÊÇ¸ù½Úµã
		if (info.ptr->is_root())
		{
			info.ptr->remove_index(info.index);
			key_num_--;
			return remove_adjust(info.ptr);
		}
		// Èç¹ûkeyÖµÒ²ÔÚ·ÇÒ¶×Ó½ÚµãÖÐ´æÔÚ
		if (info.index == 0 && left_head_ != info.ptr)
		{
			int index_temp;
			auto father = info.ptr->father_;
			auto found = father->find_key(key, index_temp);

			while (true)
			{
				if (found) father->keys_[index_temp] = info.ptr->keys_[1];
				if (father->father_ != nullptr)
					father = father->father_;
				else
					break;
				found = father->find_key(key, index_temp);
			}
			info.ptr->remove_index(info.index);
			key_num_--;
			/* test */
			// std::cout << "test" << std::endl;
			// debug_b_plus_tree();

			return remove_adjust(info.ptr);
		}
		info.ptr->remove_index(info.index);
		key_num_--;
		return remove_adjust(info.ptr);
	}
	std::cout << "Error in remove_key! this key does not exist in the tree" << std::endl;
	return false;
}

template <class T>
T b_plus_tree<T>::first_key(ptr_to_node ptr)
{
	if (ptr == nullptr)
	{
		std::cout << "Error in first key!" << std::endl;
	}

	while (!ptr->is_leaf_)
	{
		ptr = ptr->child_[0];
	}
	T first_key = ptr->keys_[0];
	return first_key;
}

template <class T>
bool b_plus_tree<T>::remove_adjust(ptr_to_node ptr)
{
	/* test */
	// std::cout << "remove adjust" << std::endl;
	// ptr->debug();
	// std::cout << std::endl;
	//std::cout << "remove adjust father" << std::endl;
	//if (ptr->is_root())
	//	std::cout << "Root" << std::endl;
	//else
	//	ptr->debug();
	// std::cout << std::endl;


	// ¸ù½Úµã
	if (ptr->is_root())
	{
		// Èç¹ûÓÐÒ»¸ökeyÖµ£¬ËµÃ÷ÓÐ2¸öÖ¸Õë£¬²»ÐèÒª½øÐÐµ÷Õû
		if (ptr->n_ >= 1)
			return true;
		// ¸ùÊÇÒ¶×Ó£¬É¾³ýºó£¬±ä³É¿ÕÊ÷
		if (ptr->is_leaf_)
		{
			delete ptr;
			root_ = nullptr;
			left_head_ = nullptr;
			height_--;
			node_num_--;
		}
		// ·ñÔò£¬¸ù½ÚµãÖ»ÓÐÒ»¸öÖ¸ÏòµÚÒ»¸ö¶ù×Ó½ÚµãµÄÖ¸Õë
		else
		{
			root_ = ptr->child_[0];
			root_->father_ = nullptr;
			delete ptr;
			height_--;
			node_num_--;
		}
		return true;
	}

	//Ò¶×Ó½Úµã
	if (ptr->is_leaf_)
	{
		// Èç¹ûkeyÖµµÄÊýÁ¿ >= ceil((degree-1)/2) ²»ÐèÒª½øÐÐµ÷Õû
		if (ptr->n_ >= degree_ / 2)
			return true;

		auto father = ptr->father_;

		ptr_to_node sibling;
		int index;
		father->find_key(ptr->keys_[0], index);

		/* ºÍ×ó×ÓÊ÷½øÐÐ²Ù×÷ */
		if (father->child_[0] != ptr)
		{
			sibling = father->child_[index]; //ptrµÄ×óÃæµÄ½Úµã
											 /* Èç¹û¿ÉÒÔ½øÐÐºÏ²¢ */
			if (sibling->n_ + ptr->n_ <= degree_ - 1)
			{
				const auto sibling_n = sibling->n_;

				for (auto i = 0; i < ptr->n_; i++)
				{
					sibling->keys_[i + sibling_n] = ptr->keys_[i];
					sibling->values_[i + sibling_n] = ptr->values_[i];
				}

				sibling->n_ += ptr->n_;
				sibling->next_leaf_ = ptr->next_leaf();

				father->remove_index(index); // ½«ptrµÄkeyÖµ´Ó¸¸Ç×µÄkeysÀïÉ¾³ý
				node_num_--;
				delete ptr;

				return remove_adjust(father);
			}
			/* ²»¿ÉÒÔ½øÐÐºÏ²¢£¬½«×ó½Úµã×î¿¿ÓÒµÄÖ¸Õë·Ö¸øptr*/
			for (auto i = ptr->n_ - 1; i >= 0; --i)
			{
				ptr->keys_[i + 1] = ptr->keys_[i];
				ptr->values_[i + 1] = ptr->values_[i];
			}
			const auto sibling_n = sibling->n_;
			ptr->keys_[0] = sibling->keys_[sibling_n - 1];
			ptr->values_[0] = sibling->values_[sibling_n - 1];
			sibling->remove_index(sibling->n_ - 1);

			++ptr->n_;
			father->keys_[index] = ptr->keys_[0];
			return true;
		}

		// ÓëÓÒÃæµÄ½Úµã½øÐÐºÏ²¢ 
		// ÊÇ×î×óÃæµÄ½Úµã
		if (father->child_[0] == ptr)
			sibling = father->child_[1];
		else
			sibling = father->child_[index + 2];
		// ¿ÉÒÔ½øÐÐºÏ²¢£¬°ÑsiblingºÏ²¢Èëptr
		if (sibling->n_ + ptr->n_ <= degree_ - 1)
		{
			const auto ptr_n = ptr->n_;
			for (auto i = 0; i < sibling->n_; i++)
			{
				ptr->keys_[ptr_n + i] = sibling->keys_[i];
				ptr->values_[ptr_n + i] = sibling->values_[i];
			}

			if (father->child_[0] == ptr)
				father->remove_index(0);
			else
				father->remove_index(index + 1);

			ptr->n_ += sibling->n_;
			ptr->next_leaf_ = sibling->next_leaf_;
			delete sibling;
			node_num_--;

			//std::cout << "test" << std::endl;
			//ptr->debug();
			//father->debug();
			//std::cout << std::endl;

			return remove_adjust(father);
		}
		// ²»¿ÉÒÔÖ±½Ó½øÐÐºÏ²¢£¬½«sibling×î×óÃæµÄ½Úµã¸øptr
		ptr->keys_[ptr->n_] = sibling->keys_[0];
		ptr->values_[ptr->n_] = sibling->values_[0];
		++ptr->n_;

		sibling->remove_index(0);
		if (father->child_[0] == ptr)
			father->keys_[0] = sibling->keys_[0];
		else
			father->keys_[index + 1] = sibling->keys_[0];
		return true;
	}

	// ·ÇÒ¶×Ó ·Ç¸ù½Úµã
	// Èç¹ûÖ¸ÕëÊý >= ceil(degree/2) ²»ÐèÒª½øÐÐµ÷Õû
	if (ptr->n_ + 1 >= (degree_ + 1) / 2)
		return true;
	int index;
	auto father = ptr->father_;

	father->find_key(first_key(ptr), index);
	//std::cout << "test first key" << std::endl;
	//std::cout << first_key(ptr) << std::endl;
	//std::cout << index << std::endl;
	// Óë×ó½Úµã½øÐÐ²Ù×÷

	if (father->child_[0] != ptr)
	{
		auto sibling = father->child_[index];
		// ¿ÉÒÔ½øÐÐºÏ²¢
		if (sibling->n_ + 1 + ptr->n_ + 1 <= degree_)
		{
			sibling->keys_[sibling->n_] = father->keys_[index];
			father->remove_index(index);
			++sibling->n_;
			const auto sibling_n = sibling->n_;

			for (auto i = 0; i < ptr->n_; i++)
			{
				sibling->keys_[i + sibling_n] = ptr->keys_[i];
				sibling->child_[i + sibling_n] = ptr->child_[i];
				sibling->child_[i + sibling_n]->father_ = sibling;
			}
			sibling->child_[ptr->n_ + sibling_n] = ptr->child_[ptr->n_];
			sibling->child_[ptr->n_ + sibling_n]->father_ = sibling;

			sibling->n_ += ptr->n_;

			delete ptr;
			node_num_--;
			return remove_adjust(father);
		}
		// ²»¿ÉÒÔ½øÐÐºÏ²¢£¬°Ñ×ó½ÚµãµÄ×îºóÒ»¸öÖ¸Õë¸øptr
		ptr->child_[ptr->n_ + 1] = ptr->child_[ptr->n_];
		for (auto i = ptr->n_ - 1; i >= 0; --i)
		{
			ptr->child_[i + 1] = ptr->child_[i];
			ptr->keys_[i + 1] = ptr->keys_[i];
		}
		ptr->child_[0] = sibling->child_[sibling->n_];
		ptr->child_[0]->father_ = ptr;
		ptr->keys_[0] = father->keys_[index];

		++ptr->n_;

		father->keys_[index] = sibling->keys_[sibling->n_ - 1];
		sibling->remove_index(sibling->n_ - 1);

		return true;
	}
	// ÓëÓÒ½Úµã½øÐÐ²Ù×÷
	// std::cout << "I need" << std::endl;
	// ptr->debug();
	// std::cout << std::endl;
	ptr_to_node sibling;
	if (father->child_[0] == ptr)
	{
		sibling = father->child_[1];
	}
	else
	{
		sibling = father->child_[index + 2];
		std::cout << "Error in remove_adjust! ptr is not the first child of father";
		exit(1);
	}
	// ¿ÉÒÔ½øÐÐºÏ²¢
	if (sibling->n_ + 1 + ptr->n_ <= degree_)
	{
		ptr->keys_[ptr->n_] = father->keys_[index];
		father->remove_index(0);

		++ptr->n_;
		for (auto i = 0; i < sibling->n_; i++)
		{
			ptr->keys_[ptr->n_ + i] = sibling->keys_[i];
			ptr->child_[ptr->n_ + i] = sibling->child_[i];
			ptr->child_[ptr->n_ + i]->father_ = ptr;
		}
		ptr->child_[ptr->n_ + sibling->n_] = sibling->child_[sibling->n_];
		ptr->child_[ptr->n_ + sibling->n_]->father_ = ptr;

		ptr->n_ += sibling->n_;
		delete sibling;
		node_num_--;
		return remove_adjust(father);
	}
	// ²»¿ÉÒÔ½øÐÐºÏ²¢£¬
	ptr->keys_[ptr->n_] = father->keys_[index + 1];
	ptr->child_[ptr->n_ + 1] = sibling->child_[0];
	ptr->child_[ptr->n_ + 1]->father_ = ptr;
	++ptr->n_;
	sibling->remove_index(0);

	father->keys_[0] = first_key(sibling);

	return true;
}

template <class T>
void b_plus_tree<T>::search_range(const T& key_left, const T& key_right, vector<int>& values_out, const int flag)
{
	if (!root_)
		return;

	if (flag == 2)
	{
		info info;
		find_leaf(root_, key_left, info);

		bool finished = false;
		ptr_to_node ptr = info.ptr;
		int index = info.index;

		do
		{
			finished = ptr->find_range_2(index, values_out);
			index = 0;
			if (ptr->next_leaf_ == nullptr)
				break;
			else
				ptr = ptr->next_leaf();
		} while (!finished);
	}
	else if (flag == 1)
	{
		info info;
		find_leaf(root_, key_right, info);

		bool finished = false;
		ptr_to_node ptr = info.ptr;
		int index = info.index;

		do
		{
			finished = ptr->find_range_2(index, values_out);
			index = 0;
			if (ptr->next_leaf_ == nullptr)
				break;
			else
				ptr = ptr->next_leaf();
		} while (!finished);
	}
	else
	{
		info info_left, info_right;
		find_leaf(root_, key_left, info_left);
		find_leaf(root_, key_right, info_right);

		bool finished = false;
		int index;

		if (key_left <= key_right)
		{
			ptr_to_node ptr = info_left.ptr;
			index = info_left.index;
			do
			{
				finished = ptr->find_range(index, key_right, values_out);
				index = 0;
				if (ptr->next_leaf_ == nullptr)
					break;
				else
					ptr = ptr->next_leaf_;
			} while (!finished);
		}
	}

	std::sort(values_out.begin(), values_out.end());
	values_out.erase(unique(values_out.begin(), values_out.end()), values_out.end());
}

template <class T>
void b_plus_tree<T>::debug_b_plus_tree() const
{
	const auto height = height_;
	std::vector<ptr_to_node> temp[100 + 1];
	for (auto i = 0; i < height_; i++)
		temp->clear();
	temp[0].push_back(root_);

	/* ¼ì²ârootµÄis_leafÊÇ·ñÕýÈ· */
	//std::cout << "root is_leaf?: " << root_->is_leaf_ << std::endl;
	//std::cout << "root is root?: " << root_->is_root() << std::endl;

	for (auto i = 0; i <= height_; i++)
	{
		for (auto ptr : temp[i])
		{
			ptr->debug();
			if (!ptr->is_leaf_)
			{
				for (auto j = 0; j <= ptr->n_; j++)
					temp[i + 1].push_back(ptr->child_[j]);
			}
		}
		std::cout << std::endl;
	}
}

template <class T>
void b_plus_tree<T>::get_file(std::string file_name)
{
	auto f = fopen(file_name.c_str(), "r");
	if (f == NULL)
	{
		f = fopen(file_name.c_str(), "w+");
		fclose(f);
		f = fopen(file_name.c_str(), "r");
	}
	fclose(f);
}

template <class T>
int b_plus_tree<T>::get_block_num(std::string table_name)
{
	// char* p;
	auto block_num = 0;
	while (true)
	{
//		auto buffer_iter = buffer_manager.BufferRead(file_name_, block_num * PAGE_SIZE);

		if ((*buffer_iter).data[0] == '\0')
		{
			break;
		}
		block_num++;
	}

	return block_num;
}

template <class T>
void b_plus_tree<T>::read_all_disk()
{
	auto file_name = "./database/index/" + file_name_;
	get_file(file_name);
	auto block_num = get_block_num(file_name);

	if (block_num <= 0)
		block_num = 1;

	for (auto i = 0; i < block_num; i++) {
		char* p = buffer_manager.getPage(file_name, i);

		read_disk(p, p + Pagesize);
	}
}

template <class T>
void b_plus_tree<T>::read_disk(char *p, char* end)
{
	T key;
	int value;

	for (auto i = 0; i < Pagesize; i++)
	{
		if (p[i] != '#')
			return;
		else
		{
			i += 2;
			char tmp[100];
			int j;
			for (j = 0; j < Pagesize && p[i] != ' '; i++)
			{
				tmp[j++] = p[i];
			}
			tmp[j] = '\0';
			std::string s_tmp(tmp);
			std::stringstream stream(s_tmp);
			stream >> key;

			memset(tmp, 0, sizeof(tmp));

			i++;
			for (j = 0; j < Pagesize && p[i] != ' '; i++)
			{
				tmp[j++] = p[i];
			}
			tmp[j] = '\0';
			std::string s_tmp1(tmp);
			std::stringstream stream1(s_tmp1);
			stream1 >> value;
			insert_key(key, value);
		}
	}
}

template <class T>
void b_plus_tree<T>::write_all_disk()
{
	auto file_name = "./database/index/" + file_name_;
	get_file(file_name);
	auto block_num = get_block_num(file_name);

	ptr_to_node tmp = left_head_;

	int i, j;

	for (i = 0, j = 0; tmp != nullptr; j++)
	{
		char* p = buffer_manager.getPage(file_name, j);
		auto offset = 0;

		memset(p, 0, PAGE_SIZE);

		for (i = 0; i < tmp->n_; i++)
		{
			p[offset++] = '#';
			p[offset++] = ' ';
			copy_string(p, offset, tmp->keys_[i]);
			p[offset++] = ' ';
			copy_string(p, offset, tmp->values_[i]);
			p[offset++] = ' ';
		}

		p[offset] = '\0';

		int page_id = buffer_manager.getPageId(file_name, j);
		buffer_manager.modifyPage(page_id);

		tmp = tmp->next_leaf_;
	}

	while (j < block_num)
	{
		char* p = buffer_manager.getPage(file_name, j);
		memset(p, 0, PAGE_SIZE);
		int page_id = buffer_manager.getPageId(file_name, j);
		buffer_manager.modifyPage(page_id);
		j++;
	}
}

template <class T>
void b_plus_tree<T>::print_leaf()
{
	auto ptr = left_head_;
	while (ptr != nullptr)
	{
		ptr->debug();
		ptr = ptr->next_leaf();
	}
}

#endif
