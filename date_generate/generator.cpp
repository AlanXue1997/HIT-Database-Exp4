#include <stdio.h>
#include <stdlib.h>

#include "../project/extmem.h"

#define MAX_LENGTH 300
#define SWAP(x,y) x^=y^=x^=y

//#define LINK_INDEX
//#define BINARY_INDEX
#define B_TREE_INDEX

int tree_index = 0x00CCCC00;

typedef struct RET {
	int min;
	int index;
	unsigned char *blk;
}ret;

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

ret build_tree(int *a, int *b, int len, Buffer &buf) {
	unsigned char* blk;
	int *arr;
	if (len <= 7) {
		blk = getNewBlockInBuffer(&buf);
		arr = (int*)blk;
		arr[0] = 0xFFFFFFFF;
		for (int i = 0; i < len; ++i) {
			arr[i * 2 + 2] = a[i];
			arr[i * 2 + 1] = b[i];
		}
		arr[15] = 0;
		return ret{ arr[2], tree_index++, blk };
	}
	else {
		int n = 8;
		blk = getNewBlockInBuffer(&buf);
		arr = (int*)blk;
		arr[0] = 0;
		ret ans, r;
		int k = 0;
		unsigned char *last_blk = NULL;
		int last_index = 0;
		while (n > 0) {
			int l = len / n;
			r = build_tree(a, b, l, buf);
			int *temp = (int*)r.blk;
			if (last_blk != NULL) {
				if (temp[0] != 0) {
					*(((int*)last_blk) + 15) = r.index;
				}
				writeBlockToDisk(last_blk, last_index, &buf);
				freeBlockInBuffer(last_blk, &buf);
			}
			last_blk = r.blk;
			last_index = r.index;
			if (k == 0) {
				ans.min = r.min;
				arr[1] = r.index;
			}
			else {
				arr[k * 2] = r.min;
				arr[k * 2 + 1] = r.index;
			}
			a += l;
			b += l;
			len -= l;
			n--;
			k++;
		}
		writeBlockToDisk(r.blk, r.index, &buf);
		freeBlockInBuffer(r.blk, &buf);
		ans.index = tree_index++;
		ans.blk = blk;
		return ans;
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
#ifdef B_TREE_INDEX
	//建立B树需要的变量
	int indexes[MAX_LENGTH];
	int values[MAX_LENGTH];
	int b_tree_flag;
	ret root;
#endif
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
#ifdef B_TREE_INDEX
	b_tree_flag = 0;
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
#ifdef B_TREE_INDEX
			values[b_tree_flag] = a[i - 6];
			indexes[b_tree_flag++] = block_index;
#endif
			*arr = i==r_length-1?0:block_index + 1;
			writeBlockToDisk(blk, block_index++, &buf);
			freeBlockInBuffer(blk, &buf);
		}
	}
#ifdef BINARY_INDEX
	fclose(r_binary_index);
#endif
#ifdef B_TREE_INDEX
	root = build_tree(values, indexes, b_tree_flag, buf);
	writeBlockToDisk(root.blk, root.index, &buf);
	freeBlockInBuffer(root.blk, &buf);
	FILE *r_tree_index = fopen("../data/r_tree.index", "w");
	fprintf(r_tree_index, "%d\n", root.index);
	fclose(r_tree_index);
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
#ifdef B_TREE_INDEX
	b_tree_flag = 0;
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
#ifdef B_TREE_INDEX
			values[b_tree_flag] = a[i - 6];
			indexes[b_tree_flag++] = block_index;
#endif
			*arr = i == s_length - 1 ? 0 : block_index + 1;
			writeBlockToDisk(blk, block_index++, &buf);
			freeBlockInBuffer(blk, &buf);
		}
	}
#ifdef BINARY_INDEX
	fclose(s_binary_index);
#endif
#ifdef B_TREE_INDEX
	root = build_tree(values, indexes, b_tree_flag, buf);
	writeBlockToDisk(root.blk, root.index, &buf);
	freeBlockInBuffer(root.blk, &buf);
	FILE *s_tree_index = fopen("../data/s_tree.index", "w");
	fprintf(s_tree_index, "%d\n", root.index);
	fclose(s_tree_index);
#endif
}