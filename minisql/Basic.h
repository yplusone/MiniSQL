#ifndef _BASIC_H_
#define _BASIC_H_ 
#include <iostream>
#include <vector>
#include"BufferManager.h"
using namespace std;

struct Attribute {
	int num;  //store the number of attributes for each table
	string name[32];  //store names
	short type[32];  //store types -1£ºint, 0:float, 1~255:string.length()+1
	bool unique[32] = { 0 };  //if unique (eg PK =1)
	short primary_key;  //the location of PK, not exist = -1
	bool has_index[32]={ 0 }; //if index exist
};

struct Index {
	int num;  //the number of index
	short location[10];  //the location of attribute of the index in Attr.num[]
	string indexname[10];  //index's name
};


//name a struct to store data, because every data may have different types
struct Data {
	int type;
	int datai;
	float dataf;
	string datas;
};

class Tuple {
private:
	vector<Data> data_;  //store data
	bool isDeleted_;
public:
	//construction function
	Attribute attr_;
	Tuple():isDeleted_(false){};
	Tuple(const Tuple &tuple_in);  
	//get datas in tuple
	vector<Data>& getData();
	//add one data into a tuple
	void addData(Data data);
	//indicate that the tuple has been deleted(lazy delete)
	void setDeleteState();
	bool getDeleteState();
	Data getDataFromAttr(string attrName);
};

//name a struct to store data, because every data may have different types


//Condition存放一组判断语句
//其中relation,小于，小于等于，等于，大于等于，大于，不等于分别为0,1,2,3,4,5
struct Condition{
	string Attribute_name;
    Data data; //数据
    int relation;
};

class Table {
private:
	string title_;  //table name 
	vector<Tuple> tuple_;  //store tuples
	Index index_;  //store index
public:
	Attribute attr_;  //store attribute
	//construction 	
	Table(string title, Attribute attr);
	Table(const Table &table_in);
	void addTuple(Tuple tuple);
	string getTitle();
	vector<Tuple> getTuples();
	Index getIndex();
};

//extern   
#endif
