#pragma once
#include <iostream>
#include "identity.h"
#include "computerRoom.h"
#include "globalFile.h"
#include "orderFile.h"
#include <vector> 
#include <fstream>
using namespace std;

class Student:public Identity {
public:
	// ѧ��ѧ��
	int m_Id;
	// �޲ι���
	Student();
	// �вι���
	Student(int id,string name,string pwd);
	// �˵�����
	virtual void openMenu();
	// ����ԤԼ
	void applyOrder();
	// �鿴����ԤԼ
	void showMyOrder();
	// �鿴����ԤԼ
	void showAllOrder();
	// ȡ��ԤԼ
	void cancelOrder();
	// ������Ϣ
	vector<ComputerRoom> vCom;
};