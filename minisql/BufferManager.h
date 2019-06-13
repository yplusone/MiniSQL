#ifndef _BUFFER_MANAGER_H_
#define _BUFFER_MANAGER_H_ 

#include <cstdio>
#include<cstring>
#include <iostream>
#include <vector>
#include <string>
#include "const.h"
using namespace std;
// Page类。磁盘文件中的每一块对应内存中的一个页（page)
class Page {
    public:
        // 构造函数
        Page();
        void initialize();
        void setFileName(string file_name);
        string getFileName();
        void setBlockId(int block_id);
        int getBlockId();
        void setCount(int count);
        void addCount();
        int getCount();
        void setDirty(bool dirty);
        bool getDirty();
        void setpin(bool pin);
        bool getpin();
        char* getBuffer();
    private:
        char buffer[Pagesize];//每一页都是一个大小为PAGESIZE字节的数组
        std::string file_name;//页所对应的文件名
        int Count;//用于LRU替换策略
        int block_id;//页在所在文件中的块号(磁盘中通常叫块)
        bool dirty;//dirty记录页是否被修改
        bool pin;//pin标示页是否可以被pin,不可替换(即将磁盘块load进该页)
};

// BufferManager类。对外提供操作缓冲区的接口。
class BufferManager {
    public: 
        //  构造函数
        BufferManager();
        // 析构函数
        ~BufferManager();
        // 通过表名，block_id获取一条记录的信息，及cache的一个Page
        char* getPage(string file_name , int block_id);
        // 通过表名，block_id获取一个page页码
        int getPageId(string file_name , int block_id);
        // 标记page_id所对应的页已经被修改
        void modifyPage(int page_id);
		//获取总共的Page数
		int getframe_num();
		int getBlockNum(string file_name);
		//刷新磁盘，讲缓存中的修改在磁盘上进行更改
		void flushDisk();
    private:
    	//缓冲池，一次最多缓冲MaxFrame个page
        Page Frames[MaxFrame];
        //缓冲池中总共的page数
        int frame_num;
        // 获取一个闲置的页的页号(如果缓冲池已满，采用LRU替换策略)
        int getEmptyPageId();
        // 讲对应文件的对应块载入对应内存页，对于文件不存在返回false，否则返回true
        bool loadDiskBlock(int page_id , string file_name , int block_id);
        // 将对应内存页写入对应文件的对应块。写入成功则返回true，写入失败则返回false
        bool flushPage(int page_id , string file_name , int block_id);

};



#endif