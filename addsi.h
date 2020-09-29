/**
 * @file addsi.h
 * @author Marius, Richard, Lenni, Kai <br>
 * Audiocommunication Group, Technical University Berlin <br>
 * @brief Header for addsi.c <br>
 * Includes type definitions and function declarations
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

/**
 * @struct addsi
 * @brief Internal data structure containing the parameters for the oscillators and buffer data variables <br>
 *
 */
typedef struct addsi
    {
        int sampleRate; /**< Sample rate. Note that this is hard coded to 44100 in this external*/
        float sine_currentIndex; /**< Current working index of the sine oscillator */
        float sine_basefrequency; /**< Working frequency of the sine oscillator*/
        float *buffer; /**< Buffer array*/
        float numberOfHarmonics; /**< Number of added harmonics to the baseFrequency*/
        float harmonicIndex[MAXNUMBEROFHARMONICS]; /**< Index of Harmonics, size is set by definition of  MAXNUMBEROFHARMONICS in addsi.h*/
        float harmonicGain[MAXNUMBEROFHARMONICS]; /**< Gain of Harmonics, size is set by definition of  MAXNUMBEROFHARMONICS in addsi.h*/
        float *envelopeTable; /**< Help array for enveloping*/
        int envelopeIndex; /**< Current working index in envelopeTable*/
        float *lfo1_table; /**< Working array of the first LFO */
        float lfo1_frequency; /**<  Working frequency of the first LFO*/
        float lfo1_depth; /**< Depth of the first LFO */
        float lfo1_currentIndex; /**< Current working index of lfo1_table */
        float *lfo2_Table; /**< Working array of the second LFO */
        float lfo2_frequency; /**< Working frequency of the second LFO */
        float lfo2_depth; /**< Depth of the second LFO */
        float lfo2_currentIndex; /**< Current working index of lfo2_table*/
        
    } addsi;

// function declarations for addsi_pd.c

addsi *addsi_new(int tableSize);

void addsi_free(addsi *x);
    
void addsi_process(addsi *x, float *in, float *out, int vector_size);

void addsi_setbasefrequency(addsi *x, float basefrequency);

void addsi_setLFO1frequency(addsi *x, float LFO1frequency);
    
void addsi_setLFO2frequency(addsi *x, float LFO2frequency);


#endif /* addsi_h */
