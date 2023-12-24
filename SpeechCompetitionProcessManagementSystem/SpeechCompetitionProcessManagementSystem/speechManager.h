#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <deque>
#include <fstream>
#include <algorithm>
#include <functional>
#include <numeric>
#include "speaker.h"
using namespace std;

class SpeechManager {
public:
	// ����
	SpeechManager();

	// �˵�����
	void show_Menu();

	// �˳�����
	void exitSystem();

	// ��ʼ������
	void initSpeech();

	// ��ȡ��¼
	void loadRecord();

	// �ж��ļ��Ƿ�Ϊ��
	bool fileIsEmpty;

	// ��������¼������
	map<int, vector<string>> m_Record;

	// ����ѡ��
	void createSpeaker();

	// ��ʼ����
	void startSpeech();

	// ��ǩ
	void speechDraw();

	// ����
	void speechContest();

	// չʾ�������
	void showScore();

	// �����¼
	void saveRecord();

	// �鿴�����¼
	void showRecord();

	// ��չ���
	void clearRecord();

	// ����
	~SpeechManager();

	// ��һ�ֱ���ѡ�ֱ��
	vector<int> v1;

	// ��һ�ֽ�����ѡ�ֱ��
	vector<int> v2;

	// ���ջ񽱵�ѡ�ֱ��
	vector<int> vVictory;

	// ��Ŷ�Ӧѡ��
	map<int, Speaker> m_Speaker;

	// ��������
	int m_Index;
};