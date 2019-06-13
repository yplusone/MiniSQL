#include"RecordManager.h"
using namespace std;
#pragma warning(disable:4996)；
//extern   
void RecordManager::createTableFile(string table_name){
	string file_name;
	file_name="./database/data/"+table_name;
	FILE* f=fopen(file_name.c_str(),"w+");

	if (f == NULL) {
		throw file_cant_open();
	}

	fclose(f);
}  
//删除数据文件
void RecordManager::dropTableFile(string table_name){
	string file_name="./database/data/"+table_name;
	if (remove(table_name.c_str()) == -1) {
		throw delete_file_error();
	}
}  
//插入一条记录
void RecordManager::insertRecord(string table_name , Tuple& tuple){
	  
	Catalog_manager catalog_manager;
	string file_name="./database/data/"+table_name;
	Attribute attr=catalog_manager.Get_attr(table_name);
	vector<Data>& v=tuple.getData();
	int deleteState = tuple.getDeleteState();
	
	//异常检测
	//检测表是否存在
	if (!catalog_manager.Has_table(table_name)) {
		throw table_not_exist();
	}
	//检测是否存在主键冲突
	Table table = selectRecord(table_name);
	vector<Tuple> tuples = table.getTuples();
	if (attr.primary_key >= 0) {
		if (isConflict(tuples, v, attr.primary_key) == true) {
			throw primary_key_conflict();
		}
	}
	//检查是否存在unique冲突
	for (int i = 0; i < attr.num; i++) {
		if (attr.unique[i] == true) {
			if (isConflict(tuples, v, i) == true) {
				throw unique_conflict();
			}
		}
	}
	//完成异常检测
	int block_num=buffer_manager.getBlockNum(file_name);
	if(block_num<=0){
		block_num=1;
	}
	//将文件最后一块的记录拉取到内存中
	char*p=buffer_manager.getPage(file_name,block_num-1);
	int begin_index;//可以从哪个空位开始写
	for(int i=0;i<Pagesize;i++){
		if(p[i]=='\0'){
			begin_index=i;
			break;
		}
	}
	int length=0;
	int len;
	for(int i=0;i<v.size();i++){
		switch(v[i].type){
			case -1:
				len=getDataLength(v[i].datai);
				length+=len;
				break;
			case 0:
				len=getDataLength(v[i].dataf);
				length+=len;
				break;
			default:
				length+=v[i].datas.length();
			
		}
	}
	length+=v.size()+5;//包括每一个属性值后面会多一个空格，一条记录后会多一个'\n',再预留一点空间
	int insert_block_id=block_num-1;//插入块的编号
	//如果这个块的剩余空间不够，开启下一个空块
	if(begin_index+length<Pagesize){
		insert_block_id=block_num-1;
		insertRecordToBuffer(p,begin_index,v);
		int page_id=buffer_manager.getPageId(file_name,insert_block_id);
		buffer_manager.modifyPage(page_id);
	}
	else{
		insert_block_id=block_num;
		p=buffer_manager.getPage(file_name,insert_block_id);
		insertRecordToBuffer(p,0,v);
		int page_id=buffer_manager.getPageId(file_name,insert_block_id);
		buffer_manager.modifyPage(page_id);
	}
	/*
	更新索引
	*/
}; 


//把一条记录插入到buffer
void RecordManager::insertRecordToBuffer(char* p,int offset,vector<Data> v){
	for(int i=0;i<v.size();i++){
		Data d=v[i];
		switch(d.type){
			case -1:
				copyString(p,offset,d.datai);
				break;
			case 0:
				copyString(p,offset,d.dataf);
				break;
			default:
				copyString(p,offset,d.datas);
		}
	}
	p[offset++] = '0';
	p[offset]='\n';
}


//删除一张表中的所有记录，采用lazy delete，但不删除表，返回删除的记录数
int RecordManager::deleteRecord(string table_name){
	  
	Catalog_manager catalog_manager;
	//Index_manager index_manager(table_name);
	string file_name="./database/data/"+table_name;
	Attribute attr=catalog_manager.Get_attr(table_name);
	/*
	异常检测
	*/
	int block_num=buffer_manager.getBlockNum(file_name);
	if(block_num<=0){
		return 0;
	}
	int deleteCount = 0;
	for (int i = 0; i < block_num; i++) {
		int count = 0;
		char* p = buffer_manager.getPage(file_name, i);
		vector<Tuple> tuplelist = readAllTuple(p, attr);
		for (int j = 0; j < tuplelist.size(); j++) {
			tuplelist[j].setDeleteState();
			count++;
		}
		deleteCount += count;
		deleteOneBlock(p, tuplelist);
		if (count > 0) {
			int page_id = buffer_manager.getPageId(file_name, i);
			buffer_manager.modifyPage(page_id);
		}
	}

	return deleteCount;
}
//将满足一定条件的记录删除，返回删除的记录数
int RecordManager::deleteRecord(string table_name, vector<Condition> cons){
	  
	Catalog_manager catalog_manager;
	//Index_manager index_manager(table_name);
	string file_name="./database/data/"+table_name;
	Attribute attr=catalog_manager.Get_attr(table_name);
	/*
	异常检测
	*/
	int block_num=buffer_manager.getBlockNum(file_name);
	if(block_num<=0){
		return 0;
	}
	int deleteCount=0;
	for(int i=0;i<block_num;i++){
		int count = 0;
		char* p=buffer_manager.getPage(file_name,i);
		vector<Tuple> tuplelist=readAllTuple(p,attr);
		for(int j=0;j<tuplelist.size();j++){
			bool result=satisfyCondition(tuplelist[j],cons,attr);
			if(result==true){
				tuplelist[j].setDeleteState();
				count++;
			}			
		}
		deleteCount += count;
		deleteOneBlock(p, tuplelist);
		if(count>0){
			int page_id=buffer_manager.getPageId(file_name,i);
			buffer_manager.modifyPage(page_id);		
		}
	}

	return deleteCount;
}
//选择一张表的所有记录，返回这张表
Table RecordManager::selectRecord(string table_name){
	 
	Catalog_manager catalog_manager;
	Attribute attr=catalog_manager.Get_attr(table_name);
	string file_name = "./database/data/" + table_name;
	Table res(table_name,attr);
	int block_num= buffer_manager.getBlockNum(file_name);
	if(block_num<=0){
		return res;
	}
	for(int i=0;i<block_num;i++){
		char* p=buffer_manager.getPage(file_name,i);
		vector<Tuple> tuplelist=readAllTuple(p,attr);
		for(int j=0;j<tuplelist.size();j++){
			res.addTuple(tuplelist[j]);	
		}
	}	
	return res;
}
//选择一张表中满足一定条件的所有记录，返回结果
Table RecordManager::selectRecord(string table_name, vector<Condition> cons){
	  
	Catalog_manager catalog_manager;
	Attribute attr= catalog_manager.Get_attr(table_name);
	string file_name = "./database/data/" + table_name;
	Table res(table_name,attr);
	int block_num=buffer_manager.getBlockNum(file_name);
	if(block_num<=0){
		return res;
	}
	for(int i=0;i<block_num;i++){
		char* p=buffer_manager.getPage(file_name,i);
		vector<Tuple> tuplelist=readAllTuple(p,attr);
		for(int j=0;j<tuplelist.size();j++){
			bool result=satisfyCondition(tuplelist[j],cons,attr);
			if(result==true){
				res.addTuple(tuplelist[j]);	
			}			
		}
	}	
	return res;
};
vector<Tuple> RecordManager::readAllTuple(char* p,Attribute attr){
	vector<Tuple> res;
	while(*p!='\0'){
		Tuple temp_tuple=readOneTuple(p,attr);
		res.push_back(temp_tuple);
	}
	return res;
}
//读取一个tuple
Tuple RecordManager::readOneTuple(char* &p,Attribute attr){
	Tuple tuple;
	for(int i=0;i<attr.num;i++){
		Data data;
		data.type=attr.type[i];
		string datastring;
		for(int j=0;*p!=' ';j++){
			datastring=datastring+*p;
			p++;
		}
		p++;
		getDataFromString(data,datastring,data.type);
		tuple.addData(data);
	}
	if(*p=='1'){
		tuple.setDeleteState();
	}
	p++;
	p++;
	return tuple;
}
//deleteflag如果为1,则要删除
//如果为0,那就让指针走到末尾不删除。
void RecordManager::deleteOneBlock(char* p,vector<Tuple> tuples){
	char* temp_follow=p;
	char* temp_success=p+1;		
	if(*p=='\0'){
		return;
	}
	int i = 0;
	while(true){
		while(*temp_success!='\n' || *temp_success!='\0'){
			temp_follow++;
			temp_success++;
		}
		if(*temp_success=='\n'){
			if(tuples[i].getDeleteState()==true)
			*temp_follow='1';
			i++;
		}		
		if(*temp_success=='\0'){
			break;
		}
	}
}

bool RecordManager::satisfyCondition(Tuple tuple,vector<Condition> cons,Attribute attr){
	for(int i=0;i<cons.size();i++){
		Data temp_data=tuple.getDataFromAttr(cons[i].Attribute_name);
		bool result=dataCompare(temp_data.type,cons[i].data,temp_data,cons[i].relation);
		if(result==false){
			return false;
		}
	}
	return true;
}

void getDataFromString(Data& data, string datastring, int type) {
	stringstream ss;
	ss << datastring;
	switch (type) {
	case -1:
		ss >> data.datai;
		break;
	case 0:
		ss >> data.dataf;
		break;
	default:
		data.datas = datastring;
	}
}

int getAttrIndex(string attrName, Attribute attr) {
	for (int i = 0; i < attr.num; i++) {
		if (attr.name[i].compare(attrName) == 0) {
			return i;
		}
	}
	return -1;
}

bool dataCompare(int type, Data data1, Data data2, int relation) {
	switch (type) {
	case -1:
		switch (relation) {
		case 0:
			if (data1.datai < data2.datai) {
				return true;
			}
			break;
		case 1:
			if (data1.datai <= data2.datai) {
				return true;
			}
			break;
		case 2:
			if (data1.datai == data2.datai) {
				return true;
			}
			break;
		case 3:
			if (data1.datai >= data2.datai) {
				return true;
			}
			break;
		case 4:
			if (data1.datai > data2.datai) {
				return true;
			}
			break;
		case 5:
			if (data1.datai != data2.datai) {
				return true;
			}
			break;
		}
		break;
	case 0:
		switch (relation) {
		case 0:
			if (data1.dataf < data2.dataf) {
				return true;
			}
			break;
		case 1:
			if (data1.dataf <= data2.dataf) {
				return true;
			}
			break;
		case 2:
			if (data1.dataf == data2.dataf) {
				return true;
			}
			break;
		case 3:
			if (data1.dataf >= data2.dataf) {
				return true;
			}
			break;
		case 4:
			if (data1.dataf > data2.dataf) {
				return true;
			}
			break;
		case 5:
			if (data1.dataf != data2.dataf) {
				return true;
			}
			break;
		}
		break;
	default:
		switch (relation) {
		case 0:
			if (data1.datas.compare(data2.datas) < 0) {
				return true;
			}
			break;
		case 1:
			if (data1.datas.compare(data2.datas) <= 0) {
				return true;
			}
			break;
		case 2:
			if (data1.datas.compare(data2.datas) == 0) {
				return true;
			}
			break;
		case 3:
			if (data1.datas.compare(data2.datas) >= 0) {
				return true;
			}
			break;
		case 4:
			if (data1.datas.compare(data2.datas) > 0) {
				return true;
			}
			break;
		case 5:
			if (data1.datas.compare(data2.datas) != 0) {
				return true;
			}
			break;
		}
	}
	return false;
}

bool RecordManager::isConflict(vector<Tuple>& tuples, vector<Data>& v, int index) {
	for (int i = 0; i < tuples.size(); i++) {
		if (tuples[i].getDeleteState() == true)
			continue;
		std::vector<Data> d = tuples[i].getData();
		switch (v[index].type) {
		case -1: {
			if (v[index].datai == d[index].datai)
				return true;
		}; break;
		case 0: {
			if (v[index].dataf == d[index].dataf)
				return true;
		}; break;
		default: {
			if (v[index].datas.compare(d[index].datas))
				return true;
		};
		}
	}
	return false;
}