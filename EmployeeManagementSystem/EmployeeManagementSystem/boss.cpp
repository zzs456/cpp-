#include "boss.h"

Boss::Boss(int id, string name, int deptId) {
	this->m_Id = id;
	this->m_Name = name;
	this->m_DeptId = deptId;
}
// ��ʾ������Ϣ
void Boss::showInfo() {
	cout << "ְ����ţ�" << this->m_Id
		<< "\tְ��������" << this->m_Name
		<< "\t��λ��" << this->getDeptName()
		<< "\t��λְ�𣺹���˾��������" << endl;
}

// ��ȡ��λ����
string Boss::getDeptName() {
	return string("�ܲ�");
}