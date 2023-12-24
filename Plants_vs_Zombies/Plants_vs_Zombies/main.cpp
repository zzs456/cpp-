#include <stdio.h>
#include <iostream>
// 图形库文件,easyx
#include <graphics.h>
#include <time.h>
#include "tools.h"	// 工具头文件
#include "vector2.h"
#include<mmsystem.h>	//包含多媒体设备接口头文件（windows)
#pragma comment(lib,"winmm.lib")	//告诉编译器加载winmm.lib静态库

const int game_window_width = 900;	// 定义游戏画面宽度
const int game_window_height = 600;	// 定义游戏画面高度

// 植物
enum { PEA,SUNFLOWER,PLANT_NUMS };

// 阳光球的状态 0:下落 1:落地 2:收集 3:生产
enum { SUNSHINE_DOWN,SUNSHINE_GROUND,SUNSHINE_COLLECT,SUNSHINE_PRODUCT };

// 总行数和总列数
const int max_row = 6;
const int max_col = 8;
// 草坪上植物的帧数最多为 20 张
const int max_map_plants_nums = 20;
// 定义杀死僵尸最多的个数
const int max_zms = 20;

// 设定游戏状态
enum { GOING,WIN,FAIL };
int kill_zms = 0;	// 杀死的僵尸数
int show_zms = 0;	// 出现的僵尸数
int game_status;	// 游戏状态

// 植物的结构体
struct plant {
	//0 表示没有，i 表示第 i 种植物
	int type;
	int x, y;	// 植物的 x,y 坐标
	int frame_index;	//图片的帧数
	int timer;	// 向日葵生产阳光的时间
	bool eated;	// 是否被吃
	int blood;	// 植物的血量
};

// 僵尸的结构体
struct zombie {
	// 僵尸的位置
	int x, y;
	// 僵尸的帧序号
	int frame_index;
	// 是否使用
	bool used;
	// 僵尸的速度
	int speed;
	// 僵尸所在的行
	int row;
	// 僵尸的血量
	int blood;
	// 僵尸是否死亡
	bool dead;
	// 僵尸是否正在吃食物
	bool eating;
};

// 僵尸的数组
struct zombie zms[16];

// 子弹的结构体
struct bullet {
	// 子弹的坐标
	int x, y;
	// 子弹所在的行数
	int row;
	// 子弹的速度
	int speed;
	// 子弹是否使用
	bool used;
	// 子弹是否爆炸
	bool blast;
	// 子弹爆炸的帧序号
	int frame_index;
};
// 子弹数组
struct bullet bullets[30];

// 阳光球的结构体
struct sunshine {
	int frame_index;	// 阳光球的帧数
	bool used;	// 是否被使用
	int timer;	// 控制阳光停留

	float t;	// 贝塞尔曲线时间点 0-1
	vector2 p1, p2, p3, p4;	// 贝塞尔曲线的 4 个控制点
	vector2 cur_pos;	// 阳光球的当前位置
	float speed;	// 速度
	int status;		// 阳光球的状态
};
struct sunshine sunshine_balls[10];		// 阳光球池
int sunshine_val = 50;	// 阳光值

struct plant map_plants[max_row][max_col];

// 当前鼠标拖动的植物,0表示没有，i 表示第 i 种植物
int cur_mouse_plant = 0;
// 是否是初始点击
int is_first_click = 0;
// 记录鼠标在拖动植物时的位置
int cur_mouse_plant_x, cur_mouse_plant_y;

IMAGE img_start;	// 开始图片
IMAGE img_menu;		// 菜单
IMAGE img_menu_click;	// 点击后的菜单
IMAGE img_Background;	// 背景图片
IMAGE img_Bar;			// 植物选择栏
IMAGE img_plants[PLANT_NUMS];	// 定义植物图片
IMAGE img_plants_gray[PLANT_NUMS];		// 定义不能选择植物时的灰色图片
IMAGE* img_map_plants[PLANT_NUMS][max_map_plants_nums];	// 放置在草坪上的植物图片
IMAGE img_sunshine[29];	// 阳光球的帧图片
IMAGE img_zm_walk[22];	// 僵尸走动的图片
IMAGE img_zm_dead[20];	// 僵尸死亡的图片
IMAGE img_zm_eat[21];	// 僵尸吃植物的图片
IMAGE img_bullet_normal;	// 正常状态下的子弹图片
IMAGE img_bullet_blast[4];	// 爆炸状态下的子弹图片
IMAGE img_zm_stand[11];		// 僵尸站立的图片数组

// 游戏初始化
void Game_Init();
// 创建开始界面
void Game_Start_UI();
// 片头巡场
void Game_Start_Cartoon();
// 渲染游戏画面
void Update_Game();
// 处理用户点击
void Handle_User_Click();
// 更新游戏数据
void Update_Game_Data();
// 创建阳光
void create_sunshine();
// 自由落体阳光
void normal_create();
// 向日葵生产阳光
void sunflower_create();
// 修改阳光
void update_sunshine();
// 创建僵尸
void create_zm();
// 更新僵尸
void update_zm();
// 创建子弹
void create_bullet();
// 更新子弹
void update_bullet();
// 碰撞检测
void check_collision();
// 子弹和僵尸的碰撞
void collision_bullet_zm();
// 僵尸和植物的碰撞
void collision_zm_plant();
// 检测游戏是否结束
bool check_game_over();
// 植物栏下降
void Game_Bar_Down();
// 检测是否收集阳光
void check_collect_sunshine(ExMessage* msg);
// 判断文件是否存在函数
bool fileExist(char * name) {
	FILE* fp = fopen(name,"r");
	if (fp == NULL) {
		return false;
	}
	else {
		// 关闭文件
		fclose(fp);
		return true;
	}
}

int main() {

	// 游戏初始化
	Game_Init();

	// 创建开始界面
	Game_Start_UI();

	// 片头巡场
	Game_Start_Cartoon();

	// 植物栏下降
	Game_Bar_Down();

	bool flag = true;
	int timer = 0;

	// 循环播放游戏音乐
	mciSendString("play res/music/UraniwaNi.mp3 repeat", 0, 0, 0);

	while (true) {
		
		// 处理用户点击
		Handle_User_Click();

		timer += getDelay();

		// 50ms 更新一次
		if (timer > 50) {
			flag = true;
			timer = 0;
		}

		if (flag) {

			flag = false;
			// 渲染游戏画面
			Update_Game();
			// 更新游戏数据
			Update_Game_Data();
			
			Sleep(10);
			// 检测游戏是否结束
			if (check_game_over()) {
				break;
			}
		}
	}

	system("pause");
	return 0;
}

// 游戏初始化
void Game_Init() {

	// 初始化数据
	kill_zms = 0;
	game_status = GOING;
	sunshine_val = 50;
	show_zms = 0;

	// 加载开始界面图片、菜单图片、点击后的菜单图片
	loadimage(&img_start,"res/menu.png");
	loadimage(&img_menu, "res/menu1.png");
	loadimage(&img_menu_click,"res/menu2.png");

	// 加载背景图片和植物选择栏图片
	loadimage(&img_Background,"res/Background_2.jpg");
	loadimage(&img_Bar,"res/bar5.png");

	// 加载植物图片
	char plant_image_pos[64] = { 0 };
	for (int i = 0; i < PLANT_NUMS;i ++) {
		sprintf_s(plant_image_pos,sizeof(plant_image_pos),"res/Cards/card_%d.png",i + 1);
		loadimage(&img_plants[i],plant_image_pos);
	}

	// 加载灰色植物图片
	char plant_image_gray_pos[64] = { 0 };
	for (int i = 0; i < PLANT_NUMS; i++) {
		sprintf_s(plant_image_gray_pos, sizeof(plant_image_gray_pos), "res/Cards_Black/card_%d.png", i + 1);
		loadimage(&img_plants_gray[i], plant_image_gray_pos);
	}

	// 初始化植物数组和植物图片数组
	memset(img_map_plants, 0, sizeof(img_map_plants));
	memset(map_plants, 0, sizeof(map_plants));

	// 初始化阳光球数组
	memset(sunshine_balls,0,sizeof(sunshine_balls));

	// 初始化僵尸数组
	memset(zms,0,sizeof(zms));

	// 加载僵尸走动的图片
	for (int i = 0; i < 22;i ++) {
		char zm_walk[64];
		sprintf_s(zm_walk,sizeof(zm_walk),"res/zm/%d.png",i+1);
		loadimage(&img_zm_walk[i],zm_walk);
	}
	// 加载僵尸死亡的图片
	for (int i = 0; i < 20; i++) {
		char zm_dead[64];
		sprintf_s(zm_dead, sizeof(zm_dead), "res/zm_dead/%d.png", i + 1);
		loadimage(&img_zm_dead[i], zm_dead);
	}
	// 加载僵尸吃植物的图片
	for (int i = 0; i < 21; i++) {
		char zm_eat[64];
		sprintf_s(zm_eat, sizeof(zm_eat), "res/zm_eat/%d.png", i + 1);
		loadimage(&img_zm_eat[i], zm_eat);
	}
	// 加载僵尸站立的图片
	for (int i = 0; i < 11;i ++) {
		char zm_stand[64];
		sprintf_s(zm_stand,sizeof(zm_stand),"res/zm_stand/%d.png",i + 1);
		loadimage(&img_zm_stand[i],zm_stand);
	}

	// 初始化子弹数组
	memset(bullets, 0, sizeof(bullets));
	// 加载正常子弹图片
	char normal_bullet[64];
	sprintf_s(normal_bullet,sizeof(normal_bullet),"res/bullets/bullet_normal.png");
	loadimage(&img_bullet_normal,normal_bullet);
	// 加载爆炸子弹图片
	//爆炸子弹数组的最后一张图片为源爆炸图片
	loadimage(&img_bullet_blast[3], "res/bullets/bullet_blast.png");
	for (int i = 0; i < 3;i ++) {
		char blast_bullet[64];
		sprintf_s(blast_bullet,sizeof(blast_bullet),"res/bullets/bullet_blast.png");
		float k = (i + 1) * 0.25;
		// 等比例缩小图片
		loadimage(&img_bullet_blast[i],blast_bullet,
			img_bullet_blast[3].getwidth() * k, img_bullet_blast[3].getheight() * k, true);
	}
	
	// 加载阳光图片
	for (int i = 0; i < 29;i ++) {
		char sunshine_pos[64] = { 0 };
		sprintf_s(sunshine_pos,sizeof(sunshine_pos), "res/sunshine/%d.png",i + 1);
		loadimage(&img_sunshine[i],sunshine_pos);
	}

	// 加载植物图片
	for (int i = 0; i < PLANT_NUMS;i ++) {
		for (int j = 0; j < max_map_plants_nums;j ++) {
			char plant_name[64] = { 0 };
			sprintf_s(plant_name,sizeof(plant_name),"res/zhiwu/%d/%d.png",i,j + 1);
			// 判断文件是否存在
			if (fileExist(plant_name)) {
				// 加载植物每帧的图片
				img_map_plants[i][j] = new IMAGE;
				loadimage(img_map_plants[i][j],plant_name);
			}
			else {
				break;
			}
		}
	}

	// 当前选择的植物为 0
	cur_mouse_plant = 0;

	// 创建游戏窗口
	initgraph(game_window_width, game_window_height, EX_SHOWCONSOLE);
	// 获取窗口句柄
	HWND h = GetHWnd();
	// 修改窗口标题
	SetWindowText(h,"植物大战僵尸");

	//设置字体
	LOGFONT f;//当前字体结构体变量
	gettextstyle(&f);//获取当前字体
	f.lfHeight = 30;
	f.lfWidth = 15;
	strcpy(f.lfFaceName, "Segoe UI Black");//修改字体类型
	f.lfQuality = ANTIALIASED_QUALITY;//抗锯齿效果
	settextstyle(&f);//设置字体文本
	setbkmode(TRANSPARENT);//设置字体背景透明
	setcolor(BLACK);

	// 随机数种子
	srand(unsigned int(time(NULL)));
}

// 创建开始界面
void Game_Start_UI() {

	// 开始播放开始音乐
	mciSendString("play res/music/bg.mp3 repeat", 0, 0, 0);

	bool flag = false;	// 是否点击开始

	while (true) {
		BeginBatchDraw();	// 开启双缓冲
		
		putimage(0, 0, &img_start);
		
		putimagePNG(475, 75,flag ? &img_menu_click : &img_menu);

		// 点击事件
		ExMessage msg;

		if (peekmessage(&msg)) {
			if (msg.message == WM_LBUTTONDOWN && msg.x > 474 && msg.x < 474 + 300 && msg.y>75 && msg.y < 75 + 140) {
				// 播放进入游戏音乐
				mciSendString("play res/music/gargantuar_thump.mp3", 0, 0, 0);
				flag = true;
			}
			else if (msg.message == WM_LBUTTONUP && msg.x > 474 && msg.x < 474 + 300 && msg.y>75 && msg.y < 75 + 140) {
				// 关闭开始界面音乐
				mciSendString("close res/music/bg.mp3",0,0,0);
				Sleep(10);
				EndBatchDraw();	// 结束双缓冲
				return;
			}
		}

		EndBatchDraw();		// 结束双缓冲
	}
}

// 片头巡场
void Game_Start_Cartoon() {

	mciSendString("play res/music/Kitanai_Sekai.mp3", 0, 0, 0);
	int x_min = game_window_width - img_Background.getwidth();
	// 设定 9 个僵尸的站位
	vector2 points[9] = { 
		{560,80},{530,160},{630,170},{540,200},{520,270},{598,290},{610,340},{710,299},{700,340} };
	int initposture[9];
	for (int i = 0; i < 9;i ++) {
		// 为每个僵尸创建随机初始帧序号
		initposture[i] = rand() % 11;
	}

	int count = 0;
	// 从左向右移动
	for (int x = 0; x >= x_min; x -= 3){
		BeginBatchDraw();
		putimage(x, 0, &img_Background);
		count++;
		for (int j = 0; j < 9; j ++){
			putimagePNG(points[j].x - x_min + x, points[j].y, &img_zm_stand[initposture[j]]);
			if (count >= 9){
				initposture[j] = (initposture[j] + 1) % 11;//变换下一帧
			}
		}
		if (count >= 9){
			count = 0;
		}
		Sleep(1);
		EndBatchDraw();
	}
	// 停留一段时间
	count = 0;
	for (int i = 0; i < 300;i ++) {
		BeginBatchDraw();
		++count;
		putimage(x_min,0,&img_Background);
		for (int j = 0; j < 9;j ++) {
			putimagePNG(points[j].x, points[j].y, &img_zm_stand[initposture[j]]);
			if (count >= 9) {
				initposture[j] = (initposture[j] + 1) % 11;//变换下一帧
			}
		}
		if (count >= 9) {
			count = 0;
		}
		EndBatchDraw();
		Sleep(10);
	}
	// 从右向左移动
	count = 0;
	for (int x = x_min; x <= 0; x += 3) {
		BeginBatchDraw();
		putimage(x, 0, &img_Background);
		count++;
		for (int j = 0; j < 9; j++) {
			putimagePNG(points[j].x - x_min + x, points[j].y, &img_zm_stand[initposture[j]]);
			if (count >= 9) {
				initposture[j] = (initposture[j] + 1) % 11;//变换下一帧
			}
		}
		if (count >= 9) {
			count = 0;
		}
		Sleep(1);
		EndBatchDraw();
	}
}

// 渲染游戏画面
void Update_Game() {
	
	BeginBatchDraw();	// 双缓冲

	// 渲染背景图片
	putimage(0,0,&img_Background);

	// 渲染植物选择栏图片
	putimagePNG(250,0,&img_Bar);

	// 渲染植物栏植物图片
	for (int i = 0; i < PLANT_NUMS; i++) {
		int x = 338 + i * 65;
		int y = 6;
		if (sunshine_val >= 100) {
			putimagePNG(x, y, &img_plants[i]);
		}
		else if (sunshine_val < 50) {
			putimagePNG(x, y, &img_plants_gray[i]);
		}
		else {
			if (i == PEA) {
				putimagePNG(x, y, &img_plants_gray[i]);
			}
			else if (i == SUNFLOWER) {
				putimagePNG(x, y, &img_plants[i]);
			}
		}
	}

	// 绘制分数
	char score_text[8];
	sprintf_s(score_text, sizeof(score_text), "%d", sunshine_val);
	outtextxy(276, 67, score_text);

	// 绘制草坪上的植物
	for (int i = 0; i < max_row; i++) {
		for (int j = 0; j < max_col; j++) {
			if (map_plants[i][j].type) {
				int index = map_plants[i][j].type - 1;
				int frame_index = map_plants[i][j].frame_index;
				putimagePNG(map_plants[i][j].x, map_plants[i][j].y-20, img_map_plants[index][frame_index]);
			}
		}
	}

	// 渲染僵尸
	int zmMax = sizeof(zms) / sizeof(zms[0]);
	for (int i = 0; i < zmMax;i ++) {
		if (zms[i].used) {
			if (zms[i].dead) {
				putimagePNG(zms[i].x, zms[i].y, &img_zm_dead[zms[i].frame_index]);
			}
			else if (zms[i].eating) {
				putimagePNG(zms[i].x,zms[i].y,&img_zm_eat[zms[i].frame_index]);
			}
			else {
				putimagePNG(zms[i].x, zms[i].y, &img_zm_walk[zms[i].frame_index]);
			}
		}
	}

	// 渲染阳光
	int ballMax = sizeof(sunshine_balls) / sizeof(sunshine_balls[0]);
	for (int i = 0; i < ballMax;i ++) {
		if (sunshine_balls[i].used) {
			putimagePNG(sunshine_balls[i].cur_pos.x,sunshine_balls[i].cur_pos.y,&img_sunshine[sunshine_balls[i].frame_index]);
		}
	}

	// 绘制子弹
	int bulletMax = sizeof(bullets) / sizeof(bullets[0]);
	for (int i = 0; i < bulletMax;i ++) {
		if (bullets[i].used) {
			if (bullets[i].blast) {
				putimagePNG(bullets[i].x,bullets[i].y,&img_bullet_blast[bullets[i].frame_index]);
			}
			else {
				putimagePNG(bullets[i].x, bullets[i].y, &img_bullet_normal);
			}
		}
	}

	// 渲染拖动过程中的植物
	if (cur_mouse_plant > 0 && !is_first_click) {
		// 绘制豌豆或者向日葵
		if (cur_mouse_plant == PEA+1 && sunshine_val >= 100) {
			IMAGE* img = img_map_plants[cur_mouse_plant - 1][0];
			putimagePNG(cur_mouse_plant_x - img->getwidth() / 2, cur_mouse_plant_y - img->getheight() / 2, img);
		}
		else if (cur_mouse_plant == SUNFLOWER + 1 && sunshine_val >= 50) {
			IMAGE* img = img_map_plants[cur_mouse_plant - 1][0];
			putimagePNG(cur_mouse_plant_x - img->getwidth() / 2, cur_mouse_plant_y - img->getheight() / 2, img);
		}
	}

	EndBatchDraw();	// 结束双缓冲
}

// 处理用户点击
void Handle_User_Click() {
	ExMessage msg;	// 保存鼠标消息
	static int status = 0;	// 防止循环执行时发生变化
	if (peekmessage(&msg)) {
		// 鼠标左键按下
		if (msg.message == WM_LBUTTONDOWN) {
			// 判断鼠标的位置
			if (msg.x > 338 && msg.x < 338 + 65 * PLANT_NUMS && msg.y > 6 && msg.y < 96) {
				// 选中的植物对应的下标
				int index = (msg.x - 338) / 65;
				// 更新 status,表示已被选中
				status = 1;
				is_first_click = 1;
				cur_mouse_plant = index + 1;
				if (cur_mouse_plant == PEA+1 && sunshine_val < 100) {
					// 播放不能选择植物的音乐
					PlaySound("res/music/cannotselect.wav", NULL, SND_FILENAME | SND_ASYNC);
				}
				else if (cur_mouse_plant == SUNFLOWER + 1 && sunshine_val < 50) {
					// 播放不能选择植物的音乐
					PlaySound("res/music/cannotselect.wav", NULL, SND_FILENAME | SND_ASYNC);
				}
				else {
					// 播放可以选择植物的音乐
					PlaySound("res/music/select.wav", NULL, SND_FILENAME | SND_ASYNC);
				}
			}
			else {
				// 检测是否收集阳光
				check_collect_sunshine(&msg);
			}
		}
		// 鼠标移动
		else if (msg.message == WM_MOUSEMOVE && status == 1) {
			cur_mouse_plant_x = msg.x;
			cur_mouse_plant_y = msg.y;
			is_first_click = 0;
		}
		// 鼠标左键抬起
		else if (msg.message == WM_LBUTTONUP) {
			// 鼠标落在草坪区域
			if (msg.x > 261 && msg.x < game_window_width && msg.y > 96 && msg.y < 600) {
				int row = (msg.y - 96) / 84;
				int col = (msg.x - 261) / 80;
				// 阳光值不够的情况
				if (sunshine_val < 100 && cur_mouse_plant == PEA + 1) return;
				if (sunshine_val < 50 && cur_mouse_plant == SUNFLOWER + 1) return;
				// 如果该位置没有植物，则进行放置植物
				if (map_plants[row][col].type == 0) {
					// 设置该位置处的植物
					// 播放放置植物的音乐
					mciSendString("play res/music/plantdown.mp3", 0, 0, 0);
					map_plants[row][col].type = cur_mouse_plant;
					map_plants[row][col].frame_index = 0;
					if (map_plants[row][col].type == PEA + 1) {
						sunshine_val -= 100;
					}
					else if (map_plants[row][col].type == SUNFLOWER + 1) {
						sunshine_val -= 50;
					}
					map_plants[row][col].x = 261 + 80 * col;
					map_plants[row][col].y = 96 + 84 * row;
					map_plants[row][col].timer = 0;
					map_plants[row][col].eated = false;
					map_plants[row][col].blood = 100;
				}
			}
			status = 0;
			cur_mouse_plant = 0;
		}
	}
}

// 更新游戏数据
void Update_Game_Data() {
	
	// 更新草坪植物的帧数
	for (int i = 0; i < max_row;i ++) {
		for (int j = 0; j < max_col;j ++) {
			// 如果该处有植物，则更新植物的帧数
			if (map_plants[i][j].type > 0) {
				++map_plants[i][j].frame_index;
				int index = map_plants[i][j].type - 1;
				int frame_index = map_plants[i][j].frame_index;
				if(img_map_plants[index][frame_index] == NULL){
					map_plants[i][j].frame_index = 0;
				}
			}
		}
	}
	// 创建阳光
	create_sunshine();
	// 修改阳光
	update_sunshine();

	// 创建僵尸
	create_zm();
	// 更新僵尸
	update_zm();

	// 创建子弹
	create_bullet();
	// 更新子弹
	update_bullet();

	// 碰撞检测
	check_collision();
}

// 创建阳光
void create_sunshine() {
	// 自由落体阳光
	normal_create();
	// 向日葵生产阳光
	sunflower_create();
}

// 修改阳光
void update_sunshine() {
	int ballMax = sizeof(sunshine_balls) / sizeof(sunshine_balls[0]);
	for (int i = 0; i < ballMax;i ++) {
		if (sunshine_balls[i].used) {
			sunshine_balls[i].frame_index = (sunshine_balls[i].frame_index + 1) % 29;
			sunshine* cur_sunshine = &sunshine_balls[i];
			// 下落状态
			if (cur_sunshine->status == SUNSHINE_DOWN) {
				cur_sunshine->t += cur_sunshine->speed;
				cur_sunshine->cur_pos = cur_sunshine->p1 + cur_sunshine->t * (cur_sunshine->p4 - cur_sunshine->p1);
				if (cur_sunshine->t >= 1) {
					cur_sunshine->status = SUNSHINE_GROUND;
					cur_sunshine->timer = 0;
				}
			} // 落地状态
			else if (cur_sunshine->status == SUNSHINE_GROUND) {
				// 阳光球是否消失
				++cur_sunshine->timer;
				if (cur_sunshine->timer >= 25) {
					cur_sunshine->used = false;
				}
			}	// 收集状态
			else if (cur_sunshine->status == SUNSHINE_COLLECT) {
				cur_sunshine->t += cur_sunshine->speed;
				cur_sunshine->cur_pos = cur_sunshine->p1 + cur_sunshine->t * (cur_sunshine->p4 - cur_sunshine->p1);
				if (cur_sunshine->t >= 1) {
					cur_sunshine->used = false;
					cur_sunshine->timer = 0;
					sunshine_val += 25;
				}
			}	// 生产状态
			else if(cur_sunshine->status == SUNSHINE_PRODUCT){
				cur_sunshine->t += cur_sunshine->speed;
				cur_sunshine->cur_pos = calcBezierPoint(cur_sunshine->t,cur_sunshine->p1, cur_sunshine->p2,
					cur_sunshine->p3, cur_sunshine->p4);
				if (cur_sunshine->t >= 1) {
					cur_sunshine->status = SUNSHINE_GROUND;
					cur_sunshine->timer = 0;
				}
			}
		}
	}
}

// 自由落体阳光
void normal_create() {
	int ballMax = sizeof(sunshine_balls) / sizeof(sunshine_balls[0]);
	// 控制阳光生产
	static int count = 0;
	static int product_sunshine_timer = 200;
	count ++;
	if (count >= product_sunshine_timer) {
		count = 0;
		product_sunshine_timer = 200 + (rand() % 200);
		int i = 0;
		for (; i < ballMax && sunshine_balls[i].used; i++);
		if (i < ballMax) {
			// 设置为使用状态
			sunshine_balls[i].used = true;
			// 阳光球帧序号设置为 0
			sunshine_balls[i].frame_index = 0;
			// 阳光停留设置为 0
			sunshine_balls[i].timer = 0;
			// 是指阳光的状态
			sunshine_balls[i].status = SUNSHINE_DOWN;
			// 设置阳光的初始位置和目标位置
			sunshine_balls[i].t = 0;
			sunshine_balls[i].p1 = vector2(rand() % (800 - 260) + 260, 96);
			sunshine_balls[i].cur_pos = sunshine_balls[i].p1;
			sunshine_balls[i].p4 = vector2(sunshine_balls[i].p1.x, (rand() % 6) * 84 + 100);
			int off = 4;
			float distance_start_end = dis(sunshine_balls[i].p4 - sunshine_balls[i].p1);
			sunshine_balls[i].speed = 1.0 / (distance_start_end / off);
		}
	}
}

// 向日葵生产阳光
void sunflower_create() {
	int ballMax = sizeof(sunshine_balls) / sizeof(sunshine_balls[0]);
	for (int i = 0; i < max_row;i ++) {
		for (int j = 0; j < max_col;j ++) {
			if (map_plants[i][j].type == SUNFLOWER + 1) {
				// 产生阳光的计数器
				++map_plants[i][j].timer;
				if (map_plants[i][j].timer >= 100) {
					map_plants[i][j].timer = 0;
					int k = 0;
					for (; k < ballMax && sunshine_balls[k].used;k ++);
					if (k < ballMax) {
						sunshine* ball = &sunshine_balls[k];
						// 设置产生的阳光球
						ball->used = true;
						// 设置向左或者向右的随机数
						int random = rand() % 2;
						int w = 100 + rand() % 50;
						if (random == 0) {
							w = -w;
						}
						ball->p1 = vector2(map_plants[i][j].x, map_plants[i][j].y);
						ball->p4 = vector2(ball->p1.x + w,
							ball->p1.y + img_map_plants[SUNFLOWER][0]->getheight()-img_sunshine[0].getheight()-10);
						ball->p2 = vector2(ball->p1.x + 0.3 * w, ball->p1.y - 50);
						ball->p3 = vector2(ball->p1.x + 0.7 * w,ball->p1.y - 50);
						// 设置状态
						ball->status = SUNSHINE_PRODUCT;
						// 设置速度
						ball->speed = 0.05;
						ball->timer = 0;
						ball->t = 0;
						ball->cur_pos = ball->p1;
					}
				}
			}
		}
	}
}

// 检测是否收集阳光
void check_collect_sunshine(ExMessage *msg) {
	int ballMax = sizeof(sunshine_balls) / sizeof(sunshine_balls[0]);
	int sunshine_width = img_sunshine[0].getwidth();
	int sunshine_height = img_sunshine[0].getheight();
	for (int i = 0; i < ballMax;i ++) {
		// 判断阳光是否被使用中
		if (sunshine_balls[i].used && sunshine_balls[i].status != SUNSHINE_COLLECT) {
			// 判断鼠标的位置和阳光的位置关系
			sunshine * ball = &sunshine_balls[i];
			if (msg->x > ball->cur_pos.x + 8 && msg->x < ball->cur_pos.x + sunshine_width - 8
				&& msg->y > ball->cur_pos.y + 8 && msg->y < ball->cur_pos.y + sunshine_height - 8) {
				// 设置阳光球状态为收集状态
				ball->status = SUNSHINE_COLLECT;
				// 播放音乐
				PlaySound("res/music/sunshine.wav",NULL,SND_FILENAME | SND_ASYNC);
				// 设置起点和终点
				ball->p1 = ball->cur_pos;
				ball->p4 = vector2(262,6);
				ball->t = 0;
				int off = 16;
				float distance_start_end = dis(sunshine_balls[i].p4 - sunshine_balls[i].p1);
				// 设置阳光球的速度
				ball->speed = 1.0 / (distance_start_end / off);
				// 退出循环
				break;
			}
		}
	}
}

// 创建僵尸
void create_zm() {
	// 如果出现的僵尸数大于最多的僵尸数，就不再进行创建
	if (show_zms >= max_zms) return;
	static int count = 0;
	++count;
	static int zm_create_timer = 50;
	if (count >= zm_create_timer) {
		count = 0;
		zm_create_timer = 100 + rand() % 100;
		int zmMax = sizeof(zms) / sizeof(zms[0]);
		int k = 0;
		for (; k < zmMax && zms[k].used; k++);
		if (k < zmMax) {
			zms[k].blood = 100;
			zms[k].dead = false;
			zms[k].eating = false;
			zms[k].frame_index = 0;
			zms[k].row = rand() % 6;
			zms[k].speed = 1;
			zms[k].used = true;
			zms[k].x = game_window_width;
			zms[k].y = 12 + 84 * zms[k].row;
			// 出现的僵尸数加 1
			++show_zms;
		}
	}
}

// 更新僵尸
void update_zm() {
	int zmMax = sizeof(zms) / sizeof(zms[0]);
	// 控制僵尸行走
	static int count1 = 0;
	++count1;
	// 控制僵尸图片帧
	static int count2 = 0;
	++count2;
	if (count1 >= 3) {
		count1 = 0;
		for (int i = 0; i < zmMax; i++) {
			if (zms[i].used) {
				zms[i].x -= zms[i].speed;
				// 游戏失败
				if (zms[i].x < 170) {
					game_status = FAIL;
				}
			}
		}
	}
	if (count2 >= 2) {
		count2 = 0;
		for (int i = 0; i < zmMax; i++) {
			if (zms[i].used) {
				if (zms[i].dead) {
					++zms[i].frame_index;
					if (zms[i].frame_index >= 20) {
						zms[i].used = false;
					}
				}
				else if(zms[i].eating){
					zms[i].frame_index = (zms[i].frame_index + 1) % 21;
				}
				else {
					zms[i].frame_index = (zms[i].frame_index + 1) % 22;
				}
			}
		}
	}
}

// 创建子弹
void create_bullet() {
	// 判断每行是否有僵尸
	bool lines[6] = { 0 };
	int shoot_dis = game_window_width - img_zm_walk[0].getwidth() + 40;
	int zmMax = sizeof(zms) / sizeof(zms[0]);
	for (int i = 0; i < zmMax;i ++) {
		if (zms[i].used && zms[i].x <= shoot_dis) {
			lines[zms[i].row] = 1;
		}
	}
	// 地图上的豌豆可以进行射击
	for (int i = 0; i < max_row;i ++) {
		for (int j = 0; j < max_col;j ++) {
			// 如果豌豆射手所在的行有僵尸
			if (map_plants[i][j].type == PEA+1 && lines[i]) {
				++map_plants[i][j].timer;
				// 控制子弹的发射
				if (map_plants[i][j].timer >= 12) {
					map_plants[i][j].timer = 0;
					int k = 0;
					int bulletMax = sizeof(bullets) / sizeof(bullets[0]);
					for (; k < bulletMax && bullets[k].used; k++);
					if (k < bulletMax) {
						bullets[k].frame_index = 0;
						bullets[k].speed = 8;
						bullets[k].row = i;
						bullets[k].blast = false;
						bullets[k].used = true;
						bullets[k].x = map_plants[i][j].x + img_map_plants[PEA][0]->getwidth() - 10;
						bullets[k].y = map_plants[i][j].y - 10;
					}
				}
			}
		}
	}
}

// 更新子弹
void update_bullet() {
	int bulletMax = sizeof(bullets) / sizeof(bullets[0]);
	for (int i = 0; i < bulletMax; i++) {
		if (bullets[i].used) {
			bullets[i].x += bullets[i].speed;
			if (bullets[i].x >= game_window_width) {
				bullets[i].used = false;
			}
			// 如果子弹发生爆炸
			if (bullets[i].blast) {
				bullets[i].frame_index++;
				if (bullets[i].frame_index >= 4) {
					bullets[i].used = false;
				}
			}
		}
	}
}

// 碰撞检测
void check_collision() {
	// 子弹和僵尸的碰撞
	collision_bullet_zm();
	// 僵尸和植物的碰撞
	collision_zm_plant();
}

// 子弹和僵尸的碰撞
void collision_bullet_zm() {
	int bulletMax = sizeof(bullets) / sizeof(bullets[0]);
	int zmMax = sizeof(zms) / sizeof(zms[0]);
	int k = 0;
	for (int k = 0; k < bulletMax;k ++) {
		if (bullets[k].blast || !bullets[k].used) continue;
		for (int j = 0; j < zmMax; j++) {
			if (!zms[j].used || zms[j].dead) continue;
			int x1 = zms[j].x + 80;
			int x2 = zms[j].x + 110;
			// 行数相同且发生碰撞
			if (bullets[k].row == zms[j].row && bullets[k].x >= x1 && bullets[k].x <= x2) {
				// 播放碰撞音乐
				PlaySound("res/music/peacrush1.wav", NULL, SND_FILENAME | SND_ASYNC);
				// 僵尸血量下降
				zms[j].blood -= 10;
				bullets[k].blast = true;
				bullets[k].frame_index = 0;
				bullets[k].speed = 0;
				// 僵尸死亡
				if (zms[j].blood <= 0) {
					zms[j].dead = true;
					zms[j].frame_index = 0;
					zms[j].speed = 0;
					++kill_zms;
					// 击杀僵尸数大于最大值，游戏通关
					if (kill_zms >= max_zms) {
						game_status = WIN;
					}
				}
				// 检测下一颗子弹
				break;
			}
		}
	}
}

// 僵尸和植物的碰撞
void collision_zm_plant() {
	int zmMax = sizeof(zms) / sizeof(zms[0]);
	for (int i = 0; i < zmMax;i ++) {
		if (!zms[i].used || zms[i].dead) continue;
		int cur_row = zms[i].row;
		for (int j = 0; j < max_col;j ++) {
			if (map_plants[cur_row][j].type > 0) {

				int x1 = map_plants[cur_row][j].x + 10;
				int x2 = map_plants[cur_row][j].x + 60;
				int x3 = zms[i].x + 80;
				// 植物和僵尸碰撞
				if (x3 >= x1 && x3 <= x2) {
					if (zms[i].eating) {
						if (map_plants[cur_row][j].eated) {
							mciSendString("play res/music/zmeat.mp3", 0, 0, 0);
							map_plants[cur_row][j].blood -= 2;
							if (map_plants[cur_row][j].blood <= 0) {
								map_plants[cur_row][j].type = 0;
								zms[i].eating = false;
								zms[i].speed = 1;
								zms[i].frame_index = 0;
								// 植物被吃的音乐
								mciSendString("play res/music/plantDead.mp3", 0, 0, 0);
							}
						}
						else {
							map_plants[cur_row][j].eated = true;
						}
					}
					else {
						if (!map_plants[cur_row][j].eated) {
							map_plants[cur_row][j].eated = true;
						}
						zms[i].eating = true;
						zms[i].speed = 0;
						zms[i].frame_index = 0;
					}
				}
			}
		}
	}
}

// 检测游戏是否结束
bool check_game_over() {
	bool ret = false;
	if (game_status == WIN) {
		Sleep(1000);
		// 关闭游戏音乐
		mciSendString("close res/music/UraniwaNi.mp3 ", 0, 0, 0);

		loadimage(0, "res/gameWin.png");
		
		// 播放胜利音乐
		mciSendString("play res/music/win.mp3", 0, 0, 0);
		
		ret = true;
	}
	else if (game_status == FAIL) {
		Sleep(1000);

		// 关闭游戏音乐
		mciSendString("close res/music/UraniwaNi.mp3 ", 0, 0, 0);

		loadimage(0,"res/fail2.png");

		// 播放失败音乐
		mciSendString("play res/music/lose.mp3", 0, 0, 0);
		
		ret = true;
	}
	return ret;
}

// 植物栏下降
void Game_Bar_Down() {
	// 关闭片头巡场音乐
	mciSendString("close res/music/Kitanai_Sekai.mp3", 0, 0, 0);
	int height = img_Bar.getheight();
	for (int y = -height; y <= 0; y += 2){
		BeginBatchDraw();
		putimage(0, 0, &img_Background);
		putimagePNG(250, 0, &img_Bar);
		// 绘制分数
		char score_text[8];
		sprintf_s(score_text, sizeof(score_text), "%d", sunshine_val);
		outtextxy(276, 67, score_text);
		for (int i = 0; i < PLANT_NUMS; i++){
			int x = 338 + i * 65;
			if (sunshine_val >= 100) {
				putimagePNG(x, 6 + y, &img_plants[i]);
			}
			else if (sunshine_val < 50) {
				putimagePNG(x, 6 + y, &img_plants_gray[i]);
			}
			else {
				if (i == PEA) {
					putimagePNG(x, 6 + y, &img_plants_gray[i]);
				}
				else if (i == SUNFLOWER) {
					putimagePNG(x, 6 + y, &img_plants[i]);
				}
			}
		}
		Sleep(5);
		EndBatchDraw();
	}
}