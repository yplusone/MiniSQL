#pragma once
#pragma once
#pragma once
//
//  MiniSQL.hpp
//
//
//  Created by HLL on 2019/6/5.
//  delete all the EXEC
//  ������ҪСд��ĸ������

#ifndef _INTERPRETER_H_
#define _INTERPRETER_H_ 

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "API.h"
#include "exception.h"

class Interpreter {
public:
	Interpreter();
	//---
	//���ܣ���ȡһ�����룬�жϲ�����ʽ�淶��
	void getQuery();

	//���ܣ����ַ������г����������ж�����ַ����Ĺ���
	//�쳣�������������Ĺؼ��ֲ����ڣ����׳��쳣input_format_error
	void GET_INP();//!!void EXEC();����

	//---
	//���룺create table student2(
	//                id int,
	//                name char(12) unique,
	//                score float,
	//                primary key(id)
	//                );
	//�����Success�����쳣
	//�쳣����ʽ�������׳�input_format_error�쳣
	void CREATE_TABLE();//EXEC_CREATE_TABLE()

	//���룺create index stuidx on student2 ( score );
	//�����Success�����쳣
	//���ܣ��ڱ��в���һ�����ֽ�stuidx�����������Ӧ����Ϊscore
	//�쳣����ʽ�������׳�input_format_error�쳣
	//��������ڣ��׳�table_not_exist�쳣
	//�����Ӧ���Բ����ڣ��׳�attribute_not_exist�쳣
	//�����Ӧ�����Ѿ������������׳�index_exist�쳣
	//void CREATE_INDEX();

	//--insert
	//���룺insert into student2 values(1080197996,��name97996��,100);
	//�����Success�����쳣
	//���ܣ���student2�ڲ���ֵ����Ϣ
	//�쳣����ʽ�������׳�Input_format_error
	//��������ڣ��׳�Table_not_exist�쳣
	//������Բ����ڣ��׳�Attribute_not_exist�쳣
	//���Where�����е������������Ͳ�ƥ�䣬�׳�Data_ype_mismatch�쳣
	void INSERT();

	//--select
	//���룺select last_name,first_name,birth,state from president where t1<10 and t2>20 ;
	//�����Success�����쳣
	//���ܣ�����ѡ�������֧�ֵ����Լ��
	//�쳣����ʽ�������׳�input_format_error
	//��������ڣ��׳�table_not_exist�쳣
	//������Բ����ڣ��׳�attribute_not_exist�쳣
	//���Where�����е������������Ͳ�ƥ�䣬�׳�data_type_conflict�쳣
	//void SELECT();

	//���룺drop table t1;
	//�����Success�����쳣
	//���ܣ�ɾ����t1
	//�쳣����ʽ�������׳�input_format_error
	//��������ڣ��׳�table_not_exist�쳣
	//void DROP_TABLE();
	//���룺drop index ID_index on t1;
	//�����Success�����쳣
	//���ܣ��ڱ�t1��ɾ��һ�����ֽ�ID_index������
	//�쳣����ʽ�������׳�input_format_error�쳣
	//��������ڣ��׳�table_not_exist�쳣
	//�����Ӧ���Բ����ڣ��׳�attribute_not_exist�쳣
	//�����Ӧ����û���������׳�index_not_exist�쳣
	//void DROP_INDEX();

	//���룺delete from MyClass where id=1;
	//     delete * from MyClass;
	//where��ֻ����һ����Ϣ
	//�����Success�����쳣
	//���ܣ���Myclass��ɾ��id=1��Ԫ��
	//�쳣����ʽ�������׳�input_format_error�쳣
	//��������ڣ��׳�table_not_exist�쳣
	//������Բ����ڣ��׳�attribute_not_exist�쳣
	//���Where�����е������������Ͳ�ƥ�䣬�׳�data_type_conflict�쳣
	//void DELETE();

	//--show
	//���룺describe T1;
	//     ���� desc T1��
	//���ܣ������T1���������ԣ������Ļ�����Ϣ
	//void SHOW();
	//���룺exit;
	//���ܣ��˳����ݿ�
	void EXIT();
	//--��ȡ
	//���룺execfile �ļ�·��
	//���ܣ������ļ�·����ȡ�ļ���Ϣ�����������ݿ�Ĳ���
	void READ(); //EXEC_FILE() ����

private:
	//�ַ����淶������
	void Normalize();
	//���������ַ����͹淶������ַ���
	std::string query;

	//���룺����Ӧ���ַ��Ŀ�ͷλ�ã����ô������ַ��Ľ�βλ��
	//��������λ������Ӧ�ĵ��ʵ��ַ���
	//���ܣ���query��ȡ��
	std::string getWord(int pos, int& end_pos);

	//--���Բ���
	//���룺����Ҫת��Сд���ַ�����posλ��Ϊ����Ӧ�ĵ��ʵĿ�ʼ��λ��
	//�������posλ�õĵ��ʸĳ�Сд��������ĺ�������ַ���
	//���ܣ����ַ���str�е�posλ�ÿ�ͷ�ĵ���ת����Сд�����ڱ�׼��
	std::string getLower(std::string str, int pos);

	//���룺����Ӧ���ַ��Ŀ�ͷλ�ã����ô������ַ��Ľ�βλ��
	//��������λ������Ӧ�Ĺ�ϵ����
	//���ܣ���query��ȡ����ϵ����
	std::string getRelation(int pos, int& end_pos);

	//���룺����Ӧ���ַ��Ŀ�ͷλ�ã����ô������ַ��Ľ�βλ��
	//���������һ�����ͣ�-1��255��
	int getType(int pos, int& end_pos);

	//���������һ��������λ��
	int getBits(int num);
	//--����
	//���������һ����������λ��������С�����4λ��
	int getBits(float num);
};

template <class Type>
Type stringToNum(const std::string& str)
{
	std::istringstream iss(str);
	Type num;
	iss >> num;
	return num;
}

#endif
