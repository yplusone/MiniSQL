
#ifndef _CATALOG_MANAGER_H_
#define _CATALOG_MANAGER_H_ 
#include<iostream>
#include<cstring>
#include<cmath>
//#include<cstdin>
#include<cstdlib>
#include"BufferManager.h"
#include"Basic.h"
#include<iomanip>
#include"exception.h"
#include"Const.h"
using namespace std;

#ifndef TABLE_MANAGER_PATH
#define TABLE_MANAGER_PATH "./database/catalog/catalog_file"
#endif
//extern   
class Catalog_manager {
public:
	void Create_table(string name, Attribute attr, int pri, Index ind);
	//Insert a Table in catalog file
	//input: Table name, attributes, the ID of PK, and index

	void Drop_table(string name);
	//Delete a Table in catalog file
	//input: Table's name

	Attribute Get_attr(string name);
	//Get the attributes of a table
	//input: the table's name

	void Create_index(string name, string attr, string ind);
	//Create an index over an attribute of a table
	//input: table's name, the specific index

	string Index2Attr(string name, string ind);
	//Fing the attribute according to the index
	//input: table's name, index's name

	void Drop_index(string name, string ind);
	//Delete an index
	//input: table's name, index's name

	void Display_table(string name);
	//Display the information of a table
	//input: table's name

	bool Has_table(std::string name);
	bool Has_attribute(std::string name, std::string attr);



private:
	string Num2Str(int num, int bit);
	//Transform number to string with fixed bit

	int Str2Num(string str);
	//Transform string to number

	string Get_name(string buf, int st, int &ed);
	//Get the table's name of that line

	int Get_place(string name, int &blo);
	//Get the table's place in the file

	Index Get_index(string name);
	//Get the index of the table

	int Get_size(string name);
	//Get the size of the file
};
extern BufferManager buffer_manager;

#endif

