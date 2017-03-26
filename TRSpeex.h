#ifndef _TRSPEEX_
#define _TRSPEEX_

#include "speex/speex.h"
#include "fifo.h"

#ifdef __cplusplus
extern "C"
{
#endif 

#ifndef NULL
#define NULL ((void *)0)
#endif

#define MAX_FRAME_SIZE 2000
#define MAX_FRAME_BYTES 2000


typedef struct _TRSpeexDecodeContex
{
	void *st;
	SpeexBits bits;
	int frame_size;

	PCMFifoBuffer* pFifo;

}TRSpeexDecodeContex;


int TRSpeexDecodeInit(TRSpeexDecodeContex* stDecode);
int TRSpeexDecode(TRSpeexDecodeContex* stDecode, char* pInput, int nInputSize, char* pOutput, int* nOutSize);
int TRSpeexDecodeRelease(TRSpeexDecodeContex* stDecode);



#ifdef __cplusplus
}
#endif


#endif