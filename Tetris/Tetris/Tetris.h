#ifndef TETRIS_H_
#define TETRIS_H_H
#include <vector>
#include <time.h>
#include "Block.h"
using std::vector;

class Tetris
{
// 公共成员方法
public:
	Tetris() = default;	// 默认构造函数
	
	// 构造函数 rows:行数 cols:列数 left_margin:左边界 top_margin:上边界 block_size:方块大小
	Tetris(int rows,int cols,int left_margin,int top_margin,int block_size);

	// 初始化函数
	void init();

	// 游戏运行函数
	void play();

// 私有成员方法
private:

	// 开始动画
	void start_animation();

	// 读取最高分
	void cin_highestScore();

	// 监听按键
	void key_event();

	// 获取延时
	int get_delay();

	// 更新游戏界面
	void update_game_UI();

	// 更新游戏数据
	void update_game_data();

	// 绘制地图
	void draw_map();

	// 更新地图
	void update_map();

	// 更新小方块
	void update_block();

	// 小方块自由下降
	void drop_block();

	// 小方块旋转
	void rotate_block();

	// 小方块左右移动
	void move_block(int);

	// 绘制数字
	void draw_Nums();

	// 检查游戏是否结束
	void check_gameOver();

	// 保存分数
	void save_score();

	// 游戏结束画面
	void display_gameOver();

// 私有成员变量
private:
	int delay;	// 控制渲染频率
	bool update;	// 控制是否进行渲染
	Block* cur_block;	// 当前方块
	Block* next_block;	// 下一个方块预告
	Block bake_block;	// 当前方块的副本

	IMAGE img_bg;	// 背景图片
	IMAGE img_gameOver; // 游戏结束图片

	bool gameOver;
	int score;	// 游戏分数
	int level;	// 关卡
	int highest_score;	// 最高分
	int linesCount;	// 消除的行数
	int rows;	// 游戏行数
	int cols;	// 游戏列数
	int block_size;	// 小方块的大小
	int left_margin;	// 左边距
	int top_margin;		// 上边距
	vector<vector<int>> map_blocks;	// 记录每个位置的小方块种类，其中 0 代表没有方块

	static IMAGE* start_imgs[80];
};

#endif // !TETRIS_H_

