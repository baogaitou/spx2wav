#include "fifo.h"
#include <malloc.h>
#include <memory.h>

int pcm_fifo_init(PCMFifoBuffer *f, int size)
{
    f->wptr = f->rptr =  f->buffer = (unsigned char*)malloc(size);

	
    f->end = f->buffer + size;
    if (!f->buffer)
        return -1;
    return 0;
}

void pcm_fifo_free(PCMFifoBuffer *f)
{
    free(f->buffer);
}

int pcm_fifo_size(PCMFifoBuffer *f)
{
    int size = f->wptr - f->rptr;
    if (size < 0)
        size += f->end - f->buffer;
    return size;
}

int pcm_fifo_read(PCMFifoBuffer *f, unsigned char *buf, int buf_size)
{
    return pcm_fifo_generic_read(f, buf_size, 0, buf);
}

void pcm_fifo_realloc(PCMFifoBuffer *f, unsigned int new_size) 
{
    unsigned int old_size= f->end - f->buffer;

    if(old_size < new_size){
        int len= pcm_fifo_size(f);
        PCMFifoBuffer f2;

        pcm_fifo_init(&f2, new_size);
        pcm_fifo_read(f, f2.buffer, len);
        f2.wptr += len;
        free(f->buffer);
        *f= f2;
    }
}

void pcm_fifo_write(PCMFifoBuffer *f, const unsigned char *buf, int size)
{
    do {
        int len = PCMMIN(f->end - f->wptr, size);
        memcpy(f->wptr, buf, len);
        f->wptr += len;
        if (f->wptr >= f->end)
            f->wptr = f->buffer;
        buf += len;
        size -= len;
    } while (size > 0);
}



int pcm_fifo_generic_read(PCMFifoBuffer *f, int buf_size, void (*func)(void*, void*, int), void* dest)
{
    int size = pcm_fifo_size(f);

    if (size < buf_size)
        return -1;
    do {
        int len = PCMMIN(f->end - f->rptr, buf_size);
        if(func) func(dest, f->rptr, len);
        else{
            memcpy(dest, f->rptr, len);
            dest = (unsigned char*)dest + len;
        }
        pcm_fifo_drain(f, len);
        buf_size -= len;
    } while (buf_size > 0);
    return 0;
}


void pcm_fifo_drain(PCMFifoBuffer *f, int size)
{
    f->rptr += size;
    if (f->rptr >= f->end)
        f->rptr -= f->end - f->buffer;
}
