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
  work on any supported EPICS  host architecture, e.g. Linux, Windows, Mac OSX. 
  This document assumes that the reader has already installed an EPICS
  development environment, and has built 
  [EPICS base](http://www.aps.anl.gov/epics/base/R3-14), the 
  [EPICS asyn module](http://www.aps.anl.gov/epics/module/soft/asyn), and
  the required 
  [EPICS synApps modules](http://www.aps.anl.gov/bcda/synApps).
  
* Installing and running  a pre-built binary distribution.

The build process attempts to make the build process easy for typical cases but
allow site specific overrides. areaDetector uses many other products:

### External Products
* The TIFF, ZLIB, JPEG, SZIP, and HDF5 packages are all required. GRAPHICSMAGIC
  is optional. These must be installed on the development machine for Linux and
  Darwin.  For Windows pre-built libraries for these are included with
  areaDetector. These do not need to be installed on machines that will run the
  pre-built binaries on Linux or Darwin, because the binaries are statically
  linked and include these packages in the executable.
* EPICS Base
  Any relatively recent 3.14 release of EPICS Base should work.
* EPICS modules
    - Each areaDetector detector module builds both a library and an EPICS IOC
      application.  To build the library only EPICS base and asynDriver are
      required.  To build the IOC application the  synApps components AUTOSAVE,
      BUSY, CALC, and SSCAN are required.  These are discussed in a later
      section.
    - Display Managers A display manager is needed to view the areaDetector
      control screens. Control screens are provided for the following display
      managers: MEDM, EDM, CSS, and caQtDM. The native  screens are created
      manually using MEDM.  The EDM, CSS and caQtDM screens are converted from
      the MEDM screens using conversion utilities. These are discussed in a
      later section.   
* Image Viewers areaDetector comes with tools to display images over EPICS
  Channel Access using ImageJ and IDL. HDFView can be used to view files saved
  with the HDF5 file writing plugins. ImageJ can also be used to view files
  saved with the TIFF, JPEG, and netCDF plugins.

After all the required products have been installed and a release of
areaDetector has been downloaded then do the following in the
areaDetector/configure directory:
    
    
### Copy files:
      
```
cp EXAMPLE_RELEASE_PATHS.local RELEASE_PATHS.local
cp EXAMPLE_RELEASE_LIBS.local  RELEASE_LIBS.local
cp EXAMPLE_RELEASE_PRODS.local RELEASE_PRODS.local
cp EXAMPLE_CONFIG_SITE.local   CONFIG_SITE.local
```
      
### Edit RELEASE_PATHS.local 
The definitions for SUPPORT, AREA_DETECTOR, and
EPICS_BASE must all be changed. All definitions must include the full path
name. 

### Optionally create RELEASE_PATHS.local.EPICS_Target_ARCH 
Some installations chose to build for multiple target architectures using
different development machines in the same directory tree on a file server.   In
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
RELEASE_LIBS.local.EPICS_Target_ARCH can be used if the ASYN version or path is
different for a specific target architecture.  This is usually not necessary
even for building Linux and Windows in the same tree, because only the
definition of SUPPORT in RELEASE_PATHS.local.EPICS_Target_ARCH needs to be
changed.

### Edit RELEASE_PRODS.local 
The definitions for CALC, BUSY, SSCAN, and AUTOSAVE must be specified. If your
versions have the same paths that appear no changes are necessary. As described
above RELEASE_LIBS.local.EPICS_Target_ARCH can be used if the ASYN version or
path is different for a specific target architecture.  This is usually not
necessary even for building Linux and Windows in the same tree, because only the
definition of SUPPORT in RELEASE_PATHS.local.EPICS_Target_ARCH needs to be
changed.

### Edit CONFIG_SITE.local 
The definitions for HDF5, SZIP, and GRAPHICS_MAGICK may need to be changed. If
GRAPHICS_MAGIC is installed then the definitions for it may also need to be
changed.

### Optionally edit areaDetector/Makefile  
You can edit this file to change which detectors will be built. Some detectors
are commented out in the distribution because they cannot be built on all
systems. For example the Roper driver can only be built on Windows systems with
the Princeton Instruments WinView or WinSpec programs installed, and the Point
Grey driver can currently only be built on Linux systems if the version of
libc.so is 2.14 or greater. You may also want to comment out detectors that you
don't need.
      
### make
Just type:
```
make
```
If this fails then some required products has probably not been installed. Read
more of this manual and install what is missing. When areaDetector builds
successfully go to the next step.

### Run SimDetector
      
```
cd ADCore/iocs/simDetectorIOC/iocBoot/iocSimDetector
### Edit Makefile to set ARCH to your EPICS target architecture
make
cp envPaths envPaths.linux
 ../../bin/linux-x86_64/simDetectorApp st.cmd.linux
```
## Downloading a release of areaDetector from github
To download areaDetector via git just execute:

    git clone --recursive https://github.com:areaDetector/areaDetector.git

## External Products Required for Building areaDetector
The products descirbed in this section are not part of EPICS (base, modules, or
extensions) but are used by areaDetector plugins. Some may already be installed
or your system so before reading this section just try to build areaDetector,
especially ADCore. If it builds this section can be ignored. If it does not then
this section can be used to locate and install any missing components.

### TIFF, JPEG, and Z
On Windows these libraries are provided with areaDetector in the ADBinaries
module, and you can skip the rest of this section.

On Linux and Darwin the libtiff, libjpeg, and libz libraries often come already
installed.   If they are not already installed then they are normally available
for installation via the standard package installation tools, e.g. "yum install"
on Redhat systems, "apt get" for Debian systems, etc.

If for some reason you cannot install these libraries using the standard
package installation tools, they can be installed from source code distributions
as follows.

For each product download the source code from a product download site and then
build. 

Each site provides some combination of .zip, .tar, .tar.gz, etc. If the file is
a zip file then just execute:
```
unzip file.zip
```
If the file is any flavor of tar just execute:
```
tar xf file
```

All of the following products put the result of unzip or tar into a
sub-directory. cd to that directory and follow the instructions for the
appropriate sub-section.

####TIFF
This can be downloaded from 
[libTIFF](http://www.remotesensing.org/libtiff)
Then click on the Master Download Site and download the latest stable release.
After the latest release is unziped cd to the release and execute the commands:
```
./configure
make
sudo make install
```
The include and library files are installed by default into **/usr/local**

####ZLIB
    
This can be downloaded from 
[zlib](http://www.zlib.net).

Then look for latest release download.

After untaring the release
```
./configure
make
sudo make install
```
    
The include and library files are installed by default into **/usr/local**

####JPEG
    
This can be downloaded from 
[libjpeg](http://www.linuxfromscratch.org/blfs/view/svn/general/libjpeg.html).
        
Read the instructions from the web site about installing. I suggest that it be
installed into **/usr/local** instead of **/usr**. Thus to install:
```
./configure --prefix=/usr/local  \
     --mandir=/usr/share/man \
     --with-jpeg8  \
     --disable-static \
     &&; \
sed -i -e '/^docdir/ s/$/\/libjpeg-turbo-1.3.0/'   \
     -e '/^exampledir/ s/$/\/libjpeg-turbo-1.3.0/' Makefile &amp;&amp;
make
make test
sodo make install
```

### HDF5, SZIP, and GRAPHICS_MAGICK
On Windows these libraries are provided with areaDetector in the ADBinaries
module, and you can skip the rest of this section.

On Linux and Darwin these libraries may not be installed, and may not be
available via the standard package installation tools.  Follow these steps
to install them from source.

#### SZIP
    
This can be downloaded from 
[SZIP](http://www.hdfgroup.org/doc_resource/SZIP)

Click on **SZIP Source** and download the release that appears.
    
After the latest release is untared, cd to the release and execute the commands:
```
./configure --prefix=/usr/local
make
make check
sudo make install
```
#### HDF5
    
This can be downloaded from 
[HDF Group](http://www.hdfgroup.org/HDF5/)
    
Click on Downloads, then Current Release, then HDF5 Software, then Source Code,
then latest release.
    
After the latest release is untared, cd to the release and execute the commands:
```
./configure --prefix=/usr/local/hdf5 
make
make check   
sudo make install
make check-install
```

#### GRAPHICSMAGICK
    
This can be downloaded from the mercurial repository with these commands:
```
hg clone http://hg.code.sf.net/p/graphicsmagick/code graphicsmagick-code
cd graphicsmagick-code/
./configure
make
sudo make install
```
    
However when I tried to build areaDetector with GrahicsMagick enabled the build
failed with:
```
...
/usr/bin/ld: /usr/local/lib/libGraphicsMagick++.a(Image.o):
relocation R_X86_64_32S against `_ZTVN6Magick5ImageE'
can not be used when making a shared object; recompile with -fPIC
/usr/local/lib/libGraphicsMagick++.a: could not read symbols: Bad value
collect2: error: ld returned 1 exit status
```

The NDFileGraphicsMagick plugin and ADURL driver have been disabled in
areaDetector R2-0 by setting USE_GRAPHICS_MAGICK=NO in
areaDetector/configure/CONFIG_SITE.local until these problems are fixed.

If the instructions in the preceeding sections are used then
CONFIG_SITE.local should be:
```
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
GRAPHICS_MAGICK_LIB     = $(GRAPHICS_MAGICK0/lib
GRAPHICS_MAGICK_INCLUDE = -I$(GRAPHICS_MAGICK)/include/GraphicsMagick
```

EPICS Products Required for Building areaDetector
-------------------------------------------------    
areaDetector is based on
[asyn](http://www.aps.anl.gov/epics/modules/soft/asyn/) 
which is based on 
[EPICS base](http://www.aps.anl.gov/epics/base/index.php)
and also uses several components from 
[synApps](http://www.aps.anl.gov/bcda/synApps).
    
The normal way of installing the EPICS components is to install and build ASYN
and all synApp components under a single directory located by **SUPPORT** in
file **RELEASE_PATHS.local**. All definitions in this file must have full
path names.

The expected version of ASYN appears in file **RELEASE_LIBS.local**. If you
want a different version then this file must be edited.

The required synApps components appear is file **RELEASE_PRODS.local**. This
file also shows which version of each component is expected. If you want a
different version then this file must be edited.
    
Display Managers
----------------    

In order to control the detectors and the plugins you must install one or 
more of the following:
MEDM, EDM, CSS, caQtDM.

### MEDM

[medm](http://www.aps.anl.gov/epics/extensions/medm/index.php)

This requires openMotif.  It can be built from source on Linux if the 
openMotif library is available (which it is not for some new releases, such as
Fedora 20). 

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


Image Viewer Products
---------------------

### ImageJ

### IDL


Installing and Running Pre-Built Binaries
=========================================
