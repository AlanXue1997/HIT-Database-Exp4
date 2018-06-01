#include <stdlib.h>
#include <stdio.h>
#include "extmem.h"
#include "utils.h"

#define MAX_N 300
 
/*
	About Search
*/
void link_search(Buffer &buf) {
	FILE *link_search_output = fopen("link_search.txt", "w");
	int p;
	unsigned char *blk;

	FILE *r_link_index = fopen("../data/r_link.index", "r");
	fscanf(r_link_index, "%d", &p);
	fclose(r_link_index);
	while ((blk = readBlockFromDisk(p, &buf)) != NULL)
	{
		int *arr = (int*)blk;
		for (int i = 0; i < 7; ++i) {
			if (arr[i * 2] == 40) {
				fprintf(link_search_output, "%d %d\n", arr[i * 2], arr[i * 2 + 1]);
			}
		}
		p = arr[2 * 7];
		freeBlockInBuffer(blk, &buf);
	}

	fprintf(link_search_output, "\n");

	FILE *s_link_index = fopen("../data/s_link.index", "r");
	fscanf(s_link_index, "%d", &p);
	fclose(s_link_index);
	while ((blk = readBlockFromDisk(p, &buf)) != NULL)
	{
		int *arr = (int*)blk;
		for (int i = 0; i < 7; ++i) {
			if (arr[i * 2] == 60) {
				fprintf(link_search_output, "%d %d\n", arr[i * 2], arr[i * 2 + 1]);
			}
		}
		p = arr[2 * 7];
		freeBlockInBuffer(blk, &buf);
	}

	fclose(link_search_output);
}

void binary_search(Buffer &buf) {
	FILE *binary_search_output = fopen("binary_search.txt", "w");
	int p[100];
	int v[100];
	int len, left, right, middle;
	unsigned char *blk;

	FILE *r_link_index = fopen("../data/r_binary.index", "r");
	len = 0;
	while(fscanf(r_link_index, "%d%d", &v[len], &p[len]) == 2)len++;
	len++;
	fclose(r_link_index);
	left = 0;
	right = len - 1;
	middle = (left + right) / 2;
	while (left < right - 1) {
		if (v[middle] < 40) {
			left = middle;
		}
		else {
			right = middle;
		}
		middle = (left + right) / 2;
	}
	while (v[right] == 40)right++;
	while (left < right && (blk = readBlockFromDisk(p[left], &buf)) != NULL)
	{
		int *arr = (int*)blk;
		for (int i = 0; i < 7; ++i) {
			if (arr[i * 2] == 40) {
				fprintf(binary_search_output, "%d %d\n", arr[i * 2], arr[i * 2 + 1]);
			}
		}
		freeBlockInBuffer(blk, &buf);
		left++;
	}

	fprintf(binary_search_output, "\n");

	FILE *s_link_index = fopen("../data/s_binary.index", "r");
	len = 0;
	while (fscanf(s_link_index, "%d%d", &v[len], &p[len])==2)len++;
	len++;
	fclose(s_link_index);
	left = 0;
	right = len - 1;
	middle = (left + right) / 2;
	while (left < right - 1) {
		if (v[middle] < 60) {
			left = middle;
		}
		else {
			right = middle;
		}
		middle = (left + right) / 2;
	}
	while (v[right] == 60)right++;
	while (left < right && (blk = readBlockFromDisk(p[left], &buf)) != NULL)
	{
		int *arr = (int*)blk;
		for (int i = 0; i < 7; ++i) {
			if (arr[i * 2] == 60) {
				fprintf(binary_search_output, "%d %d\n", arr[i * 2], arr[i * 2 + 1]);
			}
		}
		freeBlockInBuffer(blk, &buf);
		left++;
	}

	fclose(binary_search_output);
}

void tree_search(Buffer &buf) {
	FILE *tree_search_output = fopen("tree_search.txt", "w");
	int p;
	unsigned char *blk;
	int *arr;

	FILE *r_tree_index = fopen("../data/r_tree.index", "r");
	fscanf(r_tree_index, "%d", &p);
	fclose(r_tree_index);
	blk = readBlockFromDisk(p, &buf);
	arr = (int*)blk;
	while (arr[0] == 0) {
		int k = 0;
		while (k < 7 && arr[k * 2 + 2] < 40)k++;
		p = arr[k * 2 - 1];
		freeBlockInBuffer(blk, &buf);
		blk = readBlockFromDisk(p, &buf);
		arr = (int*)blk;
	}

	do {
		p = arr[15];
		int flag = 0;
		while (flag < 7 && arr[flag * 2 + 2] <= 40) {
			unsigned char *sub_blk = readBlockFromDisk(arr[flag * 2 + 1], &buf);
			if (sub_blk == NULL)break;
			int *sub_arr = (int*)sub_blk;
			for (int i = 0; i < 7; ++i) {
				if (sub_arr[i * 2] == 40) {
					fprintf(tree_search_output, "%d %d\n", sub_arr[i * 2], sub_arr[i * 2 + 1]);
				}
			}
			freeBlockInBuffer(sub_blk, &buf);
			flag++;
		}
		freeBlockInBuffer(blk, &buf);
		blk = readBlockFromDisk(p, &buf);
		arr = (int*)blk;
	} while (blk != NULL);

	fprintf(tree_search_output, "\n");

	FILE *s_tree_index = fopen("../data/s_tree.index", "r");
	fscanf(s_tree_index, "%d", &p);
	fclose(s_tree_index);
	blk = readBlockFromDisk(p, &buf);
	arr = (int*)blk;
	while (arr[0] == 0) {
		int k = 0;
		while (k < 7 && arr[k * 2 + 2] < 60)k++;
		p = arr[k * 2 - 1];
		freeBlockInBuffer(blk, &buf);
		blk = readBlockFromDisk(p, &buf);
		arr = (int*)blk;
	}

	do {
		p = arr[15];
		int flag = 0;
		while (flag < 7 && arr[flag * 2 + 2] <= 60) {
			unsigned char *sub_blk = readBlockFromDisk(arr[flag * 2 + 1], &buf);
			if (sub_blk == NULL)break;
			int *sub_arr = (int*)sub_blk;
			for (int i = 0; i < 7; ++i) {
				if (sub_arr[i * 2] == 60) {
					fprintf(tree_search_output, "%d %d\n", sub_arr[i * 2], sub_arr[i * 2 + 1]);
				}
			}
			freeBlockInBuffer(sub_blk, &buf);
			flag++;
		}
		freeBlockInBuffer(blk, &buf);
		blk = readBlockFromDisk(p, &buf);
		arr = (int*)blk;
	} while (blk != NULL);

	fclose(tree_search_output);
}

void project(Buffer &buf) {
	FILE *project_output = fopen("project.txt", "w");
	int p;
	int output[MAX_N];
	int len = 0;
	unsigned char *blk;

	FILE *r_link_index = fopen("../data/r_link.index", "r");
	fscanf(r_link_index, "%d", &p);
	fclose(r_link_index);
	while ((blk = readBlockFromDisk(p, &buf)) != NULL)
	{
		int *arr = (int*)blk;
		for (int i = 0; i < 7; ++i) {
			output[len++] = arr[i * 2];
			//fprintf(link_search_output, "%d %d\n", arr[i * 2], arr[i * 2 + 1]);
		}
		p = arr[2 * 7];
		freeBlockInBuffer(blk, &buf);
	}

	buble_sort(output, len);

	fprintf(project_output, "%d\n", output[0]);
	for (int i = 1; i < len; ++i) {
		if (output[i] != output[i - 1]) {
			fprintf(project_output, "%d\n", output[i]);
		}
	}

	fclose(project_output);
}

void nest_loop_join(Buffer &buf) {
	FILE *nest_output = fopen("nest_loop_join.txt", "w");
	int p;
	unsigned char *blk;

	FILE *r_link_index = fopen("../data/r_link.index", "r");
	fscanf(r_link_index, "%d", &p);
	fclose(r_link_index);
	while ((blk = readBlockFromDisk(p, &buf)) != NULL)
	{
		int *arr = (int*)blk;
		for (int i = 0; i < 7; ++i) {//arr[i * 2], arr[i * 2 + 1]
			FILE *s_link_index = fopen("../data/s_link.index", "r");
			int p2;
			unsigned char *blk2;
			fscanf(s_link_index, "%d", &p2);
			fclose(s_link_index);
			while ((blk2 = readBlockFromDisk(p2, &buf)) != NULL)
			{
				int *arr2 = (int*)blk2;
				for (int i2 = 0; i2 < 7; ++i2) {
					if (arr2[i2 * 2] == arr[i * 2]) {
						fprintf(nest_output, "%d %d %d %d\n", arr[i * 2], arr[i * 2 + 1], arr2[i2 * 2], arr2[i2 * 2 + 1]);
					}
				}
				p2 = arr2[2 * 7];
				freeBlockInBuffer(blk2, &buf);
			}
		}
		p = arr[2 * 7];
		freeBlockInBuffer(blk, &buf);
	}

	fclose(nest_output);
}
/*
void sort_merge_join(Buffer &buf) {
	FILE *sort_output = fopen("sort_merge_join.txt", "w");
	int p, p2;
	unsigned char *blk, *blk2;
	int *arr, *arr2;
	int i, i2;

	FILE *r_link_index = fopen("../data/r_link.index", "r");
	fscanf(r_link_index, "%d", &p);
	fclose(r_link_index);
	FILE *s_link_index = fopen("../data/s_link.index", "r");
	fscanf(s_link_index, "%d", &p2);
	fclose(s_link_index);
	if ((blk = readBlockFromDisk(p, &buf)) != NULL) {
		arr = (int*)blk;
		i = 0;
	}
	else {
		return;
	}
	if ((blk2 = readBlockFromDisk(p2, &buf)) != NULL) {
		arr2 = (int*)blk2;
		i2 = 0;
	}
	else {
		return;
	}


	while ()
	{
		for (int i = 0; i < 7; ++i) {//arr[i * 2], arr[i * 2 + 1]
			
			
			while ((blk2 = readBlockFromDisk(p2, &buf)) != NULL)
			{
				int *arr2 = (int*)blk2;
				for (int i2 = 0; i2 < 7; ++i2) {
					if (arr2[i2 * 2] == arr[i * 2]) {
						fprintf(nest_output, "%d %d %d %d\n", arr[i * 2], arr[i * 2 + 1], arr2[i2 * 2], arr2[i2 * 2 + 1]);
					}
				}
				p2 = arr2[2 * 7];
				freeBlockInBuffer(blk2, &buf);
			}
		}
		p = arr[2 * 7];
		freeBlockInBuffer(blk, &buf);
	}

	fclose(nest_output);
}
*/
int main(int argc, char **argv)
{
	Buffer buf;

	if (!initBuffer(520, 64, &buf)) {
		perror("Buffer Initialization Failed!\n");
		return -1;
	}

	//link_search(buf);
	//binary_search(buf);
	//tree_search(buf);
	
	//project(buf);

	nest_loop_join(buf);

	return 0;
}

