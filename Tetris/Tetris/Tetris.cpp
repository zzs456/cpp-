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


const int game_width = 938;		// 游戏窗口宽度
const int game_height = 756;	// 游戏窗口高度
const int nextBlock_left = 689;	// 下一个方块左边距
const int nextBlock_top = 19;	// 下一个方块上边距

IMAGE* Tetris::start_imgs[80] = { nullptr, };

// 常量值，普通速度渲染
const int normal_speed[6] = { 0,500,400,300,200,100 };
// 常量值，游戏加速渲染
const int quick_speed = 50;

// 判断文件是否存在函数
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

// 构造函数 rows:行数 cols:列数 left_margin:左边界 top_margin:上边界 block_size:方块大小
Tetris::Tetris(int rows, int cols, int left_margin, int top_margin, int block_size)
	:rows(rows),cols(cols),left_margin(left_margin),top_margin(top_margin),block_size(block_size){
	// 初始化地图每个方格
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

// 初始化函数
void Tetris::init()
{
	// 加载背景图片
	loadimage(&img_bg,"./res/bg.png");

	// 初始化游戏状态
	this->gameOver = false;

	// 加载游戏结束图片
	loadimage(&img_gameOver,"./res/over.png");

	// 初始化关卡和消除行数
	this->level = 1;
	this->linesCount = 0;

	// 读取最高分
	cin_highestScore();

	// 初始化渲染频率为普通速度
	this->delay = normal_speed[this->level];

	// 分数初始化为 0
	this->score = 0;

	// 游戏窗口初始化
	initgraph(game_width,game_height,0);

	// 设置窗口的弹出位置
	int screen_width = GetSystemMetrics(SM_CXSCREEN);
	int screen_height = GetSystemMetrics(SM_CYSCREEN);
	int x = (screen_width - game_width) / 2;
	int y = (screen_height - game_height) / 2 - 40;
	HWND h = GetHWnd();
	SetWindowPos(h,0,x,y,game_width,game_height, SWP_NOSIZE);

	// 初始化各个小方格
	for (auto &row : map_blocks) {
		for(auto &elem : row){
			elem = 0;
		}
	}
	// 配置随机种子
	srand(unsigned int(time(NULL)));
}

// 游戏运行函数
void Tetris::play()
{
	// 游戏初始化
	this->init();

	// 开始动画
	this->start_animation();

	// 创建当前方块和下一个方块
	int cur_type = rand() % 7 + 1;
	int next_type = rand() % 7 + 1;
	cur_block = new Block(cur_type);
	next_block = new Block(next_type);

	// 控制渲染频率
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
				// 保存分数
				save_score();
				// 游戏结束画面
				display_gameOver();
			}
		}

	}
}

// 开始动画
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

// 读取最高分
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

// 监听按键
void Tetris::key_event()
{
	// 控制左右移动
	int d_col = 0;
	// 控制旋转
	bool rotateFlag = false;
	//如果检测到有键盘输入再读入
	if (_kbhit()){
		char key = _getch();
		switch (key)
		{
			//上键
		case 72: 
			rotateFlag = true;
			break;
			//下键 渲染加速
		case 80:
			delay = quick_speed;
			update = true;
			break;

			//左键
		case 75:
			d_col = -1;
			break;

			//右键
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

// 获取延时
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

// 更新游戏界面
void Tetris::update_game_UI()
{
	// 双缓冲
	BeginBatchDraw();
	// 绘制背景图片
	putimage(0,0,&img_bg);

	// 绘制地图上的小方块
	draw_map();

	// 当前方块和下一个方块
	if (!gameOver) {
		cur_block->block_draw(left_margin, top_margin);
		next_block->block_draw(nextBlock_left, nextBlock_top);
	}

	draw_Nums();	// 绘制数字

	EndBatchDraw();
}

// 更新游戏数据
void Tetris::update_game_data()
{
	update_block();

	update_map();
}

// 绘制地图
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

// 更新地图
void Tetris::update_map()
{
	int k = rows - 1;
	int lines = 0;
	for (int i = rows - 1; i >= 0;i --) {
		// 检验当前行
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
		// 其余全部设置为 0
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

// 更新小方块
void Tetris::update_block()
{
	drop_block();
}

// 小方块自由下降
void Tetris::drop_block()
{
	this->bake_block = *cur_block;
	cur_block->block_drop();
	if (!cur_block->check_pos(map_blocks)) {
		bake_block.block_fix(map_blocks);
		// 用下一个方块替换当前方块，并生成一个新的方块
		cur_block = next_block;
		int next_type = rand() % 7 + 1;
		next_block = new Block(next_type);
		// 检查游戏是否结束
		check_gameOver();
	}
	// 恢复正常速度
	if (level <= 5) {
		delay = normal_speed[this->level];
	}
}

// 小方块旋转
void Tetris::rotate_block()
{
	if (cur_block->get_blockType() == 7) return;
	bake_block = *cur_block;
	cur_block->block_rotate();
	if (!cur_block->check_pos(map_blocks)) {
		*cur_block = bake_block;
	}
}

// 小方块左右移动
void Tetris::move_block(int d_col)
{
	bake_block = *cur_block;
	cur_block->block_move(d_col);
	if (!cur_block->check_pos(map_blocks)) {
		*cur_block = bake_block;
	}
}

//绘制数字
void Tetris::draw_Nums()
{
	LOGFONT f;
	setbkmode(TRANSPARENT);
	gettextstyle(&f);
	f.lfHeight = 60;
	f.lfWeight = 30;
	f.lfQuality = ANTIALIASED_QUALITY;	// 抗锯齿
	settextcolor(RGB(180,180,180));
	strcpy_s(f.lfFaceName,sizeof(f.lfFaceName),"Segoe UI Black");
	settextstyle(&f);
	char scores_string[20];
	strcpy_s(scores_string, std::to_string(score).c_str());

	// 绘制分数
	outtextxy(670,617,scores_string);

	// 绘制关卡
	char level_string[20];
	strcpy_s(level_string, std::to_string(level).c_str());
	int xPos = 220 - (std::to_string(level).size()) * 30;
	outtextxy(xPos, 617, level_string);

	// 绘制消除的行数
	char lineCounts_string[20];
	strcpy_s(lineCounts_string, std::to_string(linesCount).c_str());
	xPos = 220 - (std::to_string(linesCount).size()) * 30;
	outtextxy(xPos, 710, lineCounts_string);

	// 绘制最高分
	char highestScore_string[20];
	strcpy_s(highestScore_string, std::to_string(highest_score).c_str());
	outtextxy(670, 710, highestScore_string);
}

// 检查游戏是否结束
void Tetris::check_gameOver()
{
	if (!cur_block->check_pos(map_blocks)) gameOver = true;
}

// 保存分数
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

// 游戏结束画面
void Tetris::display_gameOver()
{
	putimagePNG(262, 280, &img_gameOver);
	mciSendString("play ./res/over.mp3",NULL,0,0);
	Sleep(4000);
}
