#include <stdio.h>
#include <stdlib.h>

#include "../project/extmem.h"

#define MAX_LENGTH 300
#define SWAP(x,y) x^=y^=x^=y

#define LINK_INDEX
#define BINARY_INDEX

void buble_sort(int *a, int *b, int len) {
	for (int i = 0; i < len; ++i) {
		for (int j = i + 1; j < len; ++j) {
			if (a[i] > a[j]) {
				SWAP(a[i], a[j]);
				SWAP(b[i], b[j]);
			}
		}
	}
}

int main() {
	srand(0);

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
	int a[MAX_LENGTH];
	int b[MAX_LENGTH];
	for (int i = 0; i < r_length; ++i) {
		a[i] = rand() % 40 + 1;
		b[i] = rand() % 1000 + 1;
	}
	buble_sort(a, b, r_length);
#ifdef LINK_INDEX
	FILE *r_link_index = fopen("../data/r_link.index", "w");
	fprintf(r_link_index, "%d\n", block_index);
	fclose(r_link_index);
#endif
#ifdef BINARY_INDEX
	FILE *r_binary_index = fopen("../data/r_binary.index", "w");
#endif
	for (int i = 0; i < r_length; ++i) {
		//这时需要新建一个block
		if (i % 7 == 0) {
			blk = getNewBlockInBuffer(&buf);
			arr = (int*)blk;
		}
		*arr++ = a[i];
		*arr++ = b[i];
		//这是block已经存满，需要被切换
		if (i % 7 == 6) {
#ifdef BINARY_INDEX
			fprintf(r_binary_index, "%d %d\n", a[i - 6], block_index);
#endif
			*arr = block_index + 1;
			writeBlockToDisk(blk, block_index++, &buf);
			freeBlockInBuffer(blk, &buf);
		}
	}
#ifdef BINARY_INDEX
	fclose(r_binary_index);
#endif

	for (int i = 0; i < s_length; ++i) {
		a[i] = rand() % 41 + 20;
		b[i] = rand() % 1000 + 1;
	}
	buble_sort(a, b, s_length);
#ifdef LINK_INDEX
	FILE *s_link_index = fopen("../data/s_link.index", "w");
	fprintf(s_link_index, "%d\n", block_index);
	fclose(s_link_index);
#endif
#ifdef BINARY_INDEX
	FILE *s_binary_index = fopen("../data/s_binary.index", "w");
#endif
	for (int i = 0; i < s_length; ++i) {
		//这时需要新建一个block
		if (i % 7 == 0) {
			blk = getNewBlockInBuffer(&buf);
			arr = (int*)blk;
		}
		*arr++ = a[i];
		*arr++ = b[i];
		//这是block已经存满，需要被切换
		if (i % 7 == 6) {
#ifdef BINARY_INDEX
			fprintf(s_binary_index, "%d %d\n", a[i - 6], block_index);
#endif
			*arr = block_index + 1;
			writeBlockToDisk(blk, block_index++, &buf);
			freeBlockInBuffer(blk, &buf);
		}
	}
#ifdef BINARY_INDEX
	fclose(s_binary_index);
#endif
}