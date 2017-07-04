# Makefile to build technobear library for Pure Data.
# Needs Makefile.pdlibbuilder as helper makefile for platform-dependent build
# settings and rules.

# library name
lib.name = technobear


CPPFLAGS += -I mi
MI_MODULES = CLOUDS
MUTABLE_INSTRUMENTS = mi
#Axoloti optional modules, here we define defaults if none supplied
ifeq ($(MI_MODULES),)
	MI_MODULES = BRAIDS RINGS CLOUDS STREAMS ELEMENTS WARPS
endif

#optional modules
ifneq (,$(findstring BRAIDS,$(MI_MODULES)))
#$(info including BRAIDS)
BRAIDS_SRC = \
       ${MUTABLE_INSTRUMENTS}/braids/braids_resources.cpp \
       ${MUTABLE_INSTRUMENTS}/braids/analog_oscillator.cpp \
       ${MUTABLE_INSTRUMENTS}/braids/digital_oscillator.cpp \
       ${MUTABLE_INSTRUMENTS}/braids/macro_oscillator.cpp 
endif

ifneq (,$(findstring WARPS,$(MI_MODULES)))
#$(info including WARPS)
WARPS_SRC = \
       ${MUTABLE_INSTRUMENTS}/warps/warps_resources.cpp \
       ${MUTABLE_INSTRUMENTS}/warps/dsp/filter_bank.cpp \
       ${MUTABLE_INSTRUMENTS}/warps/dsp/modulator.cpp \
       ${MUTABLE_INSTRUMENTS}/warps/dsp/oscillator.cpp \
       ${MUTABLE_INSTRUMENTS}/warps/dsp/vocoder.cpp 
endif

ifneq (,$(findstring ELEMENTS,$(MI_MODULES)))
#$(info including ELEMENTS)
ELEMENTS_SRC = \
       ${FIRMWARE}/axoloti_mi.cpp \
       ${MUTABLE_INSTRUMENTS}/elements/dsp/multistage_envelope.cpp \
       ${MUTABLE_INSTRUMENTS}/elements/dsp/ominous_voice.cpp \
       ${MUTABLE_INSTRUMENTS}/elements/dsp/resonator.cpp \
       ${MUTABLE_INSTRUMENTS}/elements/dsp/string.cpp \
       ${MUTABLE_INSTRUMENTS}/elements/dsp/tube.cpp \
       ${MUTABLE_INSTRUMENTS}/elements/dsp/exciter.cpp \
       ${MUTABLE_INSTRUMENTS}/elements/elements_resources.cpp \
       ${MUTABLE_INSTRUMENTS}/elements/part.cpp \
       ${MUTABLE_INSTRUMENTS}/elements/voice.cpp 
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
       ${MUTABLE_INSTRUMENTS}/rings/rings_resources.cpp 
endif

ifneq (,$(findstring STREAMS,$(MI_MODULES)))
#$(info including STREAMS)
STREAMS_SRC = \
       ${MUTABLE_INSTRUMENTS}/streams/streams_resources.cpp \
       ${MUTABLE_INSTRUMENTS}/streams/compressor.cpp \
       ${MUTABLE_INSTRUMENTS}/streams/envelope.cpp \
       ${MUTABLE_INSTRUMENTS}/streams/follower.cpp \
       ${MUTABLE_INSTRUMENTS}/streams/lorenz_generator.cpp \
       ${MUTABLE_INSTRUMENTS}/streams/processor.cpp \
       ${MUTABLE_INSTRUMENTS}/streams/svf.cpp \
       ${MUTABLE_INSTRUMENTS}/streams/vactrol.cpp 
endif

#$(info including base MI)

MISRC = \
       # ${MUTABLE_INSTRUMENTS}/mutable_resources.cpp \
       # ${MUTABLE_INSTRUMENTS}/stmlib/dsp/units.cpp \
       # ${MUTABLE_INSTRUMENTS}/stmlib/dsp/atan.cpp \
       # ${MUTABLE_INSTRUMENTS}/stmlib/utils/random.cpp \
       ${BRAIDS_SRC} \
       ${WARPS_SRC} \
       ${ELEMENTS_SRC} \
       ${CLOUDS_SRC} \
       ${RINGS_SRC} \
       ${STREAMS_SRC}

 

# input source file (class name == source file basename)
class.sources = tbtest/tbtest.cpp \
				tb_clds_reverb_tilde/tb_clds_reverb~.cpp \
				${MISRC}

# all extra files to be included in binary distribution of the library
datafiles = tbtest-help.pd \
			tbtest-meta.pd \
			tb_clds_reverb~-help.pd \
			README.md

# include Makefile.pdlibbuilder from submodule directory 'pd-lib-builder'
PDLIBBUILDER_DIR=pd-lib-builder/
include $(PDLIBBUILDER_DIR)/Makefile.pdlibbuilder
