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
the EXAMPLE_RELEASE_LIBS.local, and EXAMPLE_RELEASE_PRODS.local files respectively, in the appropriate release of the 
[top-level areaDetector](https://github.com/areaDetector/areaDetector) repository.


Versions
--------

Each submodule contains detailed release notes for each release of that
submodule. The release notes below only provide a list of the submodules
changed, with proper link to their corresponding release notes or change set
(when no release note is available).

### R3-13 (February 9, 2024)

#### Module releases

* [ADAravis](https://github.com/areaDetector/ADAravis/compare/1455cc46565e2c49ce12d29370ad6253f7e344c0..53bbf86ad1f21d925027396e1730cb0859a8e6a2) [R2-3](https://github.com/areaDetector/ADAravis/blob/master/RELEASE.md#r2-3)
* [ADCore](https://github.com/areaDetector/ADCore/compare/98f1a82e34d5cc3b5094c0ab102eb30d30b5be16..22d7871cb095a291712e07864d3549c775221b8d) [R3-13](https://github.com/areaDetector/ADCore/blob/master/RELEASE.md#r3-13)
* [ADEiger](https://github.com/areaDetector/ADEiger/compare/011daeef70ba38be951409b366767298a87545d3..f525fc9d7646cd08cf893d821f2b6eac4cff59d3) [R3-3 R3-4](https://github.com/areaDetector/ADEiger/blob/master/RELEASE.md#r3-4)
* [ADGenICam](https://github.com/areaDetector/ADGenICam/compare/2b6392dcb9e1789ffe626748a1ffeb3fd7680cff..008ae188a7dd03d9aec996b1273593693cc2dc86) [R1-9](https://github.com/areaDetector/ADGenICam/blob/master/RELEASE.md#r1-9)
* [ADPcoWin](https://github.com/areaDetector/ADPcoWin/compare/26d054e0b45c26b0bc6ec6280d4ee6b1c9d5d068..415ff98729edd4466f92edc24564ade5bf337fdd) [R6-0](https://github.com/areaDetector/ADPcoWin/blob/master/RELEASE.md#r6-0)
* [ADSpinnaker](https://github.com/areaDetector/ADSpinnaker/compare/384703a63d384cd7b451ad8511c62e205eaefb8e..39a6931dfecc73ff855120d75e45e3f7e22e542c) [R3-4 R3-5](https://github.com/areaDetector/ADSpinnaker/blob/master/RELEASE.md#r3-5)
* [ADVimba](https://github.com/areaDetector/ADVimba/compare/a8a8bdd27149f7b8b8d73a4ea8397c0e25cf5bb0..f7f376fb18a5a2ae2194c966774cd7f0aa241945) [R1-4](https://github.com/areaDetector/ADVimba/blob/master/RELEASE.md#r1-4)

#### Modules with untagged updates

* [ADAndor](https://github.com/areaDetector/ADAndor/compare/6f3a1f440dd558ebfe77750829bc10c9a540d285..5a2f546858a57c94b212cc65c759e8a1ee93aff1)
* [ADAndor3](https://github.com/areaDetector/ADAndor3/compare/6030b0b8452158f13e5ed483518608788c862c1e..5121d0c442912ee0bf359f6f57f1a3a9d90a0b4e)
* [ADCSimDetector](https://github.com/areaDetector/ADCSimDetector/compare/6d7891e29743f9550ff711b80c329242b8592aee..c514cc353436be8fe4ebf3158244434f49febdfa)
* [ADCompVision](https://github.com/areaDetector/ADCompVision/compare/6293b75fc24ac7a739e83d0fd681bce987535022..95b292b7a3fee4023501ec3f2e52987a0a1d9b65)
* [ADDexela](https://github.com/areaDetector/ADDexela/compare/3c3d6553d0b8323fb99bd4cca2dfe7db417d704d..073665f9d1f4c84187eda4e9cdec04b086d002ae)
* [ADFastCCD](https://github.com/areaDetector/ADFastCCD/compare/5c9076104bbcc4b23c97e64223179eb0c9ab1977..22c258a623721056bb2abee29a4aa3f6b9e69596)
* [ADLambda](https://github.com/areaDetector/ADLambda/compare/eba876cf8a3debf131e78ffae914efeafc4240d1..720eacf8743aa1b3c3cf953fa764c3d16ae4a258)
* [ADLightField](https://github.com/areaDetector/ADLightField/compare/49f7d3a034db83fd69644b52a768c2e1f0ca6ad6..3383118260b82d95255f303d673e04d4de0123c9)
* [ADPICam](https://github.com/areaDetector/ADPICam/compare/9836d85828964c12c58dc4ae5d4b7c1e436def57..4b237492ea35c81083a0c2ef2a63b2c38add0230)
* [ADPerkinElmer](https://github.com/areaDetector/ADPerkinElmer/compare/b2cefbebab74c254a03f18041d06015db291b19b..dedde58ce1a8a4d27d8e8c819b3239aa793d8020)
* [ADPilatus](https://github.com/areaDetector/ADPilatus/compare/968cacda9e492b8440a529065eaf49630192b52b..5a649ceb3392b581a19c005db6584fde73ab6ded)
* [ADPixirad](https://github.com/areaDetector/ADPixirad/compare/8eb939ef6a748e7db34a9a302bd2b96a02276cb5..ea3dc75bd2fe11d5d47e63f838177f97b1fe7a85)
* [ADPointGrey](https://github.com/areaDetector/ADPointGrey/compare/08d3cf4d1954ac80f311e576133a55dc92fe6167..e23ecf6d04ef41af9bc1562a5483fde8f348acc0)
* [ADProsilica](https://github.com/areaDetector/ADProsilica/compare/49e330b7389f653642ff5e82db1038438a346945..f3a1b1f962a018087808083fb3feb37dfc7ba326)
* [ADPvCam](https://github.com/areaDetector/ADPvCam/compare/bd1388a4b756caf96099ac6146f1cac300482570..965a48571cbfb3c7bb9f77e03404ffadc0fa0746)
* [ADQImaging](https://github.com/areaDetector/ADQImaging/compare/9c12e5181192b9b3cd4ed9af73a9047bbf17fb7c..173b16c9e47e6b4eb967caae7ed8790167ff5fca)
* [ADRIXSCam](https://github.com/areaDetector/ADRIXSCam/compare/9d313b7131375bc7945123a30674a7520b66a148..0fa1fdb26efc15fbbfc393859d20e49d04b353c7)
* [ADSimDetector](https://github.com/areaDetector/ADSimDetector/compare/e6c4a708f4825199632e362b3ec77f5aa5e4f49e..4f43aa1fb0d1327beae446be9179da0b5d147127)
* [ADSupport](https://github.com/areaDetector/ADSupport/compare/3bbe4ff0496210519cf3edcf00cb7443f09ba043..62b91c1154a74a1fb532d831a6ec029bb311b8f7)
* [ADURL](https://github.com/areaDetector/ADURL/compare/485eff9ae576e0f73afd369c3a7ca40a8faa6867..f164693726974104847535d15b622e8f050fbe8c)
* [ADViewers](https://github.com/areaDetector/ADViewers/compare/78cb7127d655685d9f4f2c83f0d83c7e5b758d58..88109f38fd96890bc859ba09cfa77a8ddbac3a5f)
* [ADmar345](https://github.com/areaDetector/ADmar345/compare/3fd2a5a28ca72c2d76fbf4bd894c7e1cb01434ae..6cca372d2e3f95b292ee68ce438880961e4ddb02)
* [ADmarCCD](https://github.com/areaDetector/ADmarCCD/compare/8f62ac5442ef532a0b8aa0616163d5ff9794577a..78b0654f600bb35dc037182c2935c8e007a91325)
* [NDDriverStdArrays](https://github.com/areaDetector/NDDriverStdArrays/compare/ac42ed71e3b41095c4641d7dfdf2b40b57758046..e21b2f2f74feb217a0e67ba8f8fc4bfd7efe58e0)
* [ffmpegServer](https://github.com/areaDetector/ffmpegServer/compare/9fe0d46b49572c229b20d5cd6054fd6fb3cb419f..b9885ddba015163b618ba84e5589c9564209f862)
* [pvaDriver](https://github.com/areaDetector/pvaDriver/compare/1f51a940ac37f62d140eea8efcd4af00c361d9dc..95219181dda0989ce366f29df32e67bfa1fb82c2)

#### New modules

* [ADPylon](https://github.com/areaDetector/ADPylon/compare/41585bc6f84470810bb6be1883ea883c73f494bb..0421ef930c599ae78f026aab2a104c8d542af544)

### R3-12-1 (January 22, 2022)

* ADCore R3-12, R3-12-1
  * ADAutoSaveMenu to save and restore configurations.
  * Bug fixes.

* ADEiger R3-2.
  * Fixed race condition on Stream interface.

* ADPerkinElmer R2-11.
  * Added trigger modes.
  * New vendor release.

* ADGenICam R1-8
  * Allow the asyn parameter datatype and the GenICam feature datatype to be different.
    Needed for the areaDetector Gain feature (double) on GenICam cameras that do not implement the
    GenICam Gain feature (also a double), but rather only implement the integer GainRaw or GainRawChannelA features.

* ADSpinnaker R3-3
  * Fixed a problem reading Integer GenICam feature values, and their min and max, when they exceed 2^32.
  
### R3-11 (May 26, 2021)

* ADSupport R1-9-1, R1-10
  * Changed the support for reading MJPEG streams in GraphicsMagickSrc and xml2Src.
  * Fixed compilation errors with EPICS base 7.0.5 which changed the use of undefined functions
    from being a warning to being an error.

* ADCore R3-11
  * New plugin NDPluginBadPixel.

* ADGenICam R1-6, R1-7, and R1-7-1
  * Added support for converting Mono12p and Mono12Packed formats to Mono16,
    with optional left-shift operation.
  * Fixed memory leak.

* ADAravis R2-1, R2-2, R2-2-1
  * Added support for PixelFormat=Mono12Packed and Mono12p.
  * Added new ConvertPixelFormat mbbo record with choices of Mono16Low and Mono16High.
    This record controls how Mono12Packed and Mono12p pixel formats are decompressed.
  * Added continuous integration via Github Actions.  Thanks to Michael Davidsaver for this.
  * Improve error reporting by fetching the underlying error from aravis.

* ADSpinnaker R3-1, R3-2
  * Added support for specifying the number of transport layer buffers in the configuration command.
    This may require changes to startup scripts.
  * Improved reporting of transport layer statistics.
  * Updated Spinnaker version from 2.0.0.147 to 2.4.0.147.

* ADURL R2-3
  * Converted documentation to ReST, include in documentation on github.io.
  * Added autoconverted OPI files for css/BOY, css/Phoebus, edm, and caQtDM.
  * Fix to release the lock at least briefly between each image, otherwise EPICS puts don't get processed.

* ADVimba R1-3
  * Updated the version of the Vimba SDK being used to 4.0 (Windows) and 4.1 (Linux).
  * Added automatic packet size negotiation for GigE cameras in the constructor.
    Previously cameras would default to jumbo packets, and if the network did not support that
    then streaming would fail until the packet size feature was decreased.

* ADViewers R1-7
  * Many enhancements to the Python PY_NTNDA_Viewer.
  * Updated to the latest versions of the EPICS Java libraries.

### R3-10 (September 20, 2020)

* ADCore R3-10
  * Numerous new features and bug fixes.

* ADGenICam R1-4 and R1-5
  * Added logic to pause and resume acquisition when any of the following parameters are changed:
      - ADMinX, ADMinY 
      - ADSizeX, ADSizeY
      - ADBinX, ADBinY
      - ADNumImages
      - GCPixelFormat

    Previously changing these parameters while acquiring had no effect.
  * Added fix for cameras that don't support GenICam feature AcquisitionMode=MultiFrame.
  * Added support for new cameras.

* ADSpinnaker R2-2 and R3-0
  * Updated Spinnaker version from 1.2.0 to 2.0.0.147.
  * Worked around a bug that in epicsMessageQueueWaitWithTimeout that is present in
    most versions of EPICS base.
  * Tested that ADSpinnaker runs on Centos 8. 
    It should also work on other Linux versions with gcc 7.5 and up, e.g. RHEL 8 and Debian 10.

* ADAravis R1-3 and R2-0
  * Updated from aravis 0.7.2 to 0.8.1.  The aravis API has changed, so sites will need to update their
    local aravis installation to 0.8.1.
  * Fixed a problem with GenICam boolean features.  Previously the code was calling the aravis functions
    for integer features, which did not work.  It was changed to call the functions for boolean features,
    and it now works correctly.
  * Added .bob files for Phoebus Display Manager

### R3-9 (February 24, 2020)

* All modules.
  * Added .bob files for Phoebus, autoconverted from .adl files.

* Documentation
  * Converted more of the driver HTML documentation hosted at cars.uchicago.edu 
    to ReST hosted at areaDetector.githun.io.  Much of this work was done at the recent Codeathon
    at Diamond Light Source.  Thanks to Andy Wilson, Gary Yendell, and Timo Korhonen for work on this.
  * Updated the install_guide.rst to reflect the changes to the configure/ directory described below.
  * Changes to Sphinx settings to make Web pages wider, improving appearance of tables.
  * Change from Sphinx 1.8.4 to 1.8.5.

* configure/ directory
  * Fixed problems with EXAMPLE_RELEASE_LIBS.local and EXAMPLE_RELEASE_PRODS.local.
    They were missing the definition of SUPPORT.
  * Removed EXAMPLE_RELEASE_SUPPORT.local and EXAMPLE_RELEASE_BASE.local. 
    RELEASE_SUPPORT.local and RELEASE_BASE.local are not longer used.  They were previously
    used, for example, to allow building Linux and Windows in the same tree where the definition
    of SUPPORT and EPICS_BASE would have a different syntax.  This is now handled in the more
    standard way of optionally including $(TOP)/../RELEASE.local and $(TOP)/../RELEASE.$(EPICS_HOST_ARCH).local.
  * RELEASE_LIBS_INCLUDE and RELEASE_PRODS_INCLUDE were changed to also optionally include these files from
    the level above $(AREA_DETECTOR).

* ADCore R3-9
  * Added Int64 support to NDFileNetCDF.
  * Support for compressed NDArrays in NDPluginStdArrays (Channel Access)

* ADGenICam R1-2 and R1-3
  * Change integer features from epicsInt32 to epicsInt64.  Requires asyn R4-38.
  * Added databases and OPI files for many more FLIR and AVT cameras.

* ADSpinnaker R2-1
  * Change integer features from epicsInt32 to epicsInt64.  Requires asyn R4-38.
  * Added vendor SFNC_GenTL*.xml files which are required to be present in the lib directory on Linux.

* ADAravis R1-2
  * Change integer features from epicsInt32 to epicsInt64.  Requires asyn R4-38.

* ADUVC
  * New driver for USB Video Class devices from Jakub Wlodek.
 

### R3-8 (October 22, 2019)

* ADCore R3-8
  * Int64 support.  Requires asyn R4-37.
* ADGenICam R1-1
  * Minor updates
* ADAravis R1-1
  * Support for register caching that greatly improves performance.
  * Requires aravis 0_7_2 or later
  * ADAravis should be used in place of aravisGigE.
* ADVimba R1-0
  * Initial release.  ADVimba should be used in place of ADProsilica.
* ADSimDetector R2-10
  * Support for NDInt64/NDUInt64 NDArrays and NDAttrInt64/NDAttrUInt64 attributes.

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

