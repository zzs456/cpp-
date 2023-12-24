#ifndef TETRIS_H_
#define TETRIS_H_H
#include <vector>
#include <time.h>
#include "Block.h"
using std::vector;

class Tetris
{
// ������Ա����
public:
	Tetris() = default;	// Ĭ�Ϲ��캯��
	
	// ���캯�� rows:���� cols:���� left_margin:��߽� top_margin:�ϱ߽� block_size:�����С
	Tetris(int rows,int cols,int left_margin,int top_margin,int block_size);

	// ��ʼ������
	void init();

	// ��Ϸ���к���
	void play();

// ˽�г�Ա����
private:

	// ��ʼ����
	void start_animation();

	// ��ȡ��߷�
	void cin_highestScore();

	// ��������
	void key_event();

	// ��ȡ��ʱ
	int get_delay();

	// ������Ϸ����
	void update_game_UI();

	// ������Ϸ����
	void update_game_data();

	// ���Ƶ�ͼ
	void draw_map();

	// ���µ�ͼ
	void update_map();

	// ����С����
	void update_block();

	// С���������½�
	void drop_block();

	// С������ת
	void rotate_block();

	// С���������ƶ�
	void move_block(int);

	// ��������
	void draw_Nums();

	// �����Ϸ�Ƿ����
	void check_gameOver();

	// �������
	void save_score();

	// ��Ϸ��������
	void display_gameOver();

// ˽�г�Ա����
private:
	int delay;	// ������ȾƵ��
	bool update;	// �����Ƿ������Ⱦ
	Block* cur_block;	// ��ǰ����
	Block* next_block;	// ��һ������Ԥ��
	Block bake_block;	// ��ǰ����ĸ���

	IMAGE img_bg;	// ����ͼƬ
	IMAGE img_gameOver; // ��Ϸ����ͼƬ

	bool gameOver;
	int score;	// ��Ϸ����
	int level;	// �ؿ�
	int highest_score;	// ��߷�
	int linesCount;	// ����������
	int rows;	// ��Ϸ����
	int cols;	// ��Ϸ����
	int block_size;	// С����Ĵ�С
	int left_margin;	// ��߾�
	int top_margin;		// �ϱ߾�
	vector<vector<int>> map_blocks;	// ��¼ÿ��λ�õ�С�������࣬���� 0 ����û�з���

	static IMAGE* start_imgs[80];
};

#endif // !TETRIS_H_

