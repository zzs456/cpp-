#include "speechManager.h"
// ����
SpeechManager::SpeechManager() {
	this->initSpeech();
	this->createSpeaker();
	this->loadRecord();
}

// �˵�����
void SpeechManager::show_Menu() {

	cout << "*********************************************" << endl;
	cout << "*************  ��ӭ�μ��ݽ����� *************" << endl;
	cout << "*************  1.��ʼ�ݽ�����  **************" << endl;
	cout << "*************  2.�鿴�����¼  **************" << endl;
	cout << "*************  3.��ձ�����¼  **************" << endl;
	cout << "*************  0.�˳���������  **************" << endl;
	cout << "*********************************************" << endl;
	cout << endl;
}

void SpeechManager::exitSystem() {
	cout << "��ӭ�´�ʹ�ã�" << endl;
	system("pause");
	exit(0);
}

// ��ʼ������
void SpeechManager::initSpeech() {
	this->v1.clear();
	this->v2.clear();
	this->vVictory.clear();
	this->m_Speaker.clear();
	this->m_Index = 1;
	this->m_Record.clear();
}

// ��ȡ��¼
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
	// �������ȡ�ĵ����ַ��Ż�ȥ
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

// ����ѡ��
void SpeechManager::createSpeaker() {
	string nameSeed = "ABCDEFGHIJKL";
	for (int i = 0; i < nameSeed.size();i ++) {
		string name = "ѡ�� ";
		name += nameSeed[i];

		Speaker sp;
		sp.m_Name = name;
		for (int j = 0; j < 2;j ++) {
			sp.m_Score[j] = 0;
		}
		// �� v1 �����ѡ�ֱ��
		this->v1.push_back(i + 10001);
		// ��Ŷ�Ӧ��ѡ��
		this->m_Speaker.insert(make_pair(i + 10001,sp));
	}
}

// ��ʼ����
void SpeechManager::startSpeech() {
	// ��һ�ֿ�ʼ����
	// ��ǩ
	this->speechDraw();
	// ����
	this->speechContest();
	// ��ʾ�������
	this->showScore();
	this->m_Index ++;
	// �ڶ��ֿ�ʼ����
	// ��ǩ
	this->speechDraw();
	// ����
	this->speechContest();
	// ��ʾ���ս��
	this->showScore();
	// ���浽�ļ���
	this->saveRecord();
	cout << "���������ϣ�" << endl;

	// ���ñ���
	this->initSpeech();
	this->createSpeaker();
	this->loadRecord();
	system("pause");
	system("cls");
}

// ��ǩ
void SpeechManager::speechDraw() {
	cout << "�� << " << this->m_Index << " >> �ֱ���ѡ�����ڳ�ǩ��" << endl;
	cout << "-----------------------------------------------------" << endl;
	cout << "��ǩ���ݽ�˳�����£�" << endl;
	if (this->m_Index == 1) {
		// ����˳��
		random_shuffle(v1.begin(),v1.end());
		for (vector<int>::iterator it = v1.begin(); it != v1.end();it ++) {
			cout << *it << "\t";
		}
		cout << endl;
	}
	else {
		// ����˳��
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

// ����
void SpeechManager::speechContest() {
	cout << "------------ �� << " << this->m_Index << " >> �ֱ���ѡ����ʽ��ʼ ------------" << endl;

	// ׼����ʱ���������С��ɼ�
	multimap<double, int, greater<double>> groupScore;
	int num = 0;

	vector<int> v_Src;	// ����ѡ������
	if (this->m_Index == 1) {
		v_Src = this->v1;
	}
	else {
		v_Src = this->v2;
	}
	// ��������ѡ��
	for (auto it = v_Src.begin(); it != v_Src.end();it ++) {
		num ++;
		// ��ί���
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
		
		// ��ƽ���ַŵ�������
		this->m_Speaker[*it].m_Score[this->m_Index - 1] = avg;

		groupScore.insert(make_pair(avg,*it));
		if (num % 6 == 0) {
			cout << "�� " << num / 6 << " С��������Σ�" << endl;
			for (auto it = groupScore.begin(); it != groupScore.end();it ++) {
				cout << "��ţ�" << it->second << "\t������" << this->m_Speaker[it->second].m_Name <<
					"\t�ɼ���" << it->first << endl;
			}
			// ȡ��ǰ����
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
	cout << "------------ �� << " << this->m_Index << " >> �ֱ������ ------------" << endl;
	system("pause");
}

// չʾ�������
void SpeechManager::showScore() {
	cout << "------------ �� << " << this->m_Index << " >> �ֽ���ѡ����Ϣ���£� ------------" << endl;
	vector<int> v;
	if (this->m_Index == 1) {
		v = this->v2;
	}
	else {
		v = this->vVictory;
	}
	for (auto it = v.begin(); it != v.end();it ++) {
		cout << "��ţ�" << *it << "\t������" << this->m_Speaker[*it].m_Name << "\t�÷֣�"
			<< this->m_Speaker[*it].m_Score[this->m_Index - 1] << endl;
	}
	cout << endl;
	system("pause");
	system("cls");
	this->show_Menu();
}

// �����¼
void SpeechManager::saveRecord() {
	ofstream ofs;
	ofs.open("speech.csv",ios::out | ios::app);
	for (auto it = vVictory.begin(); it != vVictory.end();it ++) {
		ofs << *it << "," << this->m_Speaker[*it].m_Score[1] << ",";
	}
	ofs << endl;
	ofs.close();
	cout << "��¼�Ѿ����棡" << endl;
	this->fileIsEmpty = false;
}

// �鿴�����¼
void SpeechManager::showRecord() {
	if (this->fileIsEmpty) {
		cout << "�ļ������ڻ��¼Ϊ�գ�" << endl;
	}
	else {
		for (int i = 0; i < m_Record.size(); i++) {
			cout << "�� " << i + 1 << " ��\t"
				<< "�ھ���ţ�" << this->m_Record[i][0] << "\t�÷֣�" << this->m_Record[i][1] << "\t"
				<< "�Ǿ���ţ�" << this->m_Record[i][2] << "\t�÷֣�" << this->m_Record[i][3] << "\t"
				<< "������ţ�" << this->m_Record[i][4] << "\t�÷֣�" << this->m_Record[i][5] << endl;
		}
	}
	system("pause");
	system("cls");
}

// ��չ���
void SpeechManager::clearRecord() {
	cout << "�Ƿ�ȷ������ļ���" << endl;
	cout << "1����" << endl;
	cout << "2����" << endl;
	int select = 0;
	cin >> select;
	if (select == 1) {
		ofstream ofs("speech.csv",ios::trunc);
		ofs.close();
		this->initSpeech();
		this->createSpeaker();
		this->loadRecord();
		cout << "��ճɹ���" << endl;
	}
	system("pause");
	system("cls");
}

// ����
SpeechManager::~SpeechManager() {

}