areaDetector
==========
The top-level repository for the
[EPICS](http://www.aps.anl.gov/epics/) 
[areaDetector](http://cars.uchicago.edu/software/epics/areaDetector.html) 
software.  This repository contains mostly documentation, configuration files,
and a top-level Makefile to build the entire areaDetector package.

The areaDetector code is contained in submodules under this module.  Two
of these are "core" submodules:

1. [ADBinaries](https://github.com/areaDetector/ADBinaries). 
  This contains prebuilt support libraries (HDF5, XML2, etc.) for Windows.
2. [ADCore](https://github.com/areaDetector/ADCore).
  This contains the base classes, plugins, documentation and an example simulation detector.
  
All of the other submodules (ADProsilica, ADPilatus, etc.) contain drivers and EPICS IOC applications
for specific detectors.

The [Installation Guide](INSTALL_GUIDE.md) provides build and install
instructions.

Additional information:
* [Overview documentation](http://cars.uchicago.edu/software/epics/areaDetector.html).
* [Detailed documentation](http://cars.uchicago.edu/software/epics/areaDetectorDoc.html).
* [Release notes and links to source and binary releases](RELEASE.md).

See the following documentation about the git submodule feature and how it can 
be used: 
* [The Git Book](http://git-scm.com/docs/git-submodule)
* [Kernel.org Git tutorial on submodules](https://git.wiki.kernel.org/index.php/GitSubmoduleTutorial)

To get a clone of the full areaDetector distribution (core, plugins and drivers) do a recursive clone of this repository:
`git clone --recursive https://github.com/areaDetector/areaDetector.git`
