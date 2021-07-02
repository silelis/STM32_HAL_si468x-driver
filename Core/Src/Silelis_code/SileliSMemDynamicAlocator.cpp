#include "SileliSMemDynamicAlocator.h"
#include "string.h"

/*//    EXAMPLE HOW TO USE IT:

   uint8_t* pPointer;		//pointer to memory address where dynamically
   	   	   	   	   	   	    //			allocated array is stored
   uint8_t size;			// variable which store dynamically  allocated array size

   clearData((uint8_t**)&pPointer, (uint8_t*)&size);			//clean data (i.e. at the class constructor) pPointer = nullptr, size = 0
   bool result =createArray((uint8_t**)&pPointer, (uint8_t*)&size, 12);	//allocate array, update stored in "pPointer" array address, update stored in "size" allocated array size
   destroyArray((uint8_t**)&pPointer, (uint8_t*)&size);			//delete array located at "pPointer". Because it also change pPointer value (to 0) and size value (to 0) those variables ar given as pointer to pointer (for pPointer) and as pointer (for size)


 *///    EXAMPLE HOW TO USE IT:




/*
 * uint8_t** pointer - pointer to variable which is pointer (store addres) to allocated array
 * uint8_t *arraySize - store size of allocated data
 */
void clearData(uint8_t** pointer, uint8_t *arraySize){
	*pointer = nullptr;		//change value of variable which store address to allocated array
	*arraySize =0;
//	return nullptr;
}


/*
 * uint8_t** pointer - pointer to variable which is pointer (store addres) to allocated array
 * uint8_t *arraySize - store size of allocated data
 * uint8_t allocationSize - amound of data which will be allocated
 */
bool createArray(uint8_t** ppointer, uint8_t* arraySize, uint8_t allocationSize){
		destroyArray(ppointer, arraySize);
		*ppointer  = new uint8_t [allocationSize];		//change value of variable which store address to allocated array
//		memset(*ppointer, 'A', allocationSize);	//test function
		if (*ppointer != nullptr){
			*arraySize = allocationSize;				//change value of allocated data variable
			return true;
		}
		return false;
}

void destroyArray(uint8_t** ppointer, uint8_t* arraySize){
	if (*ppointer != nullptr){
		delete [] *ppointer;			//delete array which is pointed by variable which store array addres
	}
	clearData(ppointer, arraySize);
}











/*
 * uint8_t** pointer - pointer to variable which is pointer (store addres) to allocated array
 * uint8_t *arraySize - store size of allocated data
 */
void clearData(uint8_t** pointer, uint16_t *arraySize){
	*pointer = nullptr;		//change value of variable which store address to allocated array
	*arraySize =0;
//	return nullptr;
}


/*
 * uint8_t** pointer - pointer to variable which is pointer (store addres) to allocated array
 * uint8_t *arraySize - store size of allocated data
 * uint8_t allocationSize - amound of data which will be allocated
 */
bool createArray(uint8_t** ppointer, uint16_t* arraySize, uint16_t allocationSize){
		destroyArray(ppointer, arraySize);
		*ppointer  = new uint8_t [allocationSize];		//change value of variable which store address to allocated array
//		memset(*ppointer, 'A', allocationSize);	//test function
		if (*ppointer != nullptr){
			*arraySize = allocationSize;				//change value of allocated data variable
			return true;
		}
		return false;
}

void destroyArray(uint8_t** ppointer, uint16_t* arraySize){
	if (*ppointer != nullptr){
		delete [] *ppointer;			//delete array which is pointed by variable which store array addres
	}
	clearData(ppointer, arraySize);
}































/*
 * uint32_t** pointer - pointer to variable which is pointer (store addres) to allocated array
 * uint8_t *arraySize - store size of allocated data
 */
void clearData(uint32_t** pointer, uint8_t *arraySize){
	*pointer = nullptr;		//change value of variable which store address to allocated array
	*arraySize =0;
//	return nullptr;
}


/*
 * uint32_t** ppointer - pointer to variable which is pointer (store addres) to allocated array
 * uint8_t *arraySize - store size of allocated data
 * uint8_t allocationSize - amound of data which will be allocated
 */
bool createArray(uint32_t** ppointer, uint8_t* arraySize, uint8_t allocationSize){
		destroyArray(ppointer, arraySize);
		*ppointer  = new uint32_t [allocationSize];		//change value of variable which store address to allocated array
//		memset(*ppointer, 'A', allocationSize);	//test function
		if (*ppointer != nullptr){
			*arraySize = allocationSize;				//change value of allocated data variable
			return true;
		}
		return false;
}

void destroyArray(uint32_t** ppointer, uint8_t* arraySize){
	if (*ppointer != nullptr){
		delete [] *ppointer;			//delete array which is pointed by variable which store array addres
	}
	clearData(ppointer, arraySize);
}
