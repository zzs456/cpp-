#pragma once
#include <iostream>
#include "identity.h"
#include "globalFile.h"
#include "student.h"
#include "teacher.h"
#include "computerRoom.h"
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

class Manager :public Identity {
public:
	// �޲ι���
	Manager();
	// �вι���
	Manager(string name, string pwd);
	// �˵�����
	virtual void openMenu();
	// ����˺�
	void addPerson();
	// �鿴�˺�
	void showPerson();
	// �鿴������Ϣ
	void showComputer();
	// ���ԤԼ��¼
	void cleanFile();
	// ��ʼ������
	void initVector();
	// ѧ������
	vector<Student> vStu;
	// ��ʦ����
	vector<Teacher> vTea;
	// ��������
	vector<ComputerRoom> vCom;
	// ����ظ�
	bool checkRepeat(int id, int type);
};