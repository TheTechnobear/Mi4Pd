## Technobears pure data library ##
This repo contains pure data externals that Im using


The first set are externals wrapping Mutable Instruments code used for Eurorack modules. 

Its truely appreciated that Olivier Gillet / Mutable Instruments has published this code under the MIT license, allowing derivatives, its a generous act.
As derivative works, these externals are **not** supported by Oliver/MI, please read the license in mi/LICENSE for details.

## Externals ##
wrps~ - based on MI Warps 
clds_reverb~ - based on reverb from MI Clouds

Note: these externals will not operate exactly as the Eurorack reasons for a number of technical reasons (e.g. sample rate), but I think they are still useful.

## Building ##
This build uses [pd-lib-builder](https://github.com/pure-data/pd-lib-builder/)

To build you will need a copy of pure data installed.
By default, pd-lib-builder will attempt to auto-locate an install of Pure Data, to find the appropriate header files.
If the defaults are used, then you can use

    make install 

This will install the externals into the library directory, under the 'technobear' library.

If your puredata setup is non-standard, you can use PDINCLUDEDIR and PDLIBDIR, e.g. for Bela you can use

	make PDINCLUDEDIR=~/Bela/include/libpd/ PDLIBDIR=~/Bela/projects/pd-externals instal


See `make help` for more details.