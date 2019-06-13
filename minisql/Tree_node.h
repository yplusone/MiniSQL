
#ifndef TREE_NODE_H
#define TREE_NODE_H

#include <iostream>
#include <algorithm>
#include <vector>

template <typename T>
class tree_node
{
private:

	int n_; // key������
	T* keys_; // �洢key
	bool is_leaf_; //�Ƿ���Ҷ��
	tree_node** child_; //���ӽڵ�
	tree_node* father_; //���׽ڵ�

	int* values_; // �洢value
	tree_node* next_leaf_; // �¸�Ҷ�ӽڵ�
	int degree_;

	template<typename T>
	friend class b_plus_tree;

public:

	/**
	* \brief ���캯��
	* \param degree B+���Ķ�
	* \param is_leaf �ýڵ��Ƿ�ΪҶ��
	*/
	tree_node(const int degree, const bool is_leaf);

	/**
	* \brief ��������
	*/
	~tree_node();

	/**
	* \brief ��key�ڽڵ��е�λ��
	* \param key Ҫ���ҵ�key
	* \param index ����λ��index
	* \return ����bool���Ƿ����ɹ�
	*/
	bool find_key(const T key, int& index) const;

	/**
	* \brief ���Ҷ�ڵ����key
	* \param key Ҫ�����key
	* \param index ���ز����λ��
	* \return bool �����Ƿ����ɹ�
	*/
	bool add_key(const T key, int& index);

	/**
	* \brief ��Ҷ�ӽڵ����key
	* \param key Ҫ�����key
	* \param value Ҫ�����value
	* \param index ���ز����λ��
	* \return bool �Ƿ����ɹ�
	*/
	bool add_key_leaf(const T key, const int value, int& index);

	/**
	* \brief �����Ҷ��, ����ָ����һ��Ҷ�ӵ�ָ��
	* \return ��һ��Ҷ�ӵ�ָ��
	*/
	tree_node* next_leaf() const;

	/**
	* \brief ���ص�ǰ�ڵ��Ƿ�Ϊ��
	* \return bool �����Ƿ�Ϊ��
	*/
	bool is_root() const;

	/**
	* \brief ���ѽڵ�
	* \param up_key Ҫ�����׵�keyֵ
	* \return ���صڶ����ڵ��ָ��
	*/
	tree_node* split_node(T& up_key);

	/**
	* \brief ɾ��key
	* \param index ɾ����key��λ��
	* \return �����Ƿ�ɾ���ɹ�
	*/
	bool remove_index(const int& index);

	/**
	* \brief ��key����һ��Ҷ�ӽڵ�����
	* \param key Ҫ�����key
	* \param value Ҫ�����value
	*/
	void push_left_leaf(const int& key, const int& value);

	/**
	* \brief ��key����һ����Ҷ�ڵ�����
	* \param key Ҫ�����key
	* \param child Ҫ�����childָ��
	*/
	void push_left(const int& key, tree_node* child);

	/**
	* \brief ��key����һ��Ҷ�ӽڵ���Ҷ�
	* \param key Ҫ�����key
	* \param value Ҫ�����value
	*/
	void push_right_leaf(const int& key, const int& value);

	/**
	* \brief ��key����һ��Ҷ�ӽڵ���Ҷ�
	* \param key Ҫ�����key
	* \param child Ҫ�����value
	*/
	void push_right(const int& key, tree_node* child);

	/**
	* \brief ����ڵ���Ϣ������debug
	*/
	void debug() const;

	bool find_range(const int index, const T& key, std::vector<int>& values_out);
	bool find_range_2(int index, std::vector<int>& values_out) const;
};

template <typename T>
tree_node<T>::tree_node(const int degree, const bool is_leaf)
{
	//std::cout << "new tree_node" << std::endl;
	degree_ = degree;
	is_leaf_ = is_leaf;
	n_ = 0;
	keys_ = new T[degree + 2];
	values_ = new int[degree + 2];
	next_leaf_ = nullptr;
	child_ = new tree_node*[degree + 3];
	father_ = nullptr;
}

template <typename T>
tree_node<T>::~tree_node()
{
	delete[] keys_;
	delete[] values_;
	delete[] child_;
}

template <typename T>
bool tree_node<T>::find_key(const T key, int& index) const
{
	/* �ڵ���keyֵ��Ϊ0 */
	if (n_ == 0)
	{
		index = 0;
		return false;
	}
	/* ��һ��key����Ҫ���ҵ�key */
	if (keys_[0] > key)
	{
		index = 0;
		return false;
	}
	/* ���һ��keyС��Ҫ���ҵ�key */
	if (keys_[n_ - 1] < key)
	{
		index = n_;
		return false;
	}

	/* ���ֲ��� */
	index = std::lower_bound(keys_, keys_ + n_, key) - keys_;
	return keys_[index] == key;
}

template <typename T>
bool tree_node<T>::add_key(const T key, int& index)
{
	if (n_ == 0)
	{
		keys_[0] = key;
		index = 0;
		n_++;
		return true;
	}

	auto position = 0;
	const auto exist = find_key(key, position);
	if (exist)
	{
		index = -1;
		return false;
	}
	n_++;
	for (auto i = n_ - 1; i > position; i--)
	{
		keys_[i] = keys_[i - 1];
	}
	keys_[position] = key;
	index = position;

	for (auto i = n_; i > position + 1; i--)
	{
		child_[i] = child_[i - 1];
	}

	child_[position + 1] = nullptr;
	return true;
}

template <typename T>
bool tree_node<T>::add_key_leaf(const T key, const int value, int& index)
{
	// std::cout << "add_key_leaf" << std::endl;
	if (n_ == 0)
	{
		keys_[0] = key;
		values_[0] = value;
		n_++;
		return true;
	}
	auto position = 0;
	const auto exist = find_key(key, position);
	if (exist)
	{
		/* �����򱨴� */
		return false;
	}
	n_++;
	for (auto i = n_ - 1; i > position; i--)
	{
		keys_[i] = keys_[i - 1];
		values_[i] = values_[i - 1];
	}
	keys_[position] = key;
	values_[position] = position;
	index = position;
	return true;
}

template <typename T>
tree_node<T>* tree_node<T>::next_leaf() const
{
	return next_leaf_;
}

template <typename T>
bool tree_node<T>::is_root() const
{
	return father_ == nullptr;
}

template <typename T>
tree_node<T>* tree_node<T>::split_node(T& up_key)
{
	// std::cout << "split_node" << is_leaf_ << std::endl;
	auto right_node = new tree_node(degree_, is_leaf_);

	if (is_leaf_)
	{
		// std::cout << "is_leaf split_node" << std::endl;
		const auto left_num = (n_ + 1) / 2; // ��������ڵ��key������

		up_key = keys_[left_num];
		for (auto i = left_num; i < n_; i++)
		{
			right_node->keys_[i - left_num] = keys_[i];
			keys_[i] = T();
			right_node->values_[i - left_num] = values_[i];
			values_[i] = int();
		}
		right_node->next_leaf_ = next_leaf_;
		next_leaf_ = right_node;
		right_node->father_ = father_;

		right_node->n_ = n_ - left_num;
		n_ = left_num;
	}
	else
	{
		// std::cout << "��Ҷ�ڵ�split" << n_ << std::endl;
		const auto left_num = (n_ + 2) / 2; // ��������ڵ��ָ��childָ�������

		up_key = keys_[left_num - 1];

		for (auto i = left_num; i < n_ + 1; i++)
		{
			right_node->child_[i - left_num] = child_[i];
			right_node->child_[i - left_num]->father_ = right_node;
			child_[i] = nullptr;
		}

		for (auto i = left_num; i < n_; i++)
		{
			// std::cout << i << "  " << keys_[i] << std::endl;
			right_node->keys_[i - left_num] = keys_[i];
			keys_[i] = T();
		}
		// std::cout << std::endl;

		this->keys_[left_num - 1] = T();
		right_node->father_ = father_;

		right_node->n_ = n_ - left_num;
		this->n_ = left_num - 1;
	}

	return right_node;
}

template <typename T>
bool tree_node<T>::remove_index(const int& index)
{
	if (index > n_)
	{
		std::cout << "Error in remove_index! the index is out of bound" << std::endl;
		return false;
	}
	/* Ҷ�ӽڵ� */
	if (is_leaf_)
	{
		for (auto i = index; i < n_ - 1; i++)
		{
			keys_[i] = keys_[i + 1];
			values_[i] = values_[i + 1];
		}
		keys_[n_ - 1] = T();
		values_[n_ - 1] = int();
		n_--;
		return true;
	}
	/* ��Ҷ�ӽڵ� */
	for (auto i = index; i < n_ - 1; i++)
	{
		keys_[i] = keys_[i + 1];
	}
	keys_[n_ - 1] = T();

	for (auto i = index + 1; i < n_; i++)
	{
		child_[i] = child_[i + 1];
	}
	child_[n_] = nullptr;
	n_--;
	return true;
}

template <typename T>
void tree_node<T>::push_left_leaf(const int& key, const int& value)
{
	for (auto i = n_ - 1; i >= 0; i--)
	{
		keys_[i + 1] = keys_[i];
		values_[i + 1] = values_[i];
	}
	keys_[0] = key;
	values_[0] = value;
	n_++;
}

template <typename T>
void tree_node<T>::push_left(const int& key, tree_node* child)
{
	for (auto i = n_ - 1; i >= 0; i--)
	{
		keys_[i + 1] = keys_[i];
		child_[i + 1] = child_[i];
	}
	child_[n_ + 1] = child_[n_];
	keys_[0] = key;
	child_[0] = child;
	n_++;
}

template <typename T>
void tree_node<T>::push_right_leaf(const int& key, const int& value)
{
	keys_[n_] = key;
	values_[n_] = value;
	n_++;
}

template <typename T>
void tree_node<T>::push_right(const int& key, tree_node* child)
{
	keys_[n_] = key;
	child_[n_ + 1] = child;
	n_++;
}

template <typename T>
void tree_node<T>::debug() const
{
	if (this == nullptr)
		std::cout << "NULL";
	else
	{
		for (auto i = 0; i < n_; i++)
			std::cout << keys_[i] << " ";
	}
	std::cout << "          ";
	//tree_node* tmp = this->father_;
	//std::cout << "debug father";
	//if (tmp == nullptr)
	//	std::cout << "NULL";
	//else
	//{
	//	for (auto i = 0; i < tmp->n_; i++)
	//		std::cout << tmp->keys_[i] << " ";
	//}
	//std::cout << std::endl;
}

template <typename T>
bool tree_node<T>::find_range(const int index, const T& key, std::vector<int>& values_out)
{
	int i;
	for (i = index; i < n_ && keys_[i] <= key; i++)
	{
		values_out.push_back(values_[i]);
	}

	if (keys_[i] >= key)
		return true;
	return false;
}

template <typename T>
bool tree_node<T>::find_range_2(int index, std::vector<int>& values_out) const
{
	for (auto i = index; i < n_; i++)
		values_out.push_back(values_[i]);

	return false;
}
#endif
