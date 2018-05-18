#pragma once

#include "extmem.h"

typedef struct INDEX_ITEM {
	int index;
	int blk_num;
}index_item;

int init(Buffer*);