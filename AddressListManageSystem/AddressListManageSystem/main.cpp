#include <iostream>
using namespace std;
#define MAX 1000

// 设计联系人结构体
struct Person {
	// 姓名
	string m_Name;
	// 性别
	int m_Sex;
	// 年龄
	int m_Age;
	// 电话
	string m_Phone;
	// 住址
	string m_Addr;
};

// 设计通讯录结构体
struct AddressBooks {
	// 联系人数组
	Person personArray[MAX];
	// 联系人个数
	int m_Size;
};

// 添加联系人
void addPerson(AddressBooks *abs) {
	// 通讯录已满，则不能再添加
	if (abs->m_Size == MAX) {
		cout << "通讯录已满，无法添加！" << endl;
	}
	else {
		// 添加具体的联系人
		// 姓名
		string name;
		cout << "请输入姓名：" << endl;
		cin >> name;
		abs->personArray[abs->m_Size].m_Name = name;
		// 性别
		int sex;
		cout << "请输入性别：" << endl;
		cout << "1 --- 男" << endl;
		cout << "2 --- 女" << endl;
		while (true) {
			cin >> sex;
			if (sex == 1 || sex == 2) {
				abs->personArray[abs->m_Size].m_Sex = sex;
				break;
			}
			else{
				cout << "输入有误，请重新输入！" << endl;
			}
		}
		// 年龄
		int age;
		cout << "请输入年龄：" << endl;
		cin >> age;
		abs->personArray[abs->m_Size].m_Age = age;
		// 电话
		string phone;
		cout << "请输入电话：" << endl;
		cin >> phone;
		abs->personArray[abs->m_Size].m_Phone = phone;
		// 住址
		string address;
		cout << "请输入家庭住址：" << endl;
		cin >> address;
		abs->personArray[abs->m_Size].m_Addr = address;

		cout << "添加成功！" << endl;
		
		// 更新人数
		abs->m_Size ++;
		system("pause");	// 按任意键继续
		system("cls");		// 清屏
	}
}

// 显示联系人
void showPerson(AddressBooks *abs) {
	if (abs->m_Size == 0) {
		cout << "当前记录为空！" << endl;
	}
	else {
		for (int i = 0; i < abs->m_Size;i ++) {
			cout << "姓名：" << abs->personArray[i].m_Name << "\t";
			cout << "性别：" << (abs->personArray[i].m_Sex == 1 ? "男" : "女") << "\t";
			cout << "年龄：" << abs->personArray[i].m_Age << "\t";
			cout << "电话：" << abs->personArray[i].m_Phone << "\t";
			cout << "住址：" << abs->personArray[i].m_Addr << endl;
		}
	}
	system("pause");	// 按任意键继续
	system("cls");		// 清屏
}

// 检测联系人是否存在
int isExit(AddressBooks* abs,string name) {
	for (int i = 0; i < abs->m_Size;i ++) {
		if (abs->personArray[i].m_Name == name) {
			return i;
		}
	}
	return -1;
}

// 删除指定联系人
void deletePerson(AddressBooks* abs) {
	string name;
	cout << "请输入您要删除的联系人：" << endl;
	cin >> name;
	int ret = isExit(abs,name);
	if (ret != -1) {
		for (int i = ret; i < abs->m_Size;i ++) {
			abs->personArray[i] = abs->personArray[i + 1];
		}
		cout << "删除成功！" << endl;
		abs->m_Size --;
	}
	else {
		cout << "查无此人！" << endl;
	}
	system("pause");	// 按任意键继续
	system("cls");		// 清屏
}

// 查找指定的联系人
void findPerson(AddressBooks *abs) {
	string name;
	cout << "请输入要查找的联系人：" << endl;
	cin >> name;
	int ret = isExit(abs, name);
	if (ret != -1) {
		cout << "姓名：" << abs->personArray[ret].m_Name << "\t";
		cout << "性别：" << (abs->personArray[ret].m_Sex == 1 ? "男" : "女") << "\t";
		cout << "年龄：" << abs->personArray[ret].m_Age << "\t";
		cout << "电话：" << abs->personArray[ret].m_Phone << "\t";
		cout << "住址：" << abs->personArray[ret].m_Addr << endl;
	}
	else {
		cout << "查无此人！" << endl;
	}
	system("pause");	// 按任意键继续
	system("cls");		// 清屏
}

//修改联系人
void modifyPerson(AddressBooks* abs) {
	string name;
	cout << "请输入要修改的联系人：" << endl;
	cin >> name;
	int ret = isExit(abs, name);
	if (ret != -1) {
		// 姓名
		string name;
		cout << "请输入姓名：" << endl;
		cin >> name;
		abs->personArray[ret].m_Name = name;
		// 性别
		int sex;
		cout << "请输入性别：" << endl;
		cout << "1 --- 男" << endl;
		cout << "2 --- 女" << endl;
		while (true) {
			cin >> sex;
			if (sex == 1 || sex == 2) {
				abs->personArray[ret].m_Sex = sex;
				break;
			}
			else {
				cout << "输入有误，请重新输入！" << endl;
			}
		}
		// 年龄
		int age;
		cout << "请输入年龄：" << endl;
		cin >> age;
		abs->personArray[ret].m_Age = age;
		// 电话
		string phone;
		cout << "请输入电话：" << endl;
		cin >> phone;
		abs->personArray[ret].m_Phone = phone;
		// 住址
		string address;
		cout << "请输入家庭住址：" << endl;
		cin >> address;
		abs->personArray[ret].m_Addr = address;

		cout << "修改成功！" << endl;
	}
	else {
		cout << "查无此人！" << endl;
	}
	system("pause");	// 按任意键继续
	system("cls");		// 清屏
}

// 清空联系人
void cleanPerson(AddressBooks* abs) {
	abs->m_Size = 0;
	cout << "通讯录已清空！" << endl;
	system("pause");	// 按任意键继续
	system("cls");		// 清屏
}

// 菜单界面
void showMenu() {
	cout << "***************************" << endl;
	cout << "*****  1、添加联系人  *****" << endl;
	cout << "*****  2、显示联系人  *****" << endl;
	cout << "*****  3、删除联系人  *****" << endl;
	cout << "*****  4、查找联系人  *****" << endl;
	cout << "*****  5、修改联系人  *****" << endl;
	cout << "*****  6、清空联系人  *****" << endl;
	cout << "*****  0、退出通讯录  *****" << endl;
	cout << "***************************" << endl;
}

int main() {

	// 创建通讯录结构体变量
	AddressBooks abs;
	// 初始化联系人个数为 0
	abs.m_Size = 0;

	int select = 0;			// 创建用户选择输入的变量
	while (true){
		showMenu();
		cin >> select;
		switch (select) {
		case 1:		// 添加联系人
			addPerson(&abs);
			break;
		case 2:		// 显示联系人
			showPerson(&abs);
			break;
		case 3:		// 删除联系人
			deletePerson(&abs);
			break;
		case 4:		// 查找联系人
			findPerson(&abs);
			break;
		case 5:		// 修改联系人
			modifyPerson(&abs);
			break;
		case 6:		// 清空联系人
			cleanPerson(&abs);
			break;
		case 0:		// 退出通讯录
			cout << "欢迎下次使用！" << endl;
			system("pause");
			return 0;
		default:
			break;
		}
	}
	system("pause");
	return 0;
}