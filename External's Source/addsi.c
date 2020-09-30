/**
 * @file addsi.c
 * @author Marius, Richard, Lenni, Kai <br>
 * Audiocommunication Group, Technical University Berlin <br>
 * @brief Implementation of the external's working code <br>
 * Includes all functions needed to create the additive synthesis of ~addsi
 */
#include "addsi.h"

/**
 * @related addsi
 * @brief Sets up new addsi object on first run and creates wave tables<br>
 * This function sets up all we need to get started with processing
 * @param sampleRate int containing the used sample rate. Note that at this time this is hard coded to 44100 in addsi_pd.c and will not work with differing sample rates<br>
 * @return An addsi struct <br>
 */
addsi *addsi_new(int sampleRate)
{
    //allocating memory and setting base value
    addsi *x = (addsi *)malloc(sizeof(addsi));
    x->tableSize = sampleRate;
    x->lookupTable1 = (float *) vas_mem_alloc(x-> tableSize * sizeof(float));
    x->envelopeTable = (float *) vas_mem_alloc(x-> tableSize * sizeof(float));
    x->LFO1_Table = (float *) vas_mem_alloc(x-> tableSize * sizeof(float));
    x->LFO2_Table = (float *) vas_mem_alloc(x-> tableSize * sizeof(float));
    
    // setting base values for sine oscillator
    x->currentIndex = 0;
    x->basefrequency = 440;
    x->numberOfHarmonics = 6;
    
    // setting base values for LFO1
    x->LFO1frequency = 0.1;
    x->LFO1_currentIndex = 0;
    x->LFO1_depth = 1;
    
    //Generating Triangle Wavetable for LFO1:
    for(int t = 0; t < sampleRate/2; t++)
    {
        x->LFO1_Table[t] = 2 * (float) t/ (float) (sampleRate/2) - 1;
    }
    for(int t = sampleRate/2; t < sampleRate; t++)
    {
        x->LFO1_Table[t] = 1 - (2 * (float) (t-sampleRate/2) / (float) (sampleRate/2));
    }
    
    // setting base values for LFO2
    x->LFO2frequency = 0.1;
    x->LFO2_currentIndex = 0;
    x->LFO2_depth = 1;
    
    
    //Generating cos Wavetable for LFO2:
    float stepSizeLFO2 = (M_PI*2) / (float)sampleRate;
    float currentXLFO2 = 0;
    
    for(int i = 0; i < x->tableSize; i++)
    {
        x->LFO2_Table[i] = cosf(currentXLFO2);
        
        currentXLFO2 += stepSizeLFO2;
    }
    
    
    //Setting HarmonicIndexes and random harmonic amplitudes between 0-1
    for(int i = 0; i < MAXNUMBEROFHARMONICS; i++)
    {
        x->harmonicIndex[i] = 0;
        x->harmonicGain[i] = (float)rand()/RAND_MAX;
    }
       
    // Create sine wavetable with amplitude of 1
    
    float stepSize = (M_PI*2) / (float)sampleRate;
    float currentX = 0;
    
    for(int i = 0; i < x->tableSize; i++)
    {
        x->lookupTable1[i] = sinf(currentX);
        x->envelopeTable[i] = 1;
        
        currentX += stepSize;
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
    vas_mem_free(x->lookupTable1);
    vas_mem_free(x->envelopeTable);
    vas_mem_free(x->LFO1_Table);
    vas_mem_free(x->LFO2_Table);
    
    free(x);
    
}

/**
 * @related addsi
 * @brief Main method: Implementing the additive synthesis of ~addsi<br>
 * Processes the wave tables
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
    float LFOosc;
    float LFO2osc;

    
    // start the processing using a while loop counting down to the beginning of the output vector
    while(i--)
    {
        
        //LFO1: adjust the LFO1 wave table by the set LFO strength
        int LFO1_index = floor(x->LFO1_currentIndex);
        LFOosc =  (1- x->LFO1_depth) + x->LFO1_depth * pow(x->LFO1_Table[LFO1_index],2);
       
        x->LFO1_currentIndex += x->LFO1frequency;
        
        if(x->LFO1_currentIndex >= x->tableSize)
            x->LFO1_currentIndex -= x->tableSize;
        
        //LFO2: adjust the LFO2 wave table by the set LFO strength
        int LFO2_index = floor(x->LFO2_currentIndex);
        LFO2osc =  (1- x->LFO2_depth) + x->LFO2_depth * pow(x->LFO2_Table[LFO2_index],2);
        
        x->LFO2_currentIndex += x->LFO2frequency;
        
        if(x->LFO2_currentIndex >= x->tableSize)
            x->LFO2_currentIndex -= x->tableSize;
        
        
        // adjust the sine wave function based on the set base frequency
        int intIndex1 = floor(x->currentIndex);
        buff = x->lookupTable1[intIndex1];
        x->currentIndex += x->basefrequency;
        
        if(x->currentIndex >= x->tableSize)
            x->currentIndex -= x->tableSize;
        
        // Add the LFo2 developed harmonics to the signal
        for(int i = 0; i < floor(x->numberOfHarmonics); i++)
        {
            float harmonicFreq1 = x->basefrequency * (i+1);
            intIndex1 = floor(x->harmonicIndex[i]);
            buff += x->lookupTable1[intIndex1] * (x->harmonicGain[i] * LFO2osc) ;
            x->harmonicIndex[i] += harmonicFreq1;
            
            if(x->harmonicIndex[i] >= x->tableSize)
                x->harmonicIndex[i] -= x->tableSize;
        }
        
        
        // add some more pure sine wave flavour, so the LFOs are not too present
        *out += LFOosc * buff;
        
        // write out into the envelope table. Currently not used any further.
        *out *= x->envelopeTable[x->envelopeIndex++];
        if(x->envelopeIndex >= x->tableSize)
        x->envelopeIndex = 1;
        
        //increment the output vector array pointer by one, to iterate through all values during the while loop
        out++;
    }
    
    // Normalize the output vector
    // help counter veriables
    int k, cc;
    // temporary variable for storing the maximum value of the output vector
    float maximum;
        
    // Calculate size of the float array
    int(size) = sizeof(&out) / sizeof(float);
        
    // set the first maximum
    maximum = out[0];
    
    // search for the maximum value in the output vector and temporary store it
    for(k=1; k < size; k++){
        if(out[k] > maximum){
            maximum = out[k];
        }
    }
    // normalize all values in the output values by the maximum value
    for(cc=1; cc<=size; cc++){
        out[cc] = out[cc] / maximum;
    }
    
}


/**
 * @related addsi
 * @brief Sets a base frequency for the osc<br>
 * @param x A pointer to the addsi object <br>
 * @param basefrequency float containing the Hz value of the base frequency of the osc <br>
 *
 */
void addsi_setbasefrequency(addsi *x, float basefrequency)
{
    if(basefrequency >= 0)
        x->basefrequency = basefrequency;
}

/**
 * @related addsi
 * @brief Sets the strength of the first LFO<br>
 * @param x A pointer to an addsi_tilde object <br>
 * @param LFO1frequency float value setting the strength of the first LFO<br>
 *
 */
void addsi_setLFO1frequency(addsi *x, float LFO1frequency)
{
    if(LFO1frequency >= 0)
        x->LFO1frequency = LFO1frequency;
}


/**
 * @related addsi
 * @brief Sets the strength of the second LFO<br>
 * @param x A pointer to an addsi_tilde object <br>
 * @param LFO2frequency float value setting the strength of the second LFO<br>
 *
 */
void addsi_setLFO2frequency(addsi *x, float LFO2frequency)
{
    if(LFO2frequency >= 0)
        x->LFO2frequency = LFO2frequency;
}

/**
 * @related addsi
 * @brief Sets the number of partials added to the base frequency<br>
 * @param x A pointer to an addsi_tilde object <br>
 * @param numberOfHarmonics float value setting the number of partials<br>
 *
 */

void addsi_setnumberOfHarmonics(addsi *x, float numberOfHarmonics)
{
    if(numberOfHarmonics >= 0)
        x->numberOfHarmonics = numberOfHarmonics;
}


