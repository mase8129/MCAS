#include "addsi.h"

addsi *addsi_new(int tableSize)
{
    addsi *x = (addsi *)malloc(sizeof(addsi));
    x->tableSize = tableSize;
    x->lookupTable1 = (float *) vas_mem_alloc(x-> tableSize * sizeof(float));
    x->envelopeTable = (float *) vas_mem_alloc(x-> tableSize * sizeof(float));
    x->LFO1_Table = (float *) vas_mem_alloc(x-> tableSize * sizeof(float));
    x->LFO2_Table = (float *) vas_mem_alloc(x-> tableSize * sizeof(float));
    
    
    x->currentIndex = 0;
    x->basefrequency = 440;
    x->numberOfHarmonics = MAXNUMBEROFHARMONICS;
    x->harmmulti = 1;
    
    x->LFO1frequency = 0.1;
    x->LFO1_currentIndex = 0;
    x->LFO1_depth = 1;
    
    //Generating Triangle Wavetable for LFO:
    for(int t = 0; t < tableSize/2; t++)
    {
        x->LFO1_Table[t] = 2 * (float) t/ (float) (tableSize/2) - 1;
    }
    for(int t = tableSize/2; t < tableSize; t++)
    {
        x->LFO1_Table[t] = 1 -
        (2 * (float) (t-tableSize/2) / (float) (tableSize/2));
    }
    
    x->LFO2frequency = 0.1;
    x->LFO2_currentIndex = 0;
    x->LFO2_depth = 2;
    
    
    //Generating cos Wavetable for LFO2:
    float stepSizeLFO2 = (M_PI*2) / (float)tableSize;
    float currentXLFO2 = 0;
    
    for(int i = 0; i < x->tableSize; i++)
    {
        x->LFO2_Table[i] = cosf(currentXLFO2);
        
        currentXLFO2 += stepSizeLFO2;
    }
    
    
    //Setting HarmonicIndexes and random harmonic amplitudes between 0-1
    for(int i = 0; i < MAXNUMBEROFHARMONICS; i ++)
    {
        x->harmonicIndex[i] = 0;
        x->harmonicGain[i] = (float)rand()/RAND_MAX;
    }
       
    float stepSize = (M_PI*2) / (float)tableSize;
    float currentX = 0;
    
    for(int i = 0; i < x->tableSize; i++)
    {
        x->lookupTable1[i] = sinf(currentX);
        x->envelopeTable[i] = 1;
        
        currentX += stepSize;
    }
       
    return x;
}


void addsi_free(addsi *x)
{
    vas_mem_free(x->lookupTable1);
    vas_mem_free(x->envelopeTable);
    vas_mem_free(x->LFO1_Table);
    vas_mem_free(x->LFO2_Table);
    
    free(x);
    
}


void addsi_process(addsi *x, float *in, float *out, int vectorSize)
{
    int i = vectorSize;
    float LFOosc;
    float LFO2osc;
    float buff;
    
    int c, cc; // Normalisierung des 256 float-sample arrays
    float maximum;
    
    // Größe des float arrays
    int(size) = sizeof(&out) / sizeof(float);
    
    while(i--)
    {
        
        //LFO1
        int LFO1_index = floor(x->LFO1_currentIndex);
        LFOosc =  (1- x->LFO1_depth) + x->LFO1_depth * pow(x->LFO1_Table[LFO1_index],2);
       
        x->LFO1_currentIndex += x->LFO1frequency;
        
        if(x->LFO1_currentIndex >= x->tableSize)
            x->LFO1_currentIndex -= x->tableSize;
        
        //LFO2
        int LFO2_index = floor(x->LFO2_currentIndex);
        LFO2osc =  (1- x->LFO2_depth) + x->LFO2_depth * pow(x->LFO2_Table[LFO2_index],2);
        
        x->LFO2_currentIndex += x->LFO2frequency;
        
        if(x->LFO2_currentIndex >= x->tableSize)
            x->LFO2_currentIndex -= x->tableSize;
        
        
        //Sine
        int intIndex1 = floor(x->currentIndex);
        buff = x->lookupTable1[intIndex1];
        x->currentIndex += x->basefrequency;
        
        if(x->currentIndex >= x->tableSize)
            x->currentIndex -= x->tableSize;
        
        //harmonics
        for(int i = 0; i < MAXNUMBEROFHARMONICS; i++)
        {
            float harmonicFreq1 = x->basefrequency * (i+1);
            intIndex1 = floor(x->harmonicIndex[i]);
            buff += x->lookupTable1[intIndex1] * (x->harmonicGain[i] * LFO2osc) ;
            x->harmonicIndex[i] += harmonicFreq1;
            
            if(x->harmonicIndex[i] >= x->tableSize)
                x->harmonicIndex[i] -= x->tableSize;
        }
        
        
        //hier nochmal mit buff also reinem addsine, sodass man nicht nur LFO hört
        *out +=  buff + 0.3*(buff * LFOosc);
        
        
        *out *= x->envelopeTable[x->envelopeIndex++];
        if(x->envelopeIndex >= x->tableSize)
        x->envelopeIndex = 1;
        
        out++;
     
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
        x->basefrequency = basefrequency;
}

void addsi_setLFO1frequency(addsi *x, float LFO1frequency)
{
    if(LFO1frequency > 0)
        x->LFO1frequency = LFO1frequency;
}

void addsi_setLFO1depth(addsi *x, float LFO1depth)
{
    if(LFO1depth > 0)
        x->LFO1_depth = LFO1depth;
}

void addsi_setLFO2frequency(addsi *x, float LFO2frequency)
{
    if(LFO2frequency > 0)
        x->LFO2frequency = LFO2frequency;
}

void addsi_setharmmulti(addsi *x, float harmmulti)
{
    if(harmmulti > 0)
        x->harmmulti = harmmulti;
}

/*
void addsi_setHarmonics1(addsi *x, float harm1)
{
    if(harm1 > 0)
        x->numberOfHarmonics1 = harm1;
}
*/
