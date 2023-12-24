#pragma once
#include <iostream>
#include <fstream>
#include "worker.h"
#include "employee.h"
#include "manager.h"
#include "boss.h"
using namespace std;
#define FILENAME "empFile.txt"

class WorkManager {
public:
	WorkManager();

	// �˵�����
	void Show_Menu();

	// �˳�����
	void ExitSystem();

	// ��¼ְ������
	int m_EmpNum;
	
	// ְ������ָ��
	Worker** m_EmpArray;

	// �ж��ļ��Ƿ�Ϊ��
	bool m_FileIsEmpty;

	// ͳ���ļ�������
	int get_EmpNum();

	// ��ʼ��Ա��
	void init_Emp();

	// ��ʾְ��
	void Show_Emp();

	// ���ְ��
	void Add_Emp();

	// �ж�ְ���Ƿ����
	int IsExist(int id);

	// ɾ��ְ��
	void Del_Emp();

	// �޸�ְ��
	void Mod_Emp();

	// ����ְ��
	void Find_Emp();

	// Ա������
	void Sort_Emp();

	// ���
	void Clean_Emp();

	// �����ļ�
	void save();

	~WorkManager();
};
