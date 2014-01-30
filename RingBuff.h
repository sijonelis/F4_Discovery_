#ifndef _RING_BUFF_H_
#define _RING_BUFF_H_
//
#include "stm32f4xx.h"

// �r������֐��̃v���g�^�C�v��`
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

// �����O�o�b�t�@�̃Z�b�g�A�b�v�֐�
extern void setupRingBuff(
		RingBuff *psrRingBuff
		,unsigned char *paucBuff
		,unsigned short unBuffSize
		,ControlFuncPtr pControlFunc);\
// �����O�o�b�t�@���f�[�^�̎擾
extern unsigned short getFromRingBuff(
		RingBuff *psrRingBuff
		,unsigned char *paucBuff
		,unsigned short unSize);
// �����O�o�b�t�@�փf�[�^�̐ݒ�
extern unsigned short setToRingBuff(
		RingBuff *psrRingBuff
		,unsigned char *paucBuff
		,unsigned short unSize);
//
extern void clearRingBuff(RingBuff *psrRingBuff);
//
#endif //_RING_BUFF_H_
