/**
 * @file addsi.c
 * @author Marius, Richard, Lenni, Kai <br>
 * Audiocommunication Group, Technical University Berlin <br>
 * @brief Implementation of the external's working code for pure data<br>
 * Includes all functions needed to create the additive synthesis of the addsi~ object
 */
#include "addsi.h"

/**
 * @related addsi
 * @brief Sets up new addsi object on first run and creates wave tables<br>
 * This function sets up all we need to get started with processing
 * @param tableSize int containing the used sample rate. Note that at this time this is hard coded to 44100 in addsi_pd.c and will not work with differing sample rates<br>
 * @return An addsi struct <br>
 */
addsi *addsi_new(int tableSize)
{
    // allocating memory and setting base value
    addsi *x = (addsi *)malloc(sizeof(addsi));
    x->tableSize = tableSize;
    x->lookupTable = (float *) vas_mem_alloc(x-> tableSize * sizeof(float));
    x->LFO1_Table = (float *) vas_mem_alloc(x->tableSize * sizeof(float));
    x->LFO2_Table = (float *) vas_mem_alloc(x->tableSize * sizeof(float));
    
    // setting initial values for sine oscillator
    x->currentIndex = 0;
    x->frequency = 220;
    
    float stepSize = (M_PI*2) / (float)tableSize;
    float currentX = 0;
    
    for(int i = 0; i < x->tableSize; i++)
    {
        x->lookupTable[i] = sinf(currentX);
        currentX += stepSize;
    }
 
    // setting initial values for LFO1
    x->LFO1frequency = 0.1;
    x->LFO1_currentIndex = 0;
    x->LFO1_depth = 1;
    
    // generating triangle wavetable for LFO1:
    for(int t = 0; t < x->tableSize/2; t++)
    {
        x->LFO1_Table[t] = 2 * (float) t/ (float) (x->tableSize/2) - 1;
    }
    for(int t = x->tableSize/2; t < x->tableSize; t++)
    {
        x->LFO1_Table[t] = 1 - (2 * (float) (t-x->tableSize/2) / (float) (x->tableSize/2));
    }
    
    // setting initial values for LFO2
    x->LFO2frequency = 0.1;
    x->LFO2_currentIndex = 0;
    x->LFO2_depth = 1;
    
    
    // generating cos wavetable for LFO2:
    float stepSizeLFO2 = (M_PI*2) / (float)tableSize;
    float currentXLFO2 = 0;
    
    for(int i = 0; i < x->tableSize; i++)
    {
        x->LFO2_Table[i] = cosf(currentXLFO2);
        currentXLFO2 += stepSizeLFO2;
    }
    
    // setting initial value for the number of harmonics
    x->numberOfHarmonics = 0;
    
    // setting harmonic indexes and random harmonic amplitudes between 0-1
    for(int i = 0; i < MAXNUMBEROFHARMONICS; i++)
    {
        x->harmonicIndex[i] = 0;
        x->harmonicGain[i] = (float)rand()/RAND_MAX;
    }
    
    return x;
}

/**
 * @related addsi
 * @brief Frees the memory<br>
 * Implements mandatory memory management function
 * @param *x pointer to addsi struct<br>
 */
void addsi_free(addsi *x)
{
    vas_mem_free(x->lookupTable);
    vas_mem_free(x->LFO1_Table);
    vas_mem_free(x->LFO2_Table);
    free(x);
}

/**
 * @related addsi
 * @brief Main method: Implementing the additive synthesis of addsi~<br>
 * processes the wave tables
 * @param *x pointer to an addsi struct<br>
 * @param *in pointer to sound input vector (currently unused)
 * @param *out pointer to sound output vector
 * @param vectorSize size of the sound vectors
 * @return An addsi struct <br>
 */
void addsi_process(addsi *x, float *in, float *out, int vectorSize)
{
    // set i as buffer index helper based on the size of the I-O vectors
    int i = vectorSize;
    
    // set buffer variables for sine wave oscillator and both LFOs
    float buff;
    float LFO1osc;
    float LFO2osc;
    
    // start the processing using a while loop counting down to the beginning of the output vector
    while(i--)
    {
        // LFO1: adjust the LFO1 wave table by the initialised LFO strength
        int LFO1_index = floor(x->LFO1_currentIndex);
        LFO1osc =  (1- x->LFO1_depth) + x->LFO1_depth * pow(x->LFO1_Table[LFO1_index],2);
       
        x->LFO1_currentIndex += x->LFO1frequency;
        
        if(x->LFO1_currentIndex >= x->tableSize)
            x->LFO1_currentIndex -= x->tableSize;
        
        // LFO2: adjust the LFO2 wave table by the initialised LFO strength
        int LFO2_index = floor(x->LFO2_currentIndex);
        LFO2osc =  (1- x->LFO2_depth) + x->LFO2_depth * pow(x->LFO2_Table[LFO2_index],2);
        
        x->LFO2_currentIndex += x->LFO2frequency;
        
        if(x->LFO2_currentIndex >= x->tableSize)
            x->LFO2_currentIndex -= x->tableSize;
        
        // adjust the sine wave function based on the set base frequency
        int intIndex = floor(x->currentIndex);
        buff = x->lookupTable[intIndex];
        x->currentIndex += x->frequency;
        
        if(x->currentIndex >= x->tableSize)
            x->currentIndex -= x->tableSize;
        
        // add the LFO2 developed harmonics to the signal
        for(int i = 0; i < x->numberOfHarmonics; i++)
        {
            float harmonicFreq1 = x->frequency * (i+1);
            intIndex = floor(x->harmonicIndex[i]);
            buff += (x->lookupTable[intIndex] * (x->harmonicGain[i] * LFO2osc))/(x->numberOfHarmonics);
            x->harmonicIndex[i] += harmonicFreq1;
            
            if(x->harmonicIndex[i] >= x->tableSize)
                x->harmonicIndex[i] -= x->tableSize;
        }
        
        // multiply first LFO with buffer to modulate the amplitude
        *out += LFO1osc * buff;
    
        // increment the output vector array pointer by one, to iterate through all values during the while loop
        out++;
        
    }

    
}

/**
 * @related addsi
 * @brief Sets a base frequency for the osc<br>
 * @param x A pointer to the addsi object <br>
 * @param frequency float containing the Hz value of the base frequency of the osc <br>
 *
 */
void addsi_setFrequency(addsi *x, float frequency)
{
    if(frequency >= 0){
        x->frequency = frequency;
    }else{
        x->frequency = 0;
    }
}

/**
 * @related addsi
 * @brief Sets the frequency of the first LFO<br>
 * @param x A pointer to an addsi_tilde object <br>
 * @param LFO1frequency float value setting the frequency of the first LFO<br>
 *
 */
void addsi_setLFO1frequency(addsi *x, float LFO1frequency)
{
    if(LFO1frequency >= 0){
        x->LFO1frequency = LFO1frequency;
    }else{
        x->LFO2frequency = 0;
    }
}
/**
 * @related addsi
 * @brief Sets the frequency of the second LFO2<br>
 * @param x A pointer to an addsi_tilde object <br>
 * @param LFO2frequency float value setting the frequency of the second LFO<br>
 *
 */
void addsi_setLFO2frequency(addsi *x, float LFO2frequency)
{
    if(LFO2frequency >= 0){
        x->LFO2frequency = LFO2frequency;
    }else{
        x->LFO2frequency = 0;
    }
}

/**
 * @related addsi
 * @brief Sets the number of partials added to the base frequency<br>
 * @param x A pointer to an addsi_tilde object <br>
 * @param numberOfHarmonics float value setting the number of partials<br>
 *
 */
void addsi_setHarmonics(addsi *x, float numberOfHarmonics)
{
    if(numberOfHarmonics > 0){
       x->numberOfHarmonics = numberOfHarmonics;
    }else{
        x->numberOfHarmonics = 0;
    }
}
