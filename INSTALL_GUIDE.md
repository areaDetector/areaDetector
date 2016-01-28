areaDetector: Installation Guide
=================================

Marty Kraimer, Brookhaven National Laboratory
---------------------------------------------
Mark Rivers, University of Chicago
-----------------------------------

This product is made available subject to acceptance of the 
[EPICS open source license](http://epics-pvdata.sourceforge.net/LICENSE.html)

Introduction
------------
This is a guide for installing and building R2-0 and later of the EPICS
areaDetector module.  This guide is intended for both areaDetector users and
developers.  areaDetector can be obtained as a release or by cloning from the
github repository. 

This guide provides instructions for:

* Installing, building, and running from source code. These instructions should
  work on any supported EPICS host architecture, e.g. Linux, Windows, Mac OSX. 
  This document assumes that the reader has already installed an EPICS
  development environment, and has built 
  [EPICS base](http://www.aps.anl.gov/epics/base/R3-14), the 
  [EPICS asyn module](http://www.aps.anl.gov/epics/module/soft/asyn), and
  the required 
  [EPICS synApps modules](http://www.aps.anl.gov/bcda/synApps).
  
* Installing and running a pre-built binary distribution.


Installing and Building from Source Code
========================================
The build process attempts to make the build process easy for typical cases but
allow site specific overrides. areaDetector uses many other products.

EPICS Products Required for Building areaDetector
-------------------------------------------------    
areaDetector requires
[EPICS base](http://www.aps.anl.gov/epics/base/index.php).
Any relatively recent R3.14 release should work.

areaDetector also requires
[asyn](http://www.aps.anl.gov/epics/modules/soft/asyn/).
The most recent release of asyn is recommended.

Each areaDetector detector module builds both a library and an EPICS IOC
application.  To build the library only EPICS base and asynDriver are
required.  To build the IOC application the 
[synApps](http://www.aps.anl.gov/bcda/synApps) modules 
AUTOSAVE, BUSY, CALC, and SSCAN are required.
If the CALC module is built with SNCSEQ support then SNCSEQ is also required.
The most recent release of the synApps modules is recommended.

The DEVIOCSTATS and ALIVE modules are optional.

EPICS base, asyn and the synApps modules must be built before building
areaDetector.


External Products Required for Building areaDetector
----------------------------------------------------
areaDetector always uses the TIFF, ZLIB, JPEG, SZIP, XML2, and HDF5 libraries, (except
on vxWorks where none of these are available). GRAPHICSMAGIC is optional. 

For Windows areaDetector includes pre-built libraries for these packages and
nothing additional needs to be installed. Windows users can skip the rest of this
section.

In areaDetector releases prior to R2-0 prebuilt versions of these libraries were
also included for Linux and Darwin.  This became too hard to maintain because
of compiler version dependencies, so they are no longer provided.

On Linux and Darwin the TIFF, JPEG, XML2, and ZLIB libraries always need to be
installed, whether building from source code or using the pre-built binaries.
The HDF5 and SZIP packages need to be installed when building from source code. 
HDF5 and SZIP do not need to be installed on machines that will only run the
pre-built binaries on Linux or Darwin, because the binaries are statically
linked and include these libraries in the executable.

### TIFF, JPEG, XML2, and Z
On Linux and Darwin the libtiff, libjpeg, libxml2, and libz libraries often come already
installed.  If they are not already installed then they are normally available
for installation via the standard package installation tools, e.g. "yum install"
on Redhat systems, "apt get" for Debian systems, etc.

If for some reason you cannot install these libraries using the standard
package installation tools, they can be installed from source code distributions
as follows.

For each product download the source code from a product download site and then
build. 

Each site provides some combination of .zip, .tar, .tar.gz, etc. If the file is
a zip file then just execute:

    unzip file.zip

If the file is any flavor of tar just execute:

    tar xf file

All of the following products put the result of unzip or tar into a
sub-directory. cd to that directory and follow the instructions for the
appropriate sub-section.

#### TIFF
This can be downloaded from 
[libTIFF](http://www.remotesensing.org/libtiff).
Then click on the Master Download Site and download the latest stable release.
After the latest release is unziped cd to the release and execute the commands:

    ./configure
    make
    sudo make install

The include and library files are installed by default into **/usr/local**

####ZLIB
  
This can be downloaded from 
[zlib](http://www.zlib.net).

Then look for latest release download.

After untaring the release

    ./configure
    make
    sudo make install

  
The include and library files are installed by default into **/usr/local**

####JPEG
  
This can be downloaded from 
[libjpeg](http://www.linuxfromscratch.org/blfs/view/svn/general/libjpeg.html).
    
Read the instructions from the web site about installing. I suggest that it be
installed into **/usr/local** instead of **/usr**. Thus to install:

    ./configure --prefix=/usr/local  \
         --mandir=/usr/share/man \
         --with-jpeg8  \
         --disable-static \
         &&; \
    sed -i -e '/^docdir/ s/$/\/libjpeg-turbo-1.3.0/'   \
         -e '/^exampledir/ s/$/\/libjpeg-turbo-1.3.0/' Makefile &amp;&amp;
    make
    make test
    sudo make install

####XML2
This can be downloaded from
[libxml2](http://www.xmlsoft.org/downloads.html).

Then look for latest release download.

After untaring the release

    ./configure
    make
    sudo make install


### HDF5, SZIP, and GRAPHICS_MAGICK

On Linux and Darwin these libraries may not be installed, and may not be
available via the standard package installation tools.  Follow these steps
to install them from source.

#### SZIP
    
This can be downloaded from 
[SZIP](http://www.hdfgroup.org/doc_resource/SZIP).

Click on **SZIP Source** and download the release that appears.
    
After the latest release is untared, cd to the release and execute the commands:

    ./configure --prefix=/usr/local
    make
    make check
    sudo make install

#### HDF5
    
This can be downloaded from 
[HDF Group](http://www.hdfgroup.org/HDF5/).
    
Click on Downloads, then Current Release, then HDF5 Software, then Source Code,
then latest release.
    
After the latest release is untared, cd to the release and execute the commands:

    ./configure --prefix=/usr/local/hdf5 --with-szlib=/usr/local
    make
    make check   
    sudo make install
    make check-install

#### GRAPHICSMAGICK
    
This can be downloaded from the mercurial repository with these commands:

    hg clone http://hg.code.sf.net/p/graphicsmagick/code graphicsmagick-code
    cd graphicsmagick-code/
    ./configure
    make
    sudo make install

However recent attempts to build or link with GraphicsMagick have led to a
variety of errors. Thus, the NDFileGraphicsMagick plugin and ADURL driver have
been disabled in areaDetector R2-0 by setting USE_GRAPHICS_MAGICK=NO in
areaDetector/configure/CONFIG_SITE.local until these problems are fixed.

Downloading and Installing areaDetector Source Code
---------------------------------------------------

The areaDetector source code is kept on
[gitub.com/areaDetector](https://github.com/areaDetector).
     
It can be downloaded 2 ways:

1. Via the "git clone" command:

    <code>git clone --recursive https://github.com/areaDetector/areaDetector.git</code>

2. By downloading tar.gz or zip files for a specific release of each module
   through a Web browser or by the wget command:

    <code>wget https://github.com/areaDetector/areaDetector/archive/R2-0.tar.gz</code>
    
If downloading tar files then each repository must be downloaded separately. To
build the "core" of areaDetector the following repositories must be downloaded:

* areaDetector/areaDetector
* areaDetector/ADBinaries
* areaDetector/ADCore

To build the simulation detector, which is very useful for learning areaDetector and for testing,
also download
* areaDetector/ADExample

To also build a specific detector, for example the ADProsilica, also download
* areaDetector/ADProsilica

All of the submodules (i.e. ADBinaries, ADCore, ADProsilica, etc.) should be 
placed under the top-level areaDetector directory.  

After all the required products have been installed and a release of
areaDetector has been downloaded then do the following in the
areaDetector/configure directory:
       
    cp EXAMPLE_RELEASE.local       RELEASE.local
    cp EXAMPLE_RELEASE_PATHS.local RELEASE_PATHS.local
    cp EXAMPLE_RELEASE_LIBS.local  RELEASE_LIBS.local
    cp EXAMPLE_RELEASE_PRODS.local RELEASE_PRODS.local
    cp EXAMPLE_CONFIG_SITE.local   CONFIG_SITE.local
      
### Edit RELEASE_PATHS.local 
The definitions for SUPPORT, AREA_DETECTOR, and EPICS_BASE must all be changed.
All definitions must include the full path name. 

The normal way of installing the EPICS components is to install and build ASYN
and all synApp components under a single directory located by SUPPORT in
this file.

### Optionally create RELEASE_PATHS.local.$(EPICS_HOST_ARCH) 
Some installations chose to build for multiple target architectures using
different development machines in the same directory tree on a file server.  In
this case the path to SUPPORT, AREA_DETECTOR and BASE may be different for each
architecture. For example BASE on Linux might be
/usr/local/epics/base-3.14.12.4, while on a Windows machine using the same copy
of BASE the path might be H:/epics/base-3.14.12.4.  In this case
RELEASE_PATHS.local could specify the path for Linux while
RELEASE_PATHS.win32-x86 could specify the path for the win32-x86 build host. 
RELEASE_PATHS.local is read first, and then any definitions there will be
replaced by RELEASE_PATHS.EPICS_Target_ARCH if it exists.
      
### Edit RELEASE_LIBS.local 
The location of ASYN must be specified.  It is normally placed in the SUPPORT
directory defined in RELEASE_PATHS.local. If your version has the same path as
the one that appears then no changes are necessary.  As described above
RELEASE_LIBS.local.$(EPICS_HOST_ARCH) can be used if the ASYN version or path is
different for a specific target architecture.  This is usually not necessary
even for building Linux and Windows in the same tree, because only the
definition of SUPPORT in RELEASE_PATHS.local.$(EPICS_HOST_ARCH) needs to be
changed.

### Edit RELEASE_PRODS.local 
The definitions for AUTOSAVE, BUSY, CALC, and SSCAN, and  must be specified. 
If the CALC module is built with SNCSEQ support then SNCSEQ must also be specified.
If DEVIOCSTATS or ALIVE are defined in RELEASE_PRODS.local then IOC applications
will be built with these modules as well.
If your versions have the same paths that appear no changes are necessary. As described
above RELEASE_LIBS.local.$(EPICS_HOST_ARCH) can be used if a module version or
path is different for a specific target architecture.  This is usually not
necessary even for building Linux and Windows in the same tree, because only the
definition of SUPPORT in RELEASE_PATHS.local.$(EPICS_HOST_ARCH) needs to be
changed.

### Edit CONFIG_SITE.local 
The definitions for HDF5, SZIP, and GRAPHICS_MAGICK may need to be changed.
This only needs to be done on architectures where these libraries are installed
externally to the areaDetector package.  On WIN32 these libraries
are provided in the ADBinaries module, and so will be automatically found by the
EPICS build system.  Thus, on WIN32 the locations should not be defined 
in CONFIG_SITE.local.

If the instructions in the preceeding sections are used then
CONFIG_SITE.local should contain:

    # Define the location of HDF5
    HDF5         = /usr/local/hdf5
    HDF5_LIB     = $(HDF5)/lib
    HDF5_INCLUDE = -I$(HDF5)/include

    # Define the location of SZLIB
    SZIP         = /usr/local
    SZIP_LIB     = $(SZIP)/lib
    SZIP_INCLUDE = -I$(SZIP)/include

    # Define the location of Graphics Magic
    GRAPHICS_MAGICK         = /usr/local
    GRAPHICS_MAGICK_LIB     = $(GRAPHICS_MAGICK)/lib
    GRAPHICS_MAGICK_INCLUDE = -I$(GRAPHICS_MAGICK)/include/GraphicsMagick

    # Define the location of the libxml2 include files
    XML2_INCLUDE = -I/usr/include/libxml2

### Edit RELEASE.local  
Uncomment the lines for the drivers that should be built. None of the detector
drivers are included by default. Some detectors cannot be built on all systems.
For example the Roper driver can only be built on Windows systems with the 
Princeton Instruments WinView or WinSpec programs installed, and the Point Grey
driver can currently only be built on Linux systems if the version of libc.so 
is 2.14 or greater.

### make
Just type:

    make

If this fails then some required products have probably not been installed. 

### Example files in ADCore/iocBoot
Copy EXAMPLE_commonPlugins.cmd to commonPlugins.cmd and EXAMPLE_commonPlugins_settings.req to
commonPlugin_settings.req.

Edit commonPlugins.cmd and commonPlugin_settings.req
Change whether or not the lines for optional modules (e.g. DEVIOCSTATS, ALIVE) are 
commented out depending on whether these modules were defined in RELEASE_PRODS.local.

### Run SimDetector      

    cd ADExample/iocs/simDetectorIOC/iocBoot/iocSimDetector
    ### Edit Makefile to set ARCH to your $(EPICS_HOST_ARCH) architecture
    make
    cp envPaths envPaths.linux
    ../../bin/linux-x86_64/simDetectorApp st.cmd.linux


Installing Pre-Built Binary Versions of areaDetector
====================================================

Pre-built binary versions of areaDetector are provided for ADCore and for each 
detector.  This is provided as a convenience so that it is not necessary to
set up a build system to run areaDetector on a specific detector.

The instructions here use the Pilatus (ADPilatus) module as an example.  
Substitute Pilatus with the name of the detector you are working with.

The pre-built binaries can be found on the
[CARS Web site](http://cars.uchicago.edu/software/pub/). There is a subdirectory there
for each detector (e.g. ADPilatus) that contains all of the releases for that detector.

The pre-built binaries contain executables for one or more of the following
architectures:
- linux-x86 (32-bit Linux built on Fedora Core 15, gcc 4.6.3, libc 2.14.1)
- linux-x86_64 (64-bit Linux built on Fedora Core 15, gcc 4.6.3, libc 2.14.1)
- linux-x86_gcc43 (32-bit Linux build on Fedora Core 9, gcc 4.3.0, libc 2.8)
- linux-x86_64-gcc42 (64-bit Linux built on SUSE, gcc 4.2.1, libc 2.6.1)
- darwin-x86 (64-bit Mac OSX built on Darwin 11.4.2,  ??, clang 4.2)
- win32-x86 (32-bit Windows, VS2010 compiler, statically linked)
- win32-x86-dynamic (32-bit Windows, VX2010 compiler, dyanamically linked)
- windows-x64 (64-bit Windows, VS2010 compiler, statically linked)
- windows-x64-dynamic (64-bit Windows, VX2010 compiler, dyanamically linked)

Note that the linux-x86 and linux-x86_64 builds are done a relatively new Linux system
and will not run on RHEL 6, for example.  The linux-x86-gcc43 and linux-x86_64-gcc42 
architectures are built on much older versions of Linux, and should run on most Linux 
systems.

Follow these steps to use the prebuilt version.  

* Create an installation directory for the module. On Windows I typically use C:\EPICS\support.
  On Linux I typically use /home/ACCOUNT/epics/support, where ACCOUNT is the name
  of the account that is normally used to run the detector software, e.g. marccd on
  a marCCD detector, mar345 on a mar345 detector, det on a Pilatus detector, etc.

* Place the distribution file in this directory. Then issue the commands (Unix style)

    <code>tar xvzf ADPilatus_RX-Y.tgz</code>
    
  On Windows it is more convenient to download the zip file and extract it using
  Windows Explorer.
  
    
* In the ADPilatus/iocs/pilatusIOC/iocBoot/ directory make a *copy* of the example 
  iocPilatus directory and give it a new local name, e.g. ioc13Pilatus1. By doing this you
  will be able to update to later versions of areaDetector without overwriting modifications
  you make in the ioc13Pilatus1 directory.

* In the new io13Pilatus1 directory you just created edit st.cmd to change the PV prefix
  $(PREFIX) to one that is unique to your site. PV prefixes must be unique on the subnet, and
  if you use the default prefix there could be a conflict with other detectors of the same
  type.

* In the same ioc13Pilatus1 directory edit the file envPaths to point to the locations of all of the
  support modules on your system. Normally this is handled by the EPICS build system, but
  when using the prebuilt version this must be manually edited. Do not worry about the path
  to EPICS_BASE, it is not required.


Display Managers
================    
A display manager is needed to view the areaDetector control screens. Control screens are
provided for the following display managers: MEDM, EDM, CSS, and caQtDM. The native screens
are created manually using MEDM.  The EDM, CSS and caQtDM screens are converted from the MEDM
screens using conversion utilities. These are discussed in a later section.   

In order to control the detectors and the plugins you should install one or more of
MEDM, EDM, CSS, caQtDM.

### MEDM

The source code for medm can be downloaded from:
[medm](http://www.aps.anl.gov/epics/extensions/medm/index.php)

This requires [Motif](http://motif.ics.com/).  medm can be built from source on Linux if the Motif library is
available (which it is not for some new releases, such as Fedora 20). 

It is available for Windows as via an
[EPICS Windows Tools MSI installer 
package](http://www.aps.anl.gov/epics/distributions/win32/index.php).


### EDM
This can be downloaded through links on the 
[EDM home page](http://ics-web.sns.ornl.gov/edm).

### CSS
This can be downloaded through links on the 
[CSS home page](http://controlsystemstudio.org).

### caQtDM
This can be downloaded through links on the 
[caQtDM home page](http://epics.web.psi.ch/software/caqtdm).


Configuration
=============
Before running an areaDetector application it is usually necessary to configure
    a number of items.

* EPICS environment variables. There are several environment variables that EPICS
  uses. I suggest setting these in the .cshrc (or .bashrc) file for the account that
  will be used to run the detector.

    - EPICS_CA_AUTO_ADDR_LIST and EPICS_CA_ADDR_LIST.
      These variables control the IP addresses that EPICS clients use when searching for
      EPICS PVs. The default is EPICS_CA_AUTO_ADDR_LIST=YES and EPICS_CA_ADDR_LIST to be the
      broadcast address of all networks connected to the host. Some detectors, for example
      the marCCD and mar345, come with 2 network cards, which are on 2 different subnets,
      typically a private one connected to the detector and a public one connected to the
      local LAN. If the default value of these variables is used then EPICS clients (e.g.
      medm) running on the detector host computer will generate many errors because each
      EPICS PV will appear to be coming from both networks. The solution is to set these
      variables as follows:

      <code>setenv EPICS_CA_AUTO_ADDR_LIST NO</code>
      
      <code>setenv EPICS_CA_ADDR_LIST localhost:XX.YY.ZZ.255</code>

      where XX.YY.ZZ.255 should be replaced with the broadcast address for the public
      network on this computer.

    - EPICS_CA_MAX_ARRAY_BYTES.
      This variable controls the maximum array size that EPICS can transmit with Channel
      Access. The default is only 16kB, which is much too small for most detector data. This
      value must be set to a large enough value on both the EPICS server computer (e.g. the
      one running the areaDetector IOC) and client computer (e.g. the one running medm,
      ImageJ, IDL, etc.). This should be set to a value that is larger than the largest
      waveform record that will be used for the detector.  For example if using a detector
      with 1024x1024 pixels and 4-bytes per pixel (waveform record FTVL=LONG) then
      EPICS_CA_MAX_ARRAY_BYTES would need to be at least 1024 * 1024 * 4 = 4153344.  
      In practice it should be set at least 100 bytes larger than this because there 
      is some overhead.  For example:

      <code>setenv EPICS_CA_MAX_ARRAY_BYTES 4154000</code>

      Do not simply set EPICS_CA_MAX_ARRAY_BYTES to a very large number like 100MB or 1GB.
      EPICS Channel Access allocates buffers of exactly EPICS_CA_MAX_ARRAY bytes whenever
      the required buffer size exceeds 16 kB, and one does not want unnecessarily large
      buffers to be allocated.
      
    - EPICS_DISPLAY_PATH. This variable controls where medm looks for .adl display files.
      If the recommendation below is followed to copy all adl files to a single directory,
      then this environment variable should be defined to point to that directory. For
      example:

      <code>setenv EPICS_DISPLAY_PATH /home/det/epics/adls</code>


* medm display files. 
  It is convenient to copy all medm .adl files to a single directory and then point the
  environment variable EPICS_DISPLAY_PATH to this directory. The alternative is to point
  EPICS_DISPLAY_PATH to a long list of directories where the adl files are located in the
  distributions, which is harder to maintain. On the Pilatus, for example, create a
  directory called /home/det/epics/adls, and put all of the adl files there. To simplify
  copying the adl files to that location use the following one-line script, which can 
  placed in /home/det/bin/sync_adls.

  <code>find /home/det/epics/support -name '*.adl' -exec cp -fv {} /home/det/epics/adls \;</code>

  This script finds all adl files in the epics/support tree and copies them to /home/det/epics/adls.
  That directory must be created before running this script. Similar scripts can be
  used for other Linux detectors (marCCD, mar345, etc.) and can be used on Windows
  as well if Cygwin is installed. Each time a new release of areaDetector is installed
  remove the old versions of each support module (areaDetector, asyn, autosave, etc.)
  and then run this script to install the latest medm files.


Running the IOC Application
===========================

Each example IOC directory comes with a Linux script (start_epics) or a Windows
batch file (start_epics.bat) or both depending on the architectures that the detector
runs on. These scripts provide simple examples of how to start medm and the EPICS
IOC. For example, for the mar345 iocBoot/iocMAR345/start_epics contains the following:

    medm -x -macro "P=13MAR345_1:, R=cam1:" mar345.adl &
    ../../bin/linux-x86/mar345App st.cmd

This script starts medm in execute mode with the appropriate medm display file and
macro parameters, running it in the background. It then runs the IOC application.
This script assumes that iocBoot/iocMAR345 is the default directory when it is run,
which could be added to the command or set in the configuration if this script is
set as the target of a desktop shortcut, etc. The script assumes that EPICS_DISPLAY_PATH
has been defined to be a location where the mar345.adl and related displays that
it loads can be found. You will need to edit the script in your copy of the iocXXX
directory to change the prefix (P) from 13MAR345_1: to whatever prefix you chose
for your IOC. The start_epics script could also be copied to a location in your
PATH (e.g. /home/mar345/bin/start_epics). Add a command like 

    cd /home/mar345/epics/support/areaDetector-2-0/ADmar345/iocs/mar345IOC/iocBoot/iocMAR345
   
at the beginning of the script and then type

    start_epics
    
from any directory to start the EPICS IOC.


Image Viewers
=============

areaDetector comes with tools to display images over EPICS Channel Access using ImageJ and
IDL. These viewers are described in
[areaDetectorViewers.html](http://cars.uchicago.edu/software/epics/areaDetectorViewers.html).

HDFView can be used to view files saved with the HDF5 file writing plugins. ImageJ can
also be used to view files saved with the TIFF, JPEG, and netCDF plugins.
