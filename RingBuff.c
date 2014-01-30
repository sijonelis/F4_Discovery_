#include "RingBuff.h"
#include <stdio.h>
#include <stdarg.h>

/////////////////////////////////////////////////////
// リングバッファセットアップ処理
void setupRingBuff(
		RingBuff *psrRingBuff
		,unsigned char *paucBuff
		,unsigned short unBuffSize
		,ControlFuncPtr pControlFunc)
{	//
	psrRingBuff->paucBuff 		= paucBuff;
	psrRingBuff->unBuffSize 	= unBuffSize;
	psrRingBuff->unWrPoint 		= 0;
	psrRingBuff->unRdPoint 		= 0;
	psrRingBuff->unCount		= 0;
	psrRingBuff->pControlFunc 	= pControlFunc;
	psrRingBuff->enumCltrlState	= DISABLE;
}

/////////////////////////////////////////////////////
// get data from ring buffer.
unsigned short getFromRingBuff(
				RingBuff *psrRingBuff
				,unsigned char *paucBuff
				,unsigned short unSize)
{	//
	unsigned short unCount = 0;
	unsigned short i;
	//
	if ((psrRingBuff == NULL) || (paucBuff == NULL) || (unSize == 0)) {
		return unCount;
	}
	//
	if (psrRingBuff->pControlFunc != NULL){
		psrRingBuff->pControlFunc(DISABLE);
	}
    //
    unCount = unSize < psrRingBuff->unCount ? unSize : psrRingBuff->unCount;
    if (unCount > 0){
    	for (i = 0; i < unCount; i++){
    		paucBuff[i] = psrRingBuff->paucBuff[psrRingBuff->unRdPoint++];
    		if (psrRingBuff->unRdPoint >= psrRingBuff->unBuffSize){
    			psrRingBuff->unRdPoint = 0;
    		}
    	}
    	psrRingBuff->unCount -= unCount;
    }
    //
	if (psrRingBuff->pControlFunc != NULL){
		psrRingBuff->pControlFunc(ENABLE);
	}
    //
	return unCount;
}

/////////////////////////////////////////////////////
// set data to ring buffer.
unsigned short setToRingBuff(
					RingBuff *psrRingBuff
					,unsigned char *paucBuff
					,unsigned short unSize)
{	//
	unsigned short unCount = 0;
	unsigned short i, unSpaceCount;
	//
	if ((psrRingBuff == NULL) || (paucBuff == NULL) || (unSize == 0)) {
		return unCount;
	}
	//
	if (psrRingBuff->pControlFunc != NULL){
		psrRingBuff->pControlFunc(DISABLE);
	}
    //
    unSpaceCount = psrRingBuff->unBuffSize - psrRingBuff->unCount;
    unCount = unSize < unSpaceCount ? unSize : unSpaceCount;
    if (unCount > 0){
    	for (i = 0; i < unCount; i++){
    		psrRingBuff->paucBuff[psrRingBuff->unWrPoint++] = paucBuff[i];
    		if (psrRingBuff->unWrPoint >= psrRingBuff->unBuffSize){
    			psrRingBuff->unWrPoint = 0;
    		}
    	}
    	psrRingBuff->unCount += unCount;
    }
    //
	if (psrRingBuff->pControlFunc != NULL){
		psrRingBuff->pControlFunc(ENABLE);
	}
    //
	return unCount;
}

/////////////////////////////////////////////////////
// clear ring buffer
////////////////////////////////////////////////////
void clearRingBuff(RingBuff *psrRingBuff)
{	//
	if (psrRingBuff == NULL) {
		return;
	}
	//
	if (psrRingBuff->pControlFunc != NULL){
		psrRingBuff->pControlFunc(DISABLE);
	}
    //
	psrRingBuff->unCount = 0;
	psrRingBuff->unRdPoint = 0;
	psrRingBuff->unWrPoint = 0;
    //
	if (psrRingBuff->pControlFunc != NULL){
		psrRingBuff->pControlFunc(ENABLE);
	}
}


