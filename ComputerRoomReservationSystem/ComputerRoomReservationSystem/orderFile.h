#pragma once
#include <iostream>
#include "globalFile.h"
#include <fstream>
#include <map>
using namespace std;

class OrderFile {
public:
	OrderFile();

	// 更新预约记录
	void updateOrder();

	// 记录预约条数
	int m_Size;

	// 记录预约信息的容器
	map<int, map<string, string>> m_orderData;
};