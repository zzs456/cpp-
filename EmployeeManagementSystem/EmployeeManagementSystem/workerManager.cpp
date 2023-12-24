#include "workerManager.h"

WorkManager::WorkManager() {

	ifstream ifs;
	ifs.open(FILENAME,ios::in);
	// 文件不存在
	if (!ifs.is_open()) {
		this->m_EmpNum = 0;
		this->m_EmpArray = NULL;
		this->m_FileIsEmpty = true;
		return;
	}

	// 文件存在，数据为空
	char ch;
	ifs >> ch;
	if (ifs.eof()) {
		this->m_EmpNum = 0;
		this->m_EmpArray = NULL;
		this->m_FileIsEmpty = true;
		ifs.close();
		return;
	}
	this->m_EmpNum = this->get_EmpNum();
	this->m_EmpArray = new Worker*[this->m_EmpNum];
	// 将文件中的数据存到数组中
	this->init_Emp();
}

void WorkManager::Show_Menu() {
	cout << "********************************************" << endl;
	cout << "*********  欢迎使用职工管理系统！  *********" << endl;
	cout << "*************  0.退出管理程序  *************" << endl;
	cout << "*************  1.增加职工信息  *************" << endl;
	cout << "*************  2.显示职工信息  *************" << endl;
	cout << "*************  3.删除离职职工  *************" << endl;
	cout << "*************  4.修改职工信息  *************" << endl;
	cout << "*************  5.查找职工信息  *************" << endl;
	cout << "*************  6.按照编号排序  *************" << endl;
	cout << "*************  7.清空所有文档  *************" << endl;
	cout << "********************************************" << endl;
	cout << endl;
}

void WorkManager::Add_Emp() {
	cout << "请输入添加职工数量：" << endl;
	int addNum = 0;
	cin >> addNum;
	if(addNum > 0){
		int newSize = this->m_EmpNum + addNum;
		// 开辟新空间
		Worker** newSpace = new Worker * [newSize];

		// 将原来的数据拷贝到新空间下
		if (this->m_EmpArray != NULL) {
			for (int i = 0; i < this->m_EmpNum;i ++) {
				newSpace[i] = this->m_EmpArray[i];
			}
		}

		// 批量添加新数据
		for (int i = 0; i < addNum;i ++) {
			int id;
			string name;
			int dSelect;
			cout << "请输入第 " << i + 1 << " 个新职工编号：" << endl;
			cin >> id;
			cout << "请输入第 " << i + 1 << " 个新职工姓名：" << endl;
			cin >> name;			
			cout << "请选择该职工岗位：" << endl;
			cout << "1、员工" << endl;
			cout << "2、经理" << endl;
			cout << "3、总裁" << endl;
			cin >> dSelect;
			Worker* worker = NULL;
			switch (dSelect) {
			case 1:
				worker = new Employee(id,name,1);
				break;
			case 2:
				worker = new Manager(id,name,2);
				break;
			case 3:
				worker = new Boss(id,name,3);
				break;
			default:
				break;
			}
			// 添加到新空间中
			newSpace[this->m_EmpNum + i] = worker;
		}
		// 释放原有空间
		delete[] this->m_EmpArray;
		// 更改新空间指向
		this->m_EmpArray = newSpace;
		// 更新职工人数
		this->m_EmpNum = newSize;
		cout << "成功添加 " << addNum << " 名新职工！" << endl;
		this->m_FileIsEmpty = false;
		this->save();
	}else{
		cout << "输入数据有误！" << endl;
	}
	system("pause");
	system("cls");
}

void WorkManager::ExitSystem() {
	cout << "欢迎下次使用！" << endl;
	system("pause");
	exit(0);
}

void WorkManager::save() {
	ofstream ofs;
	ofs.open(FILENAME,ios::out);
	for (int i = 0; i < this->m_EmpNum;i ++) {
		ofs << this->m_EmpArray[i]->m_Id << "\t"
			<< this->m_EmpArray[i]->m_Name << "\t"
			<< this->m_EmpArray[i]->m_DeptId << endl;
	}
	ofs.close();
}

int WorkManager::get_EmpNum() {

	ifstream ifs;
	ifs.open(FILENAME,ios::in);
	int id;
	string name;
	int dId;
	int num = 0;
	while (ifs >> id && ifs >> name && ifs >> dId) {
		num++;
	}
	ifs.close();
	return num;
}

void WorkManager::init_Emp() {
	ifstream ifs;
	ifs.open(FILENAME, ios::in);
	int id;
	string name;
	int dId;
	int num = 0;
	while (ifs >> id && ifs >> name && ifs >> dId) {
		Worker* worker = NULL;
		if (dId == 1) {
			worker = new Employee(id,name,dId);
		}
		else if (dId == 2) {
			worker = new Manager(id, name, dId);
		}
		else {
			worker = new Boss(id, name, dId);
		}
		this->m_EmpArray[num] = worker;
		num++;
	}
	ifs.close();
}

void WorkManager::Show_Emp() {
	if (this->m_FileIsEmpty) {
		cout << "文件不存在或记录为空！" << endl;
	}
	else {
		for (int i = 0; i < this->m_EmpNum;i ++) {
			this->m_EmpArray[i]->showInfo();
		}
	}
	system("pause");
	system("cls");
}

int WorkManager::IsExist(int id) {
	
	int index = -1;
	for (int i = 0; i < this->m_EmpNum;i ++) {
		if (this->m_EmpArray[i]->m_Id == id) {
			index = i;
			break;
		}
	}
	return index;
}

void WorkManager::Del_Emp() {
	if (this->m_FileIsEmpty) {
		cout << "文件不存在或记录为空！" << endl;
	}
	else {
		cout << "请输入想要删除的职工编号：" << endl;
		int id;
		cin >> id;
		int index = this->IsExist(id);
		if (index != -1) {
			for (int i = index; i < this->m_EmpNum - 1;i ++) {
				this->m_EmpArray[i] = this->m_EmpArray[i + 1];
			}
			this->m_EmpNum --;
			cout << "删除成功！" << endl;
		}else{
			cout << "删除失败，未找到该职工！" << endl;
		}
	}
	system("pause");
	system("cls");
}

void WorkManager::Mod_Emp() {
	if (this->m_FileIsEmpty) {
		cout << "文件不存在或记录为空！" << endl;
	}
	else {
		cout << "请输入想要修改的职工编号：" << endl;
		int id;
		cin >> id;
		int index = this->IsExist(id);
		if (index != -1) {
			delete this->m_EmpArray[index];
			int newId;
			string newName;
			int newDId;
			cout << "查到：" << id << " 号职工，请输入新职工号：" << endl;
			cin >> newId;
			cout << "请输入新姓名：" << endl;
			cin >> newName;
			cout << "请选择该职工岗位：" << endl;
			cout << "1、员工" << endl;
			cout << "2、经理" << endl;
			cout << "3、总裁" << endl;
			cin >> newDId;
			Worker* worker = NULL;
			switch (newDId) {
			case 1:
				worker = new Employee(newId, newName, 1);
				break;
			case 2:
				worker = new Manager(newId, newName, 2);
				break;
			case 3:
				worker = new Boss(newId, newName, 3);
				break;
			default:
				break;
			}
			this->m_EmpArray[index] = worker;
			cout << "修改成功！" << endl;
			this->save();
		}
		else {
			cout << "修改失败，未找到该职工！" << endl;
		}
	}
	system("pause");
	system("cls");
}

void WorkManager::Find_Emp() {
	if (this->m_FileIsEmpty) {
		cout << "文件不存在或记录为空！" << endl;
	}
	else {
		cout << "请输入查找的方式：" << endl;
		cout << "1、按职工编号查找" << endl;
		cout << "2、按职工姓名查找" << endl;
		int select;
		cin >> select;
		if (select == 1) {
			cout << "请输入想要查找的职工编号：" << endl;
			int id;
			cin >> id;
			int index = this->IsExist(id);
			if (index != -1) {
				cout << "查找成功，该职工信息如下：" << endl;
				this->m_EmpArray[index]->showInfo();
			}
			else {
				cout << "查找失败，未找到该职工！" << endl;
			}
		}
		else if (select == 2) {
			cout << "请输入想要查找的职工姓名：" << endl;
			string name;
			cin >> name;
			bool flag = false;
			for (int i = 0; i < this->m_EmpNum;i ++) {
				if (this->m_EmpArray[i]->m_Name == name) {
					flag = true;
					cout << "查找成功，" << this->m_EmpArray[i]->m_Id << " 号职工信息如下：" << endl;
					this->m_EmpArray[i]->showInfo();
				}
			}
			if (!flag) {
				cout << "查找失败，未找到该职工！" << endl;
			}
		}
		else {
			cout << "输入选项有误！" << endl;
		}
	}
	system("pause");
	system("cls");
}

void WorkManager::Sort_Emp() {
	if (this->m_FileIsEmpty) {
		cout << "文件不存在或记录为空！" << endl;
		system("pause");
		system("cls");
	}
	else {
		cout << "请选择排序方式：" << endl;
		cout << "1、按职工号进行升序" << endl;
		cout << "2、按职工号进行降序" << endl;
		int select = 0;
		cin >> select;
		for (int i = 0; i < this->m_EmpNum;i ++) {
			int minOrMax = i;
			for (int j = i + 1; j < this->m_EmpNum;j ++) {
				if (select == 1) {
					if (this->m_EmpArray[j]->m_Id < this->m_EmpArray[minOrMax]->m_Id) {
						minOrMax = j;
					}
				}
				else {
					if (this->m_EmpArray[j]->m_Id > this->m_EmpArray[minOrMax]->m_Id) {
						minOrMax = j;
					}
				}
			}
			if (minOrMax != i) {
				Worker* temp = this->m_EmpArray[i];
				this->m_EmpArray[i] = this->m_EmpArray[minOrMax];
				this->m_EmpArray[minOrMax] = temp;
			}
		}
		cout << "排序成功，排序后的结果为：" << endl;
		this->save();
		this->Show_Emp();
	}
}

// 清空
void WorkManager::Clean_Emp() {
	cout << "确认清空？" << endl;
	cout << "1、确定" << endl;
	cout << "2、返回" << endl;
	int select;
	cin >> select;
	if (select == 1) {
		ofstream ofs(FILENAME,ios::trunc);
		ofs.close();
	}
	if (this->m_EmpArray != NULL) {
		for (int i = 0; i < this->m_EmpNum; i++) {
			if (this->m_EmpArray[i] != NULL) {
				delete this->m_EmpArray[i];
				this->m_EmpArray[i] = NULL;
			}
		}
		delete[] this->m_EmpArray;
		this->m_EmpArray = NULL;
		this->m_EmpNum = 0;
		this->m_FileIsEmpty = true;
	}
	cout << "清空成功！" << endl;
	system("pause");
	system("cls");
}

WorkManager::~WorkManager() {
	if (this->m_EmpArray != NULL){
		for (int i = 0; i < this->m_EmpNum;i ++) {
			if (this->m_EmpArray[i] != NULL) {
				delete this->m_EmpArray[i];
				this->m_EmpArray[i] = NULL;
			}
		}
		delete[] this->m_EmpArray;
		this->m_EmpArray = NULL;
	}
}