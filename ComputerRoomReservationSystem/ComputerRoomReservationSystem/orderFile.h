#pragma once
#include <iostream>
#include "globalFile.h"
#include <fstream>
#include <map>
using namespace std;

class OrderFile {
public:
	OrderFile();

	// ����ԤԼ��¼
	void updateOrder();

	// ��¼ԤԼ����
	int m_Size;

	// ��¼ԤԼ��Ϣ������
	map<int, map<string, string>> m_orderData;
};