#include "workerManager.h"

WorkManager::WorkManager() {

	ifstream ifs;
	ifs.open(FILENAME,ios::in);
	// �ļ�������
	if (!ifs.is_open()) {
		this->m_EmpNum = 0;
		this->m_EmpArray = NULL;
		this->m_FileIsEmpty = true;
		return;
	}

	// �ļ����ڣ�����Ϊ��
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
	// ���ļ��е����ݴ浽������
	this->init_Emp();
}

void WorkManager::Show_Menu() {
	cout << "********************************************" << endl;
	cout << "*********  ��ӭʹ��ְ������ϵͳ��  *********" << endl;
	cout << "*************  0.�˳��������  *************" << endl;
	cout << "*************  1.����ְ����Ϣ  *************" << endl;
	cout << "*************  2.��ʾְ����Ϣ  *************" << endl;
	cout << "*************  3.ɾ����ְְ��  *************" << endl;
	cout << "*************  4.�޸�ְ����Ϣ  *************" << endl;
	cout << "*************  5.����ְ����Ϣ  *************" << endl;
	cout << "*************  6.���ձ������  *************" << endl;
	cout << "*************  7.��������ĵ�  *************" << endl;
	cout << "********************************************" << endl;
	cout << endl;
}

void WorkManager::Add_Emp() {
	cout << "���������ְ��������" << endl;
	int addNum = 0;
	cin >> addNum;
	if(addNum > 0){
		int newSize = this->m_EmpNum + addNum;
		// �����¿ռ�
		Worker** newSpace = new Worker * [newSize];

		// ��ԭ�������ݿ������¿ռ���
		if (this->m_EmpArray != NULL) {
			for (int i = 0; i < this->m_EmpNum;i ++) {
				newSpace[i] = this->m_EmpArray[i];
			}
		}

		// �������������
		for (int i = 0; i < addNum;i ++) {
			int id;
			string name;
			int dSelect;
			cout << "������� " << i + 1 << " ����ְ����ţ�" << endl;
			cin >> id;
			cout << "������� " << i + 1 << " ����ְ��������" << endl;
			cin >> name;			
			cout << "��ѡ���ְ����λ��" << endl;
			cout << "1��Ա��" << endl;
			cout << "2������" << endl;
			cout << "3���ܲ�" << endl;
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
			// ��ӵ��¿ռ���
			newSpace[this->m_EmpNum + i] = worker;
		}
		// �ͷ�ԭ�пռ�
		delete[] this->m_EmpArray;
		// �����¿ռ�ָ��
		this->m_EmpArray = newSpace;
		// ����ְ������
		this->m_EmpNum = newSize;
		cout << "�ɹ���� " << addNum << " ����ְ����" << endl;
		this->m_FileIsEmpty = false;
		this->save();
	}else{
		cout << "������������" << endl;
	}
	system("pause");
	system("cls");
}

void WorkManager::ExitSystem() {
	cout << "��ӭ�´�ʹ�ã�" << endl;
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
		cout << "�ļ������ڻ��¼Ϊ�գ�" << endl;
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
		cout << "�ļ������ڻ��¼Ϊ�գ�" << endl;
	}
	else {
		cout << "��������Ҫɾ����ְ����ţ�" << endl;
		int id;
		cin >> id;
		int index = this->IsExist(id);
		if (index != -1) {
			for (int i = index; i < this->m_EmpNum - 1;i ++) {
				this->m_EmpArray[i] = this->m_EmpArray[i + 1];
			}
			this->m_EmpNum --;
			cout << "ɾ���ɹ���" << endl;
		}else{
			cout << "ɾ��ʧ�ܣ�δ�ҵ���ְ����" << endl;
		}
	}
	system("pause");
	system("cls");
}

void WorkManager::Mod_Emp() {
	if (this->m_FileIsEmpty) {
		cout << "�ļ������ڻ��¼Ϊ�գ�" << endl;
	}
	else {
		cout << "��������Ҫ�޸ĵ�ְ����ţ�" << endl;
		int id;
		cin >> id;
		int index = this->IsExist(id);
		if (index != -1) {
			delete this->m_EmpArray[index];
			int newId;
			string newName;
			int newDId;
			cout << "�鵽��" << id << " ��ְ������������ְ���ţ�" << endl;
			cin >> newId;
			cout << "��������������" << endl;
			cin >> newName;
			cout << "��ѡ���ְ����λ��" << endl;
			cout << "1��Ա��" << endl;
			cout << "2������" << endl;
			cout << "3���ܲ�" << endl;
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
			cout << "�޸ĳɹ���" << endl;
			this->save();
		}
		else {
			cout << "�޸�ʧ�ܣ�δ�ҵ���ְ����" << endl;
		}
	}
	system("pause");
	system("cls");
}

void WorkManager::Find_Emp() {
	if (this->m_FileIsEmpty) {
		cout << "�ļ������ڻ��¼Ϊ�գ�" << endl;
	}
	else {
		cout << "��������ҵķ�ʽ��" << endl;
		cout << "1����ְ����Ų���" << endl;
		cout << "2����ְ����������" << endl;
		int select;
		cin >> select;
		if (select == 1) {
			cout << "��������Ҫ���ҵ�ְ����ţ�" << endl;
			int id;
			cin >> id;
			int index = this->IsExist(id);
			if (index != -1) {
				cout << "���ҳɹ�����ְ����Ϣ���£�" << endl;
				this->m_EmpArray[index]->showInfo();
			}
			else {
				cout << "����ʧ�ܣ�δ�ҵ���ְ����" << endl;
			}
		}
		else if (select == 2) {
			cout << "��������Ҫ���ҵ�ְ��������" << endl;
			string name;
			cin >> name;
			bool flag = false;
			for (int i = 0; i < this->m_EmpNum;i ++) {
				if (this->m_EmpArray[i]->m_Name == name) {
					flag = true;
					cout << "���ҳɹ���" << this->m_EmpArray[i]->m_Id << " ��ְ����Ϣ���£�" << endl;
					this->m_EmpArray[i]->showInfo();
				}
			}
			if (!flag) {
				cout << "����ʧ�ܣ�δ�ҵ���ְ����" << endl;
			}
		}
		else {
			cout << "����ѡ������" << endl;
		}
	}
	system("pause");
	system("cls");
}

void WorkManager::Sort_Emp() {
	if (this->m_FileIsEmpty) {
		cout << "�ļ������ڻ��¼Ϊ�գ�" << endl;
		system("pause");
		system("cls");
	}
	else {
		cout << "��ѡ������ʽ��" << endl;
		cout << "1����ְ���Ž�������" << endl;
		cout << "2����ְ���Ž��н���" << endl;
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
		cout << "����ɹ��������Ľ��Ϊ��" << endl;
		this->save();
		this->Show_Emp();
	}
}

// ���
void WorkManager::Clean_Emp() {
	cout << "ȷ����գ�" << endl;
	cout << "1��ȷ��" << endl;
	cout << "2������" << endl;
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
	cout << "��ճɹ���" << endl;
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