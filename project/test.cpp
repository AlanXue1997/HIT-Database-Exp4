#include <stdlib.h>
#include <stdio.h>
#include "extmem.h"
#include "init.h"

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
	fclose(link_search_output);
}

int main(int argc, char **argv)
{
	Buffer buf;

	if (!initBuffer(520, 64, &buf))
	{
		perror("Buffer Initialization Failed!\n");
		return -1;
	}

	link_search(buf);

	return 0;
}

