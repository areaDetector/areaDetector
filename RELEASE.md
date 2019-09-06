areaDetector Releases
=====================

Introduction
------------

The latest untagged master branch can be obtained at
https://github.com/areaDetector/areaDetector.

Tagged source code and pre-built binary releases prior to R2-0 are included
in the areaDetector releases available via links at
https://cars.uchicago.edu/software/epics/areaDetector.html.

Tagged source code releases from R2-0 onward can be obtained at 
https://github.com/areaDetector/areaDetector/releases.

Tagged prebuilt binaries from R2-0 onward can be obtained in the detector-specific directories in:
https://cars.uchicago.edu/software/pub/.  For example the Pilatus binaries are in 
https://cars.uchicago.edu/software/pub/ADPilatus.

The versions of EPICS base, asyn, and other synApps modules used for each release can be obtained from 
the EXAMPLE_RELEASE_PATHS.local, EXAMPLE_RELEASE_LIBS.local, and EXAMPLE_RELEASE_PRODS.local
files respectively, in the appropriate release of the 
[top-level areaDetector](https://github.com/areaDetector/areaDetector) repository.


Versions
--------

Each submodule contains detailed release notes for each release of that submodule.  The release notes below
only provide a short summary of the most significant items from the submodules.

### R3-7 (August 9, 2019)

* New submodule repositories
  * ADRIXSCam
    * Driver for the RIXSCAM developed by [XCAM UK](http://www.xcam.co.uk/)
* ADCore R3-7.
  * Bug fixes.  Requires asyn R4-36.
* ADSupport R1-9.
  * Fixes for HDF5 compression plugins.
* ADSpinnaker
  * The master branch now uses ADGenICam, and is ready for testing.

### R3-6 (May 29, 2019)

* New submodule repositories
  * specsAnalyser
    * Driver for SPECS Phoibos electron spectrometers, with DLD (Delay Line Detector)
    * From Pete Leicester, Stuart Wilkins and others.
  * ADCompVision
    * Plugin to perform imaging processing using opencv library
    * From Jakub Wlodek and Kaz Gofron.
  * ADPluginBar
    * Plugin to process bar codes
    * From Jakub Wlodek and Kaz Gofron.
  * ADPcoWin
    * Driver for PCO cameras, using their SDK which is Windows only.
    * From Jonathan Thompson and others.
  * ADMMPAD
    * Driver for the MMPAD detectors from Sol Gruner's group at Cornell.
    * From Mark Rivers.
  * ADGenICam
    * New base class for GenICam cameras.  Used by ADAravis, ADSpinnaker, and ADVimba.
    * Not yet ready for production use.
    * From Mark Rivers.
  * ADAravis
    * Designed to replace aravisGigE.  It is simpler because it uses ADGeniICam.
    * Not yet ready for production use.
    * From Mark Rivers, based on aravisGigE which is by Tom Cobb and others.
  * ADVimba
    * Driver for AVT/Prosilica cameras using their new Vimba SDK.  Uses ADGenICam.
    * Not yet ready for production use.
    * From Mark Rivers.
  * ADSpinnaker
    * Driver for FLIR/Point Grey cameras using their new Spinnaker SDK.
    * This is not a new repository, but it now contains a new GenICam branch that uses ADGenICam.
    * GenICam branch is not yet ready for production use
    * From Mark Rivers.

### R3-5 (April 12, 2019)

* Documentation
  * Began conversion of all areaDetector documentation from raw HTML documentation to .rst files using Sphinx.
  * Added a new docs/ directory which replaces the old documentation directory.
  * The old .html files were converted to .rst using [pandoc](https://pandoc.org).
    Most documentation includes tables that describe the driver or plugin parameters and
    record names.  These tables do not convert well to .rst so they have been left as native
    html in the files.
  * The new documentation is hosted at [areaDetector.github.io](https://areaDetector.github.io).
  * At this point the top-level documentation in this areaDetector respository has been converted,
    as well as ADCore, ADViewers, ADProsilica, and ADFastCDD.
  * Many thanks to Stuart Wilkins for this major effort.

* configure/EXAMPLE_CONFIG_SITE.local
  Added WITH_BITSHUFFLE and BITSHUFFLE_EXTERNAL for bitshuffle which is supported in new
  versions of ADSupport, ADCore, and ADEiger.

### R3-4 (December 3, 2018)

* No significant changes in this top-level repository.
* Committed ADCore R3-4, ADSupport R1-6, ADViewers R1-3.

### R3-3-2 (July 9, 2018)

* configure/EXAMPLE_CONFIG_SITE.local

  Added new WITH_QSRV variable.  3.14.12 supports pvAccess but does not support qsrv.
  By setting WITH_PVA=YES and WITH_QSRV=NO IOCs can be built with pvAccess support but without qsrv support.

### R3-3-1 (July 1, 2018)

* configure/EXAMPLE_CONFIG_SITE.local*

  Prior to areaDetector R3-3-1 these files had commented out lines like the following for defining include library paths:
  ```
  #HDF5_INCLUDE = -I$(HDF5)/include
  ```
  These examples were incorrect, because the Makefiles in ADCore and driver repositories were adding the -I before
  these variables.  It has been decided that the Makefiles are the correct place to add the -I flags, so the -I
  has been removed from configure/EXAMPLE_CONFIG_SITE.local* files.
  
  The Makefiles in ADCore and all detector driver repositories now all use $(addprefix -I, $(XXX_INCLUDE)) to add the
  -I prefix (where XXX is HDF5, XML2, etc.)  This means that it will work even if these variables contain multiple paths.
 
* configure/EXAMPLE_CONFIG_SITE.local
  
  Changed the definition of the GLIB include files from this
  ```
  GLIB_INC1=/usr/include/glib-2.0
  GLIB_INC2=/usr/lib64/glib-2.0/include
  ```
  to this:
  ```
  GLIB_INCLUDE = /usr/include/glib-2.0 /usr/lib64/glib-2.0/include
  ```
  This is cleaner and is now possible because the Makefile (in ararisGigE) uses addprefix to add the -I to multiple paths.

* INSTALL_GUIDE.md

  Documented that user-defined include directories should not include the -I flag.

### R3-3 (June 27, 2018)

* Changes in the configure/RELEASE* files to ensure that EPICS_BASE is defined last.
* Changes to allow building when some modules come from a Debian package and some do not.
  - If a Debian package is being used for any module then it must also be used for EPICS_BASE.
* The INSTALL_GUIDE.md has been updated to describe the new system.
* This is a brief description of what has changed and what users need to do.
  - In the areaDetector/configure directory type
    ```./copyFromExample```.
    This will copy the `EXAMPLE_*` files to CONFIG* and RELEASE*.
  - Edit CONFIG_SITE.local and CONFIG_SITE.local.$(EPICS_HOST_ARCH) as required.
  - Edit RELEASE.local and RELEASE.local.$(EPICS_HOST_ARCH) as required.
  - Edit RELEASE_SUPPORT.local.  This file replaces RELEASE_PATHS.local from earlier releases. 
    Define SUPPORT to be the top location of the modules that you **do not** want to get from the Debian package.
    - RELEASE_SUPPORT.$(EPICS_HOST_ARCH) can be used to define an alternative location of SUPPORT for a specific
      architecture.  It is typically only needed when building Windows and Linux in the same tree.
  - Edit RELEASE_LIBS.local.  Define the locations of the modules. Modules from the Debian package must be defined last.  
    - These would normally be defined using the SUPPORT definition done above.  
    - Define the location of EPICS_BASE.  
      - If using the Debian package then define EPICS_BASE to be the location of the Debian distribution.
  - Edit RELEASE_PRODS.local.  Do the same as for RELEASE_LIBS.local above.  
    - If using the Debian package then the definitions for modules that you want to use from the 
      Debian package (e.g. CALC, BUSY, etc.) must come last.
  - RELEASE_BASE.$(EPICS_HOST_ARCH) can be used to define an alternative location of EPICS_BASE for a specific
    architecture.  It is typically only needed when building Windows and Linux in the same tree.
* The RELEASE files in many detector modules have been updated so that they only load RELEASE_LIBS_INCLUDE or 
  RELEASE_PRODS_INCLUDE from areaDetector/configure.  This change was previously optional and had already been done for
  some detector modules in previous releases.  This change is now required and so all detector modules have been updated 
  to use it.
* Added new ADSpinnaker submodule.  This is for detectors from Point Grey/FLIR using their new Spinnaker SDK.
* Added ADEiger submodule.  This is for Eiger detectors from Dectris.  Thanks to Bruno Martins for this.

### R3-2 (January 28, 2018)

* Requirements:
  - ADCore R3-2
  - ADSupport R1-4 or higher
  - asyn R4-32 or higher
  - EPICS base 3.14.12.4 or higher
* Added the blosc compression library to ADSupport and to the NDFileHDF5 writer in ADCore.
* Added capability to automatically convert medm adl files to edm edl, caQtDM ui, and CSS-BOY opi files.
  This is done by running make in the yyyAPP/op/ directory.  The files are placed in the edl/autoconvert
  ui/autoconvert, and opi/autoconvert directories.  This will keep these files up to date with the medm files.


### R3-1 (July 3, 2017)

* Requirements:
  - ADCore R3-1
  - ADSupport R1-3 or higher
  - asyn R4-32 or higher
  - EPICS base 3.14.12.4 or higher
* Added GraphicsMagick to ADSupport.
* Changed ADCore to allow building with GraphicsMagick either from ADSupport or using a system library.
* Added support for 32-bit images in NDFileMagick in ADCore.
* Changed configure/EXAMPLE_CONFIG_SITE.local to use XXX_EXTERNAL=NO for all libraries in ADSupport.


### R3-0 (May 5, 2017)

* Requirements
  - ADCore R3-0
  - ADSupport R1-2 or higher
  - asyn R4-31 or higher
  - EPICS base 3.14.12.4 or higher
* Prior to releasing R3-0 the following tags were applied to modules that needed to change for R3-0.  
  These changes were required because these modules contain plugins and the NDPluginDriver base class 
  has different constructor arguments and methods.
  Thus these tags are compatible with R2-6.  Everything after this requires R3-0 of ADCore.

Repository   | Tag
----------   | ---
ADFastCCD    | R1-0 (initial tag)
ADPCO        | R1-0
ADPluginEdge | R1-1
ADnED        | rel1.8_20170502
ffmpegServer | R1-0 (initial tag)

* ADAndor is the only driver that required changes for R3-0 because it was using TinyXml and has been
  changed to use libxml2. ADAndor R2-5 is the last release that works with ADCore R2-6, everything later 
  will require ADCore R3-0


### R2-6 (February 22, 2017)

* Requirements
  - ADCore R2-6
  - ADSupport R1-0 or higher
  - Requires asyn R4-31
* ADExample has been replaced by 3 new repositories
  * ADSimDetector  The simulation detector
  * ADCSimDetector The ADC simulation detector
  * pvaDriver      The EPICS V4 PVAccess driver
* New NDDriverStdArrays that supports sending NDArrays to an areaDetector IOC from Channel Access clients
  via waveform records and other PVs.
* New releases of many drivers with minor changes to source code and medm screens for the enhancements 
  in ADCore R2-6.


### R2-5 (October 28, 2016)

* Requires R2-5 of ADCore.  This requires some changes to CONFIG_SITE.local.
  ADCore R2-5 has many new features, including support for HDF5 SWMR mode, support for EPICS V4
  NTNDArrays, and new TimeSeries and FFT plugins.
* Added ADSupport repository.  This replaces ADBinaries and some of the source
  directories that were previously in ADCore.  It builds support for TIFF, JPEG, NETCDF
  ZLIB, SZIP, HDF5, XML2, and NEXUS.  All of these support libraries are now optional.
  The configure directory and INSTALL_GUIDE.md have many changes to support this.
* New ADLambda repository that supports the detectors from Lambda.  Thanks to John Hammonds for this.
* New ADCameraLink repository that supports Silicon Software and Dalsa/Coreco.  Thanks to Tim Madden for this.
* New ADPCO repository that supports PCO CameraLink cameras.  Thanks to Tim Madden for this.
* Added LICENSE files to all repositories. 

### R2-4 (September 18, 2015)

* Requires R2-4 of ADCore.  This requires some changes to CONFIG_SITE.local.
* Moved the simDetector from ADCore into a new repository ADExample.
* New ADMythen module for Dectris Mythen detectors.  Written by Matthew Moore at the APS.
* New ADPluginEdge module that provides an edge detection plugin using the opencv library.  Written by
  Keith Brister from Northwestern University.
* configure/RELEASE.local now optionally includes configure/RELEASE.local.$(EPICS_HOST_ARCH).
  This allows restricting building of a detector module to a subset of supported architectures when 
  building from the top-level Makefile.


### R2-3 (July 24, 2015)

* Changed top-level Makefile so that a detector sub-module is only built if the appropriate
  variable is set in configure/RELEASE.local.  EXAMPLE_RELEASE.local contains all detector
  submodules commented out.  This file should be copied to RELEASE.local and the lines for
  the desired detectors uncommented.
* Added new ADnED driver for neutron event data.  This is from Matt Pearson at ORNL.


### R2-2 (March 23, 2015)

* Added new modules ffmegViewer, ffmpegServer, aravisGigE, firewireDCAM.  These are all from Tom Cobb
  and Ulrik Pedersen from Diamond Light Source.
* Added new ADQImaging module.  This is from Arthur Glowacki at the APS.
* Added new ADDexela module.  This is from Mark Rivers at the University of Chicago.
* Added new ADFastCCD module.  This is from Stuart Wilkins at the NSLS-II.
* Added new ADPICam module.  This is from John Hammonds at the APS.
* Added new ADMerlin module.  This is from Giles Knap at Diamond Light Source.
* Changed to use HTTPS (https://github.com/areaDetector) for submodules, rather 
  than SSH (git@github.com:areaDetector).  This allows a recursive checkout without a Github account.
* This release includes the following releases of the submodules:

| Module        | Release     |
| --------------|--------------
| ADCore        | R2-2        |
| ADBinaries    | R2-2        |
| ADADSC        | R2-0        |
| ADAndor       | R2-2        |
| ADAndor3      | R2-0        |
| ADBruker      | R2-0        |
| ADDexela      | R2-0-beta1  |
| ADFastCCD     | none        |
| ADFireWireWin | R2-0        | 
| ADLightField  | R2-1        |
| ADMerlin      | 3-1-1       |
| ADPICam       | none        | 
| ADPSL         | R2-1        |
| ADPerkinElmer | R2-0        |
| ADPilatus     | R2-0        |
| ADPixirad     | R2-0        | 
| ADPointGrey   | R2-0        |
| ADProsilica   | R2-1        |
| ADPvCam       | R2-0        |
| ADQImaging    | none        |
| ADRoper       | R2-0        |
| ADURL         | R2-0        |
| ADmar345      | R2-0        |
| ADmarCCD      | R2-0        |
| aravisGigE    | dls-r105549 |
| ffmpegServer  | none        |
| ffmpegViewer  | none        |
| firewireDCAM  | none        |


### R2-0 (April 4, 2014)

* Moved the repository to [Github](https://github.com/areaDetector/areaDetector).
* Re-organized the directory structure to separate the driver libraries from the example IOC applications.
* Added new detector drivers:
    - ADLightField:  Driver for newer Princeton Instruments detectors using their LightField program
    - ADPixirad: Driver for CdTe pixel array detectors from Pixirad
    - ADPointGrey: Driver for Firewire, USB, and GigE cameras from Point Grey
* ADmarCCD driver: Added support for triggered and timed acquisition modes on the new high-speed (-HS) series
  of detectors.
* ADProsilica driver: Added support for conversion from raw Bayer to RGB1, RGB2, and RGB3 in the driver. 
  In areaDetector releases prior to R2-0 this could be done in the NDPluginColorConvert plugin.  
  That capability has been removed from the plugin to make the plugin independent of the AVT PvApi library.
* All submodules now have display files for medm (adl), edm (edl), CSS Boy (opi), and caQtDM (ui).  
  The original files are the medm adl files.  The edm, CSS Boy, and caQtDm files that were created by converting 
  the medm files are found in the autoconvert directories (e.g. pilatusApp/op/opi/autoconvert).
  If there are manually tweaked files they are found in the level above the autoconvert directory 
  (e.g. pilatusApp/op/opi).  Thanks to Tim Mooney for doing all of these conversions.
* Added new NDPluginAttribute plugin from Matt Pearson.

### R1-9-1 and earlier

Release notes are part of the
[areaDetector Release Notes](https://cars.uchicago.edu/software/epics/areaDetectorReleaseNotes.html).

