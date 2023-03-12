areaDetector
==========

[![Build Status](https://travis-ci.com/areaDetector/areaDetector.svg?branch=master)](https://travis-ci.com/areaDetector/areaDetector)

The top-level repository for the
[EPICS](http://www.aps.anl.gov/epics/) 
[areaDetector](https://github.com/areaDetector/areaDetector/blob/master/README.md) 
software.  This repository contains mostly documentation, configuration files,
and a top-level Makefile to build the entire areaDetector package.

The areaDetector code is contained in submodules under this module.  Four
of these are "core" submodules:

1. [ADSupport](https://github.com/areaDetector/ADSupport). 
  This contains the source code for support libraries (TIFF, JPEG, HDF5, XML2, etc.).
  This is required for Windows and vxWorks, and can optionally be used on Linux and Darwin.
2. [ADCore](https://github.com/areaDetector/ADCore).
  This contains the base classes, plugins, and documentation.
3. [ADSimDetector](https://github.com/areaDetector/ADSimDetector).
  This contains an example 2-D simulation detector driver and IOC.
4. [ADViewers](https://github.com/areaDetector/ADViewers).
  This contains viewers for displaying areaDetector images in ImageJ and IDL.
  
All of the other submodules (ADProsilica, ADPilatus, etc.) contain drivers and EPICS IOC applications
for specific detectors.

Additional information:
* [Documentation home](https://areadetector.github.io/areaDetector/index.html).
* [Installation Guide](https://areadetector.github.io/areaDetector/install_guide.html)
* [User Guide](https://areadetector.github.io/areaDetector/user_guide.html).
* [Release notes and links to source and binary releases](RELEASE.md).

See the following documentation about the git submodule feature and how it can 
be used: 
* [The Git Book](https://git-scm.com/docs/git-submodule)
* [Git Book tutorial on submodules](https://git-scm.com/book/en/v2/Git-Tools-Submodules)

To get a clone of the full areaDetector distribution (core, plugins and drivers) do a recursive clone of this repository:

`git clone --recursive https://github.com/areaDetector/areaDetector.git`
