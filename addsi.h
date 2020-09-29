/**
 * @file addsi.h
 * @author Marius, Richard, Lenni, Kai <br>
 * Audiocommunication Group, Technical University Berlin <br>
 * A sine-osc object for pure data <br>
 * <br>
 * @brief A Pure Data sine-osc object for making cool sounds<br>
 * <br>
 * addsi~ allows to synthsize multiple sine waves<br>
 * <br>
 */
 
#ifndef addsi_h
#define addsi_h

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "vas_mem.h"
#include "vas_util.h"
    
#ifndef M_PI
#define M_PI (3.141592654)
#endif
#define TWOPI (2.0 * M_PI)
    
#define MAXNUMBEROFHARMONICS 6

typedef struct addsi
    {
        int tableSize;
        float currentIndex;
        float basefrequency;
        float *lookupTable1;
        float numberOfHarmonics;
        float harmonicIndex[MAXNUMBEROFHARMONICS];
        float harmonicGain[MAXNUMBEROFHARMONICS];
        float *envelopeTable;
        int envelopeIndex;
        float *LFO1_Table;
        float LFO1frequency;
        float LFO1_depth;
        float LFO1_currentIndex;
        float *LFO2_Table;
        float LFO2frequency;
        float LFO2_depth;
        float LFO2_currentIndex;
        float harmmulti;
        
    } addsi;


addsi *addsi_new(int tableSize);


void addsi_free(addsi *x);
    
void addsi_process(addsi *x, float *in, float *out, int vector_size);

void addsi_setbasefrequency(addsi *x, float basefrequency);

void addsi_setLFO1frequency(addsi *x, float LFO1frequency);

void addsi_setLFO1depth(addsi *x, float LFO1depth);
    
void addsi_setLFO2frequency(addsi *x, float LFO2frequency);

void addsi_setharmmulti(addsi *x, float harmmulti);

//void addsi_setHarmonics1(addsi *x, float harm1);

#endif /* addsi_h */
