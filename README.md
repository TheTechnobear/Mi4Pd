## Technobear's Mutable Instruments for Pure Data , Mi4Pd

The first set are externals wrapping Mutable Instruments code used for Eurorack modules. 

Its truely appreciated that Olivier Gillet / Mutable Instruments has published this code under the MIT license, allowing derivatives, its a very generous act, which Id like to acknowledege.

If use code from this library, you should also make your code open source, and also respect the license for mutable instruments code.

all work in this repo has no guarantee and you use at your own risk.

As derivative works, these externals are **not** supported by Oliver/MI, please read the license in mi/LICENSE for details.
see also http://mutable-instruments.net

Grids comes from :
https://github.com/spoitras/Pd-Grids
developed by Spoitras and Henri David

## Status ##
This externals and patches are still in development, so their interfaces may changes.

## Contributions ##
Its best for users if there is a single source for these externals, so if you wish to extend/fix/contribute, Im very happy to take Pull Requests for enhancements / bug fixes etc.

## Externals ##
brds~
clds_reverb~
clds~
lmnts~
rngs_chorus~
rngs_ensemble~
rngs_reverb~
wrps~
grids

## Folder Structure ##
mi - contains MI code, which has some modifications for this purpose, see git logs for details
Organelle - contains Organelle patches based on this code, included binary externals. basically the source for patches uploaded to patchstorage.

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
