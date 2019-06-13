#pragma once

//
//  API.h
//
//
//  Created by HLL on 2019/6/5.
//

#ifndef API_h
#define API_h

#include "Basic.h"
#include "RecordManager.h"
#include "catalog_manager.h"
#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
//API�ӿڡ���ΪInterpreter������͵ײ�Manager���ӵĽӿ�
//������������й��ܽӿ�
//APIֻ�������ĳ������жϣ�������쳣�׳����ɸ��ײ�Manager���
class API {
public:
	//���캯��
	API();
	//��������
	~API();

	//���룺������Where������������Where����ֵ��
	//�����Table���Ͷ���(������Ӧ������Ԫ��)
	//���ܣ����ذ�������Ŀ����������Where�����ļ�¼�ı�
	//�ڶ�������ѯ����£�����Where�µ��߼���������Table��ƴ��
	//�쳣���ɵײ㴦��
	//��������ڣ��׳�table_not_exist�쳣
	//������Բ����ڣ��׳�attribute_not_exist�쳣
	//���Where�����е������������Ͳ�ƥ�䣬�׳�data_type_conflict�쳣


	//void selectRecord(std::string table_name, std::vector<std::string> target_attr, std::vector<Condition> where, char operation);
	//Table

	//���룺������Where������������Where����ֵ��
	//�����void
	//���ܣ�ɾ����Ӧ�����µ�Table�ڼ�¼(��ɾ�����ļ�)
	//�쳣���ɵײ㴦��
	//��������ڣ��׳�table_not_exist�쳣
	//������Բ����ڣ��׳�attribute_not_exist�쳣
	//���Where�����е������������Ͳ�ƥ�䣬�׳�data_type_conflict�쳣


	//void deleteRecord(std::string table_name, std::string target_attr, Condition where);

	//���룺������һ��Ԫ�����
	//�����void
	//���ܣ����Ӧ���ڲ���һ����¼
	//�쳣���ɵײ㴦��
	//���Ԫ�����Ͳ�ƥ�䣬�׳�tuple_type_conflict�쳣
	//���������ͻ���׳�primary_key_conflict�쳣
	//���unique���Գ�ͻ���׳�unique_conflict�쳣
	//��������ڣ��׳�table_not_exist�쳣


	void insertRecord(std::string table_name, Tuple& tuple);

	//���룺���������ԣ�����������
	//������Ƿ񴴽��ɹ�
	//���ܣ������ݿ��в���һ�����Ԫ��Ϣ
	//�쳣���ɵײ㴦��
	//����Ѿ�����ͬ�����ı���ڣ����׳�table_exist�쳣


	bool createTable(std::string table_name, Attribute attribute, int primary, Index index);


	//���룺����
	//������Ƿ�ɾ���ɹ�
	//���ܣ������ݿ���ɾ��һ�����Ԫ��Ϣ�����������м�¼(ɾ�����ļ�)
	//�쳣���ɵײ㴦��
	//��������ڣ��׳�table_not_exist�쳣
	//bool dropTable(std::string table_name);

	//���룺��������������������
	//������Ƿ񴴽��ɹ�
	//���ܣ������ݿ��и��¶�Ӧ���������Ϣ����ָ�������Ͻ���һ��������
	//�쳣���ɵײ㴦��
	//��������ڣ��׳�table_not_exist�쳣
	//�����Ӧ���Բ����ڣ��׳�attribute_not_exist�쳣
	//�����Ӧ�����Ѿ������������׳�index_exist�쳣
	//bool createIndex(std::string table_name, std::string index_name, std::string attr_name);

	//���룺������������
	//������Ƿ�ɾ���ɹ�
	//���ܣ�ɾ����Ӧ��Ķ�Ӧ�����ϵ�����
	//�쳣���ɵײ㴦��
	//��������ڣ��׳�table_not_exist�쳣
	//�����Ӧ���Բ����ڣ��׳�attribute_not_exist�쳣
	//�����Ӧ����û���������׳�index_not_exist�쳣
	//bool dropIndex(std::string table_name, std::string index_name);

private:
	//˽�к��������ڶ�������ѯʱ��and�����ϲ�


	//Table unionTable(Table& table1, Table& table2, std::string target_attr, Condition where);
	//˽�к��������ڶ�������ѯʱ��or�����ϲ�
	//Table joinTable(Table& table1, Table& table2, std::string target_attr, Condition where);
	//private: �Ķ�
	//RecordManager record;
	//CatalogManager catalog;
};

////���ڶ�vector��sortʱ����
//bool sortcmp(Tuple& tuple1, Tuple& tuple2);
////���ڶ�vector�Ժϲ�ʱ������
//bool calcmp(Tuple& tuple1, Tuple& tuple2);
//bool isSatisfied(Tuple& tuple, int target_attr, Condition where);

#endif /* API_hpp */
