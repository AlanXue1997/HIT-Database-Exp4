#include "utils.h"

#define SWAP(x,y) x^=y^=x^=y

void buble_sort(int *a, int len) {
	for (int i = 0; i < len; ++i) {
		for (int j = i; j < len; ++j) {
			if (a[i] > a[j]) {
				SWAP(a[i], a[j]);
			}
		}
	}
}