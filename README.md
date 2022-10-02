# SPS-SABRE Data Analysis Package
Version 4
This is a software package designed to help experimenters analyze data from SPS-SABRE at FSU. 
It can convert CoMPASS data to ROOT, sort the data in time, build events, perform preliminary analysis, and provide basic plots.

## Installation
To build and install the event builder, the CMake build system is used. To build, simply run the following commands from the SPS_SABRE_EventBuilder directory:
```
mkdir build
cd build
cmake ..
make
```

To clone the repository use `git clone --recursive https://github.com/sesps/SPS_SABRE_EventBuilder.git`. If you're using the devel branch be sure to specify this with the `--branch` flag. The recursive flag is important; this tells github to pull all submodules associated with the repository. 

The binaries are installed to the `bin` directory of the event builder, and should be run from the event builder directory (i.e. `./bin/EventBuilderGui`). THe `bin` directory also contains a shell script named `archivist` for transferring data from a CoMPASS project to the
event builder workspace.

In general, one should only build for Release (this is the default), for maximum optimization. However, it can be useful to run in Debug (change the cmake command to `cmake -DCMAKE_BUILD_TYPE_DEBUG ..`) when testing new features.

## EventBuilder vs. EventBuilderGui
There are two programs provided. They are `EventBuilderGui` and `EventBuilder`. The first is a full GUI version of the event builder. The GUI supports all conversion methods and the plotting tool.

## Details
For more information see the [wiki](https://github.com/sesps/SPS_SABRE_EventBuilder/wiki), which describes the app in much more detail.
Additionally, check the [FAQ](https://github.com/sesps/SPS_SABRE_EventBuilder/wiki/FAQ) if you're having trouble with something.

## System Requirements
- Requires ROOT version which supports CMake dictionary generation
- Requires CMake >= 3.16
- This version is for data from CAEN CoMPASS >= 2.0. Data from older CoMPASS versions are not compatible.