#pragma once
#include <iostream>
using namespace std;

class Identity {
public:
	
	// 操作菜单  纯虚函数
	virtual void openMenu() = 0;
	
	// 用户名
	string m_Name;
	
	// 密码
	string m_Pwd;
};