/*
 * SileliSMemDynamicAlocator.h
 *
 *  Created on: 22 sty 2021
 *      Author: dbank
 */

#ifndef SILELISMEMDYNAMICALOCATOR_H_
#define SILELISMEMDYNAMICALOCATOR_H_
#include "stdio.h"


void clearData(uint8_t** ppointer, uint8_t* arraySize);
bool createArray(uint8_t** ppointer, uint8_t* arraySize, uint8_t allocationSize);
void destroyArray(uint8_t** ppointer, uint8_t* arraySize);


void clearData(uint8_t** ppointer, uint16_t* arraySize);
bool createArray(uint8_t** ppointer, uint16_t* arraySize, uint16_t allocationSize);
void destroyArray(uint8_t** ppointer, uint16_t* arraySize);




void clearData(uint32_t** ppointer, uint8_t* arraySize);
bool createArray(uint32_t** ppointer, uint8_t* arraySize, uint8_t allocationSize);
void destroyArray(uint32_t** ppointer, uint8_t* arraySize);


#endif /* SILELISMEMDYNAMICALOCATOR_H_ */
