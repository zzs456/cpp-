#ifndef BLOCK_H_H
#define BLOCK_H_H
#include <graphics.h>
#include <vector>

// ��ɷ����С����ṹ��
struct Point {
	int row;	// ������
	int col;	// ������
};

class Block
{
// ������Ա����
public:
	// Ĭ�Ϲ��캯��
	Block() = default;

	// ������������Ĺ��캯��
	Block(int block_type);

	// ������ֵ
	Block& operator=(const Block&);

	// ���Ʒ����½�����
	void block_drop();

	// �����ƶ�����
	void block_move(int);

	// ������ת
	void block_rotate();

	// �ж�λ�úϷ�
	bool check_pos(std::vector<std::vector<int>> &maps);

	// �̶�����λ��
	void block_fix(std::vector<std::vector<int>>& maps);

	// ���Ʒ��麯�� left_margin:��߽� top_margin:�ϱ߽�
	void block_draw(int left_margin,int top_margin);

	// ��ȡ����
	int get_blockType()const { return this->block_type; }

	// ��ȡ��̬ͼƬ
	static IMAGE** get_blockImgs() { return block_imgs; }

// ˽�г�Ա����
private:

// ˽�г�Ա����
public:
	int block_type;		// С��������
	Point small_blocks[4];	// ��ɷ���� 4 ��С����
	// IMAGE* block_img;		// ����ͼƬ

// ��̬����
private:
	static int block_size;	// С����Ĵ�С
	static IMAGE* block_imgs[7];	// 7 ��С����ͼƬ
};


#endif // !BLOCK_H_H