#pragma once
#include <iostream>
#include "identity.h"
#include "globalFile.h"
#include "orderFile.h"
#include <vector>
using namespace std;

class Teacher :public Identity {
public:
	// ְ����
	int m_Id;
	// �޲ι���
	Teacher();
	// �вι���
	Teacher(int id,string name,string pwd);
	// �˵�����
	virtual void openMenu();
	// �鿴����ԤԼ
	void showAllOrder();
	// ���ԤԼ
	void validOrder();
};
