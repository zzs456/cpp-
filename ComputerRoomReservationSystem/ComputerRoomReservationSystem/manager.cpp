#include "manager.h"

// �޲ι���
Manager::Manager() {

}

// �вι���
Manager::Manager(string name, string pwd) {
	this->m_Name = name;
	this->m_Pwd = pwd;
	this->initVector();
	// ��ʼ��������Ϣ
	ifstream ifs;
	ifs.open(COMPUTER_FILE,ios::in);
	ComputerRoom com;
	while (ifs >> com.m_ComId && ifs >> com.m_MaxNum) {
		vCom.push_back(com);
	}
	ifs.close();
}

// �˵�����
void Manager::openMenu() {
	cout << "��ӭ����Ա " + this->m_Name << " ��¼��" << endl;
	cout << "\t\t ---------------------- \n";
	cout << "\t\t|                      |\n";
	cout << "\t\t|      1.����˺�      |\n";
	cout << "\t\t|                      |\n";
	cout << "\t\t|      2.�鿴�˺�      |\n";
	cout << "\t\t|                      |\n";
	cout << "\t\t|      3.�鿴����      |\n";
	cout << "\t\t|                      |\n";
	cout << "\t\t|      4.���ԤԼ      |\n";
	cout << "\t\t|                      |\n";
	cout << "\t\t|      0.ע����¼      |\n";
	cout << "\t\t|                      |\n";
	cout << "\t\t ---------------------- \n";
	cout << "����������ѡ��" << endl;
}

// ����˺�
void Manager::addPerson() {
	cout << "����������˺ŵ����ͣ�" << endl;
	cout << "1.���ѧ��" << endl;
	cout << "2.��ӽ�ʦ" << endl;
	string fileName;	// �����ļ���
	string tip;
	string errorTip;
	ofstream ofs;
	int select = 0;
	cin >> select;
	if (select == 1) {
		fileName = STUDENT_FILE;
		tip = "������ѧ�ţ�";
		errorTip = "ѧ���ظ������������룺";
	}
	else {
		fileName = TEACHER_FILE;
		tip = "������ְ����ţ�";
		errorTip = "ְ������ظ������������룺";
	}
	ofs.open(fileName, ios::out | ios::app);
	int id;
	string name;
	string pwd;
	cout << tip << endl;
	while (true) {
		cin >> id;
		bool ret = this->checkRepeat(id,select);
		if (ret) {
			cout << errorTip << endl;
		}
		else {
			break;
		}
	}
	cout << "������������" << endl;
	cin >> name;
	cout << "���������룺" << endl;
	cin >> pwd;

	// ���ļ����������
	ofs << id << " " << name << " " << pwd << " " << endl;
	cout << "��ӳɹ���" << endl;
	system("pause");
	system("cls");
	ofs.close() ;
	this->initVector();
}

// ѧ����Ϣ
void printStudent(Student& s) {
	cout << "ѧ�ţ�" << s.m_Id << " ������" << s.m_Name << endl;
}

// ��ʦ��Ϣ
void printTeacher(Teacher& t) {
	cout << "ְ����ţ�" << t.m_Id << " ������" << t.m_Name << endl;
}

// �鿴�˺�
void Manager::showPerson() {
	cout << "��ѡ��鿴���ݣ�" << endl;
	cout << "1.�鿴����ѧ��" << endl;
	cout << "2.�鿴���н�ʦ" << endl;
	int select = 0;
	cin >> select;
	if (select == 1) {
		cout << "����ѧ����Ϣ���£�" << endl;
		for_each(vStu.begin(),vStu.end(),printStudent);
	}
	else {
		cout << "���н�ʦ��Ϣ���£�" << endl;
		for_each(vTea.begin(), vTea.end(), printTeacher);
	}
	system("pause");
	system("cls");
}

// �鿴������Ϣ
void Manager::showComputer() {
	cout << "������Ϣ���£�" << endl;
	for (auto it = vCom.begin(); it != vCom.end();it ++) {
		cout << "������ţ�" << it->m_ComId << " �������������" << it->m_MaxNum << endl;
	}
	system("pause");
	system("cls");
}

// ���ԤԼ��¼
void Manager::cleanFile() {
	ofstream ofs(ORDER_FILE,ios::trunc);
	ofs.close();
	cout << "��ճɹ���" << endl;
	system("pause");
	system("cls");
}

// ��ʼ������
void Manager::initVector() {
	ifstream ifs;
	vStu.clear();
	vTea.clear();
	ifs.open(STUDENT_FILE,ios::in);
	if (!ifs.is_open()) {
		
	}
	else {
		Student s;
		while (ifs >> s.m_Id && ifs >> s.m_Name && ifs >> s.m_Pwd) {
			vStu.push_back(s);
		}
	}
	ifs.close();
	ifs.open(TEACHER_FILE,ios::in);
	if (!ifs.is_open()) {
		return;
	}
	else {
		Teacher t;
		while (ifs >> t.m_Id && ifs >> t.m_Name && ifs >> t.m_Pwd) {
			vTea.push_back(t);
		}
	}
	ifs.close();
}

// ����ظ�
bool Manager::checkRepeat(int id, int type) {
	if (type == 1) {
		for (auto it = vStu.begin(); it != vStu.end();it ++) {
			if (id == it->m_Id) {
				return true;
			}
		}
	}
	else {
		for (auto it = vTea.begin(); it != vTea.end(); it++) {
			if (id == it->m_Id) {
				return true;
			}
		}
	}
	return false;
}