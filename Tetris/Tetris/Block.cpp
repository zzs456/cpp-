#include "Block.h"
#include <iostream>
// ��̬�����ĳ�ʼ��
IMAGE* Block::block_imgs[7] = { nullptr, };
int Block::block_size = 36;

Block::Block(int block_type)
{
	// ��̬�����ĸ�ֵ
	if (block_imgs[0] == nullptr) {
		IMAGE work_img;
		loadimage(&work_img, "./res/tiles.png");
		// ���ù�����
		SetWorkingImage(&work_img);
		for (int i = 0; i < 7; i++) {
			block_imgs[i] = new IMAGE;
			getimage(block_imgs[i], i * block_size, 0, block_size, block_size);
		}
		// ��ԭ������
		SetWorkingImage();
	}

	// 7 �ַ���
	int blocks[7][4] = {
		1,3,5,7, // I
		2,4,5,7, // Z 1��
		3,5,4,6, // Z 2��
		3,5,4,7, // T
		2,3,5,7, // L
		3,5,7,6, // J
		2,3,4,5, // ��
	};

	this->block_type = block_type;
	//block_img = new IMAGE;
	// ��ɸ÷����С����ͼƬ
	//block_img = block_imgs[this->block_type - 1];

	// С������к���
	for (int i = 0; i < 4; i++) {
		small_blocks[i].row = blocks[this->block_type - 1][i] / 2;
		small_blocks[i].col = blocks[this->block_type - 1][i] % 2;
	}
}

// ������ֵ
Block& Block::operator=(const Block& rhs)
{
	if (this == &rhs) return *this;
	//delete block_img;
	//this->block_img = new IMAGE;
	this->block_type = rhs.block_type;
	//*(this->block_img) = *(block_imgs[this->block_type - 1]);
	for (int i = 0; i < 4;i ++) {
		this->small_blocks[i] = rhs.small_blocks[i];
	}
	return *this;
}

// ���Ʒ����½�����
void Block::block_drop()
{
	for (int i = 0; i < 4;i ++) {
		++small_blocks[i].row;
	}
}

// �����ƶ�����
void Block::block_move(int d_col)
{
	for (int i = 0; i < 4; i++) {
		small_blocks[i].col += d_col;
	}
}

// ������ת
void Block::block_rotate()
{
	int row_flag = small_blocks[1].row;
	int col_flag = small_blocks[1].col;
	for (int i = 0; i < 4;i ++) {
		if (i == 1) continue;
		int temp_row = small_blocks[i].row;
		int temp_col = small_blocks[i].col;
		small_blocks[i].col = col_flag - temp_row + row_flag;
		small_blocks[i].row = row_flag + temp_col - col_flag;
	}
}

// �ж�λ�úϷ�
bool Block::check_pos(std::vector<std::vector<int>>& maps)
{
	auto rows = maps.size();
	auto cols = maps[0].size();
	for (int i = 0; i < 4;i ++) {
		int row = small_blocks[i].row;
		int col = small_blocks[i].col;
		if (row < 0 || col < 0 || row >= rows || col >= cols || maps[row][col]) return false;
	}
	return true;
}

// �̶�����λ��
void Block::block_fix(std::vector<std::vector<int>>& maps)
{
	for (int i = 0; i < 4; i++) {
		int row = small_blocks[i].row;
		int col = small_blocks[i].col;
		maps[row][col] = block_type;
	}
}

// ���Ʒ��麯�� left_margin:��߽� top_margin:�ϱ߽�
void Block::block_draw(int left_margin, int top_margin)
{
	for (int i = 0; i < 4;i ++) {
		// ���� x �� y
		int x = left_margin + block_size * small_blocks[i].col;
		int y = top_margin + block_size * small_blocks[i].row;
		if (small_blocks[i].col == 19) {
			std::cout << "Hello" << std::endl;
		}
		
		// ����С����
		putimage(x,y,block_imgs[block_type - 1]);
	}
}
