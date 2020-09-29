/**
 * @file addsi_pd.c
 * @author Marius, Richard, Lenni, Kai <br>
 * Audiocommunication Group, Technical University Berlin <br>
 * @brief Pure data integration of the addsi external<br>
 * The file includes the basic setup needed in order for the external to work with pure data
 */

#include "m_pd.h"
#include "addsi.h"

static t_class *addsi_tilde_class;

/**
 * @struct addsi_tilde
 * @brief The Pure Data struct of the addsi~ object. <br>
 */
typedef struct addsi_tilde
{
    t_object  x_obj; /**< Necessary for every signal object in Pure Data*/
    t_sample f; /**< Also necessary for signal objects, float dummy dataspace for converting a float to signal if no signal is connected (CLASS_MAINSIGNALIN) */
    addsi *osc; /**< custom addsi type containing all signal generating parameters and buffer data*/
    t_outlet *x_out; /**< needed to store handles to the outlet of the signal*/
} addsi_tilde;

/**
 * @brief Perform function, mandatory for PureData. Calculates the output vector<br>
 * For more information please refer to the <a href = "https://github.com/pure-data/externals-howto" > Pure Data Docs </a> <br>
 * @param w A pointer to the object, input and output vectors. <br>
 * @return A pointer to the signal chain right behind the addsi_tilde object. <br>
 */

t_int *addsi_tilde_perform(t_int *w)
{
    addsi_tilde *x = (addsi_tilde *)(w[1]);
    t_sample  *in = (t_sample *)(w[2]);
    t_sample  *out =  (t_sample *)(w[3]);
    int n =  (int)(w[4]);

   addsi_process(x->osc, in, out, n);
    
    /* return a pointer to the dataspace for the next dsp-object */
    return (w+5);
}

/**
 * @brief DSP function mandatory for PureData. Adds addsi_tilde_perform to the signal chain. <br>
 * For more information please refer to the <a href = "https://github.com/pure-data/externals-howto" > Pure Data Docs </a> <br>
 * @param x A pointer the addsi_tilde object <br>
 * @param sp A pointer to the input and output vectors <br>
 */

void addsi_tilde_dsp(addsi_tilde *x, t_signal **sp)
{
    dsp_add(addsi_tilde_perform, 4, x, sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n);
}

/**
 * @brief Memory Management function mandatory for Pure Data. Frees our addsi_tilde object. <br>
 * For more information please refer to the <a href = "https://github.com/pure-data/externals-howto" > Pure Data Docs </a> <br>
 * @param x A pointer to an addsi_tilde object <br>

 */

void addsi_tilde_free(addsi_tilde *x)
{
    outlet_free(x->x_out);
    addsi_free(x->osc);
}

/**
 * @brief Creates new addsi_tilde object and sets its outlet and sampling rate. note that the externals sampling rate is set at 44100 <br>
 * For more information please refer to the <a href = "https://github.com/pure-data/externals-howto" > Pure Data Docs </a> <br>
 */
void *addsi_tilde_new(void)
{
    addsi_tilde *x = (addsi_tilde *)pd_new(addsi_tilde_class);
    
    //The outlet is created automatically
    x->x_out = outlet_new(&x->x_obj, &s_signal);
    // sampling rate is hard coded for 44100 samples per second
    x->osc = addsi_new(44100);

    return (void *)x;
}

/**
 * @brief Sets a base frequency for the osc<br>
 * @param x A pointer to an addsi_tilde object <br>
 * @param basefrequency float containing the Hz value of the base frequency of the osc <br>
 *
 */
void addsi_tilde_setbasefrequency(addsi_tilde *x, float basefrequency)
{
    addsi_setbasefrequency(x->osc, basefrequency);
}

/**
 * @brief Sets the strength for the first LFO<br>
 * @param x A pointer to an addsi_tilde object <br>
 * @param LFO1frequency float value setting the strength of the first LFO<br>
 *
 */
void addsi_tilde_setLFO1frequency(addsi_tilde *x, float LFO1frequency)
{
    addsi_setLFO1frequency(x->osc, LFO1frequency);
}
/**
 * @brief Sets the strength of the second LFO<br>
 * @param x A pointer to an addsi_tilde object <br>
 * @param LFO2frequency float value setting the strength of the second LFO<br>
 *
 */
void addsi_tilde_setLFO2frequency(addsi_tilde *x, float LFO2frequency)
{
    addsi_setLFO2frequency(x->osc, LFO2frequency);
}



/**
 * @brief Setup function<br>
 * This function (or functions called by it) declares the new classes and their properties of the addsi-tilde external. It is only called once, when the external is loaded. <br>
 * For more information please refer to the <a href = "https://github.com/pure-data/externals-howto" > Pure Data Docs </a> <br>
 */

void addsi_tilde_setup(void)
{
    addsi_tilde_class = class_new(gensym("addsi~"),
                                      (t_newmethod)addsi_tilde_new,
                                      (t_method)addsi_tilde_free,
                                      sizeof(addsi_tilde),
                                      CLASS_DEFAULT,
                                      A_DEFFLOAT, 0);
    
    class_addmethod(addsi_tilde_class, (t_method)addsi_tilde_dsp, gensym("dsp"), 0);
    
    // this sets up the message names for our object
    class_addmethod(addsi_tilde_class, (t_method)addsi_tilde_setbasefrequency, gensym("basefrequency"), A_DEFFLOAT, 0);
    class_addmethod(addsi_tilde_class, (t_method)addsi_tilde_setLFO1frequency, gensym("LFO1frequency"), A_DEFFLOAT, 0);
    class_addmethod(addsi_tilde_class, (t_method)addsi_tilde_setLFO2frequency, gensym("LFO2frequency"), A_DEFFLOAT, 0);
    
    CLASS_MAINSIGNALIN(addsi_tilde_class, addsi_tilde, f);
}
