#include "addsi.h"

addsi *addsi_new(int bufferSize)
{
    addsi *x = (addsi *)malloc(sizeof(addsi));
    x->bufferSize = bufferSize;
    x->buffer = (float *) vas_mem_alloc(x-> bufferSize * sizeof(float));
    x->writePointer = x->buffer;
    x->readPointer = x->buffer;
    x->readIndex = 0;
    x->writeIndex = 0;
    return x;
}

void addsi_free(addsi *x)
{
    vas_mem_free(x->buffer);
    free(x);
}

void addsi_process(addsi *x, float *in, float *out, int vectorSize)
{
    int i = vectorSize;
    
    while(i--)
    {
        x->writePointer[x->writeIndex++] = *in++;
        if(x->writeIndex >= x->bufferSize)
            x->writeIndex = 0;
        
        *out++ = x->readPointer[x->readIndex++];
        if(x->readIndex >= x->bufferSize)
            x->readIndex = 0;
    }
}

void addsi_setDelayTime(addsi *x, float delayInSamples)
{
    if (delayInSamples < 0 || (delayInSamples >= x-> bufferSize) )
        delayInSamples = 0;
    x->writeIndex = (int) x->readIndex + delayInSamples;
}

//void addsi_setDelayTime(addsi *x, float delayInSamples)
//{
//    if (delayInSamples < 0 || (delayInSamples >= x-> bufferSize) )
//        delayInSamples = 0;
//    x->readIndex = (x->bufferSize - (int)delayInSamples + x->writeIndex) % x->bufferSize;
//}




