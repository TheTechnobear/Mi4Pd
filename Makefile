# Makefile to build technobear library for Pure Data.
# Needs Makefile.pdlibbuilder as helper makefile for platform-dependent build
# settings and rules.

# library name
lib.name = technobear

ifneq ($(machine), armv7l)

	CPPFLAGS += -DTEST
endif

CPPFLAGS += -I mi

MI_MODULES = CLOUDS WARPS
MUTABLE_INSTRUMENTS = mi
#optional modules, here we define defaults if none supplied
ifeq ($(MI_MODULES),)
	MI_MODULES = BRAIDS RINGS CLOUDS STREAMS ELEMENTS WARPS
endif

#optional modules
ifneq (,$(findstring BRAIDS,$(MI_MODULES)))
#$(info including BRAIDS)
BRAIDS_SRC = \
       ${MUTABLE_INSTRUMENTS}/braids/braids_resources.cc \
       ${MUTABLE_INSTRUMENTS}/braids/analog_oscillator.cc \
       ${MUTABLE_INSTRUMENTS}/braids/digital_oscillator.cc \
       ${MUTABLE_INSTRUMENTS}/braids/macro_oscillator.cc 
endif

ifneq (,$(findstring WARPS,$(MI_MODULES)))
#$(info including WARPS)
WARPS_SRC = \
       ${MUTABLE_INSTRUMENTS}/warps/warps_resources.cc \
       ${MUTABLE_INSTRUMENTS}/warps/dsp/filter_bank.cc \
       ${MUTABLE_INSTRUMENTS}/warps/dsp/modulator.cc \
       ${MUTABLE_INSTRUMENTS}/warps/dsp/oscillator.cc \
       ${MUTABLE_INSTRUMENTS}/warps/dsp/vocoder.cc 
endif

ifneq (,$(findstring ELEMENTS,$(MI_MODULES)))
#$(info including ELEMENTS)
ELEMENTS_SRC = \
       ${MUTABLE_INSTRUMENTS}/elements/dsp/multistage_envelope.cc \
       ${MUTABLE_INSTRUMENTS}/elements/dsp/ominous_voice.cc \
       ${MUTABLE_INSTRUMENTS}/elements/dsp/resonator.cc \
       ${MUTABLE_INSTRUMENTS}/elements/dsp/string.cc \
       ${MUTABLE_INSTRUMENTS}/elements/dsp/tube.cc \
       ${MUTABLE_INSTRUMENTS}/elements/dsp/exciter.cc \
       ${MUTABLE_INSTRUMENTS}/elements/elements_resources.cc \
       ${MUTABLE_INSTRUMENTS}/elements/part.cc \
       ${MUTABLE_INSTRUMENTS}/elements/voice.cc 
endif

ifneq (,$(findstring CLOUDS,$(MI_MODULES)))
#$(info including CLOUDS)
CLOUDS_SRC = \
       ${MUTABLE_INSTRUMENTS}/clouds/clouds_resources.cc \
       ${MUTABLE_INSTRUMENTS}/clouds/dsp/correlator.cc \
       ${MUTABLE_INSTRUMENTS}/clouds/dsp/mu_law.cc \
       ${MUTABLE_INSTRUMENTS}/clouds/dsp/granular_processor.cc \
       ${MUTABLE_INSTRUMENTS}/clouds/dsp/pvoc/frame_transformation.cc \
       ${MUTABLE_INSTRUMENTS}/clouds/dsp/pvoc/phase_vocoder.cc \
       ${MUTABLE_INSTRUMENTS}/clouds/dsp/pvoc/stft.cc 
endif

ifneq (,$(findstring RINGS,$(MI_MODULES)))
#$(info  including RINGS)
RINGS_SRC = \
       ${MUTABLE_INSTRUMENTS}/rings/rings_resources.cc 
endif

ifneq (,$(findstring STREAMS,$(MI_MODULES)))
#$(info including STREAMS)
STREAMS_SRC = \
       ${MUTABLE_INSTRUMENTS}/streams/streams_resources.cc \
       ${MUTABLE_INSTRUMENTS}/streams/compressor.cc \
       ${MUTABLE_INSTRUMENTS}/streams/envelope.cc \
       ${MUTABLE_INSTRUMENTS}/streams/follower.cc \
       ${MUTABLE_INSTRUMENTS}/streams/lorenz_generator.cc \
       ${MUTABLE_INSTRUMENTS}/streams/processor.cc \
       ${MUTABLE_INSTRUMENTS}/streams/svf.cc \
       ${MUTABLE_INSTRUMENTS}/streams/vactrol.cc 
endif

#$(info including base MI)

# ${MUTABLE_INSTRUMENTS}/mutable_resources.cpp 
MISRC = \
       ${MUTABLE_INSTRUMENTS}/stmlib/dsp/units.cc \
       ${MUTABLE_INSTRUMENTS}/stmlib/dsp/atan.cc \
       ${MUTABLE_INSTRUMENTS}/stmlib/utils/random.cc \
       ${BRAIDS_SRC} \
       ${WARPS_SRC} \
       ${ELEMENTS_SRC} \
       ${CLOUDS_SRC} \
       ${RINGS_SRC} \
       ${STREAMS_SRC}

common.sources = ${MISRC}

# input source file (class name == source file basename)
class.sources = tbtest/tbtest.cpp \
				tb_clds_reverb_tilde/tb_clds_reverb~.cpp \
				tb_wrps_tilde/tb_wrps~.cpp 

# all extra files to be included in binary distribution of the library
datafiles = tbtest-help.pd \
			tbtest-meta.pd \
			tb_clds_reverb~-help.pd \
			README.md


include Makefile.pdlibbuilder
