#include "Tetris.h"
#include <conio.h>
#include <iostream>
#include <fstream>
#include <mmsystem.h>
#pragma comment (lib, "winmm.lib")
#include <string>
#include <cstring>
#include "tools.h"
#include <graphics.h>


const int game_width = 938;		// ��Ϸ���ڿ��
const int game_height = 756;	// ��Ϸ���ڸ߶�
const int nextBlock_left = 689;	// ��һ��������߾�
const int nextBlock_top = 19;	// ��һ�������ϱ߾�

IMAGE* Tetris::start_imgs[80] = { nullptr, };

// ����ֵ����ͨ�ٶ���Ⱦ
const int normal_speed[6] = { 0,500,400,300,200,100 };
// ����ֵ����Ϸ������Ⱦ
const int quick_speed = 50;

// �ж��ļ��Ƿ���ں���
bool fileExist(std::string& name) {
	std::ifstream ifs(name);
	if (!ifs.is_open()) {
		return false;
	}
	else {
		ifs.close();
		return true;
	}
}

// ���캯�� rows:���� cols:���� left_margin:��߽� top_margin:�ϱ߽� block_size:�����С
Tetris::Tetris(int rows, int cols, int left_margin, int top_margin, int block_size)
	:rows(rows),cols(cols),left_margin(left_margin),top_margin(top_margin),block_size(block_size){
	// ��ʼ����ͼÿ������
	for (int i = 0; i < this->rows;i ++) {
		vector<int> row_elems(this->cols,0);
		map_blocks.push_back(row_elems);
	}
	if (start_imgs[0] == nullptr) {
		for (int i = 0; i < 80;i ++) {
			std::string temp = "./res/welcome/";
			temp += std::to_string(i + 1);
			temp += ".png";
			if (fileExist(temp)) {
				LPCTSTR str = temp.c_str();
				start_imgs[i] = new IMAGE;
				loadimage(start_imgs[i], str);
			}
		}
	}
}

// ��ʼ������
void Tetris::init()
{
	// ���ر���ͼƬ
	loadimage(&img_bg,"./res/bg.png");

	// ��ʼ����Ϸ״̬
	this->gameOver = false;

	// ������Ϸ����ͼƬ
	loadimage(&img_gameOver,"./res/over.png");

	// ��ʼ���ؿ�����������
	this->level = 1;
	this->linesCount = 0;

	// ��ȡ��߷�
	cin_highestScore();

	// ��ʼ����ȾƵ��Ϊ��ͨ�ٶ�
	this->delay = normal_speed[this->level];

	// ������ʼ��Ϊ 0
	this->score = 0;

	// ��Ϸ���ڳ�ʼ��
	initgraph(game_width,game_height,0);

	// ���ô��ڵĵ���λ��
	int screen_width = GetSystemMetrics(SM_CXSCREEN);
	int screen_height = GetSystemMetrics(SM_CYSCREEN);
	int x = (screen_width - game_width) / 2;
	int y = (screen_height - game_height) / 2 - 40;
	HWND h = GetHWnd();
	SetWindowPos(h,0,x,y,game_width,game_height, SWP_NOSIZE);

	// ��ʼ������С����
	for (auto &row : map_blocks) {
		for(auto &elem : row){
			elem = 0;
		}
	}
	// �����������
	srand(unsigned int(time(NULL)));
}

// ��Ϸ���к���
void Tetris::play()
{
	// ��Ϸ��ʼ��
	this->init();

	// ��ʼ����
	this->start_animation();

	// ������ǰ�������һ������
	int cur_type = rand() % 7 + 1;
	int next_type = rand() % 7 + 1;
	cur_block = new Block(cur_type);
	next_block = new Block(next_type);

	// ������ȾƵ��
	int timer = 0;

	while (true) {
		this->key_event();

		timer += this->get_delay();

		if (timer > delay) {
			timer = 0;
			this->update_game_data();
			this->update = true;
		}

		if (this->update) {
			this->update = false;
			this->update_game_UI();
			if (gameOver) {
				// �������
				save_score();
				// ��Ϸ��������
				display_gameOver();
			}
		}

	}
}

// ��ʼ����
void Tetris::start_animation()
{
	mciSendString("play res/bg.mp3 repeat", NULL, 0, 0);
	for (int j = 0; j < 4;j ++) {
		for (int i = 0; i < 80; i++) {
			if (start_imgs[i] != nullptr) {
				//std::cout << i << "\t" << start_imgs[i] << std::endl;
				BeginBatchDraw();
				putimage(30, -158, start_imgs[i]);
				Sleep(100);
				EndBatchDraw();
			}
			else {
				break;
			}
		}
	}
}

// ��ȡ��߷�
void Tetris::cin_highestScore()
{
	std::ifstream ifs("./res/highest_score.txt");
	if (!ifs.is_open()) {
		highest_score = 0;
	}
	else {
		if (ifs.eof()) {
			highest_score = 0;
		}
		else {
			ifs >> highest_score;
		}
		ifs.close();
	}
}

// ��������
void Tetris::key_event()
{
	// ���������ƶ�
	int d_col = 0;
	// ������ת
	bool rotateFlag = false;
	//�����⵽�м��������ٶ���
	if (_kbhit()){
		char key = _getch();
		switch (key)
		{
			//�ϼ�
		case 72: 
			rotateFlag = true;
			break;
			//�¼� ��Ⱦ����
		case 80:
			delay = quick_speed;
			update = true;
			break;

			//���
		case 75:
			d_col = -1;
			break;

			//�Ҽ�
		case 77:
			d_col = 1;
			break;
		default:
			break;
		}
	}

	if (rotateFlag) {
		rotate_block();
		update = true;
	}

	if (d_col) {
		move_block(d_col);
		update = true;
	}
}

// ��ȡ��ʱ
int Tetris::get_delay()
{
	static unsigned long long lastTime = 0;
	unsigned long long currentTime = GetTickCount();
	if (lastTime == 0) {
		lastTime = currentTime;
		return 0;
	}
	else {
		auto ret = currentTime - lastTime;
		lastTime = currentTime;
		return ret;
	}
}

// ������Ϸ����
void Tetris::update_game_UI()
{
	// ˫����
	BeginBatchDraw();
	// ���Ʊ���ͼƬ
	putimage(0,0,&img_bg);

	// ���Ƶ�ͼ�ϵ�С����
	draw_map();

	// ��ǰ�������һ������
	if (!gameOver) {
		cur_block->block_draw(left_margin, top_margin);
		next_block->block_draw(nextBlock_left, nextBlock_top);
	}

	draw_Nums();	// ��������

	EndBatchDraw();
}

// ������Ϸ����
void Tetris::update_game_data()
{
	update_block();

	update_map();
}

// ���Ƶ�ͼ
void Tetris::draw_map()
{
	IMAGE** block_imgs = Block::get_blockImgs();
	for (int i = 0; i < map_blocks.size(); i++) {
		for (int j = 0; j < map_blocks[0].size(); j++) {
			if (!map_blocks[i][j]) continue;
			int x = left_margin + j * block_size;
			int y = top_margin + i * block_size;
			putimage(x, y, block_imgs[map_blocks[i][j] - 1]);
		}
	}
}

// ���µ�ͼ
void Tetris::update_map()
{
	int k = rows - 1;
	int lines = 0;
	for (int i = rows - 1; i >= 0;i --) {
		// ���鵱ǰ��
		bool flag = true;
		for (int j = 0; j < cols;j ++) {
			map_blocks[k][j] = map_blocks[i][j];
			if (!map_blocks[i][j]) {
				flag = false;
			}
		}
		if (flag) {
			lines++;
		}
		else {
			k--;
		}
	}
	if (lines > 0) {
		mciSendString("play res/xiaochu2.mp3", NULL, 0, 0);
		// ����ȫ������Ϊ 0
		for (int i = k; i >= 0;i --) {
			for (int j = 0; j < cols;j ++) {
				map_blocks[i][j] = 0;
			}
		}
		linesCount += lines;
		level = score / 100 + 1;
		if (level <= 5) {
			delay = normal_speed[level];
		}
		else {
			delay = quick_speed;
		}
		switch (lines) {
		case 1:
			score += 10; 
			break;
		case 2:
			score += 30; 
			break;
		case 3:
			score += 50; 
			break;
		case 4:
			score += 80; 
			break;
		default:
			break;
		}
		update = true;
	}
}

// ����С����
void Tetris::update_block()
{
	drop_block();
}

// С���������½�
void Tetris::drop_block()
{
	this->bake_block = *cur_block;
	cur_block->block_drop();
	if (!cur_block->check_pos(map_blocks)) {
		bake_block.block_fix(map_blocks);
		// ����һ�������滻��ǰ���飬������һ���µķ���
		cur_block = next_block;
		int next_type = rand() % 7 + 1;
		next_block = new Block(next_type);
		// �����Ϸ�Ƿ����
		check_gameOver();
	}
	// �ָ������ٶ�
	if (level <= 5) {
		delay = normal_speed[this->level];
	}
}

// С������ת
void Tetris::rotate_block()
{
	if (cur_block->get_blockType() == 7) return;
	bake_block = *cur_block;
	cur_block->block_rotate();
	if (!cur_block->check_pos(map_blocks)) {
		*cur_block = bake_block;
	}
}

// С���������ƶ�
void Tetris::move_block(int d_col)
{
	bake_block = *cur_block;
	cur_block->block_move(d_col);
	if (!cur_block->check_pos(map_blocks)) {
		*cur_block = bake_block;
	}
}

//��������
void Tetris::draw_Nums()
{
	LOGFONT f;
	setbkmode(TRANSPARENT);
	gettextstyle(&f);
	f.lfHeight = 60;
	f.lfWeight = 30;
	f.lfQuality = ANTIALIASED_QUALITY;	// �����
	settextcolor(RGB(180,180,180));
	strcpy_s(f.lfFaceName,sizeof(f.lfFaceName),"Segoe UI Black");
	settextstyle(&f);
	char scores_string[20];
	strcpy_s(scores_string, std::to_string(score).c_str());

	// ���Ʒ���
	outtextxy(670,617,scores_string);

	// ���ƹؿ�
	char level_string[20];
	strcpy_s(level_string, std::to_string(level).c_str());
	int xPos = 220 - (std::to_string(level).size()) * 30;
	outtextxy(xPos, 617, level_string);

	// ��������������
	char lineCounts_string[20];
	strcpy_s(lineCounts_string, std::to_string(linesCount).c_str());
	xPos = 220 - (std::to_string(linesCount).size()) * 30;
	outtextxy(xPos, 710, lineCounts_string);

	// ������߷�
	char highestScore_string[20];
	strcpy_s(highestScore_string, std::to_string(highest_score).c_str());
	outtextxy(670, 710, highestScore_string);
}

// �����Ϸ�Ƿ����
void Tetris::check_gameOver()
{
	if (!cur_block->check_pos(map_blocks)) gameOver = true;
}

// �������
void Tetris::save_score()
{
	std::ofstream ofs("./res/highest_score.txt",std::ios::out | std::ios::trunc);
	if (score >= highest_score) {
		ofs << score;
	}
	else {
		ofs << highest_score;
	}
	ofs.close();
}

// ��Ϸ��������
void Tetris::display_gameOver()
{
	putimagePNG(262, 280, &img_gameOver);
	mciSendString("play ./res/over.mp3",NULL,0,0);
	Sleep(4000);
}
