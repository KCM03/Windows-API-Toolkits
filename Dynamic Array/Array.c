/*
 * Array.c
 *
 *  Created on: 28 Jun 2023
 *      Author: kisha
 */

#include "Array.h"

void initArray(struct Array* a, int size) {


	if (size < 1) {a->Array =  (uint8_t*)malloc(sizeof(uint8_t)); }


	else { a->Array = (uint8_t*)malloc(size * sizeof(uint8_t)); }

	a->used = 0;
	a->size = size;

	return;
}

void zeroArray(struct Array* a){
	 memset(a->Array,0,sizeof(uint8_t) * a->size);

}
void push_back(struct Array* a, uint8_t element) {

	if (a->Array == NULL) {
		initArray(a, 1);
	}

	if (a->used == a->size) {
		a->size *= 2;
		a->Array = (uint8_t*)realloc(a->Array, (a->size * sizeof(uint8_t)));
	}
	a->Array[a->used++] = element;

	return;
}

void destroy(struct Array* a) {
	free(a->Array);
	a->Array = NULL;
	a->used = a->size = 0;
	return;
}
