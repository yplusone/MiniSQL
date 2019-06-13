#include "BufferManager.h"
#define _CRT_SECURE_NO_DEPRECATE；
#pragma warning(disable:4996)；



// Page类的实现
Page::Page() {
    initialize();
}
// 初始化
void Page::initialize() {
    file_name = "";
    block_id = -1;
    Count = -1;
    dirty = false;
    pin = false;
	memset(buffer, '\0', Pagesize);
}

void Page::setFileName(std::string file_name) {
    this->file_name = file_name;
}

std::string Page::getFileName() {
    return file_name;
}

void Page::setBlockId(int block_id) {
    this->block_id = block_id;
}

int Page::getBlockId() {
    return block_id;
}

void Page::setCount(int pin_count) {
    this->Count = pin_count;
}

void Page::addCount() {
    Count++;
}

int Page::getCount() {
    return Count;
}

void Page::setDirty(bool dirty) {
    this->dirty = dirty;
}

bool Page::getDirty() {
    return dirty;
}

void Page::setpin(bool pin) {
    this->pin = pin;
}

bool Page::getpin() {
    return pin;
}

char* Page::getBuffer() {
    return buffer;
}

int BufferManager::getframe_num() {
    return frame_num;
}

BufferManager::BufferManager(){
	frame_num = 0;
};
// 析构函数
BufferManager::~BufferManager(){
    //将缓冲池的内容（如果被修改了）都写入到文件中
    for(int i=0;i<frame_num;i++){
        //如果记录内容没有被修改，则跳过
        if(Frames[i].getDirty()==false){
            continue;
        }
        //否则将记录重新写回文件
        string file_name;
        int block_id;
        file_name=Frames[i].getFileName();
        block_id=Frames[i].getBlockId();
        flushPage(i,file_name,block_id);
    }
 
};
void BufferManager::flushDisk() {
	//将缓冲池的内容（如果被修改了）都写入到文件中
	for (int i = 0; i < frame_num; i++) {
		//如果记录内容没有被修改，则跳过
		if (Frames[i].getDirty() == false) {
			continue;
		}
		//否则将记录重新写回文件
		string file_name;
		int block_id;
		file_name = Frames[i].getFileName();
		block_id = Frames[i].getBlockId();
		flushPage(i, file_name, block_id);
	}
}
// 将对应内存页写入对应文件的对应块。写入成功则返回true，写入失败则返回false
bool BufferManager::flushPage(int page_id , string file_name , int block_id){
	
    FILE* f=fopen(file_name.c_str(),"w");
    if(f==NULL){
		cout << "flush page open failed";
        return false;
    }
    fseek(f,Pagesize*block_id,SEEK_SET);
    char *buffer=Frames[page_id].getBuffer();
    fwrite(buffer,Pagesize,1,f);
    fclose(f);
    return true;
};
// 通过表名，block_id获取一条记录的信息，及cache的一个Page
char* BufferManager::getPage(string file_name , int block_id){
    int flag=0;
    int page_id;

    for(int i=0;i<frame_num;i++){
        Frames[i].addCount();
        if(file_name==Frames[i].getFileName()&&block_id==Frames[i].getBlockId()){
            flag=1;
            page_id=i;
        }
    }
    if(flag==1){
        //访问过的Page的count值设为0（替换算法的需要）        Frames[page_id].setCount(0);
        return Frames[page_id].getBuffer();
    }
    page_id=getEmptyPageId();
    loadDiskBlock(page_id,file_name,block_id);
    return Frames[page_id].getBuffer();
};
// 获取一个闲置的页的页号(如果缓冲池已满，采用LRU替换策略)
int BufferManager::getEmptyPageId(){
    if(frame_num<MaxFrame){

        return frame_num;
    }
    int maxCount=0;
    int page_id;
    for(int i=0;i<frame_num;i++){
        int temp_count;
        temp_count=Frames[i].getCount();
        bool pin_state;
        pin_state=Frames[i].getpin();
        if(temp_count>maxCount&&pin_state==false){
            maxCount=temp_count;
            page_id=i;
        }
    }
    if(Frames[page_id].getDirty()==true){
        flushPage(page_id,Frames[page_id].getFileName(),Frames[page_id].getBlockId());
    }
    Frames[page_id].initialize();
    return page_id;
};
// 标记page_id所对应的页已经被修改
void BufferManager::modifyPage(int page_id){
    Frames[page_id].setDirty(true);
};
// 讲对应文件的对应块载入对应内存页，对于文件不存在返回false，否则返回true
bool BufferManager::loadDiskBlock(int page_id , string file_name , int block_id){
    Frames[page_id].initialize();
    FILE* f=fopen(file_name.c_str(),"r");
    if(f==NULL){
		cout << "loadDisk failed" << endl;
        return false;
    }
	frame_num++;
    fseek(f,Pagesize*block_id,SEEK_SET);
    char *buffer=Frames[page_id].getBuffer();
    fread(buffer,Pagesize,1,f);
    fclose(f);
    Frames[page_id].setCount(0);
    Frames[page_id].setpin(false);
    Frames[page_id].setDirty(false);
    Frames[page_id].setBlockId(block_id);
    Frames[page_id].setFileName(file_name);
    return true;
};
int BufferManager::getPageId(string file_name , int block_id){
    int flag=0;
    int page_id;
    for(int i=0;i<frame_num;i++){
        Frames[i].addCount();
        if(file_name==Frames[i].getFileName()&&block_id==Frames[i].getBlockId()){
            flag=1;
            page_id=i;
        }
    }
    if(flag==1){
        //访问过的Page的count值设为0（替换算法的需要）
        Frames[page_id].setCount(0);
        return page_id;
    }
    page_id=getEmptyPageId();
    loadDiskBlock(page_id,file_name,block_id);
    return page_id;
};

//获得表格的block数量
int BufferManager::getBlockNum(string file_name) {
	char *p;
	int block_num = -1;
	do {
		p = getPage(file_name, block_num + 1);
		block_num++;
	} while (p[0] != '\0');
	return block_num;
}

