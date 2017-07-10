#include "m_pd.h"

//IMPROVE - inlets
//IMPROVE - variable  buffer size

#include <algorithm> 
#include "braids/macro_oscillator.h"
#include "braids/envelope.h"

static t_class *brds_tilde_class;

inline float constrain(float v, float vMin, float vMax) {
  return std::max<float>(vMin,std::min<float>(vMax, v));
}

inline int constrain(int v, int vMin, int vMax) {
  return std::max<int>(vMin,std::min<int>(vMax, v));
}

typedef struct _brds_tilde {
  t_object  x_obj;

  braids::MacroOscillator osc;
  braids::Envelope envelope;

  t_float f_dummy;

  t_float f_shape;
  t_float f_pitch;
  t_float f_trig;
  t_float t_fm;
  t_float t_modulation;
  t_float f_colour;
  t_float f_timbre;

  t_float f_ad_attack;
  t_float f_ad_decay;


  // CLASS_MAINSIGNALIN  = in_sync;
  t_inlet*  x_in_pitch;
  t_inlet*  x_in_shape;
  t_inlet*  x_in_trig;
  t_outlet* x_out;
} t_brds_tilde;


//define pure data methods
extern "C"  {
  t_int*  brds_tilde_render(t_int *w);
  void    brds_tilde_dsp(t_brds_tilde *x, t_signal **sp);
  void    brds_tilde_free(t_brds_tilde *x);
  void*   brds_tilde_new(t_floatarg f);
  void    brds_tilde_setup(void);

  void brds_tilde_pitch(t_brds_tilde *x, t_floatarg f);
  void brds_tilde_shape(t_brds_tilde *x, t_floatarg f);
}




// void RenderBlock() {
//   static int16_t previous_pitch = 0;
//   static int16_t previous_shape = 0;
//   static uint16_t gain_lp;
//   envelope.Update(f_attach,f_decay);
//   uint32_t ad_value = envelope.Render();
  
//   if (ui.paques()) {
//     osc.set_shape(MACRO_OSC_SHAPE_QUESTION_MARK);
//   } else if (settings.meta_modulation()) {
//     int16_t shape = adc.channel(3);
//     shape -= settings.data().fm_cv_offset;
//     if (shape > previous_shape + 2 || shape < previous_shape - 2) {
//       previous_shape = shape;
//     } else {
//       shape = previous_shape;
//     }
//     shape = MACRO_OSC_SHAPE_LAST * shape >> 11;
//     shape += settings.shape();
//     if (shape >= MACRO_OSC_SHAPE_LAST_ACCESSIBLE_FROM_META) {
//       shape = MACRO_OSC_SHAPE_LAST_ACCESSIBLE_FROM_META;
//     } else if (shape <= 0) {
//       shape = 0;
//     }
//     MacroOscillatorShape osc_shape = static_cast<MacroOscillatorShape>(shape);
//     osc.set_shape(osc_shape);
//     ui.set_meta_shape(osc_shape);
//   } else {
//     osc.set_shape(settings.shape());
//   }
  
//   // Set timbre and color: CV + internal modulation.
//   uint16_t parameters[2];
//   for (uint16_t i = 0; i < 2; ++i) {
//     int32_t value = settings.adc_to_parameter(i, adc.channel(i));
//     Setting ad_mod_setting = i == 0 ? SETTING_AD_TIMBRE : SETTING_AD_COLOR;
//     value += ad_value * settings.GetValue(ad_mod_setting) >> 5;
//     CONSTRAIN(value, 0, 32767);
//     parameters[i] = value;
//   }
//   osc.set_parameters(parameters[0], parameters[1]);
  
//   // Check if the pitch has changed to cause an auto-retrigger
//   int32_t pitch_delta = pitch - previous_pitch;
//   if (settings.data().auto_trig &&
//       (pitch_delta >= 0x40 || -pitch_delta >= 0x40)) {
//     trigger_detected_flag = true;
//   }
//   previous_pitch = pitch;
  
//   pitch += jitter_source.Render(settings.vco_drift());
//   pitch += ad_value * settings.GetValue(SETTING_AD_FM) >> 7;
  
//   if (pitch > 16383) {
//     pitch = 16383;
//   } else if (pitch < 0) {
//     pitch = 0;
//   }
  
//   if (settings.vco_flatten()) {
//     pitch = Interpolate88(lut_vco_detune, pitch << 2);
//   }
//   osc.set_pitch(pitch + settings.pitch_transposition());

//   if (trigger_flag) {
//     osc.Strike();
//     envelope.Trigger(ENV_SEGMENT_ATTACK);
//     trigger_flag = false;
//   }
  
//   uint8_t* sync_buffer = sync_samples[render_block];
//   int16_t* render_buffer = audio_samples[render_block];
  
//   if (settings.GetValue(SETTING_AD_VCA) != 0
//     || settings.GetValue(SETTING_AD_TIMBRE) != 0
//     || settings.GetValue(SETTING_AD_COLOR) != 0
//     || settings.GetValue(SETTING_AD_FM) != 0) {
//     memset(sync_buffer, 0, kBlockSize);
//   }
  
//   osc.Render(sync_buffer, render_buffer, kBlockSize);
  
//   // Copy to DAC buffer with sample rate and bit reduction applied.
//   int16_t sample = 0;
//   size_t decimation_factor = decimation_factors[settings.data().sample_rate];
//   uint16_t bit_mask = bit_reduction_masks[settings.data().resolution];
//   int32_t gain = settings.GetValue(SETTING_AD_VCA) ? ad_value : 65535;
//   uint16_t signature = settings.signature() * settings.signature() * 4095;
//   for (size_t i = 0; i < kBlockSize; ++i) {
//     if ((i % decimation_factor) == 0) {
//       sample = render_buffer[i] & bit_mask;
//     }
//     sample = sample * gain_lp >> 16;
//     gain_lp += (gain - gain_lp) >> 4;
//     int16_t warped = ws.Transform(sample);
//     render_buffer[i] = Mix(sample, warped, signature);
//   }
//   render_block = (render_block + 1) % kNumBlocks;
// }


// puredata methods implementation -start
t_int* brds_tilde_perform(t_int *w)
{
  t_brds_tilde *x = (t_brds_tilde *)(w[1]);
  t_sample  *in_sync  =    (t_sample *)(w[2]);
  t_sample  *out =    (t_sample *)(w[3]);
  int n =  (int)(w[4]);

  x->envelope.Update(int(x->f_ad_attack * 8.0f ) , int(x->f_ad_decay * 8.0f) );
  uint32_t ad_value = x->envelope.Render();

  x->osc.set_pitch(x->f_pitch);
  x->osc.set_shape( (braids::MacroOscillatorShape) (x->f_shape * braids::MACRO_OSC_SHAPE_LAST)) ;
  x->osc.set_parameters(constrain(int(x->f_timbre * 32768.0f),0,32768), constrain(int(x->f_colour *32768.0f),0,32768)); 

  uint8_t* sync = new uint8_t[n];
  int16_t* outint = new int16_t[n];
  for (int i = 0; i < n; i++) {
    sync[i] = in_sync[i] * (1<<8) ;
  }


  x->osc.Render(sync, outint, n);

  for (int i = 0; i < n; i++) {
    out[i] = outint[i] / 65536.0f ;
  }

  delete [] sync;
  delete [] outint;

  return (w + 5); // # args + 1
}

void brds_tilde_dsp(t_brds_tilde *x, t_signal **sp)
{
  // add the perform method, with all signal i/o
  dsp_add(brds_tilde_render, 4,
          x,
          sp[0]->s_vec, sp[1]->s_vec, // signal i/o (clockwise)
          sp[0]->s_n);
}

void brds_tilde_free(t_brds_tilde *x)
{
  inlet_free(x->x_in_shape);
  inlet_free(x->x_in_pitch);
  outlet_free(x->x_out);
}

void *brds_tilde_new(t_floatarg f)
{
  t_brds_tilde *x = (t_brds_tilde *) pd_new(brds_tilde_class);

  x->f_shape= 0.0f;
  x->f_pitch = f;

  x->x_in_shape  = floatinlet_new (&x->x_obj, &x->f_shape);
  x->x_in_pitch  = floatinlet_new (&x->x_obj, &x->f_pitch);
  x->x_out   = outlet_new(&x->x_obj, &s_signal);

  x->osc.Init();
  x->osc.set_pitch(x->f_pitch);
  x->osc.set_shape( (braids::MacroOscillatorShape) (x->f_shape * braids::MACRO_OSC_SHAPE_LAST)) ;
  return (void *)x;
}

void brds_tilde_pitch(t_brds_tilde *x, t_floatarg f)
{
  x->f_pitch = f;
}
void brds_tilde_shape(t_brds_tilde *x, t_floatarg f)
{
  x->f_shape = f;
}

void brds_tilde_setup(void) {
  brds_tilde_class = class_new(gensym("brds~"),
                                         (t_newmethod)brds_tilde_new,
                                         0, sizeof(t_brds_tilde),
                                         CLASS_DEFAULT,
                                         A_DEFFLOAT, A_NULL);

  class_addmethod(  brds_tilde_class,
                    (t_method)brds_tilde_dsp,
                    gensym("dsp"), A_NULL);
  CLASS_MAINSIGNALIN(brds_tilde_class, t_brds_tilde, f_dummy);


  class_addmethod(brds_tilde_class,
                  (t_method) brds_tilde_pitch, gensym("pitch"),
                  A_DEFFLOAT, A_NULL);
  class_addmethod(brds_tilde_class,
                  (t_method) brds_tilde_shape, gensym("shape"),
                  A_DEFFLOAT, A_NULL);
}
// puredata methods implementation - end

