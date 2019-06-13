#include "API.h"
//#include "template_function.h"
#include <algorithm>
#include <vector>
#include <iterator>

using namespace std;
//构造函数
API::API() {}

//析构函数
API::~API() {}

void API::insertRecord(std::string table_name, Tuple& tuple)//
{
	printf("insertRecord %s ", table_name.c_str());
	//vector<Data>& v = tuple.getData();
	//
	//printf("%d,", v.size());

	/*for (int i = 0; i < data.size; i++)
	{
		switch (A[i].type) {
		case -1:
			cout << A[i].datai << std::endl;
			break;
		case 0:
			cout << A[i].dataf << std::endl;
		default:
			cout << A[i].datas << std::endl;
		}
	}*/
	//tuple.showTuple();
	RecordManager record;
	try {
		record.insertRecord(table_name, tuple);
	}
	catch (exception& ex) {
		ex.what();
	}
	
	return;
}

bool API::createTable(std::string table_name, Attribute attribute, int primary, Index index)
{

	RecordManager record_manager;
	record_manager.createTableFile(table_name);
	Catalog_manager catalog_manager;
	catalog_manager.Create_table(table_name, attribute, primary, index);




	return true;
}