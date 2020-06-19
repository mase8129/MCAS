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

#ifdef __cplusplus
extern "C" {
#endif
    
/**
 * @struct addsi
 * @brief A structure for a delay object <br>
 * @var addsi::buffer The buffer we save the incoming signal in <br>
 * @var addsi::delay_in_samples The parameter value for adjusting the <br>
 * delay of the incoming signal
 * @var addsi::buffer_size The size of the delay buffer <br>
 * @var addsi::circular_pointer Circular pointer to the delay buffer <br>
 * @var addsi::delay_sample The current sample from the delay buffer <br>
 */
typedef struct addsi
{
    float *buffer;              /**< Our delay buffer */
    int delayInSamples;     /**< Our delay in samples */
    int bufferSize;           /**< Size of the delay buffer */
    int writeIndex;
    int readIndex;
    float *writePointer;    /**< Circular pointer to delay buffer */
    float *readPointer;
} addsi;

/**
 * @related addsi
 * @brief Creates a new delay object<br>
 * The function sets the buffer size and delay parameter of <br>
 * the delay class
 * @return a pointer to the newly created addsi object <br>
 */
addsi *addsi_new(int bufferSize);

/**
 * @related addsi
 * @brief Frees a delay object<br>
 * @param x My delay object <br>
 * The function frees the allocated memory<br>
 * of a delay object
 */
void addsi_free(addsi *x);

/**
 * @related addsi
 * @brief Performs the delay in realtime. <br>
 * @param x My delay object <br>
 * @param in The input vector <br>
 * @param out The output vector <br>
 * @param vector_size The size of the i/o vectors <br>
 * The function addsi_perform delays any <br>
 * incoming signal and copies the result to the output vector <br>
 */
void addsi_process(addsi *x, float *in, float *out, int vector_size);

/**
 * @related addsi
 * @brief Sets the delay time in samples with floating point precision. <br>
 * @param x My delay object <br>
 * @param _delay_in_samples The delay in samples <br>
 * Sets the delay time in samples with floating point precision. <br>
 * Delays exceeding the buffer size are handeled by setting the delay to zero. <br>
 */
void addsi_setDelayTime(addsi *x, float _delay_in_samples);

    
#ifdef __cplusplus
}
#endif

#endif /* addsi_h */
