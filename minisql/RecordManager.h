#ifndef _RECORD_MANAGER_H_
#define _RECORD_MANAGER_H_

#include <cstdio> 
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "Basic.h"
//#include "index_manager.h"
#include "catalog_manager.h"
#include "BufferManager.h"
#include "const.h"

using namespace std;
extern BufferManager buffer_manager;
class RecordManager {
    public:
    	//创建数据文件
        void createTableFile(string table_name);   
        //删除数据文件
        void dropTableFile(string table_name);     
        //插入一条记录
        void insertRecord(string table_name , Tuple& tuple); 
        //删除一张表中的所有记录，但不删除表，返回删除的记录数
        int deleteRecord(string table_name);
		//将满足一定条件的记录删除，返回删除的记录数
        int deleteRecord(string table_name, vector<Condition> cons);
        //选择一张表的所有记录，返回这张表
        Table selectRecord(string table_name);
        //选择一张表中满足一定条件的所有记录，返回结果
        Table selectRecord(string table_name, vector<Condition> cons);
    	//创建索引
    private:
        //判断插入的记录是否和其他记录冲突 index要判断的属性下标
        bool isConflict(vector<Tuple>& tuples , vector<Data>& v , int index);
        //获得表格的block数量
        //int getBlockNum(string table_name);
        //把一条记录插入到buffer
        void insertRecordToBuffer(char* buffer,int offset,vector<Data> v);
        //判断一个元组是否满足条件
        bool satisfyCondition(Tuple tuple,vector<Condition> cons,Attribute attr);
        //将buffer中的一条记录删除
        void deleteOneBlock(char* p, vector<Tuple> tuples);
		//读取一个block的所有tuple
		vector<Tuple> readAllTuple(char* p, Attribute attr);
		//读取一个tuple
		Tuple readOneTuple(char* &p, Attribute attr);



};


template <typename T>
int getDataLength(T data) {
    std::stringstream stream;
    stream << data;
    return stream.str().length();
}


template <typename T>
void copyString(char* p , int& offset , T data) {
    std::stringstream stream;
    stream << data;
    std::string s1 = stream.str();
    for (int i = 0;i < s1.length();i++,offset++){
        p[offset] = s1[i];
    }
    p[offset++]=' ';
}

void getDataFromString(Data& data, string datastring, int type);

int getAttrIndex(string attrName, Attribute attr);

bool dataCompare(int type, Data data1, Data data2, int relation);

int rightAttr(string attribute_name, Attribute attr);
#endif