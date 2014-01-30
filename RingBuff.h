#ifndef _RING_BUFF_H_
#define _RING_BUFF_H_
//
#include "stm32f4xx.h"

// 排他制御関数のプロトタイプ定義
typedef void  (*ControlFuncPtr)(FunctionalState enumState);
// RingBuffer type definition
typedef struct RingBuff_t
{	//
	unsigned char 	*paucBuff;		// pointer to data buffer
	unsigned short 	unBuffSize;		// size of data buffer
	unsigned short 	unWrPoint;		// write point
	unsigned short 	unRdPoint;		// read point
	unsigned short 	unCount;		// data valid count
	ControlFuncPtr	pControlFunc;	// function pointer
	FunctionalState	enumCltrlState;	// Control State
} RingBuff;

// リングバッファのセットアップ関数
extern void setupRingBuff(
		RingBuff *psrRingBuff
		,unsigned char *paucBuff
		,unsigned short unBuffSize
		,ControlFuncPtr pControlFunc);\
// リングバッファよりデータの取得
extern unsigned short getFromRingBuff(
		RingBuff *psrRingBuff
		,unsigned char *paucBuff
		,unsigned short unSize);
// リングバッファへデータの設定
extern unsigned short setToRingBuff(
		RingBuff *psrRingBuff
		,unsigned char *paucBuff
		,unsigned short unSize);
//
extern void clearRingBuff(RingBuff *psrRingBuff);
//
#endif //_RING_BUFF_H_
