An example Pure Data external using [pd-lib-builder](https://github.com/pure-data/pd-lib-builder). You can use this project to bootstrap your own Pure Data external development.

## Usage ##

Clone and build this example via:

    git clone --recursive https://github.com/pure-data/helloworld.git
    cd helloworld
    make

Make sure you use the `--recursive` flag when checking out the repository so that the pd-lib-builder dependency is also checked out. If you forgot to add it, you can also manually fetch pd-lib-builder via:

    cd helloworld
    git submodule init
    git submodule update

_Note: The "Download zip" option on GitHub currently does not check out dependencies so pd-lib-builder will be missing in your download. The preferred method is to use git._

## Build ##

You should have a copy of the pure-data source code - the following build command assumes it is in the `../pure-data` directory. [This page explains how you can get the pure-data source code](https://puredata.info/docs/developer/GettingPdSource).

By default, pd-lib-builder will attempt to auto-locate an install of Pure Data. The following command will build the external and install the distributable files into a subdirectory called `build/helloworld`.

    make install pdincludepath=../pure-data/src/ objectsdir=./build

See `make help` for more details.

## Distribute ##

If you are using the [deken](https://github.com/pure-data/deken/) externals packaging tool you can then submit your external to the [puredata.info repository](http://puredata.info) for other people to find, like this:

    deken upload ./build/helloworld

You will need to have an account on the site. You probably also want to have a valid GPG key to sign the package so that users can prove that it waas created by you.
