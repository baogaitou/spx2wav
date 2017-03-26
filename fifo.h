#ifndef FIFO_H
#define FIFO_H

#ifdef __cplusplus
extern "C"
{
#endif 


#define PCMMAX(a,b) ((a) > (b) ? (a) : (b))
#define PCMMIN(a,b) ((a) > (b) ? (b) : (a))



typedef struct PCMFifoBuffer {
    unsigned char *buffer;
    unsigned char *rptr, *wptr, *end;
} PCMFifoBuffer;

int pcm_fifo_init(PCMFifoBuffer *f, int size);

void pcm_fifo_free(PCMFifoBuffer *f);

int pcm_fifo_size(PCMFifoBuffer *f);

int pcm_fifo_read(PCMFifoBuffer *f, unsigned char *buf, int buf_size);

int pcm_fifo_generic_read(PCMFifoBuffer *f, int buf_size, void (*func)(void*, void*, int), void* dest);

void pcm_fifo_write(PCMFifoBuffer *f, const unsigned char *buf, int size);

void pcm_fifo_realloc(PCMFifoBuffer *f, unsigned int size);

void pcm_fifo_drain(PCMFifoBuffer *f, int size);

static unsigned char pcm_fifo_peek(PCMFifoBuffer *f, int offs)
{
    unsigned char *ptr = f->rptr + offs;
    if (ptr >= f->end)
        ptr -= f->end - f->buffer;
    return *ptr;
}

#ifdef __cplusplus
}
#endif

#endif /* FIFO_H */
