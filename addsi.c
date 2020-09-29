/**
 * @file addsi.c
 * @author Marius, Richard, Lenni, Kai <br>
 * Audiocommunication Group, Technical University Berlin <br>
 * @brief Implementation of the external's working code <br>
 * Includes all functions needed to create the additive synthesis of ~addsi
 */

#include "addsi.h"

/**
 * @brief Sets up new addsi object on first run and creates wave tables<br>
 * This function sets up all we need to get started with processing
 * @param sampleRate int containing the used sample rate. Note that at this time this is hard coded to 44100 in addsi_pd.c and will not work with differing sample rates<br>
 * @return An addsi struct <br>
 */
addsi *addsi_new(int sampleRate)
{
    //allocating memory and setting base value
    addsi *x = (addsi *)malloc(sizeof(addsi));
    x->buffer = (float *) vas_mem_alloc(x-> sampleRate * sizeof(float));
    x->envelopeTable = (float *) vas_mem_alloc(x-> sampleRate * sizeof(float));
    x->lfo1_table = (float *) vas_mem_alloc(x-> sampleRate * sizeof(float));
    x->lfo2_Table = (float *) vas_mem_alloc(x-> sampleRate * sizeof(float));
    x->sampleRate = sampleRate;
    
    // setting base values for sine oscillator
    x->sine_currentIndex = 0;
    x->sine_basefrequency = 440;
    x->numberOfHarmonics = MAXNUMBEROFHARMONICS;
    
    // setting base values for lfo1
    x->lfo1_frequency = 0.1;
    x->lfo1_currentIndex = 0;
    x->lfo1_depth = 1;
    
    //Generating Triangle Wavetables for LFO1:
    for(int t = 0; t < sampleRate/2; t++)
    {
        x->lfo1_table[t] = 2 * (float) t/ (float) (sampleRate/2) - 1;
    }
    for(int t = sampleRate/2; t < sampleRate; t++)
    {
        x->lfo1_table[t] = 1 -
        (2 * (float) (t-sampleRate/2) / (float) (sampleRate/2));
    }
    
    // setting base values for LFO2
    x->lfo2_frequency = 0.1;
    x->lfo2_currentIndex = 0;
    x->lfo2_depth = 2;
    
    
    //Generating cos Wavetable for LFO2:
    float stepSizeLFO2 = (M_PI*2) / (float)sampleRate;
    float currentXLFO2 = 0;
    
    for(int i = 0; i < x->sampleRate; i++)
    {
        x->lfo2_Table[i] = cosf(currentXLFO2);
        
        currentXLFO2 += stepSizeLFO2;
    }
    
    
    //Setting HarmonicIndexes and random harmonic amplitudes between 0-1
    for(int i = 0; i < MAXNUMBEROFHARMONICS; i ++)
    {
        x->harmonicIndex[i] = 0;
        x->harmonicGain[i] = (float)rand()/RAND_MAX;
    }
    
    // Create sine wavetable with amplitude of 1
    
    float stepSize = (M_PI*2) / (float)sampleRate;
    float currentX = 0;
    
    for(int i = 0; i < x->sampleRate; i++)
    {
        x->buffer[i] = sinf(currentX);
        x->envelopeTable[i] = 1;
        
        currentX += stepSize;
    }
       
    return x;
}

/**
 * @brief Frees the memory<br>
 * Implements mandatory memory management function
 * @param *x pointer to addsi struct<br>
 */
void addsi_free(addsi *x)
{
    vas_mem_free(x->buffer);
    vas_mem_free(x->envelopeTable);
    vas_mem_free(x->lfo1_table);
    vas_mem_free(x->lfo2_Table);
    free(x);
}

/**
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
    int i = vectorSize;
    
    float LFOosc;
    float LFO2osc;
    float buff;
    
    
    // Größe des float arrays
    int(size) = sizeof(&out) / sizeof(float);
    
    while(i--)
    {
        
        //LFO1
        int LFO1_index = floor(x->lfo1_currentIndex);
        LFOosc =  (1- x->lfo1_depth) + x->lfo1_depth * pow(x->lfo1_table[LFO1_index],2);
       
        x->lfo1_currentIndex += x->lfo1_frequency;
        
        if(x->lfo1_currentIndex >= x->sampleRate)
            x->lfo1_currentIndex -= x->sampleRate;
        
        //LFO2
        int LFO2_index = floor(x->lfo2_currentIndex);
        LFO2osc =  (1- x->lfo2_depth) + x->lfo2_depth * pow(x->lfo2_Table[LFO2_index],2);
        
        x->lfo2_currentIndex += x->lfo2_frequency;
        
        if(x->lfo2_currentIndex >= x->sampleRate)
            x->lfo2_currentIndex -= x->sampleRate;
        
        
        //Sine
        int intIndex1 = floor(x->sine_currentIndex);
        buff = x->buffer[intIndex1];
        x->sine_currentIndex += x->sine_basefrequency;
        
        if(x->sine_currentIndex >= x->sampleRate)
            x->sine_currentIndex -= x->sampleRate;
        
        //harmonics
        for(int i = 0; i < MAXNUMBEROFHARMONICS; i++)
        {
            float harmonicFreq1 = x->sine_basefrequency * (i+1);
            intIndex1 = floor(x->harmonicIndex[i]);
            buff += x->buffer[intIndex1] * (x->harmonicGain[i] * LFO2osc) ;
            x->harmonicIndex[i] += harmonicFreq1;
            
            if(x->harmonicIndex[i] >= x->sampleRate)
                x->harmonicIndex[i] -= x->sampleRate;
        }
        
        
        //hier nochmal mit buff also reinem addsine, sodass man nicht nur LFO hört
        *out +=  buff + 0.3*(buff * LFOosc);
        
        
        *out *= x->envelopeTable[x->envelopeIndex++];
        if(x->envelopeIndex >= x->sampleRate)
        x->envelopeIndex = 1;
        
        out++;
        
        // Normalising of the 256 float-sample arrays
        int c, cc;
        float maximum;
        for (c=0; c < size; c++)
            scanf("%f", &out[c]);

        maximum = out[0];

        for(c=1; c < size;c++){
            if(out[c] > maximum){
                maximum = out[c];
            }
        }
        // eigentliche Normalisation
        for(cc=1; c<size; c++){
            out[cc] = out[cc] / maximum;
        }
    }
}



void addsi_setbasefrequency(addsi *x, float basefrequency)
{
    if(basefrequency > 0)
        x->sine_basefrequency = basefrequency;
}

void addsi_setLFO1frequency(addsi *x, float LFO1frequency)
{
    if(LFO1frequency > 0)
        x->lfo1_frequency = LFO1frequency;
}

void addsi_setLFO2frequency(addsi *x, float LFO2frequency)
{
    if(LFO2frequency > 0)
        x->lfo2_frequency = LFO2frequency;
}
