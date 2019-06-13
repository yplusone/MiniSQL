#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_ 
#include "API.h"
#include <exception>
using namespace std;

#include <exception>
class BPTreeInnerException : public std::exception {
public:
	explicit BPTreeInnerException(const char *ptr = "Inner erro in B+ tree")
		: ptr(ptr) {}

	char const *what() const noexcept override {
		std::cout << this->ptr << std::endl;
		return ptr;
	}

private:
	const char *ptr;
};

class table_exist : public std::exception {
public:
	explicit table_exist(const char *ptr = "Table is already exist!")
		: ptr(ptr) {}

	char const *what() const noexcept override {
		std::cout << this->ptr << std::endl;
		return ptr;
	}

private:
	const char *ptr;
};

class file_cant_open : public std::exception {
public:
	explicit file_cant_open(const char *ptr = "Open file failed!")
		: ptr(ptr) {}

	char const *what() const noexcept override {
		std::cout << this->ptr << std::endl;
		return ptr;
	}

private:
	const char *ptr;
};
class delete_file_error : public std::exception {
public:
	explicit delete_file_error(const char *ptr = "Open file failed!")
		: ptr(ptr) {}

	char const *what() const noexcept override {
		std::cout << this->ptr << std::endl;
		return ptr;
	}

private:
	const char *ptr;
};
class table_not_exist : public std::exception {
public:
	explicit table_not_exist(const char *ptr = "The table is not exist!")
		: ptr(ptr) {}

	char const *what() const noexcept override {
		std::cout << this->ptr << std::endl;
		return ptr;
	}

private:
	const char *ptr;
};

class attribute_not_exist : public std::exception {
    
};

class index_exist : public std::exception {
    
};

class index_not_exist : public std::exception {
    
};

class tuple_type_conflict : public std::exception {
    
};

class primary_key_conflict : public std::exception {
public:
	explicit primary_key_conflict(const char *ptr = "There is a primary key conflict in the inserted data!")
		: ptr(ptr) {}

	char const *what() const noexcept override {
		std::cout << this->ptr << std::endl;
		return ptr;
	}

private:
	const char *ptr;
};

class data_type_conflict : public std::exception {
    
};

//增加
class index_full : public std::exception{
    
};

class input_format_error : public std::exception{
    
};

class exit_command : public std::exception{
    
};

class unique_conflict:public std::exception{
public:
	explicit unique_conflict(const char *ptr = "There is a unique conflict in the inserted data!")
		: ptr(ptr) {}

	char const *what() const noexcept override {
		std::cout << this->ptr << std::endl;
		return ptr;
	}

private:
	const char *ptr;
};

#endif 
