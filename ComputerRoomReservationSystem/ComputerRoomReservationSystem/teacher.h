#pragma once
#include <iostream>
#include "identity.h"
#include "globalFile.h"
#include "orderFile.h"
#include <vector>
using namespace std;

class Teacher :public Identity {
public:
	// 职工号
	int m_Id;
	// 无参构造
	Teacher();
	// 有参构造
	Teacher(int id,string name,string pwd);
	// 菜单界面
	virtual void openMenu();
	// 查看所有预约
	void showAllOrder();
	// 审核预约
	void validOrder();
};
