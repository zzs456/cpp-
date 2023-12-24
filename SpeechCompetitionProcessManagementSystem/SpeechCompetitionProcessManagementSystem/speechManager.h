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
	// 构造
	SpeechManager();

	// 菜单功能
	void show_Menu();

	// 退出功能
	void exitSystem();

	// 初始化功能
	void initSpeech();

	// 读取记录
	void loadRecord();

	// 判断文件是否为空
	bool fileIsEmpty;

	// 存放往届记录的容器
	map<int, vector<string>> m_Record;

	// 创建选手
	void createSpeaker();

	// 开始比赛
	void startSpeech();

	// 抽签
	void speechDraw();

	// 比赛
	void speechContest();

	// 展示晋级结果
	void showScore();

	// 保存记录
	void saveRecord();

	// 查看往届记录
	void showRecord();

	// 清空功能
	void clearRecord();

	// 析构
	~SpeechManager();

	// 第一轮比赛选手编号
	vector<int> v1;

	// 第一轮晋级的选手编号
	vector<int> v2;

	// 最终获奖的选手编号
	vector<int> vVictory;

	// 编号对应选手
	map<int, Speaker> m_Speaker;

	// 比赛轮数
	int m_Index;
};