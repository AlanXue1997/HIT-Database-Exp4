#include "init.h"

#include <stdio.h>

#define ARRAY_LEN(A) (sizeof(A)/sizeof(A[0]))
#define SWAP(x,y,temp) temp=x,x=y,y=temp
#define SWAP(x,y) x^=y^=x^=y

#define MAX_LENGTH 300

index_item r_items[MAX_LENGTH];
index_item s_items[MAX_LENGTH];
int r_length, s_length;
int block_index = 0x0F0F0F0F;

void sort_index(index_item* items, int len) {
	for (int i = 0; i < len; ++i) {
		for (int j = i+1; j < len; ++j) {
			if (items[i].index > items[j].index) {
				SWAP(items[i].index, items[j].index);
				SWAP(items[i].blk_num, items[j].blk_num);
			}
		}
	}
}
/*
void get_data(Buffer* buf) {
	FILE *input = fopen("input.txt", "r");
	fscanf(input, "%d", &r_length);
	for (int i = 0; i < r_length; ++i) {
		int a, b;
		fscanf(input, "%d%d", &a, &b);
		int blk_num = save_record(a, b, buf);
		r_items[i].index = a;
		r_items[i].blk_num = blk_num;
	}
	fscanf(input, "%d", &s_length);
	for (int i = 0; i < s_length; ++i) {
		int c, d;
		fscanf(input, "%d%d", &c, &d);
		int blk_num = save_record(c, d, buf);
		s_items[i].index = c;
		s_items[i].blk_num = blk_num;
	}
	fclose(input);
}*/

void generate_data(Buffer *buf) {
	r_length = 112;
	unsigned char* blk;
	int *arr = NULL;
	for (int i = 0; i < r_length; ++i) {
		//这时需要新建一个block
		if (i % 7 == 0) {
			blk = getNewBlockInBuffer(buf);
			arr = (int*)blk;
		}
		*arr++ = i+1;
		*arr++ = (i+1)<<4;
		//这是block已经存满，需要被切换
		if (i % 7 == 6) {
			*arr = block_index + 1;
			writeBlockToDisk(blk, block_index++, buf);
		}
	}
	
}

int init(Buffer* buf) {

	/* Initialize the buffer */
	if (!initBuffer(520, 64, buf))
	{
		perror("Buffer Initialization Failed!\n");
		return -1;
	}

	generate_data(buf);
	/*
	sort_index(r_items, r_length);
	FILE *SIndex = fopen("S.INDEX", "w");
	for (int i = 0; i < s_length; ++i) {
		fprintf(SIndex, "%d %d\n", s_items[i].index, s_items[i].blk_num);
	}
	fclose(SIndex);
	*/
	return 0;
}