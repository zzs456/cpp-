#include <stdio.h>
#include <iostream>
// ͼ�ο��ļ�,easyx
#include <graphics.h>
#include <time.h>
#include "tools.h"	// ����ͷ�ļ�
#include "vector2.h"
#include<mmsystem.h>	//������ý���豸�ӿ�ͷ�ļ���windows)
#pragma comment(lib,"winmm.lib")	//���߱���������winmm.lib��̬��

const int game_window_width = 900;	// ������Ϸ������
const int game_window_height = 600;	// ������Ϸ����߶�

// ֲ��
enum { PEA,SUNFLOWER,PLANT_NUMS };

// �������״̬ 0:���� 1:��� 2:�ռ� 3:����
enum { SUNSHINE_DOWN,SUNSHINE_GROUND,SUNSHINE_COLLECT,SUNSHINE_PRODUCT };

// ��������������
const int max_row = 6;
const int max_col = 8;
// ��ƺ��ֲ���֡�����Ϊ 20 ��
const int max_map_plants_nums = 20;
// ����ɱ����ʬ���ĸ���
const int max_zms = 20;

// �趨��Ϸ״̬
enum { GOING,WIN,FAIL };
int kill_zms = 0;	// ɱ���Ľ�ʬ��
int show_zms = 0;	// ���ֵĽ�ʬ��
int game_status;	// ��Ϸ״̬

// ֲ��Ľṹ��
struct plant {
	//0 ��ʾû�У�i ��ʾ�� i ��ֲ��
	int type;
	int x, y;	// ֲ��� x,y ����
	int frame_index;	//ͼƬ��֡��
	int timer;	// ���տ����������ʱ��
	bool eated;	// �Ƿ񱻳�
	int blood;	// ֲ���Ѫ��
};

// ��ʬ�Ľṹ��
struct zombie {
	// ��ʬ��λ��
	int x, y;
	// ��ʬ��֡���
	int frame_index;
	// �Ƿ�ʹ��
	bool used;
	// ��ʬ���ٶ�
	int speed;
	// ��ʬ���ڵ���
	int row;
	// ��ʬ��Ѫ��
	int blood;
	// ��ʬ�Ƿ�����
	bool dead;
	// ��ʬ�Ƿ����ڳ�ʳ��
	bool eating;
};

// ��ʬ������
struct zombie zms[16];

// �ӵ��Ľṹ��
struct bullet {
	// �ӵ�������
	int x, y;
	// �ӵ����ڵ�����
	int row;
	// �ӵ����ٶ�
	int speed;
	// �ӵ��Ƿ�ʹ��
	bool used;
	// �ӵ��Ƿ�ը
	bool blast;
	// �ӵ���ը��֡���
	int frame_index;
};
// �ӵ�����
struct bullet bullets[30];

// ������Ľṹ��
struct sunshine {
	int frame_index;	// �������֡��
	bool used;	// �Ƿ�ʹ��
	int timer;	// ��������ͣ��

	float t;	// ����������ʱ��� 0-1
	vector2 p1, p2, p3, p4;	// ���������ߵ� 4 �����Ƶ�
	vector2 cur_pos;	// ������ĵ�ǰλ��
	float speed;	// �ٶ�
	int status;		// �������״̬
};
struct sunshine sunshine_balls[10];		// �������
int sunshine_val = 50;	// ����ֵ

struct plant map_plants[max_row][max_col];

// ��ǰ����϶���ֲ��,0��ʾû�У�i ��ʾ�� i ��ֲ��
int cur_mouse_plant = 0;
// �Ƿ��ǳ�ʼ���
int is_first_click = 0;
// ��¼������϶�ֲ��ʱ��λ��
int cur_mouse_plant_x, cur_mouse_plant_y;

IMAGE img_start;	// ��ʼͼƬ
IMAGE img_menu;		// �˵�
IMAGE img_menu_click;	// �����Ĳ˵�
IMAGE img_Background;	// ����ͼƬ
IMAGE img_Bar;			// ֲ��ѡ����
IMAGE img_plants[PLANT_NUMS];	// ����ֲ��ͼƬ
IMAGE img_plants_gray[PLANT_NUMS];		// ���岻��ѡ��ֲ��ʱ�Ļ�ɫͼƬ
IMAGE* img_map_plants[PLANT_NUMS][max_map_plants_nums];	// �����ڲ�ƺ�ϵ�ֲ��ͼƬ
IMAGE img_sunshine[29];	// �������֡ͼƬ
IMAGE img_zm_walk[22];	// ��ʬ�߶���ͼƬ
IMAGE img_zm_dead[20];	// ��ʬ������ͼƬ
IMAGE img_zm_eat[21];	// ��ʬ��ֲ���ͼƬ
IMAGE img_bullet_normal;	// ����״̬�µ��ӵ�ͼƬ
IMAGE img_bullet_blast[4];	// ��ը״̬�µ��ӵ�ͼƬ
IMAGE img_zm_stand[11];		// ��ʬվ����ͼƬ����

// ��Ϸ��ʼ��
void Game_Init();
// ������ʼ����
void Game_Start_UI();
// ƬͷѲ��
void Game_Start_Cartoon();
// ��Ⱦ��Ϸ����
void Update_Game();
// �����û����
void Handle_User_Click();
// ������Ϸ����
void Update_Game_Data();
// ��������
void create_sunshine();
// ������������
void normal_create();
// ���տ���������
void sunflower_create();
// �޸�����
void update_sunshine();
// ������ʬ
void create_zm();
// ���½�ʬ
void update_zm();
// �����ӵ�
void create_bullet();
// �����ӵ�
void update_bullet();
// ��ײ���
void check_collision();
// �ӵ��ͽ�ʬ����ײ
void collision_bullet_zm();
// ��ʬ��ֲ�����ײ
void collision_zm_plant();
// �����Ϸ�Ƿ����
bool check_game_over();
// ֲ�����½�
void Game_Bar_Down();
// ����Ƿ��ռ�����
void check_collect_sunshine(ExMessage* msg);
// �ж��ļ��Ƿ���ں���
bool fileExist(char * name) {
	FILE* fp = fopen(name,"r");
	if (fp == NULL) {
		return false;
	}
	else {
		// �ر��ļ�
		fclose(fp);
		return true;
	}
}

int main() {

	// ��Ϸ��ʼ��
	Game_Init();

	// ������ʼ����
	Game_Start_UI();

	// ƬͷѲ��
	Game_Start_Cartoon();

	// ֲ�����½�
	Game_Bar_Down();

	bool flag = true;
	int timer = 0;

	// ѭ��������Ϸ����
	mciSendString("play res/music/UraniwaNi.mp3 repeat", 0, 0, 0);

	while (true) {
		
		// �����û����
		Handle_User_Click();

		timer += getDelay();

		// 50ms ����һ��
		if (timer > 50) {
			flag = true;
			timer = 0;
		}

		if (flag) {

			flag = false;
			// ��Ⱦ��Ϸ����
			Update_Game();
			// ������Ϸ����
			Update_Game_Data();
			
			Sleep(10);
			// �����Ϸ�Ƿ����
			if (check_game_over()) {
				break;
			}
		}
	}

	system("pause");
	return 0;
}

// ��Ϸ��ʼ��
void Game_Init() {

	// ��ʼ������
	kill_zms = 0;
	game_status = GOING;
	sunshine_val = 50;
	show_zms = 0;

	// ���ؿ�ʼ����ͼƬ���˵�ͼƬ�������Ĳ˵�ͼƬ
	loadimage(&img_start,"res/menu.png");
	loadimage(&img_menu, "res/menu1.png");
	loadimage(&img_menu_click,"res/menu2.png");

	// ���ر���ͼƬ��ֲ��ѡ����ͼƬ
	loadimage(&img_Background,"res/Background_2.jpg");
	loadimage(&img_Bar,"res/bar5.png");

	// ����ֲ��ͼƬ
	char plant_image_pos[64] = { 0 };
	for (int i = 0; i < PLANT_NUMS;i ++) {
		sprintf_s(plant_image_pos,sizeof(plant_image_pos),"res/Cards/card_%d.png",i + 1);
		loadimage(&img_plants[i],plant_image_pos);
	}

	// ���ػ�ɫֲ��ͼƬ
	char plant_image_gray_pos[64] = { 0 };
	for (int i = 0; i < PLANT_NUMS; i++) {
		sprintf_s(plant_image_gray_pos, sizeof(plant_image_gray_pos), "res/Cards_Black/card_%d.png", i + 1);
		loadimage(&img_plants_gray[i], plant_image_gray_pos);
	}

	// ��ʼ��ֲ�������ֲ��ͼƬ����
	memset(img_map_plants, 0, sizeof(img_map_plants));
	memset(map_plants, 0, sizeof(map_plants));

	// ��ʼ������������
	memset(sunshine_balls,0,sizeof(sunshine_balls));

	// ��ʼ����ʬ����
	memset(zms,0,sizeof(zms));

	// ���ؽ�ʬ�߶���ͼƬ
	for (int i = 0; i < 22;i ++) {
		char zm_walk[64];
		sprintf_s(zm_walk,sizeof(zm_walk),"res/zm/%d.png",i+1);
		loadimage(&img_zm_walk[i],zm_walk);
	}
	// ���ؽ�ʬ������ͼƬ
	for (int i = 0; i < 20; i++) {
		char zm_dead[64];
		sprintf_s(zm_dead, sizeof(zm_dead), "res/zm_dead/%d.png", i + 1);
		loadimage(&img_zm_dead[i], zm_dead);
	}
	// ���ؽ�ʬ��ֲ���ͼƬ
	for (int i = 0; i < 21; i++) {
		char zm_eat[64];
		sprintf_s(zm_eat, sizeof(zm_eat), "res/zm_eat/%d.png", i + 1);
		loadimage(&img_zm_eat[i], zm_eat);
	}
	// ���ؽ�ʬվ����ͼƬ
	for (int i = 0; i < 11;i ++) {
		char zm_stand[64];
		sprintf_s(zm_stand,sizeof(zm_stand),"res/zm_stand/%d.png",i + 1);
		loadimage(&img_zm_stand[i],zm_stand);
	}

	// ��ʼ���ӵ�����
	memset(bullets, 0, sizeof(bullets));
	// ���������ӵ�ͼƬ
	char normal_bullet[64];
	sprintf_s(normal_bullet,sizeof(normal_bullet),"res/bullets/bullet_normal.png");
	loadimage(&img_bullet_normal,normal_bullet);
	// ���ر�ը�ӵ�ͼƬ
	//��ը�ӵ���������һ��ͼƬΪԴ��ըͼƬ
	loadimage(&img_bullet_blast[3], "res/bullets/bullet_blast.png");
	for (int i = 0; i < 3;i ++) {
		char blast_bullet[64];
		sprintf_s(blast_bullet,sizeof(blast_bullet),"res/bullets/bullet_blast.png");
		float k = (i + 1) * 0.25;
		// �ȱ�����СͼƬ
		loadimage(&img_bullet_blast[i],blast_bullet,
			img_bullet_blast[3].getwidth() * k, img_bullet_blast[3].getheight() * k, true);
	}
	
	// ��������ͼƬ
	for (int i = 0; i < 29;i ++) {
		char sunshine_pos[64] = { 0 };
		sprintf_s(sunshine_pos,sizeof(sunshine_pos), "res/sunshine/%d.png",i + 1);
		loadimage(&img_sunshine[i],sunshine_pos);
	}

	// ����ֲ��ͼƬ
	for (int i = 0; i < PLANT_NUMS;i ++) {
		for (int j = 0; j < max_map_plants_nums;j ++) {
			char plant_name[64] = { 0 };
			sprintf_s(plant_name,sizeof(plant_name),"res/zhiwu/%d/%d.png",i,j + 1);
			// �ж��ļ��Ƿ����
			if (fileExist(plant_name)) {
				// ����ֲ��ÿ֡��ͼƬ
				img_map_plants[i][j] = new IMAGE;
				loadimage(img_map_plants[i][j],plant_name);
			}
			else {
				break;
			}
		}
	}

	// ��ǰѡ���ֲ��Ϊ 0
	cur_mouse_plant = 0;

	// ������Ϸ����
	initgraph(game_window_width, game_window_height, EX_SHOWCONSOLE);
	// ��ȡ���ھ��
	HWND h = GetHWnd();
	// �޸Ĵ��ڱ���
	SetWindowText(h,"ֲ���ս��ʬ");

	//��������
	LOGFONT f;//��ǰ����ṹ�����
	gettextstyle(&f);//��ȡ��ǰ����
	f.lfHeight = 30;
	f.lfWidth = 15;
	strcpy(f.lfFaceName, "Segoe UI Black");//�޸���������
	f.lfQuality = ANTIALIASED_QUALITY;//�����Ч��
	settextstyle(&f);//���������ı�
	setbkmode(TRANSPARENT);//�������屳��͸��
	setcolor(BLACK);

	// ���������
	srand(unsigned int(time(NULL)));
}

// ������ʼ����
void Game_Start_UI() {

	// ��ʼ���ſ�ʼ����
	mciSendString("play res/music/bg.mp3 repeat", 0, 0, 0);

	bool flag = false;	// �Ƿ�����ʼ

	while (true) {
		BeginBatchDraw();	// ����˫����
		
		putimage(0, 0, &img_start);
		
		putimagePNG(475, 75,flag ? &img_menu_click : &img_menu);

		// ����¼�
		ExMessage msg;

		if (peekmessage(&msg)) {
			if (msg.message == WM_LBUTTONDOWN && msg.x > 474 && msg.x < 474 + 300 && msg.y>75 && msg.y < 75 + 140) {
				// ���Ž�����Ϸ����
				mciSendString("play res/music/gargantuar_thump.mp3", 0, 0, 0);
				flag = true;
			}
			else if (msg.message == WM_LBUTTONUP && msg.x > 474 && msg.x < 474 + 300 && msg.y>75 && msg.y < 75 + 140) {
				// �رտ�ʼ��������
				mciSendString("close res/music/bg.mp3",0,0,0);
				Sleep(10);
				EndBatchDraw();	// ����˫����
				return;
			}
		}

		EndBatchDraw();		// ����˫����
	}
}

// ƬͷѲ��
void Game_Start_Cartoon() {

	mciSendString("play res/music/Kitanai_Sekai.mp3", 0, 0, 0);
	int x_min = game_window_width - img_Background.getwidth();
	// �趨 9 ����ʬ��վλ
	vector2 points[9] = { 
		{560,80},{530,160},{630,170},{540,200},{520,270},{598,290},{610,340},{710,299},{700,340} };
	int initposture[9];
	for (int i = 0; i < 9;i ++) {
		// Ϊÿ����ʬ���������ʼ֡���
		initposture[i] = rand() % 11;
	}

	int count = 0;
	// ���������ƶ�
	for (int x = 0; x >= x_min; x -= 3){
		BeginBatchDraw();
		putimage(x, 0, &img_Background);
		count++;
		for (int j = 0; j < 9; j ++){
			putimagePNG(points[j].x - x_min + x, points[j].y, &img_zm_stand[initposture[j]]);
			if (count >= 9){
				initposture[j] = (initposture[j] + 1) % 11;//�任��һ֡
			}
		}
		if (count >= 9){
			count = 0;
		}
		Sleep(1);
		EndBatchDraw();
	}
	// ͣ��һ��ʱ��
	count = 0;
	for (int i = 0; i < 300;i ++) {
		BeginBatchDraw();
		++count;
		putimage(x_min,0,&img_Background);
		for (int j = 0; j < 9;j ++) {
			putimagePNG(points[j].x, points[j].y, &img_zm_stand[initposture[j]]);
			if (count >= 9) {
				initposture[j] = (initposture[j] + 1) % 11;//�任��һ֡
			}
		}
		if (count >= 9) {
			count = 0;
		}
		EndBatchDraw();
		Sleep(10);
	}
	// ���������ƶ�
	count = 0;
	for (int x = x_min; x <= 0; x += 3) {
		BeginBatchDraw();
		putimage(x, 0, &img_Background);
		count++;
		for (int j = 0; j < 9; j++) {
			putimagePNG(points[j].x - x_min + x, points[j].y, &img_zm_stand[initposture[j]]);
			if (count >= 9) {
				initposture[j] = (initposture[j] + 1) % 11;//�任��һ֡
			}
		}
		if (count >= 9) {
			count = 0;
		}
		Sleep(1);
		EndBatchDraw();
	}
}

// ��Ⱦ��Ϸ����
void Update_Game() {
	
	BeginBatchDraw();	// ˫����

	// ��Ⱦ����ͼƬ
	putimage(0,0,&img_Background);

	// ��Ⱦֲ��ѡ����ͼƬ
	putimagePNG(250,0,&img_Bar);

	// ��Ⱦֲ����ֲ��ͼƬ
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

	// ���Ʒ���
	char score_text[8];
	sprintf_s(score_text, sizeof(score_text), "%d", sunshine_val);
	outtextxy(276, 67, score_text);

	// ���Ʋ�ƺ�ϵ�ֲ��
	for (int i = 0; i < max_row; i++) {
		for (int j = 0; j < max_col; j++) {
			if (map_plants[i][j].type) {
				int index = map_plants[i][j].type - 1;
				int frame_index = map_plants[i][j].frame_index;
				putimagePNG(map_plants[i][j].x, map_plants[i][j].y-20, img_map_plants[index][frame_index]);
			}
		}
	}

	// ��Ⱦ��ʬ
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

	// ��Ⱦ����
	int ballMax = sizeof(sunshine_balls) / sizeof(sunshine_balls[0]);
	for (int i = 0; i < ballMax;i ++) {
		if (sunshine_balls[i].used) {
			putimagePNG(sunshine_balls[i].cur_pos.x,sunshine_balls[i].cur_pos.y,&img_sunshine[sunshine_balls[i].frame_index]);
		}
	}

	// �����ӵ�
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

	// ��Ⱦ�϶������е�ֲ��
	if (cur_mouse_plant > 0 && !is_first_click) {
		// �����㶹�������տ�
		if (cur_mouse_plant == PEA+1 && sunshine_val >= 100) {
			IMAGE* img = img_map_plants[cur_mouse_plant - 1][0];
			putimagePNG(cur_mouse_plant_x - img->getwidth() / 2, cur_mouse_plant_y - img->getheight() / 2, img);
		}
		else if (cur_mouse_plant == SUNFLOWER + 1 && sunshine_val >= 50) {
			IMAGE* img = img_map_plants[cur_mouse_plant - 1][0];
			putimagePNG(cur_mouse_plant_x - img->getwidth() / 2, cur_mouse_plant_y - img->getheight() / 2, img);
		}
	}

	EndBatchDraw();	// ����˫����
}

// �����û����
void Handle_User_Click() {
	ExMessage msg;	// ���������Ϣ
	static int status = 0;	// ��ֹѭ��ִ��ʱ�����仯
	if (peekmessage(&msg)) {
		// ����������
		if (msg.message == WM_LBUTTONDOWN) {
			// �ж�����λ��
			if (msg.x > 338 && msg.x < 338 + 65 * PLANT_NUMS && msg.y > 6 && msg.y < 96) {
				// ѡ�е�ֲ���Ӧ���±�
				int index = (msg.x - 338) / 65;
				// ���� status,��ʾ�ѱ�ѡ��
				status = 1;
				is_first_click = 1;
				cur_mouse_plant = index + 1;
				if (cur_mouse_plant == PEA+1 && sunshine_val < 100) {
					// ���Ų���ѡ��ֲ�������
					PlaySound("res/music/cannotselect.wav", NULL, SND_FILENAME | SND_ASYNC);
				}
				else if (cur_mouse_plant == SUNFLOWER + 1 && sunshine_val < 50) {
					// ���Ų���ѡ��ֲ�������
					PlaySound("res/music/cannotselect.wav", NULL, SND_FILENAME | SND_ASYNC);
				}
				else {
					// ���ſ���ѡ��ֲ�������
					PlaySound("res/music/select.wav", NULL, SND_FILENAME | SND_ASYNC);
				}
			}
			else {
				// ����Ƿ��ռ�����
				check_collect_sunshine(&msg);
			}
		}
		// ����ƶ�
		else if (msg.message == WM_MOUSEMOVE && status == 1) {
			cur_mouse_plant_x = msg.x;
			cur_mouse_plant_y = msg.y;
			is_first_click = 0;
		}
		// ������̧��
		else if (msg.message == WM_LBUTTONUP) {
			// ������ڲ�ƺ����
			if (msg.x > 261 && msg.x < game_window_width && msg.y > 96 && msg.y < 600) {
				int row = (msg.y - 96) / 84;
				int col = (msg.x - 261) / 80;
				// ����ֵ���������
				if (sunshine_val < 100 && cur_mouse_plant == PEA + 1) return;
				if (sunshine_val < 50 && cur_mouse_plant == SUNFLOWER + 1) return;
				// �����λ��û��ֲ�����з���ֲ��
				if (map_plants[row][col].type == 0) {
					// ���ø�λ�ô���ֲ��
					// ���ŷ���ֲ�������
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

// ������Ϸ����
void Update_Game_Data() {
	
	// ���²�ƺֲ���֡��
	for (int i = 0; i < max_row;i ++) {
		for (int j = 0; j < max_col;j ++) {
			// ����ô���ֲ������ֲ���֡��
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
	// ��������
	create_sunshine();
	// �޸�����
	update_sunshine();

	// ������ʬ
	create_zm();
	// ���½�ʬ
	update_zm();

	// �����ӵ�
	create_bullet();
	// �����ӵ�
	update_bullet();

	// ��ײ���
	check_collision();
}

// ��������
void create_sunshine() {
	// ������������
	normal_create();
	// ���տ���������
	sunflower_create();
}

// �޸�����
void update_sunshine() {
	int ballMax = sizeof(sunshine_balls) / sizeof(sunshine_balls[0]);
	for (int i = 0; i < ballMax;i ++) {
		if (sunshine_balls[i].used) {
			sunshine_balls[i].frame_index = (sunshine_balls[i].frame_index + 1) % 29;
			sunshine* cur_sunshine = &sunshine_balls[i];
			// ����״̬
			if (cur_sunshine->status == SUNSHINE_DOWN) {
				cur_sunshine->t += cur_sunshine->speed;
				cur_sunshine->cur_pos = cur_sunshine->p1 + cur_sunshine->t * (cur_sunshine->p4 - cur_sunshine->p1);
				if (cur_sunshine->t >= 1) {
					cur_sunshine->status = SUNSHINE_GROUND;
					cur_sunshine->timer = 0;
				}
			} // ���״̬
			else if (cur_sunshine->status == SUNSHINE_GROUND) {
				// �������Ƿ���ʧ
				++cur_sunshine->timer;
				if (cur_sunshine->timer >= 25) {
					cur_sunshine->used = false;
				}
			}	// �ռ�״̬
			else if (cur_sunshine->status == SUNSHINE_COLLECT) {
				cur_sunshine->t += cur_sunshine->speed;
				cur_sunshine->cur_pos = cur_sunshine->p1 + cur_sunshine->t * (cur_sunshine->p4 - cur_sunshine->p1);
				if (cur_sunshine->t >= 1) {
					cur_sunshine->used = false;
					cur_sunshine->timer = 0;
					sunshine_val += 25;
				}
			}	// ����״̬
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

// ������������
void normal_create() {
	int ballMax = sizeof(sunshine_balls) / sizeof(sunshine_balls[0]);
	// ������������
	static int count = 0;
	static int product_sunshine_timer = 200;
	count ++;
	if (count >= product_sunshine_timer) {
		count = 0;
		product_sunshine_timer = 200 + (rand() % 200);
		int i = 0;
		for (; i < ballMax && sunshine_balls[i].used; i++);
		if (i < ballMax) {
			// ����Ϊʹ��״̬
			sunshine_balls[i].used = true;
			// ������֡�������Ϊ 0
			sunshine_balls[i].frame_index = 0;
			// ����ͣ������Ϊ 0
			sunshine_balls[i].timer = 0;
			// ��ָ�����״̬
			sunshine_balls[i].status = SUNSHINE_DOWN;
			// ��������ĳ�ʼλ�ú�Ŀ��λ��
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

// ���տ���������
void sunflower_create() {
	int ballMax = sizeof(sunshine_balls) / sizeof(sunshine_balls[0]);
	for (int i = 0; i < max_row;i ++) {
		for (int j = 0; j < max_col;j ++) {
			if (map_plants[i][j].type == SUNFLOWER + 1) {
				// ��������ļ�����
				++map_plants[i][j].timer;
				if (map_plants[i][j].timer >= 100) {
					map_plants[i][j].timer = 0;
					int k = 0;
					for (; k < ballMax && sunshine_balls[k].used;k ++);
					if (k < ballMax) {
						sunshine* ball = &sunshine_balls[k];
						// ���ò�����������
						ball->used = true;
						// ��������������ҵ������
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
						// ����״̬
						ball->status = SUNSHINE_PRODUCT;
						// �����ٶ�
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

// ����Ƿ��ռ�����
void check_collect_sunshine(ExMessage *msg) {
	int ballMax = sizeof(sunshine_balls) / sizeof(sunshine_balls[0]);
	int sunshine_width = img_sunshine[0].getwidth();
	int sunshine_height = img_sunshine[0].getheight();
	for (int i = 0; i < ballMax;i ++) {
		// �ж������Ƿ�ʹ����
		if (sunshine_balls[i].used && sunshine_balls[i].status != SUNSHINE_COLLECT) {
			// �ж�����λ�ú������λ�ù�ϵ
			sunshine * ball = &sunshine_balls[i];
			if (msg->x > ball->cur_pos.x + 8 && msg->x < ball->cur_pos.x + sunshine_width - 8
				&& msg->y > ball->cur_pos.y + 8 && msg->y < ball->cur_pos.y + sunshine_height - 8) {
				// ����������״̬Ϊ�ռ�״̬
				ball->status = SUNSHINE_COLLECT;
				// ��������
				PlaySound("res/music/sunshine.wav",NULL,SND_FILENAME | SND_ASYNC);
				// ���������յ�
				ball->p1 = ball->cur_pos;
				ball->p4 = vector2(262,6);
				ball->t = 0;
				int off = 16;
				float distance_start_end = dis(sunshine_balls[i].p4 - sunshine_balls[i].p1);
				// ������������ٶ�
				ball->speed = 1.0 / (distance_start_end / off);
				// �˳�ѭ��
				break;
			}
		}
	}
}

// ������ʬ
void create_zm() {
	// ������ֵĽ�ʬ���������Ľ�ʬ�����Ͳ��ٽ��д���
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
			// ���ֵĽ�ʬ���� 1
			++show_zms;
		}
	}
}

// ���½�ʬ
void update_zm() {
	int zmMax = sizeof(zms) / sizeof(zms[0]);
	// ���ƽ�ʬ����
	static int count1 = 0;
	++count1;
	// ���ƽ�ʬͼƬ֡
	static int count2 = 0;
	++count2;
	if (count1 >= 3) {
		count1 = 0;
		for (int i = 0; i < zmMax; i++) {
			if (zms[i].used) {
				zms[i].x -= zms[i].speed;
				// ��Ϸʧ��
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

// �����ӵ�
void create_bullet() {
	// �ж�ÿ���Ƿ��н�ʬ
	bool lines[6] = { 0 };
	int shoot_dis = game_window_width - img_zm_walk[0].getwidth() + 40;
	int zmMax = sizeof(zms) / sizeof(zms[0]);
	for (int i = 0; i < zmMax;i ++) {
		if (zms[i].used && zms[i].x <= shoot_dis) {
			lines[zms[i].row] = 1;
		}
	}
	// ��ͼ�ϵ��㶹���Խ������
	for (int i = 0; i < max_row;i ++) {
		for (int j = 0; j < max_col;j ++) {
			// ����㶹�������ڵ����н�ʬ
			if (map_plants[i][j].type == PEA+1 && lines[i]) {
				++map_plants[i][j].timer;
				// �����ӵ��ķ���
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

// �����ӵ�
void update_bullet() {
	int bulletMax = sizeof(bullets) / sizeof(bullets[0]);
	for (int i = 0; i < bulletMax; i++) {
		if (bullets[i].used) {
			bullets[i].x += bullets[i].speed;
			if (bullets[i].x >= game_window_width) {
				bullets[i].used = false;
			}
			// ����ӵ�������ը
			if (bullets[i].blast) {
				bullets[i].frame_index++;
				if (bullets[i].frame_index >= 4) {
					bullets[i].used = false;
				}
			}
		}
	}
}

// ��ײ���
void check_collision() {
	// �ӵ��ͽ�ʬ����ײ
	collision_bullet_zm();
	// ��ʬ��ֲ�����ײ
	collision_zm_plant();
}

// �ӵ��ͽ�ʬ����ײ
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
			// ������ͬ�ҷ�����ײ
			if (bullets[k].row == zms[j].row && bullets[k].x >= x1 && bullets[k].x <= x2) {
				// ������ײ����
				PlaySound("res/music/peacrush1.wav", NULL, SND_FILENAME | SND_ASYNC);
				// ��ʬѪ���½�
				zms[j].blood -= 10;
				bullets[k].blast = true;
				bullets[k].frame_index = 0;
				bullets[k].speed = 0;
				// ��ʬ����
				if (zms[j].blood <= 0) {
					zms[j].dead = true;
					zms[j].frame_index = 0;
					zms[j].speed = 0;
					++kill_zms;
					// ��ɱ��ʬ���������ֵ����Ϸͨ��
					if (kill_zms >= max_zms) {
						game_status = WIN;
					}
				}
				// �����һ���ӵ�
				break;
			}
		}
	}
}

// ��ʬ��ֲ�����ײ
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
				// ֲ��ͽ�ʬ��ײ
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
								// ֲ�ﱻ�Ե�����
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

// �����Ϸ�Ƿ����
bool check_game_over() {
	bool ret = false;
	if (game_status == WIN) {
		Sleep(1000);
		// �ر���Ϸ����
		mciSendString("close res/music/UraniwaNi.mp3 ", 0, 0, 0);

		loadimage(0, "res/gameWin.png");
		
		// ����ʤ������
		mciSendString("play res/music/win.mp3", 0, 0, 0);
		
		ret = true;
	}
	else if (game_status == FAIL) {
		Sleep(1000);

		// �ر���Ϸ����
		mciSendString("close res/music/UraniwaNi.mp3 ", 0, 0, 0);

		loadimage(0,"res/fail2.png");

		// ����ʧ������
		mciSendString("play res/music/lose.mp3", 0, 0, 0);
		
		ret = true;
	}
	return ret;
}

// ֲ�����½�
void Game_Bar_Down() {
	// �ر�ƬͷѲ������
	mciSendString("close res/music/Kitanai_Sekai.mp3", 0, 0, 0);
	int height = img_Bar.getheight();
	for (int y = -height; y <= 0; y += 2){
		BeginBatchDraw();
		putimage(0, 0, &img_Background);
		putimagePNG(250, 0, &img_Bar);
		// ���Ʒ���
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