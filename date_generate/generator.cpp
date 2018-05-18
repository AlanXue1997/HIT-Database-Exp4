#include <stdio.h>

#include "../project/extmem.h"

int main() {
	Buffer buf;
	if (!initBuffer(520, 64, &buf))
	{
		perror("Buffer Initialization Failed!\n");
		return -1;
	}
	int r_length = 112;
	int s_length = 224;
	int block_index = 0x0FFFFF00;
	unsigned char* blk = NULL;
	int *arr = NULL;
	for (int i = 0; i < r_length; ++i) {
		//这时需要新建一个block
		if (i % 7 == 0) {
			blk = getNewBlockInBuffer(&buf);
			arr = (int*)blk;
		}
		*arr++ = i % 40 + 1;
		*arr++ = i + 1;
		//这是block已经存满，需要被切换
		if (i % 7 == 6) {
			*arr = block_index + 1;
			writeBlockToDisk(blk, block_index++, &buf);
			freeBlockInBuffer(blk, &buf);
		}
	}
	for (int i = 0; i < s_length; ++i) {
		//这时需要新建一个block
		if (i % 7 == 0) {
			blk = getNewBlockInBuffer(&buf);
			arr = (int*)blk;
		}
		*arr++ = i % 41 + 20;
		*arr++ = i + 1;
		//这是block已经存满，需要被切换
		if (i % 7 == 6) {
			*arr = block_index + 1;
			writeBlockToDisk(blk, block_index++, &buf);
			freeBlockInBuffer(blk, &buf);
		}
	}
}