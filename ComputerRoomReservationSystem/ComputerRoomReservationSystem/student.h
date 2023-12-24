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
	// 学生学号
	int m_Id;
	// 无参构造
	Student();
	// 有参构造
	Student(int id,string name,string pwd);
	// 菜单界面
	virtual void openMenu();
	// 申请预约
	void applyOrder();
	// 查看自身预约
	void showMyOrder();
	// 查看所有预约
	void showAllOrder();
	// 取消预约
	void cancelOrder();
	// 机房信息
	vector<ComputerRoom> vCom;
};