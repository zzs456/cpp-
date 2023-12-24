#ifndef BLOCK_H_H
#define BLOCK_H_H
#include <graphics.h>
#include <vector>

// 组成方块的小方块结构体
struct Point {
	int row;	// 所在行
	int col;	// 所在列
};

class Block
{
// 公共成员方法
public:
	// 默认构造函数
	Block() = default;

	// 传递种类参数的构造函数
	Block(int block_type);

	// 拷贝赋值
	Block& operator=(const Block&);

	// 控制方块下降函数
	void block_drop();

	// 方块移动函数
	void block_move(int);

	// 方块旋转
	void block_rotate();

	// 判断位置合法
	bool check_pos(std::vector<std::vector<int>> &maps);

	// 固定方块位置
	void block_fix(std::vector<std::vector<int>>& maps);

	// 绘制方块函数 left_margin:左边界 top_margin:上边界
	void block_draw(int left_margin,int top_margin);

	// 获取类型
	int get_blockType()const { return this->block_type; }

	// 获取静态图片
	static IMAGE** get_blockImgs() { return block_imgs; }

// 私有成员方法
private:

// 私有成员变量
public:
	int block_type;		// 小方块种类
	Point small_blocks[4];	// 组成方块的 4 个小方块
	// IMAGE* block_img;		// 方块图片

// 静态变量
private:
	static int block_size;	// 小方块的大小
	static IMAGE* block_imgs[7];	// 7 种小方块图片
};


#endif // !BLOCK_H_H