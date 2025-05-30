/*
 * Array
 *
 *  Created on: 28 Jun 2023
 *      Author: kisha
 */

#ifndef INC_ARRAY_H_
#define INC_ARRAY_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Array {

	uint8_t* Array;
	size_t used;
	size_t size;
};

void initArray(struct Array* a, int size);
void push_back(struct Array* a, uint8_t element);
void zeroArray(struct Array* a);
void destroy(struct Array* a);


#endif /* INC_ARRAY_H_ */
