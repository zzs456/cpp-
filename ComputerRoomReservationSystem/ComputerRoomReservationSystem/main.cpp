#include <iostream>
#include "identity.h"
#include "globalFile.h"
#include "manager.h"
#include "student.h"
#include "teacher.h"
#include <fstream>
using namespace std;

// ��¼����		�����ļ���		�����������
void LoginIn(string fileName, int type);

// �������Ա�Ӳ˵�
void managerMenu(Identity * &manager);

// ����ѧ���Ӳ˵�
void studentMenu(Identity * &student);

// �����ʦ�Ӳ˵�
void teacherMenu(Identity*& teacher);

int main() {

	int select = 0; // �û�ѡ��
	while (true) {
		cout << "----------------------  ��ӭ��������ԤԼϵͳ  ----------------------" << endl;
		cout << "������������ݣ�" << endl;
		cout << "\t\t ---------------------- \n";
		cout << "\t\t|                      |\n";
		cout << "\t\t|      1.ѧ������      |\n";
		cout << "\t\t|                      |\n";
		cout << "\t\t|      2.��    ʦ      |\n";
		cout << "\t\t|                      |\n";
		cout << "\t\t|      3.�� �� Ա      |\n";
		cout << "\t\t|                      |\n";
		cout << "\t\t|      0.��    ��      |\n";
		cout << "\t\t|                      |\n";
		cout << "\t\t ---------------------- \n";
		cout << "����������ѡ��";
		cin >> select;	// �����û�ѡ��
		switch (select)	
		{
		case 1:
			LoginIn(STUDENT_FILE,1);
			break;
		case 2:
			LoginIn(TEACHER_FILE,2);
			break;
		case 3:
			LoginIn(ADMIN_FILE,3);
			break;
		case 0:
			cout << "��ӭ��һ��ʹ�ã�" << endl;
			exit(0);
			break;
		default:
			cout << "�����������������룺" << endl;
			system("pause");
			system("cls");
			break;
		}
	}

	return 0;
}

// ��¼����		�����ļ���		�����������
void LoginIn(string fileName, int type) {
	// ����ָ�룬����ָ���������
	Identity* person = NULL;

	// ���ļ�
	ifstream ifs;
	ifs.open(fileName,ios::in);
	if (!ifs.is_open()) {
		cout << "�ļ������ڣ�" << endl;
		ifs.close();
		return;
	}
	// �����û���Ϣ
	int id = 0;
	string name;
	string pwd;
	// �ж����
	if (type == 1) {	// ѧ��
		cout << "����������ѧ�ţ�" << endl;
		cin >> id;
	}
	else if (type == 2) {
		cout << "����������ְ���ţ�" << endl;
		cin >> id;
	}
	cout << "�������û�����" << endl;
	cin >> name;
	cout << "���������룺" << endl;
	cin >> pwd;
	if (type == 1) {
		// ѧ�������֤
		// �ļ��е���Ϣ
		int fId;
		string fName;
		string fPwd;
		while (ifs >> fId && ifs >> fName && ifs >> fPwd) {
			// �Ա��û�����
			if (fId == id && fName == name && fPwd == pwd) {
				cout << "ѧ����֤��¼�ɹ���" << endl;
				system("pause");
				system("cls");
				person = new Student(id,name,pwd);
				// ����ѧ���Ӳ˵�
				studentMenu(person);
				return;
			}
		}
	}
	else if (type == 2) {
		// ��ʦ�����֤
		// �ļ��е���Ϣ
		int fId;
		string fName;
		string fPwd;
		while (ifs >> fId && ifs >> fName && ifs >> fPwd) {
			// �Ա��û�����
			if (fId == id && fName == name && fPwd == pwd) {
				cout << "��ʦ��֤��¼�ɹ���" << endl;
				system("pause");
				system("cls");
				person = new Teacher(id, name, pwd);
				// �����ʦ�Ӳ˵�
				teacherMenu(person);
				return;
			}
		}
	}
	else if(type == 3){
		// ����Ա�����֤
		// �ļ��е���Ϣ
		string fName;
		string fPwd;
		while (ifs >> fName && ifs >> fPwd) {
			// �Ա��û�����
			if (fName == name && fPwd == pwd) {
				cout << "����Ա��֤��¼�ɹ���" << endl;
				system("pause");
				system("cls");
				person = new Manager(name, pwd);
				// �������Ա�Ӳ˵�
				managerMenu(person);
				return;
			}
		}
	}

	cout << "��֤��¼ʧ�ܣ�" << endl;
	system("pause");
	system("cls");
}

// �������Ա�Ӳ˵�
void managerMenu(Identity*& manager) {
	while (true) {
		// ���ù���Ա�Ӳ˵�
		manager->openMenu();

		// ������ָ��תΪ����ָ�룬���������������ӿ�
		Manager* man = (Manager*)manager;

		int select = 0;
		cin >> select;
		if (select == 1) {
			man->addPerson();
		}
		else if (select == 2) {
			man->showPerson();
		}
		else if (select == 3){
			man->showComputer();
		}
		else if (select == 4) {
			man->cleanFile();
		}
		else {
			delete manager;
			cout << "ע����¼��" << endl;
			system("pause");
			system("cls");
			return;
		}
	}
}

// ����ѧ���Ӳ˵�
void studentMenu(Identity*& student) {
	while (true) {
		student->openMenu();
		Student* stu = (Student*)student;
		int select = 0;
		cin >> select;
		if (select == 1) {
			stu->applyOrder();
		}
		else if (select == 2) {
			stu->showMyOrder();
		}	
		else if (select == 3) {
			stu->showAllOrder();
		}
		else if (select == 4) {
			stu->cancelOrder();
		}
		else {
			delete student;
			cout << "ע���ɹ���" << endl;
			system("pause");
			system("cls");
			return;
		}
	}
}

// �����ʦ�Ӳ˵�
void teacherMenu(Identity*& teacher) {
	while (true) {
		teacher->openMenu();
		Teacher* tea = (Teacher*)teacher;
		int select = 0;
		cin >> select;
		if (select == 1) {
			tea->showAllOrder();
		}
		else if (select == 2) {
			tea->validOrder();
		}
		else {
			delete teacher;
			cout << "ע���ɹ���" << endl;
			system("pause");
			system("cls");
			return;
		}
	}
}