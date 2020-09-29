/**
 * @file addsi_pd.c
 * @author Marius, Richard, Lenni, Kai <br>
 * Audiocommunication Group, Technical University Berlin <br>
 * A sine-osc object for pure data <br>
 * <br>
 * @brief A Pure Data sine-osc object for making cool sounds<br>
 * <br>
 * addsi~ allows to synthsize multiple sine waves<br>
 * <br>
 */

#include "m_pd.h"
#include "addsi.h"

static t_class *addsi_tilde_class;

/**
 * @struct addsi_tilde
 * @brief The Pure Data struct of the addsi~ object. <br>
 * @var addsi_tilde::x_obj Necessary for every signal object in Pure Data <br>
 * @var addsi_tilde::f Also necessary for signal objects, float dummy dataspace <br>
 * for converting a float to signal if no signal is connected (CLASS_MAINSIGNALIN) <br>
 * @var addsi_tilde::gain The gain object for the actual signal processing <br>
 * @var addsi_tilde::x_out A signal outlet for the adjusted signal
 * level of the incoming signal
 */

typedef struct addsi_tilde
{
    t_object  x_obj;
    t_sample f;
    addsi *osc;
    
    t_outlet *out;
} addsi_tilde;

/**
 * @related addsi_tilde
 * @brief Calculates the volume adjusted output vector<br>
 * @param w A pointer to the object, input and output vectors. <br>
 * For more information please refer to the Pure Data Docs <br>
 * The function calls the addsi_perform method. <br>
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
 * @related addsi_tilde
 * @brief Adds addsi_tilde_perform to the signal chain. <br>
 * @param x A pointer the addsi_tilde object <br>
 * @param sp A pointer the input and output vectors <br>
 * For more information please refer to the <a href = "https://github.com/pure-data/externals-howto" > Pure Data Docs </a> <br>
 */

void addsi_tilde_dsp(addsi_tilde *x, t_signal **sp)
{
    dsp_add(addsi_tilde_perform, 4, x, sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n);
}

/**
 * @related addsi_tilde
 * @brief Frees our object. <br>
 * @param x A pointer the addsi_tilde object <br>
 * For more information please refer to the <a href = "https://github.com/pure-data/externals-howto" > Pure Data Docs </a> <br>
 */

void addsi_tilde_free(addsi_tilde *x)
{
    outlet_free(x->out);
    addsi_free(x->osc);
}

//setexternTABLE/ARRAY:
void *addsi_tilde_new(t_floatarg f)
{
    addsi_tilde *x = (addsi_tilde *)pd_new(addsi_tilde_class);
    
    //The main inlet is created automatically
    x->out = outlet_new(&x->x_obj, &s_signal);
    x->osc = addsi_new(44100);

    return (void *)x;
}

void addsi_tilde_setbasefrequency(addsi_tilde *x, float basefrequency)
{
    addsi_setbasefrequency(x->osc, basefrequency);
}

void addsi_tilde_setLFO1frequency(addsi_tilde *x, float LFO1frequency)
{
    addsi_setLFO1frequency(x->osc, LFO1frequency);
}

void addsi_tilde_setLFO1depth(addsi_tilde *x, float LFO1depth)
{
    addsi_setLFO1depth(x->osc, LFO1depth);
}

void addsi_tilde_setLFO2frequency(addsi_tilde *x, float LFO2frequency)
{
    addsi_setLFO2frequency(x->osc, LFO2frequency);
}

void addsi_tilde_sethammulti(addsi_tilde *x, float harmmulti)
{
    addsi_setharmmulti(x->osc, harmmulti);
}
/*
void addsi_tilde_setHarmonics1(addsi_tilde *x, float harm1)
{
    addsi_setHarmonics1(x->osc, harm1);
}

*/
 
/**
 * @related addsi_tilde
 * @brief Setup of addsi_tilde <br>
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
    
    // this adds the freq1 message to our object
    class_addmethod(addsi_tilde_class, (t_method)addsi_tilde_setbasefrequency, gensym("basefrequency"), A_DEFFLOAT, 0);
    class_addmethod(addsi_tilde_class, (t_method)addsi_tilde_setLFO1frequency, gensym("LFO1frequency"), A_DEFFLOAT, 0);
    class_addmethod(addsi_tilde_class, (t_method)addsi_tilde_setLFO1depth, gensym("LFO1depth"), A_DEFFLOAT, 0);
    class_addmethod(addsi_tilde_class, (t_method)addsi_tilde_sethammulti, gensym("harmmulti"), A_DEFFLOAT, 0);
    class_addmethod(addsi_tilde_class, (t_method)addsi_tilde_setLFO2frequency, gensym("LFO2frequency"), A_DEFFLOAT, 0);
    /*
    class_addmethod(addsi_tilde_class, (t_method)addsi_tilde_setHarmonics1, gensym("harm1"), A_DEFFLOAT, 0);
    */
    
    CLASS_MAINSIGNALIN(addsi_tilde_class, addsi_tilde, f);
}
