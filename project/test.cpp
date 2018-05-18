/*
* test.c
* Zhaonian Zou
* Harbin Institute of Technology
* Jun 22, 2011
*/

#include <stdlib.h>
#include <stdio.h>
#include "extmem.h"
#include "init.h"

int main(int argc, char **argv)
{
	Buffer buf; /* A buffer */
	init(&buf);
	
	unsigned char *blk; /* A pointer to a block */
	int i = 0;

	/* Get a new block in the buffer */
	blk = getNewBlockInBuffer(&buf);

	/* Fill data into the block */
	*(int*)blk = 0xFF00FF00;
	*(int*)(blk+4) = 0xEECCEECC;

	/* Write the block to the hard disk */
	if (writeBlockToDisk(blk, 31415926, &buf) != 0)
	{
		perror("Writing Block Failed!\n");
		return -1;
	}

	/* Read the block from the hard disk */
	if ((blk = readBlockFromDisk(31415926, &buf)) == NULL)
	{
		perror("Reading Block Failed!\n");
		return -1;
	}

	/* Process the data in the block */
	for (i = 0; i < 8; i++)
		printf("%c", *(blk + i));

	printf("\n");
	printf("# of IO's is %d\n", buf.numIO); /* Check the number of IO's */

	return 0;
}

