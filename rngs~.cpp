#include "m_pd.h"

//IMPROVE - 
//IMPROVE - 
//TODO - hep file



#include "rings/dsp/part.h"
#include "rings/dsp/patch.h"
#include "rings/dsp/strummer.h"
#include "rings/dsp/string_synth_part.h"


inline float constrain(float v, float vMin, float vMax) {
  return std::max<float>(vMin,std::min<float>(vMax, v));
}

static t_class *rngs_tilde_class;

typedef struct _rngs_tilde {
  t_object  x_obj;

  t_float  f_dummy;

  t_float f_pitch;
  t_float f_structure;
  t_float f_brightness;
  t_float f_damping;
  t_float f_position;
  t_float f_bypass;
  t_float f_easter_egg;

  // CLASS_MAINSIGNALIN  = in
  t_outlet* x_out_odd;
  t_outlet* x_out_even;

  rings::Part part;
  rings::PerformanceState performance_state;
  rings::StringSynthPart string_synth;
  rings::Strummer strummer;
  rings::Patch patch;

  uint32_t seed=0;
  uint32_t resonator = 0;
  bool panic = false;

  const float kNoiseGateThreshold = 0.00003f;
  float in_level = 0.0f;
  float* in;
  int iobufsz;

  static const int REVERB_SZ= 32768;
  uint16_t buffer[REVERB_SZ];


} t_rngs_tilde;


//define pure data methods
extern "C"  {
  t_int*  rngs_tilde_render(t_int *w);
  void    rngs_tilde_dsp(t_rngs_tilde *x, t_signal **sp);
  void    rngs_tilde_free(t_rngs_tilde *x);
  void*   rngs_tilde_new(t_floatarg f);
  void    rngs_tilde_setup(void);

  void rngs_tilde_gate(t_rngs_tilde *x, t_floatarg f);

  void rngs_tilde_pitch(t_rngs_tilde *x, t_floatarg f);
  void rngs_tilde_resonator(t_rngs_tilde *x, t_floatarg f);
  void rngs_tilde_structure(t_rngs_tilde *x, t_floatarg f);
  void rngs_tilde_brightness(t_rngs_tilde *x, t_floatarg f);
  void rngs_tilde_damping(t_rngs_tilde *x, t_floatarg f);
  void rngs_tilde_position(t_rngs_tilde *x, t_floatarg f);
  void rngs_tilde_bypass(t_rngs_tilde *x, t_floatarg f);
  void rngs_tilde_easter_egg(t_rngs_tilde *x, t_floatarg f);
}

// puredata methods implementation -start
t_int *rngs_tilde_render(t_int *w)
{
  t_rngs_tilde *x   = (t_rngs_tilde *)(w[1]);
  t_sample  *in  = (t_sample *)(w[2]);
  t_sample  *out  = (t_sample *)(w[3]);
  t_sample  *aux = (t_sample *)(w[4]);
  int n =  (int)(w[5]);
  size_t size = n;

  if (n > x->iobufsz) {
    delete [] x->in;
    x->iobufsz = n;
    x->in = new float[ x->iobufsz];
  }


  // x->part.mutable_patch()->resonator_geometry   = constrain(x->f_geometry   ,0.0f, 0.9995f);
  // x->part.mutable_patch()->resonator_brightness = constrain(x->f_brightness ,0.0f, 0.9995f);
  // x->part.mutable_patch()->resonator_damping    = constrain(x->f_damping    ,0.0f, 0.9995f);
  // x->part.mutable_patch()->resonator_position   = constrain( x->f_position  ,0.0f, 0.9995f);



  // uint32_t nresonator = (int (x->f_resonator) % 3);
  // if(x->resonator != nresonator) {
  //   x->resonator = nresonator;
  //   x->part.set_resonator_model(elements::ResonatorModel(x->resonator));
  // }


  x->part.set_bypass(x->f_bypass > 0.5);
  // x->part.set_easter_egg(x->f_easter_egg > 0.5);

  if (x->f_easter_egg>0.5) {
    x->string_synth.Process(x->performance_state, x->patch, in, out, aux, size);
  } else {
    // Apply noise gate.
    for (size_t i = 0; i < size; ++i) {
      float in_sample = in[i] ;
      float error, gain;
      error = in_sample * in_sample - x->in_level;
      x->in_level += error * (error > 0.0f ? 0.1f : 0.0001f);
      gain = x->in_level <= x->kNoiseGateThreshold 
            ? (1.0f / x->kNoiseGateThreshold) * x->in_level : 1.0f;
      x->in[i] = gain * in_sample;
    }
    x->strummer.Process(x->in, size, &(x->performance_state));
    x->part.Process(x->performance_state, x->patch, x->in, out, aux, size);
  }
  
  //TODO
  return (w + 6); // # args + 1
}


void rngs_tilde_dsp(t_rngs_tilde *x, t_signal **sp)
{
  // add the perform method, with all signal i/o
  dsp_add(rngs_tilde_render, 6,
          x,
          sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, // signal i/o (clockwise)
          sp[0]->s_n);
}

void rngs_tilde_free(t_rngs_tilde *x)
{
  delete [] x->in;
  outlet_free(x->x_out_odd);
  outlet_free(x->x_out_even);
}

void *rngs_tilde_new(t_floatarg)
{
  t_rngs_tilde *x = (t_rngs_tilde *) pd_new(rngs_tilde_class);

  x->f_pitch = 0.0;
  x->f_structure= 0.0f;
  x->f_brightness= 0.0f;
  x->f_damping= 0.0f;
  x->f_position= 0.0f;
  x->f_bypass= 0.0f;
  x->f_easter_egg= 0.0f;

  x->part.Init(x->buffer);

  // x->seed = 0;
  // x->part.Seed(&x->seed, 1);

  // x->resonator = 0;
  // x->part.set_resonator_model(elements::ResonatorModel(x->resonator));
  // x->state.gate = false;
  // x->state.note = 0.0f;
  // x->state.modulation = 0.0f;
  // x->state.strength = 0.0f;

  x->iobufsz = 64;
  x->in = new float[ x->iobufsz];

  //x_in_strike = main input 
  x->x_out_odd   = outlet_new(&x->x_obj, &s_signal);
  x->x_out_even  = outlet_new(&x->x_obj, &s_signal);

  return (void *)x;
}


void rngs_tilde_setup(void) {
  rngs_tilde_class = class_new(  gensym("rngs~"),
                                  (t_newmethod)rngs_tilde_new,
                                  (t_method) rngs_tilde_free,
                                  sizeof(t_rngs_tilde),
                                  CLASS_DEFAULT,
                                  A_DEFFLOAT, A_NULL);

  class_addmethod(  rngs_tilde_class,
                    (t_method)rngs_tilde_dsp,
                    gensym("dsp"), A_NULL);

  // represents strike input
  CLASS_MAINSIGNALIN(rngs_tilde_class, t_rngs_tilde, f_dummy);

  class_addmethod(rngs_tilde_class,
    (t_method) rngs_tilde_pitch, gensym("pitch"),
    A_DEFFLOAT,A_NULL);
  class_addmethod(rngs_tilde_class,
    (t_method) rngs_tilde_structure, gensym("structure"),
    A_DEFFLOAT,A_NULL);
  class_addmethod(rngs_tilde_class,
    (t_method) rngs_tilde_brightness, gensym("brightness"),
    A_DEFFLOAT,A_NULL);
  class_addmethod(rngs_tilde_class,
    (t_method) rngs_tilde_damping, gensym("damping"),
    A_DEFFLOAT,A_NULL);
  class_addmethod(rngs_tilde_class,
    (t_method) rngs_tilde_position, gensym("position"),
    A_DEFFLOAT,A_NULL);
  class_addmethod(rngs_tilde_class,
    (t_method) rngs_tilde_bypass, gensym("bypass"),
    A_DEFFLOAT,A_NULL);
  class_addmethod(rngs_tilde_class,
    (t_method) rngs_tilde_easter_egg, gensym("easter_egg"),
    A_DEFFLOAT,A_NULL);

}

void rngs_tilde_pitch(t_rngs_tilde *x, t_floatarg f) 
{
  x->f_pitch = f;
}


void rngs_tilde_structure(t_rngs_tilde *x, t_floatarg f) 
{
  x->f_structure = f;
}

void rngs_tilde_brightness(t_rngs_tilde *x, t_floatarg f) 
{
  x->f_brightness = f;
}

void rngs_tilde_damping(t_rngs_tilde *x, t_floatarg f) 
{
  x->f_damping = f;
}

void rngs_tilde_position(t_rngs_tilde *x, t_floatarg f) 
{
  x->f_position = f;
}


void rngs_tilde_bypass(t_rngs_tilde *x, t_floatarg f) 
{
  x->f_bypass = f;
}

void rngs_tilde_easter_egg(t_rngs_tilde *x, t_floatarg f) 
{
  x->f_easter_egg = f;

}



// puredata methods implementation - end
