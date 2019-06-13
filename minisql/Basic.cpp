#include "Basic.h"

//Tuple类的实现
Tuple::Tuple(const Tuple &tuple_in){
	attr_ = tuple_in.attr_;
	data_ = tuple_in.data_;
	isDeleted_ = tuple_in.isDeleted_;
}

//get datas in tuple
vector<Data>&Tuple::getData() {
	return data_;
};
//add one data into a tuple
void Tuple::addData(Data data) {
	data_.push_back(data);
};
//indicate that the tuple has been deleted(lazy delete)
void Tuple::setDeleteState() {
	isDeleted_ = true;
};
bool Tuple::getDeleteState() {
	return isDeleted_;
};
Data Tuple::getDataFromAttr(string attrName) {
	Data data;
	int id=-1;
	for (int i = 0; i < attr_.num; i++) {
		if (attr_.name[i].compare(attrName) == 0) {
			id = i;
			break;
		}
	}
	if (id == -1) {
		return data;
	}
	else {
		return data_[id];
	}
};

//Table类的实现
Table::Table(string title, Attribute attr) {
	title_ = title;
	attr_ = attr;
};
Table::Table(const Table &table_in) {
	title_ = table_in.title_;
	tuple_ = table_in.tuple_;
	index_ = table_in.index_;
	attr_ = table_in.attr_;
};
void Table::addTuple(Tuple tuple) {
	tuple_.push_back(tuple);
};

string Table::getTitle() {
	return title_;
}
vector<Tuple> Table::getTuples() {
	return tuple_;
}
Index Table::getIndex() {
	return index_;
}