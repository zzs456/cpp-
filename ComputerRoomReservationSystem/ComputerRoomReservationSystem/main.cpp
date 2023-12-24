#include <iostream>
#include "identity.h"
#include "globalFile.h"
#include "manager.h"
#include "student.h"
#include "teacher.h"
#include <fstream>
using namespace std;

// 登录功能		操作文件名		操作身份类型
void LoginIn(string fileName, int type);

// 进入管理员子菜单
void managerMenu(Identity * &manager);

// 进入学生子菜单
void studentMenu(Identity * &student);

// 进入教师子菜单
void teacherMenu(Identity*& teacher);

int main() {

	int select = 0; // 用户选择
	while (true) {
		cout << "----------------------  欢迎来到机房预约系统  ----------------------" << endl;
		cout << "请输入您的身份：" << endl;
		cout << "\t\t ---------------------- \n";
		cout << "\t\t|                      |\n";
		cout << "\t\t|      1.学生代表      |\n";
		cout << "\t\t|                      |\n";
		cout << "\t\t|      2.老    师      |\n";
		cout << "\t\t|                      |\n";
		cout << "\t\t|      3.管 理 员      |\n";
		cout << "\t\t|                      |\n";
		cout << "\t\t|      0.退    出      |\n";
		cout << "\t\t|                      |\n";
		cout << "\t\t ---------------------- \n";
		cout << "请输入您的选择：";
		cin >> select;	// 接受用户选择
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
			cout << "欢迎下一次使用！" << endl;
			exit(0);
			break;
		default:
			cout << "输入有误，请重新输入：" << endl;
			system("pause");
			system("cls");
			break;
		}
	}

	return 0;
}

// 登录功能		操作文件名		操作身份类型
void LoginIn(string fileName, int type) {
	// 父类指针，用于指向子类对象
	Identity* person = NULL;

	// 读文件
	ifstream ifs;
	ifs.open(fileName,ios::in);
	if (!ifs.is_open()) {
		cout << "文件不存在！" << endl;
		ifs.close();
		return;
	}
	// 接受用户信息
	int id = 0;
	string name;
	string pwd;
	// 判断身份
	if (type == 1) {	// 学生
		cout << "请输入您的学号：" << endl;
		cin >> id;
	}
	else if (type == 2) {
		cout << "请输入您的职工号：" << endl;
		cin >> id;
	}
	cout << "请输入用户名：" << endl;
	cin >> name;
	cout << "请输入密码：" << endl;
	cin >> pwd;
	if (type == 1) {
		// 学生身份验证
		// 文件中的信息
		int fId;
		string fName;
		string fPwd;
		while (ifs >> fId && ifs >> fName && ifs >> fPwd) {
			// 对比用户输入
			if (fId == id && fName == name && fPwd == pwd) {
				cout << "学生验证登录成功！" << endl;
				system("pause");
				system("cls");
				person = new Student(id,name,pwd);
				// 进入学生子菜单
				studentMenu(person);
				return;
			}
		}
	}
	else if (type == 2) {
		// 教师身份验证
		// 文件中的信息
		int fId;
		string fName;
		string fPwd;
		while (ifs >> fId && ifs >> fName && ifs >> fPwd) {
			// 对比用户输入
			if (fId == id && fName == name && fPwd == pwd) {
				cout << "教师验证登录成功！" << endl;
				system("pause");
				system("cls");
				person = new Teacher(id, name, pwd);
				// 进入教师子菜单
				teacherMenu(person);
				return;
			}
		}
	}
	else if(type == 3){
		// 管理员身份验证
		// 文件中的信息
		string fName;
		string fPwd;
		while (ifs >> fName && ifs >> fPwd) {
			// 对比用户输入
			if (fName == name && fPwd == pwd) {
				cout << "管理员验证登录成功！" << endl;
				system("pause");
				system("cls");
				person = new Manager(name, pwd);
				// 进入管理员子菜单
				managerMenu(person);
				return;
			}
		}
	}

	cout << "验证登录失败！" << endl;
	system("pause");
	system("cls");
}

// 进入管理员子菜单
void managerMenu(Identity*& manager) {
	while (true) {
		// 调用管理员子菜单
		manager->openMenu();

		// 将父类指针转为子类指针，调用子类里其他接口
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
			cout << "注销登录！" << endl;
			system("pause");
			system("cls");
			return;
		}
	}
}

// 进入学生子菜单
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
			cout << "注销成功！" << endl;
			system("pause");
			system("cls");
			return;
		}
	}
}

// 进入教师子菜单
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
			cout << "注销成功！" << endl;
			system("pause");
			system("cls");
			return;
		}
	}
}