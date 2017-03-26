#include <malloc.h>
#include "TRSpeex.h"



int TRSpeexDecodeInit(TRSpeexDecodeContex* stDecode)
{

	int modeID = -1;
	const SpeexMode *decmode=NULL;
	int nframes;
	int vbr_enabled;
	int chan;
	int rate;
	void *st;
	int quality;
	int dec_frame_size;
	int complexity;
	int nbBytes;
	int ret;
	int enh_enabled;
	int decrate;
	int declookahead;

	if(stDecode == NULL)
		return -1;

	modeID = SPEEX_MODEID_WB;

	speex_bits_init(&(stDecode->bits));

	decmode = speex_lib_get_mode (modeID);	

	stDecode->st = speex_decoder_init(decmode);
	if(stDecode->st == NULL)
		return -1;


	enh_enabled = 1;
	decrate = 16000;
		
	speex_decoder_ctl(stDecode->st, SPEEX_SET_ENH, &enh_enabled);

	speex_decoder_ctl(stDecode->st, SPEEX_SET_SAMPLING_RATE, &decrate);

	speex_decoder_ctl(stDecode->st, SPEEX_GET_FRAME_SIZE, &dec_frame_size);

	speex_decoder_ctl(stDecode->st, SPEEX_GET_LOOKAHEAD, &declookahead);

	stDecode->frame_size = dec_frame_size;

	stDecode->pFifo = (PCMFifoBuffer*)malloc(sizeof(PCMFifoBuffer));

	if(stDecode->pFifo != NULL)
	{	
		ret = pcm_fifo_init(stDecode->pFifo, 1024*10000);
		if(ret == -1)
			return -1;
	}
	else 
		return -1;


	return 1;



}

int TRSpeexDecode(TRSpeexDecodeContex* stDecode,char* pInput, int nInputSize, char* pOutput, int* nOutSize)
{

	int nbBytes;
	char aInputBuffer[MAX_FRAME_BYTES];
	int nFrameNo;
	int nDecSize;
	int nTmpSize;
	int ret = 0;
	
	
	if(stDecode == NULL)
		return -1;

	if(pInput == NULL)
		return -1;
	
	if(pOutput == NULL)
		return -1;
	
	if(nInputSize < 0)
		return -1;

	if(nInputSize > 1024*10000)
		return -1;


	if(stDecode->pFifo != NULL)
		pcm_fifo_write(stDecode->pFifo, (unsigned char*)pInput, nInputSize);
	else
		return -1;

	nFrameNo = 0;
	nDecSize = 0;
	nTmpSize = 0;
	

	while(pcm_fifo_size(stDecode->pFifo) >= 60)
	{
		pcm_fifo_read(stDecode->pFifo, (unsigned char*)aInputBuffer, 60);
		
		speex_bits_read_from(&(stDecode->bits), aInputBuffer, 60);

		ret = speex_decode_int(stDecode->st, &(stDecode->bits), (spx_int16_t*)pOutput+nFrameNo*(stDecode->frame_size));
		if(ret == -1 || ret == -2)
		{
			nOutSize = 0;
			return -1;	
		}
		
		
		nTmpSize += stDecode->frame_size*2;

		nFrameNo ++;
	}

	*nOutSize = nTmpSize;

	return 1;


}

int TRSpeexDecodeRelease(TRSpeexDecodeContex* stDecode)
{

	if(stDecode == NULL)
		return -1;

	if (stDecode->st != NULL)
		speex_decoder_destroy(stDecode->st);

	speex_bits_destroy(&(stDecode->bits));

	if(stDecode->pFifo != NULL)
	{
		pcm_fifo_free(stDecode->pFifo);
		free(stDecode->pFifo);
		stDecode->pFifo = NULL;
	}

	return 1;


}



