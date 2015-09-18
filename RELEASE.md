areaDetector Releases
====================

The latest untagged master branch can be obtained at
https://github.com/areaDetector/areaDetector.

Tagged source code and pre-built binary releases prior to R2-0 are included
in the areaDetector releases available via links at
http://cars.uchicago.edu/software/epics/areaDetector.html.

Tagged source code releases from R2-0 onward can be obtained at 
https://github.com/areaDetector/areaDetector/releases.

Tagged prebuilt binaries from R2-0 onward can be obtained in the detector-specific directories in:
http://cars.uchicago.edu/software/pub/.  For example the Pilatus binaries are in 
http://cars.uchicago.edu/software/pub/ADPilatus.

The versions of EPICS base, asyn, and other synApps modules used for each release can be obtained from 
the EXAMPLE_RELEASE_PATHS.local, EXAMPLE_RELEASE_LIBS.local, and EXAMPLE_RELEASE_PRODS.local
files respectively, in the appropriate release of the 
[top-level areaDetector](https://github.com/areaDetector/areaDetector) repository.


Release Notes
=============

Each submodule contains detailed release notes for each release of that submodule.  The release notes below
only provide a short summary of the most significant items from the submodules.

R2-4 (September 18, 2015)
----
* Requires R2-4 of ADCore.  This requires some changes to CONFIG_SITE.local.
* Moved the simDetector from ADCore into a new repository ADExample.
* New ADMythen module for Dectris Mythen detectors.  Written by Matthew Moore at the APS.
* New ADPluginEdge module that provides an edge detection plugin using the opencv library.  Written by
  Keith Brister from Northwestern University.
* configure/RELEASE.local now optionally includes configure/RELEASE.local.$(EPICS_HOST_ARCH).
  This allows restricting building of a detector module to a subset of supported architectures when 
  building from the top-level Makefile.


R2-3 (July 24, 2015)
----
* Changed top-level Makefile so that a detector sub-module is only built if the appropriate
  variable is set in configure/RELEASE.local.  EXAMPLE_RELEASE.local contains all detector
  submodules commented out.  This file should be copied to RELEASE.local and the lines for
  the desired detectors uncommented.
* Added new ADnED driver for neutron event data.  This is from Matt Pearson at ORNL.


R2-2 (March 23, 2015)
----
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

    Module        | Release
    --------------|--------
    ADCore        | R2-2
    ADBinaries    | R2-2
    ADADSC        | R2-0
    ADAndor       | R2-2
    ADAndor3      | R2-0
    ADBruker      | R2-0
    ADDexela      | R2-0-beta1
    ADFastCCD     | none
    ADFireWireWin | R2-0
    ADLightField  | R2-1
    ADMerlin      | 3-1-1
    ADPICam       | none
    ADPSL         | R2-1
    ADPerkinElmer | R2-0
    ADPilatus     | R2-0
    ADPixirad     | R2-0
    ADPointGrey   | R2-0
    ADProsilica   | R2-1
    ADPvCam       | R2-0
    ADQImaging    | none
    ADRoper       | R2-0
    ADURL         | R2-0
    ADmar345      | R2-0
    ADmarCCD      | R2-0
    aravisGigE    | dls-r105549
    ffmpegServer  | none
    ffmpegViewer  | none
    firewireDCAM  | none


R2-0 (April 4, 2014)
----
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

R1-9-1 and earlier
------------------
Release notes are part of the
[areaDetector Release Notes](http://cars.uchicago.edu/software/epics/areaDetectorReleaseNotes.html).

