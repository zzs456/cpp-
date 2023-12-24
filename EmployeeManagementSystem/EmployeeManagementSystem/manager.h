#pragma once
#include <iostream>
#include "worker.h"
using namespace std;

class Manager :public Worker {
public:

	Manager(int id, string name, int deptId);
	// 显示个人信息
	void showInfo();

	// 获取岗位名称
	string getDeptName();

};