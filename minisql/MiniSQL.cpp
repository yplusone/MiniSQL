//
//  MiniSQL.cpp
//
//
//  Created by HLL on 2019/6/4.
//
#include "MiniSQL.h"

Interpreter::Interpreter() {
}

//��query���и�ֵ
void Interpreter::getQuery() {
	std::string tmp;
	//�õ�һ�е������ַ��������һ���ַ�Ϊ�ֺ�ʱ����
	do {
		std::cout << ">>> ";
		getline(std::cin, tmp);
		query += tmp;
		query += ' ';
	} while (tmp[tmp.length() - 1] != ';');
	//�����һ����β��ʶ��
	query[query.length() - 2] = '\0';
	//����Normalize�����ַ����Ĺ淶��
	Normalize();
}

void Interpreter::Normalize() {
	//�����е�������ŵ�ǰ������һ���ո��Բ�ֶ���
	for (int pos = 0; pos < query.length(); pos++) {
		if (query[pos] == '*' || query[pos] == '=' || query[pos] == ',' || query[pos] == '(' || query[pos] == ')' || query[pos] == '<' || query[pos] == '>') {
			if (query[pos - 1] != ' ')
				query.insert(pos++, " ");
			if (query[pos + 1] != ' ')
				query.insert(++pos, " ");
		}
	}
	//�ڽ�β��һ���ո��Թ淶��
	query.insert(query.length() - 2, " ");
	//ɾ�������еĶ���ո�
	std::string::iterator it;
	int flag = 0;
	for (it = query.begin(); it < query.end(); it++) {
		if (flag == 0 && (*it == ' ' || *it == '\t')) {
			flag = 1;
			continue;
		}
		if (flag == 1 && (*it == ' ' || *it == '\t')) {
			query.erase(it);
			if (it != query.begin())
				it--;
			continue;
		}
		if (*it != ' ' && *it != '\t') {
			flag = 0;
			continue;
		}
	}
	//������俪ʼ�пո񣬾�ɾ������ո��Թ淶��
	if (query[0] == ' ')
		query.erase(query.begin());
	//��query�ĵ�һ����ȫ��ת��ΪСд������֮����ַ����Ľ���
	query = getLower(query, 0);
}

void Interpreter::GET_INP() {
	try {
		//�����ַ����ĵ�һ���������������еĲ�������
		if (query.substr(0, 6) == "select") {
			printf(">>conduct select");
			//SELECT();
		}
		//��������drop�����������������Ҫ���н�һ���Ľ���
		else if (query.substr(0, 4) == "drop") {
			query = getLower(query, 5);
			if (query.substr(5, 5) == "table")
				//DROP_TABLE()
				;
			//else if (query.substr(5, 5) == "index")
			//	cout << "TBC";

				//DROP_INDEX();
		}
		else if (query.substr(0, 6) == "insert") {
			INSERT();
			printf(">> conduct insert\n");
		}
		//createҲ���������
		else if (query.substr(0, 6) == "create") {
			query = getLower(query, 7);
			if (query.substr(7, 5) == "table") {
				printf(">> conduct createtable\n");
				CREATE_TABLE();
			}
			else if (query.substr(7, 5) == "index") {
				//CREATE_INDEX();
			}
		}
		else if (query.substr(0, 6) == "delete") {
			//DELETE();
		}
		//����describe�����ַ�ʽ������ʹ�û��߼�
		else if (query.substr(0, 8) == "describe" || query.substr(0, 4) == "desc") {
			//SHOW();
		}
		else if (query.substr(0, 4) == "exit" && query[5] == '\0') {
			EXIT();
		}
		else if (query.substr(0, 8) == "execfile") {
			READ();
		}
		//�������ָ����ܶ�Ӧ�����׳������ʽ����
		else {
			throw 1;
		}
	}
	/*
	catch(table_exist error){
		std::cout<<">>> Error: Table has existed!"<<std::endl;
	}
	catch(table_not_exist error) {
		std::cout<<">>> Error: Table not exist!"<<std::endl;
	}
	catch(attribute_not_exist error) {
		std::cout<<">>> Error: Attribute not exist!"<<std::endl;
	}
	catch(index_exist error) {
		std::cout<<">>> Error: Index has existed!"<<std::endl;
	}
	catch(index_not_exist error) {
		std::cout<<">>> Error: Index not existed!"<<std::endl;
	}
	catch(tuple_type_conflict error) {
		std::cout<<">>> Error: Tuple type conflict!"<<std::endl;
	}
	catch(primary_key_conflict error) {
		std::cout<<">>> Error: Primary key conflict!"<<std::endl;
	}
	catch(data_type_conflict error) {
		std::cout<<">>> Error: data type conflict!"<<std::endl;
	}
	catch(index_full error) {
		std::cout<<">>> Error: Index full!"<<std::endl;
	}
	catch(unique_conflict error) {
		std::cout<<">>> Error: unique conflict!"<<std::endl;
	}
	catch(exit_command error){
		std::cout<<">>> Bye bye~"<<std::endl;
		exit(0);
	}
	*/
	catch (...) {
		std::cout << ">>> Error: Input format error!" << std::endl;
	}
}

void Interpreter::CREATE_TABLE() {
	//�������
	std::string table_name;
	//��λ����λ��
	int check_index;
	table_name = getWord(13, check_index);
	//���������ʼ��
	Index index_create;
	index_create.num = 0;
	//��������
	Attribute attr_create;
	std::string attr_name;
	int primary = -1;
	int attr_num = 0;
	while (1) {
		check_index += 3;
		//����Ѿ�������string�ˣ����˳�
		if (query[check_index] == '\0') {
			if (query[check_index - 2] == '\0')
				throw 1;
			else
				break;
		}
		//�õ�һ�����Ե�����
		attr_name = getWord(check_index, check_index);
		//�����������ǲ���primary
		std::string check_primary(attr_name);
		check_primary = getLower(check_primary, 0);
		if (check_primary == "primary") {
			int tmp_end = check_index;
			std::string check_key = getWord(tmp_end + 1, tmp_end);
			//�ټ��������Ĺؼ����ǲ���key��������ǣ���primary������������
			if (check_key != "key") {
				attr_create.name[attr_num] = attr_name;
				break;
			}
			//�������Ե�primary
			else {
				check_index = tmp_end + 3;
				std::string unique_name = getWord(check_index, check_index);
				int hasset = 1;
				for (int find_name = 0; find_name < attr_create.num; find_name++) {
					if (attr_create.name[find_name] == unique_name) {
						hasset = 0;
						primary = find_name;
						attr_create.unique[find_name] = true;
						check_index += 2;
						break;
					}
				}
				if (hasset)
					throw 1;
				continue;
			}
		}
		//�������primary key����ֱ�Ӱ�����������ֲ����ȥ
		else
			attr_create.name[attr_num] = attr_name;
		check_index++;
		//�õ�һ�����Ե�����
		attr_create.type[attr_num] = getType(check_index, check_index);
		attr_create.unique[attr_num] = false;
		//�ж�֮����û��unique�ؼ���
		if (query[check_index + 1] == 'u' || query[check_index + 1] == 'U') {
			query = getLower(query, 0);
			//����У�����Ϊunique�������ͷΪu�����ǲ�Ϊunique���Ǿ���һ������
			if (getWord(check_index + 1, check_index) == "unique") {
				attr_create.unique[attr_num] = true;
			}
			else
				throw 1;
		}
		//�������Ե�����
		attr_num++;
		attr_create.num = attr_num;
	}
	//����CatalogManager���������Ϣ�����ȥ
	API API;
	API.createTable(table_name, attr_create, primary, index_create);
	std::cout << ">>> SUCCESS" << std::endl;
}


void Interpreter::INSERT() {
	API API;
	Catalog_manager CM;
	std::string table_name;
	int check_index;
	Tuple tuple_insert;
	Attribute attr_exist;
	if (getLower(query, 7).substr(7, 4) != "into")
		printf(">>>  select input_format_error\n");
	table_name = getWord(12, check_index);
	if (getLower(query, check_index + 1).substr(check_index + 1, 6) != "values")
		printf(">>>  input_format_error\n");
	check_index += 8;
	if (query[check_index] != '(')
		printf(">>>  input_format_error\n");

	if (!CM.Has_table(table_name))
		printf(">>>  table_not_exist\n");
	attr_exist = CM.Get_attr(table_name);
	check_index--;
	int num_of_insert = 0;
	//�������ڵ�����Ԫ�ؽ��б���
	while (query[check_index + 1] != '\0' && query[check_index + 1] != ')') {
		if (num_of_insert >= attr_exist.num)
			throw 1;//��������ƥ��
		check_index += 3;
		std::string value_insert = getWord(check_index, check_index);
		Data insert_data;
		insert_data.type = attr_exist.type[num_of_insert];
		switch (attr_exist.type[num_of_insert]) {
		case -1:
			try {
				insert_data.datai = stringToNum<int>(value_insert);
				insert_data.type = -1;
			}
			catch (...) {
				printf(">>> data_type_conflict\n");
			}
			insert_data.type = -1;
			break;
		case 0:
			try {
				insert_data.dataf = stringToNum<float>(value_insert);
				insert_data.type = 0;
			}
			catch (...) {
				printf(">>> data_type_conflict\n");
			}
			break;
		default:
			try{
				if (!(value_insert[0] == '\'' && value_insert[value_insert.length() - 1] == '\'') && !(value_insert[0] == '"' && value_insert[value_insert.length() - 1] == '"'))
					printf(">>> input_format_error\n");
				if (value_insert.length() - 1 > attr_exist.type[num_of_insert])
					printf(">>> too long \n");
					//���ȳ�������
				insert_data.datas = value_insert.substr(1, value_insert.length() - 2);
				insert_data.type = value_insert.length() - 2;
			}
			catch (...) {
				printf(">>> data_type_conflict\n");
			}
			break;
		}
		tuple_insert.addData(insert_data);
		switch (insert_data.type) {
		case -1:
			cout << insert_data.datai<<std::endl;
			break;
		case 0:
			cout << insert_data.dataf << std::endl;
		default:
			cout << insert_data.datas << std::endl;
		}
		num_of_insert++;
	}
	if (query[check_index + 1] == '\0')
		printf(">>> input_format_error\n");
	if (num_of_insert != attr_exist.num)
		printf(">>> wrong in numbers of inputs\n");//�������������ȷ

	API.insertRecord(table_name, tuple_insert);
	std::cout << ">>> SUCCESS" << std::endl;
}

void Interpreter::EXIT() {
	//�����Ҫ�˳���ֱ���׳�һ��exit command
	return;
}

void Interpreter::READ() {
	int check_index = 0;
	int start_index = 0;
	std::string tmp_query;
	//�õ��ļ�·��
	std::string file_path = getWord(9, check_index);
	//���·�����ж�����ַ������Ǹ�ʽ����
	if (query[check_index + 1] != '\0')
		throw 1;//��ʽ����
	std::string::iterator it;
	//�������ļ�������,����
	std::fstream fs(file_path);
	//�����ַ���������
	std::stringstream ss;
	//���ļ����е��ַ����뵽�ַ�������
	ss << fs.rdbuf();
	//��ȡ���е��ַ���
	tmp_query = ss.str();
	//��ִ��һ��
	check_index = 0;
	do {
		while (tmp_query[check_index] != '\n')
			check_index++;
		query = tmp_query.substr(start_index, check_index - start_index);
		check_index++;
		start_index = check_index;
		Normalize();
		GET_INP();
	} while (tmp_query[check_index] != '\0');
}





//����Ҫtable����ʾ



//�õ�һ��λ�õ���������
int Interpreter::getType(int pos, int& end_pos) {
	std::string type = getWord(pos, end_pos);
	if (type == "int")
		return -1;
	else if (type == "float")
		return 0;
	else if (type == "char") {
		end_pos += 3;
		std::string length = getWord(end_pos, end_pos);
		end_pos += 2;
		return atoi(length.c_str()) + 1;
	}
	throw 1;
}

//ȡ��һ�����ʣ�ͬʱ��end_pos���µ����ʺ���Ǹ��ո��λ�ô�
std::string Interpreter::getWord(int pos, int& end_pos) {
	std::string PartWord = "";
	for (int pos1 = pos; pos1 < (int)query.length(); pos1++) {
		if (query[pos1] == ' ' || query[pos1] == '\0')
		{
			PartWord = query.substr(pos, pos1 - pos);
			//std::cout<<PartWord<<std::endl;
			end_pos = pos1;
			return PartWord;
		}
	}
	return PartWord;
}

//�õ�һ���ַ���Сд��ʽ
std::string Interpreter::getLower(std::string str, int pos) {
	for (int index = pos;; index++) {
		if (str[index] == ' ' || str[index] == '\0')
			break;
		else if (str[index] >= 'A' && str[index] <= 'Z')
			str[index] += 32;
	}
	return str;
}

//�õ�һ����ϵ���ŵ��ַ����������ϵ�����������ַ������ַ��л����һ���ո���"< ="
std::string Interpreter::getRelation(int pos, int& end_pos) {
	std::string PartWord = "";
	for (int pos1 = pos; pos1 < (int)query.length(); pos1++) {
		if (query[pos1] == ' ')
			continue;
		if (query[pos1] != '<' && query[pos1] != '>' && query[pos1] != '=' && query[pos1] != '!')
		{
			PartWord = query.substr(pos, pos1 - pos - 1);
			//std::cout<<PartWord<<std::endl;
			end_pos = pos1 - 1;
			return PartWord;
		}
	}
	return PartWord;
}

//���ݳ����õ����ε����ֵĳ���
int Interpreter::getBits(int num) {
	int bit = 0;
	if (num == 0)
		return 1;
	if (num < 0) {
		bit++;
		num = -num;
	}
	while (num != 0) {
		num /= 10;
		bit++;
	}
	return bit;
}

//���ݳ����õ�С�������ֵĳ���
int Interpreter::getBits(float num) {
	int bit = 0;
	if ((int)num == 0)
		return 4;
	if (num < 0) {
		bit++;
		num = -num;
	}
	int integer_part = num;
	while (integer_part != 0) {
		bit++;
		integer_part /= 10;
	}
	return bit + 3;//Ϊ�˱���С����ĺ�λ
}

