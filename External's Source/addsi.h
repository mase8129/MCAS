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

#ifdef __cplusplus
extern "C" {
#endif

#ifndef M_PI
#define M_PI (3.141592654)
#endif

#ifndef MAXNUMBEROFHARMONICS
#define MAXNUMBEROFHARMONICS (16)
#endif
    
/**
 * @struct addsi
 * @brief Internal data structure containing the parameters for the oscillators and buffer data variables <br>
 */

typedef struct addsi
{
    int tableSize; /**< Size of waveform tables, based on sample rate. Note that this is hard coded to 44100 in this external*/
    float currentIndex;/**< Current working index of the sine oscillator */
    float frequency;/**< Working frequency of the sine oscillator*/
    float *lookupTable;/**< SIne wave table*/
    int numberOfHarmonics;/**< Number of added harmonics to the baseFrequency*/
    float harmonicIndex[MAXNUMBEROFHARMONICS];/**< Index of Harmonics, size is set by definition of  MAXNUMBEROFHARMONICS in addsi.h*/
    float harmonicGain[MAXNUMBEROFHARMONICS];/**< Gain of Harmonics, size is set by definition of  MAXNUMBEROFHARMONICS in addsi.h*/
    float *envelopeTable;/**< Help array for enveloping. Currently this is not used*/
    int envelopeIndex;/**< Current working index in envelopeTable. Currently this is not used*/
    float *LFO1_Table;/**< Working array of the first LFO */
    float LFO1frequency;/**<  Working frequency of the first LFO*/
    int LFO1_depth;/**< Depth of the first LFO */
    float LFO1_currentIndex;/**< Current working index of lfo1_table */
    float *LFO2_Table;/**< Working array of the second LFO */
    float LFO2frequency; /**< Working frequency of the second LFO */
    int LFO2_depth;/**< Depth of the second LFO */
    float LFO2_currentIndex;/**< Current working index of lfo2_table*/

} addsi;

/**
 * @related addsi
 * @brief Creates a new addsi object<br>
 * @return a pointer to the newly created addsi object <br>
 */
addsi *addsi_new(int tableSize);

/**
 * @related addsi
 * @brief Frees a addsi object<br>
 * @param x my addsi object <br>
 * The function frees the allocated memory<br>
 * of a addsi object
 */
void addsi_free(addsi *x);

/**
 * @related addsi
 * @brief Performs the additive synthesis in realtime. <br>
 * @param x my addsi object <br>
 * @param in The input vector <br>
 * @param out The output vector <br>
 * @param vector_size The size of the i/o vectors <br>
 */
void addsi_process(addsi *x, float *in, float *out, int vector_size);

/**
 * @related addsi
 * @brief Sets the frequency in Hz with floating point precision. <br>
 * @param x my addsi object <br>
 * @param frequency Frequency in Hz <br>
 */
void addsi_setFrequency(addsi *x, float frequency);

/**
 * @related addsi
 * @brief Sets the frequency of LFO1 in Hz with floating point precision. <br>
 * @param x my addsi object <br>
 * @param LFO1frequency Frequency in Hz <br>
 */
void addsi_setLFO1frequency(addsi *x, float LFO1frequency);

/**
 * @related addsi
 * @brief Sets the frequency of LFO2 in Hz with floating point precision. <br>
 * @param x my addsi object <br>
 * @param LFO2frequency Frequency in Hz <br>
 */
void addsi_setLFO2frequency(addsi *x, float LFO2frequency);

/**
 * @related addsi
 * @brief Sets the number of the partials. <br>
 * @param x my addsi object <br>
 * @param numberOfHarmonics Number of Partials <br>
 */
void addsi_setHarmonics(addsi *x, float numberOfHarmonics);

#endif /* addsi_h */
