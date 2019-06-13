#include"catalog_manager.h"
#pragma warning(disable:4996)£»
void Catalog_manager::Create_table(string name, Attribute attr, int pri, Index ind) {
	  
	if (Has_table(name)) {
		throw table_exist();
	}

	attr.unique[pri] = 1;

	string tmp;

	tmp = " " + name + " " + Num2Str(attr.num, 2);

	for (int i = 0; i < attr.num; i++) {
		tmp += " " + Num2Str(attr.type[i], 3) + " " + attr.name[i] + " " + ("0" + attr.unique[i]);
	}
	tmp += " " + Num2Str(pri, 2) + " ;" + Num2Str(ind.num, 2);

	for (int i = 0; i < ind.num; i++) {
		tmp += " " + Num2Str(ind.location[i], 2) + " " + ind.indexname[i];
	}
	tmp += "\n" + '#';

	tmp = Num2Str(tmp.length() + 3, 4) + tmp;

	int num_blo = Get_size(TABLE_MANAGER_PATH) / Pagesize;

	for (int i = 0; i < num_blo; i++) {
		char *buf = buffer_manager.getPage(TABLE_MANAGER_PATH, i);
		int id = buffer_manager.getPageId(TABLE_MANAGER_PATH, i);
		int j;
		for (j = 0; j < Pagesize && buf[j] != '\0'&&buf[j] != '#'; j++);
		if (j + tmp.length() < Pagesize) {
			if (j && buf[j - 1] == '#') buf[j - 1] = '\0';
			else if (buf[j] == '#') buf[j] = '\0';
			strcat(buf, tmp.c_str());
			buffer_manager.modifyPage(id);
			return;
		}
	}
	char *buf = buffer_manager.getPage(TABLE_MANAGER_PATH, num_blo);
	int id = buffer_manager.getPageId(TABLE_MANAGER_PATH, num_blo);
	strcat(buf, tmp.c_str());
	buffer_manager.modifyPage(id);
}
void Catalog_manager::Drop_table(string name) {
	if (Has_table(name) == 0) {
		//throw Table_nonexist();
	}
	int blo;
	int st = Get_place(name, blo);
	char *buf = buffer_manager.getPage(TABLE_MANAGER_PATH, blo);
	int id = buffer_manager.getPageId(TABLE_MANAGER_PATH, blo);

	string buffer_check(buf);

	int ed = st + Str2Num(buffer_check.substr(st, 4));
	int ind = 0, i = 0;
	while (buf[ind] != '#') {
		if (ind < st || ind >= ed)
			buf[i++] = buf[ind];
		ind++;
	}
	buf[i++] = '#';
	buf[i] = '\0';
	buffer_manager.modifyPage(id);
}


bool Catalog_manager::Has_attribute(string name, string attr) {
	if (Has_table(name) == 0) {
		//throw Table_nonexist();
	}
	Attribute tmp = Get_attr(name);
	for (int i = 0; i < tmp.num; i++) {
		if (attr == tmp.name[i]) return true;
	}
	return false;
}

Attribute Catalog_manager::Get_attr(string name) {
	if (Has_table(name) == 0) {
		//throw Table_nonexist();
	}
	int blo;
	Attribute attr;
	int st = Get_place(name, blo);
	char *buf = buffer_manager.getPage(TABLE_MANAGER_PATH, blo);
	string buffer_check(buf);
	int ed = 0;
	string name_t = Get_name(buffer_check, st, ed);
	st = ed + 1;
	attr.num = Str2Num(buffer_check.substr(st, 2));
	st += 3;
	for (int i = 0l; i < attr.num; i++) {
		if (buffer_check[st] == '-') {
			attr.type[i] = -1;
			st += 5;
			while (buffer_check[st] != ' ') {
				attr.name[i] += buffer_check[st++];
			}
			st++;
			//attr.unique[i] = (buffer_check[st] - '0');
		}
		else if (Str2Num(buffer_check.substr(st, 3)) == 0) {
			attr.type[i] = 0;
			st += 4;
			while (buffer_check[st] != ' ') {
				attr.name[i] += buffer_check[st++];
			}
			st++;

		}
		else {
			attr.type[i] = Str2Num(buffer_check.substr(st, 3));
			st += 4;
			while (buffer_check[st] != ' ') {
				attr.name[i] += buffer_check[st++];
			}
			st++;
		}
		attr.unique[i] = (buffer_check[i] - '0');
		st += 2;
	}
	if (buffer_check[st] == '-')
		attr.primary_key = -1;
	else
		attr.primary_key = Str2Num(buffer_check.substr(st, 2));

	Index ind = Get_index(name_t);
	for (int i = 0; i < 32; i++)
		attr.has_index[i] = false;
	for (int i = 0; i < ind.num; i++)
		attr.has_index[ind.location[i]] = true;

	return attr;
}
void Catalog_manager::Create_index(string name, string attr, string ind) {
	if (Has_table(name) == 0) {
		//throw Table_nonexist();
	}
	if (Has_attribute(name,attr) == 0) {
		//throw _nonexist();
	}
	Index tmp_i = Get_index(name);
	Attribute tmp_a = Get_attr(name);

	tmp_i.indexname[tmp_i.num] = ind;

	for (int i = 0; i < tmp_a.num; i++) {
		if (attr == tmp_a.name[i]) {
			tmp_i.location[tmp_i.num] = i;
			break;
		}
	}
	tmp_i.num++;
	Drop_table(name);
	Create_table(name, tmp_a, tmp_a.primary_key, tmp_i);

}

string Catalog_manager::Index2Attr(string name, string ind) {
	if (!Has_table(name)) {
		//throw Exist();
	}
	Index tmp = Get_index(name);
	int pos = -1;
	for (int i = 0; i < tmp.num; i++) {
		if (tmp.indexname[i] == ind) {
			pos = i;
			break;
		}
	}
	if (pos = -1) {
		//throw Exist();
	}
	Attribute attr = Get_attr(name);
	return attr.name[tmp.location[pos]];
}

void Catalog_manager::Drop_index(string name, string ind) {
	if (Has_table(name) == 0) {
		//throw Table_nonexist();
	}
	Index tmp_i = Get_index(name);
	Attribute attr_a = Get_attr(name);
	int pos = -1;
	for (int i = 0; i < tmp_i.num; i++) {
		if (tmp_i.indexname[i] == ind) {
			pos = i;
			break;
		}
	}
	tmp_i.indexname[pos] = tmp_i.indexname[tmp_i.num - 1];
	tmp_i.location[pos] = tmp_i.location[tmp_i.num - 1];
	tmp_i.num--;
	Drop_table(name);
	Create_table(name, attr_a, attr_a.primary_key, tmp_i);
}

void Catalog_manager::Display_table(string name) {
	if (Has_table(name) == 0) {
		//throw Table_nonexist();
	}
	cout << "Table name: " << name << endl;
	Attribute attr = Get_attr(name);
	Index ind = Get_index(name);
	int maxn = -1;
	for (int i = 0; i < attr.num; i++) {
		if (attr.name[i].length() > maxn)
			maxn = attr.name[i].length();
	}
	string type;
	cout << "Attribute:" << endl;
	cout << "Num|" << "Name " << setw(maxn) << "|Type" << type << setw(6) <<
		"|" << "Unique|Primary Key" << endl;
	for (int i = 0; i < maxn + 35; i++)
		cout << "-";
	cout << endl;
	for (int i = 0; i < attr.num; i++) {
		if (attr.type[i] == -1) {
			type = "int";
		}
		else if (attr.type[i] == 0) {
			type = "float";
		}
		else
			type = "char(" + Num2Str(attr.type[i] - 1, 3) + ")";
			cout << i << setw(3 - i / 10) << "|" << attr.name[i] << setw(maxn - attr.name[i].length() + 2) << "|" << type << setw(10 - type.length()) << "|";
			if (attr.unique[i])
				cout << "unique" << "|";
			else
				cout << setw(7) << "|";
		if (attr.primary_key == i)
			cout << "primary key";
		cout << endl;
	}

	for (int i = 0; i < maxn + 35; i++)
		cout << "-";

	cout << endl;
	cout << "Index: " << endl;
	cout << "Num|Location|Name" << endl;
	maxn = -1;
	for (int i = 0; i < ind.num; i++) {
		if (ind.indexname[i].length() > maxn)
			maxn = ind.indexname[i].length();
	}
	for (int i = 0; i < ((maxn + 14) > 18 ? (maxn + 14) : 18); i++)
		cout << "-";
	cout << endl;
	for (int i = 0; i < ind.num; i++) {
		cout << i << setw(3 - i / 10) << "|" << ind.location[i] << setw(8 - ind.location[i] / 10)
			<< "|" << ind.indexname[i] << endl;
	}
	for (int i = 0; i < ((maxn + 14) > 18 ? (maxn + 14) : 18); i++)
		cout << "-";
	cout << endl;
}
bool Catalog_manager::Has_table(std::string name) {
	  

	int blo_num = Get_size(TABLE_MANAGER_PATH) / Pagesize;
	if (blo_num <= 0)
		blo_num = 1;

	for (int i = 0; i < blo_num; i++) {
		char* buffer = buffer_manager.getPage(TABLE_MANAGER_PATH, i);
		std::string buffer_check(buffer);
		std::string str_tmp = "";
		int start_index = 0, end_index = 0;
		do {
			if (buffer_check[0] == '#')
				break;

			else if (Get_name(buffer, start_index, end_index) == name) {
				return true;
			}
			else {

				start_index += Str2Num(buffer_check.substr(start_index, 4));

				if (!start_index)
					break;
			}
		} while (buffer_check[start_index] != '#');
	}
	return false;
}

string Catalog_manager::Num2Str(int num, int bit) {
	string str; str.clear();
	if (num < 0) {
		num = -num;
		str += '-';
	}
	int div = pow(10, bit - 1);
	for (int i = 0; i < bit; i++) {
		str += num / div + '0';
		num %= div;
		div /= 10;
	}
	return str;
}

int Catalog_manager::Str2Num(string str) {
	return atoi(str.c_str());
}

string Catalog_manager::Get_name(string buf, int st, int &ed) {
	string str; str.clear();
	ed = 0;
	if (buf.empty()) return buf;
	while (buf[st + ed + 5] != ' ') ed++;
	str = buf.substr(st + 5, ed);
	ed = st + 5 + ed;
	return str;
}

int Catalog_manager::Get_place(string name, int &blo) {
	  
	int num = Get_size(TABLE_MANAGER_PATH);
	if (num <= 0)
		num = 1;
	for (blo = 0; blo < num; blo++) {
		char *buf = buffer_manager.getPage(TABLE_MANAGER_PATH, blo);
		string buffer_check(buf);
		string str; str.clear();
		int st = 0, ed = 0;
		while (buffer_check[st] != '#') {
			if (buffer_check[0] == '#')
				break;
			if (Get_name(buf, st, ed) == name) {
				return st;
			}
			else {
				st += Str2Num(buffer_check.substr(st, 4));
				if (!st)
					break;
			}
		}
	}
	return -1;
}

Index Catalog_manager::Get_index(string name) {
	  
	Index ind;
	int blo;
	int st = Get_place(name, blo);
	char* buf = buffer_manager.getPage(TABLE_MANAGER_PATH, blo);
	string buffer_check(buf);
	while (buffer_check[st] != ';')
		st++;
	st++;
	ind.num = Str2Num(buffer_check.substr(st, 2));
	for (int i = 0; i < ind.num; i++) {
		st += 3;
		ind.location[i] = Str2Num(buffer_check.substr(st, 2));
		st += 3;
		while (buffer_check[st] != ' ' && buffer_check[st] != '#' && buffer_check[st] != '\n') {
			ind.indexname[i] += buffer_check[st++];
		}
		st -= 2;
	}
	return ind;
}

int Catalog_manager::Get_size(string name) {
	  
	char *str;
	int blo = -1;
	while (1) {
		str = buffer_manager.getPage(name, blo + 1);
		blo++;
		if (str[0] == '\0') break;
	}
	return blo;
}



