#include "m_pd.h"

//IMPROVE - inlets
//FIXME   - fixed sample rate
//TODO - help file

#include "warps/dsp/modulator.h"

static t_class *wrps_tilde_class;

typedef struct _wrps_tilde {
  t_object  x_obj;

  t_float  f_dummy;

  t_float  f_drive1;
  t_float  f_drive2;
  t_float  f_algo;
  t_float  f_density;

  // CLASS_MAINSIGNALIN  = in_left;
  t_inlet*  x_in_right;
  t_outlet* x_out_left;
  t_outlet* x_out_right;

  // clouds:reverb
  warps::Modulator processor;
  warps::ShortFrame* ibuf;
  warps::ShortFrame* obuf;
  int iobufsz;
} t_wrps_tilde;


//define pure data methods
extern "C"  {
  t_int* wrps_tilde_render(t_int *w);
  void wrps_tilde_dsp(t_wrps_tilde *x, t_signal **sp);
  void wrps_tilde_free(t_wrps_tilde *x);
  void* wrps_tilde_new(t_floatarg f);
  void wrps_tilde_setup(void);
  void wrps_tilde_drive1(t_wrps_tilde *x, t_floatarg f);
  void wrps_tilde_drive2(t_wrps_tilde *x, t_floatarg f);
  void wrps_tilde_algo(t_wrps_tilde *x, t_floatarg f);
  void wrps_tilde_density(t_wrps_tilde *x, t_floatarg f);
}

// puredata methods implementation -start
t_int *wrps_tilde_render(t_int *w)
{
  t_wrps_tilde *x   = (t_wrps_tilde *)(w[1]);
  t_sample  *in_left   = (t_sample *)(w[2]);
  t_sample  *in_right  = (t_sample *)(w[3]);
  t_sample  *out_left  = (t_sample *)(w[4]);
  t_sample  *out_right = (t_sample *)(w[5]);
  int n =  (int)(w[6]);

  if (n > x->iobufsz) {
    delete [] x->ibuf;
    delete [] x->obuf;
    x->iobufsz = n;
    x->ibuf = new warps::ShortFrame[x->iobufsz];
    x->obuf = new warps::ShortFrame[x->iobufsz];
  }

  x->processor.mutable_parameters()->channel_drive[0]=x->f_drive1;
  x->processor.mutable_parameters()->channel_drive[1]=x->f_drive2;
  x->processor.mutable_parameters()->modulation_parameter=x->f_density;
  x->processor.mutable_parameters()->modulation_algorithm=x->f_algo;

  for (int i = 0; i < n; i++) {
    x->ibuf[i].l = in_left[i] * 1024;
    x->ibuf[i].r = in_right[i] * 1024;
  }

  x->processor.Process(x->ibuf, x->obuf,n);

  for (int i = 0; i < n; i++) {
    out_left[i] = x->obuf[i].l / 1024.0f;
    out_right[i] = x->obuf[i].r / 1024.0f;
  }

  return (w + 7); // # args + 1
}

void wrps_tilde_dsp(t_wrps_tilde *x, t_signal **sp)
{
  // add the perform method, with all signal i/o
  dsp_add(wrps_tilde_render, 6,
          x,
          sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec, // signal i/o (clockwise)
          sp[0]->s_n);
}

void wrps_tilde_free(t_wrps_tilde *x)
{
  delete [] x->ibuf;
  delete [] x->obuf;

  inlet_free(x->x_in_right);
  outlet_free(x->x_out_left);
  outlet_free(x->x_out_right);
}

void *wrps_tilde_new(t_floatarg f)
{
  t_wrps_tilde *x = (t_wrps_tilde *) pd_new(wrps_tilde_class);
  x->iobufsz = 64;
  x->ibuf = new warps::ShortFrame[x->iobufsz];
  x->obuf = new warps::ShortFrame[x->iobufsz];

  x->f_drive1 = 1.0f;
  x->f_drive2 = 1.0f;
  x->f_algo   = 0.5f;
  x->f_density = 0.5f;

  x->x_in_right   = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal);
  x->x_out_left   = outlet_new(&x->x_obj, &s_signal);
  x->x_out_right  = outlet_new(&x->x_obj, &s_signal);

  x->processor.Init(44100.0f);
  return (void *)x;
}

void wrps_tilde_drive1(t_wrps_tilde *x, t_floatarg f)
{
  x->f_drive1 = f;
}
void wrps_tilde_drive2(t_wrps_tilde *x, t_floatarg f)
{
  x->f_drive2 = f;
}
void wrps_tilde_density(t_wrps_tilde *x, t_floatarg f)
{
  x->f_density= f;
}
void wrps_tilde_algo(t_wrps_tilde *x, t_floatarg f)
{
  x->f_algo = f;
}

void wrps_tilde_setup(void) {
  wrps_tilde_class = class_new(gensym("wrps~"),
                                         (t_newmethod)wrps_tilde_new,
                                         (t_method) wrps_tilde_free,
                                         sizeof(t_wrps_tilde),
                                         CLASS_DEFAULT,
                                         A_DEFFLOAT, A_NULL);

  class_addmethod(  wrps_tilde_class,
                    (t_method)wrps_tilde_dsp,
                    gensym("dsp"), A_NULL);
  CLASS_MAINSIGNALIN(wrps_tilde_class, t_wrps_tilde, f_dummy);


  class_addmethod(wrps_tilde_class,
                  (t_method) wrps_tilde_drive1, gensym("drive1"),
                  A_DEFFLOAT, A_NULL);
  class_addmethod(wrps_tilde_class,
                  (t_method) wrps_tilde_drive2, gensym("drive2"),
                  A_DEFFLOAT, A_NULL);
  class_addmethod(wrps_tilde_class,
                  (t_method) wrps_tilde_density, gensym("density"),
                  A_DEFFLOAT, A_NULL);
  class_addmethod(wrps_tilde_class,
                  (t_method) wrps_tilde_algo, gensym("algo"),
                  A_DEFFLOAT, A_NULL);
}
// puredata methods implementation - end

