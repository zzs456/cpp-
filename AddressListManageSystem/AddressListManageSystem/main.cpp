#include <iostream>
using namespace std;
#define MAX 1000

// �����ϵ�˽ṹ��
struct Person {
	// ����
	string m_Name;
	// �Ա�
	int m_Sex;
	// ����
	int m_Age;
	// �绰
	string m_Phone;
	// סַ
	string m_Addr;
};

// ���ͨѶ¼�ṹ��
struct AddressBooks {
	// ��ϵ������
	Person personArray[MAX];
	// ��ϵ�˸���
	int m_Size;
};

// �����ϵ��
void addPerson(AddressBooks *abs) {
	// ͨѶ¼���������������
	if (abs->m_Size == MAX) {
		cout << "ͨѶ¼�������޷���ӣ�" << endl;
	}
	else {
		// ��Ӿ������ϵ��
		// ����
		string name;
		cout << "������������" << endl;
		cin >> name;
		abs->personArray[abs->m_Size].m_Name = name;
		// �Ա�
		int sex;
		cout << "�������Ա�" << endl;
		cout << "1 --- ��" << endl;
		cout << "2 --- Ů" << endl;
		while (true) {
			cin >> sex;
			if (sex == 1 || sex == 2) {
				abs->personArray[abs->m_Size].m_Sex = sex;
				break;
			}
			else{
				cout << "�����������������룡" << endl;
			}
		}
		// ����
		int age;
		cout << "���������䣺" << endl;
		cin >> age;
		abs->personArray[abs->m_Size].m_Age = age;
		// �绰
		string phone;
		cout << "������绰��" << endl;
		cin >> phone;
		abs->personArray[abs->m_Size].m_Phone = phone;
		// סַ
		string address;
		cout << "�������ͥסַ��" << endl;
		cin >> address;
		abs->personArray[abs->m_Size].m_Addr = address;

		cout << "��ӳɹ���" << endl;
		
		// ��������
		abs->m_Size ++;
		system("pause");	// �����������
		system("cls");		// ����
	}
}

// ��ʾ��ϵ��
void showPerson(AddressBooks *abs) {
	if (abs->m_Size == 0) {
		cout << "��ǰ��¼Ϊ�գ�" << endl;
	}
	else {
		for (int i = 0; i < abs->m_Size;i ++) {
			cout << "������" << abs->personArray[i].m_Name << "\t";
			cout << "�Ա�" << (abs->personArray[i].m_Sex == 1 ? "��" : "Ů") << "\t";
			cout << "���䣺" << abs->personArray[i].m_Age << "\t";
			cout << "�绰��" << abs->personArray[i].m_Phone << "\t";
			cout << "סַ��" << abs->personArray[i].m_Addr << endl;
		}
	}
	system("pause");	// �����������
	system("cls");		// ����
}

// �����ϵ���Ƿ����
int isExit(AddressBooks* abs,string name) {
	for (int i = 0; i < abs->m_Size;i ++) {
		if (abs->personArray[i].m_Name == name) {
			return i;
		}
	}
	return -1;
}

// ɾ��ָ����ϵ��
void deletePerson(AddressBooks* abs) {
	string name;
	cout << "��������Ҫɾ������ϵ�ˣ�" << endl;
	cin >> name;
	int ret = isExit(abs,name);
	if (ret != -1) {
		for (int i = ret; i < abs->m_Size;i ++) {
			abs->personArray[i] = abs->personArray[i + 1];
		}
		cout << "ɾ���ɹ���" << endl;
		abs->m_Size --;
	}
	else {
		cout << "���޴��ˣ�" << endl;
	}
	system("pause");	// �����������
	system("cls");		// ����
}

// ����ָ������ϵ��
void findPerson(AddressBooks *abs) {
	string name;
	cout << "������Ҫ���ҵ���ϵ�ˣ�" << endl;
	cin >> name;
	int ret = isExit(abs, name);
	if (ret != -1) {
		cout << "������" << abs->personArray[ret].m_Name << "\t";
		cout << "�Ա�" << (abs->personArray[ret].m_Sex == 1 ? "��" : "Ů") << "\t";
		cout << "���䣺" << abs->personArray[ret].m_Age << "\t";
		cout << "�绰��" << abs->personArray[ret].m_Phone << "\t";
		cout << "סַ��" << abs->personArray[ret].m_Addr << endl;
	}
	else {
		cout << "���޴��ˣ�" << endl;
	}
	system("pause");	// �����������
	system("cls");		// ����
}

//�޸���ϵ��
void modifyPerson(AddressBooks* abs) {
	string name;
	cout << "������Ҫ�޸ĵ���ϵ�ˣ�" << endl;
	cin >> name;
	int ret = isExit(abs, name);
	if (ret != -1) {
		// ����
		string name;
		cout << "������������" << endl;
		cin >> name;
		abs->personArray[ret].m_Name = name;
		// �Ա�
		int sex;
		cout << "�������Ա�" << endl;
		cout << "1 --- ��" << endl;
		cout << "2 --- Ů" << endl;
		while (true) {
			cin >> sex;
			if (sex == 1 || sex == 2) {
				abs->personArray[ret].m_Sex = sex;
				break;
			}
			else {
				cout << "�����������������룡" << endl;
			}
		}
		// ����
		int age;
		cout << "���������䣺" << endl;
		cin >> age;
		abs->personArray[ret].m_Age = age;
		// �绰
		string phone;
		cout << "������绰��" << endl;
		cin >> phone;
		abs->personArray[ret].m_Phone = phone;
		// סַ
		string address;
		cout << "�������ͥסַ��" << endl;
		cin >> address;
		abs->personArray[ret].m_Addr = address;

		cout << "�޸ĳɹ���" << endl;
	}
	else {
		cout << "���޴��ˣ�" << endl;
	}
	system("pause");	// �����������
	system("cls");		// ����
}

// �����ϵ��
void cleanPerson(AddressBooks* abs) {
	abs->m_Size = 0;
	cout << "ͨѶ¼����գ�" << endl;
	system("pause");	// �����������
	system("cls");		// ����
}

// �˵�����
void showMenu() {
	cout << "***************************" << endl;
	cout << "*****  1�������ϵ��  *****" << endl;
	cout << "*****  2����ʾ��ϵ��  *****" << endl;
	cout << "*****  3��ɾ����ϵ��  *****" << endl;
	cout << "*****  4��������ϵ��  *****" << endl;
	cout << "*****  5���޸���ϵ��  *****" << endl;
	cout << "*****  6�������ϵ��  *****" << endl;
	cout << "*****  0���˳�ͨѶ¼  *****" << endl;
	cout << "***************************" << endl;
}

int main() {

	// ����ͨѶ¼�ṹ�����
	AddressBooks abs;
	// ��ʼ����ϵ�˸���Ϊ 0
	abs.m_Size = 0;

	int select = 0;			// �����û�ѡ������ı���
	while (true){
		showMenu();
		cin >> select;
		switch (select) {
		case 1:		// �����ϵ��
			addPerson(&abs);
			break;
		case 2:		// ��ʾ��ϵ��
			showPerson(&abs);
			break;
		case 3:		// ɾ����ϵ��
			deletePerson(&abs);
			break;
		case 4:		// ������ϵ��
			findPerson(&abs);
			break;
		case 5:		// �޸���ϵ��
			modifyPerson(&abs);
			break;
		case 6:		// �����ϵ��
			cleanPerson(&abs);
			break;
		case 0:		// �˳�ͨѶ¼
			cout << "��ӭ�´�ʹ�ã�" << endl;
			system("pause");
			return 0;
		default:
			break;
		}
	}
	system("pause");
	return 0;
}