#include "Tetris.h"
#include <time.h>

const int rows = 20;
const int cols = 10;
const int left_margin = 263;
const int top_margin = 19;
const int block_size = 36;


int main() {

	// 创建对象
	Tetris game(rows,cols,left_margin,top_margin,block_size);
	game.play();
	return 0;
}