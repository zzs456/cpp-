#include "speechManager.h"
// 构造
SpeechManager::SpeechManager() {
	this->initSpeech();
	this->createSpeaker();
	this->loadRecord();
}

// 菜单功能
void SpeechManager::show_Menu() {

	cout << "*********************************************" << endl;
	cout << "*************  欢迎参加演讲比赛 *************" << endl;
	cout << "*************  1.开始演讲比赛  **************" << endl;
	cout << "*************  2.查看往届记录  **************" << endl;
	cout << "*************  3.清空比赛记录  **************" << endl;
	cout << "*************  0.退出比赛程序  **************" << endl;
	cout << "*********************************************" << endl;
	cout << endl;
}

void SpeechManager::exitSystem() {
	cout << "欢迎下次使用！" << endl;
	system("pause");
	exit(0);
}

// 初始化功能
void SpeechManager::initSpeech() {
	this->v1.clear();
	this->v2.clear();
	this->vVictory.clear();
	this->m_Speaker.clear();
	this->m_Index = 1;
	this->m_Record.clear();
}

// 读取记录
void SpeechManager::loadRecord() {
	ifstream ifs;
	ifs.open("speech.csv",ios::in);
	if (!ifs.is_open()) {
		this->fileIsEmpty = true;
		ifs.close();
		return;
	}
	char ch;
	ifs >> ch;
	if (ifs.eof()) {
		this->fileIsEmpty = true;
		ifs.close();
		return;
	}
	this->fileIsEmpty = false;
	// 将上面读取的单个字符放回去
	ifs.putback(ch);
	string data;
	int index = 0;
	while (ifs >> data) {
		vector<string> v;
		int pos = -1;
		int start = 0;
		while (true) {
			pos = data.find(',',start);
			if (pos == -1) {
				break;
			}
			string temp = data.substr(start,pos - start);
			start = pos + 1;
			v.push_back(temp);
		}
		m_Record.insert(make_pair(index,v));
		index ++;
	}
	ifs.close();
}

// 创建选手
void SpeechManager::createSpeaker() {
	string nameSeed = "ABCDEFGHIJKL";
	for (int i = 0; i < nameSeed.size();i ++) {
		string name = "选手 ";
		name += nameSeed[i];

		Speaker sp;
		sp.m_Name = name;
		for (int j = 0; j < 2;j ++) {
			sp.m_Score[j] = 0;
		}
		// 向 v1 中添加选手编号
		this->v1.push_back(i + 10001);
		// 编号对应的选手
		this->m_Speaker.insert(make_pair(i + 10001,sp));
	}
}

// 开始比赛
void SpeechManager::startSpeech() {
	// 第一轮开始比赛
	// 抽签
	this->speechDraw();
	// 比赛
	this->speechContest();
	// 显示晋级结果
	this->showScore();
	this->m_Index ++;
	// 第二轮开始比赛
	// 抽签
	this->speechDraw();
	// 比赛
	this->speechContest();
	// 显示最终结果
	this->showScore();
	// 保存到文件中
	this->saveRecord();
	cout << "本届比赛完毕！" << endl;

	// 重置比赛
	this->initSpeech();
	this->createSpeaker();
	this->loadRecord();
	system("pause");
	system("cls");
}

// 抽签
void SpeechManager::speechDraw() {
	cout << "第 << " << this->m_Index << " >> 轮比赛选手正在抽签：" << endl;
	cout << "-----------------------------------------------------" << endl;
	cout << "抽签后演讲顺序如下：" << endl;
	if (this->m_Index == 1) {
		// 打乱顺序
		random_shuffle(v1.begin(),v1.end());
		for (vector<int>::iterator it = v1.begin(); it != v1.end();it ++) {
			cout << *it << "\t";
		}
		cout << endl;
	}
	else {
		// 打乱顺序
		random_shuffle(v2.begin(), v2.end());
		for (vector<int>::iterator it = v2.begin(); it != v2.end(); it++) {
			cout << *it << "\t";
		}
		cout << endl;
	}
	cout << "-----------------------------------------------------" << endl;
	system("pause");
	cout << endl;
}

// 比赛
void SpeechManager::speechContest() {
	cout << "------------ 第 << " << this->m_Index << " >> 轮比赛选手正式开始 ------------" << endl;

	// 准备临时容器，存放小组成绩
	multimap<double, int, greater<double>> groupScore;
	int num = 0;

	vector<int> v_Src;	// 比赛选手容器
	if (this->m_Index == 1) {
		v_Src = this->v1;
	}
	else {
		v_Src = this->v2;
	}
	// 遍历所有选手
	for (auto it = v_Src.begin(); it != v_Src.end();it ++) {
		num ++;
		// 评委打分
		deque<double> d;
		for (int i = 0; i < 10;i ++) {
			double score = (rand() % 401 + 600) / 10.0f;
			d.push_back(score);
		}
		sort(d.begin(),d.end(),greater<double>());
		d.pop_front();
		d.pop_back();
		double sum = accumulate(d.begin(),d.end(),0.0f);
		double avg = sum / (double)d.size();
		
		// 将平均分放到容器中
		this->m_Speaker[*it].m_Score[this->m_Index - 1] = avg;

		groupScore.insert(make_pair(avg,*it));
		if (num % 6 == 0) {
			cout << "第 " << num / 6 << " 小组比赛名次：" << endl;
			for (auto it = groupScore.begin(); it != groupScore.end();it ++) {
				cout << "编号：" << it->second << "\t姓名：" << this->m_Speaker[it->second].m_Name <<
					"\t成绩：" << it->first << endl;
			}
			// 取走前三名
			int count = 0;
			for (auto it = groupScore.begin(); it != groupScore.end() && count < 3; it++,count ++) {
				if (this->m_Index == 1) {
					this->v2.push_back(it->second);
				}
				else {
					this->vVictory.push_back(it->second);
				}
			}
			groupScore.clear();
			cout << endl;
		}
	}
	cout << "------------ 第 << " << this->m_Index << " >> 轮比赛完毕 ------------" << endl;
	system("pause");
}

// 展示晋级结果
void SpeechManager::showScore() {
	cout << "------------ 第 << " << this->m_Index << " >> 轮晋级选手信息如下： ------------" << endl;
	vector<int> v;
	if (this->m_Index == 1) {
		v = this->v2;
	}
	else {
		v = this->vVictory;
	}
	for (auto it = v.begin(); it != v.end();it ++) {
		cout << "编号：" << *it << "\t姓名：" << this->m_Speaker[*it].m_Name << "\t得分："
			<< this->m_Speaker[*it].m_Score[this->m_Index - 1] << endl;
	}
	cout << endl;
	system("pause");
	system("cls");
	this->show_Menu();
}

// 保存记录
void SpeechManager::saveRecord() {
	ofstream ofs;
	ofs.open("speech.csv",ios::out | ios::app);
	for (auto it = vVictory.begin(); it != vVictory.end();it ++) {
		ofs << *it << "," << this->m_Speaker[*it].m_Score[1] << ",";
	}
	ofs << endl;
	ofs.close();
	cout << "记录已经保存！" << endl;
	this->fileIsEmpty = false;
}

// 查看往届记录
void SpeechManager::showRecord() {
	if (this->fileIsEmpty) {
		cout << "文件不存在或记录为空！" << endl;
	}
	else {
		for (int i = 0; i < m_Record.size(); i++) {
			cout << "第 " << i + 1 << " 届\t"
				<< "冠军编号：" << this->m_Record[i][0] << "\t得分：" << this->m_Record[i][1] << "\t"
				<< "亚军编号：" << this->m_Record[i][2] << "\t得分：" << this->m_Record[i][3] << "\t"
				<< "季军编号：" << this->m_Record[i][4] << "\t得分：" << this->m_Record[i][5] << endl;
		}
	}
	system("pause");
	system("cls");
}

// 清空功能
void SpeechManager::clearRecord() {
	cout << "是否确定清空文件？" << endl;
	cout << "1、是" << endl;
	cout << "2、否" << endl;
	int select = 0;
	cin >> select;
	if (select == 1) {
		ofstream ofs("speech.csv",ios::trunc);
		ofs.close();
		this->initSpeech();
		this->createSpeaker();
		this->loadRecord();
		cout << "清空成功！" << endl;
	}
	system("pause");
	system("cls");
}

// 析构
SpeechManager::~SpeechManager() {

}