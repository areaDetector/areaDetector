Area Detector: Installation Guide
=================================

Marty Kraimer, BNL
------------------
Mark Rivers, University of Chicago
-----------------------------------

This product is made available subject to acceptance of the 
[EPICS open source license](http://epics-pvdata.sourceforge.net/LICENSE.html)

----
This is the 14-Jan-2014 version of the install instructions for areaDetector.

This version has a complete description of installing everything required to build
areaDetector on Linux. However section 7 (Installition Details) is not written.

The instructions should also work for Darwin, i. e. macOS.

What remains is to describe how to build the GUI plugin configuration tools, i,e.
MEDM, EDM, CSS, and pvPy.

So far no instructions are available for building for windows, vxWorks, or RTEMS.

Introduction
------------
This is a guide for installing and building the areaDetector. This guide is only
valid for releases 2.0 and later of areaDetector. This guide is intended for both
areaDetector users and developers. areaDetector can be obtained as a release or
by cloning from the github repository. This guide provides instructions for installing
and building on linux. The guidelines should also work for darwin, i. e. macOS.
At this time there are no guidelines for windows, vxWorks, or RTEMS.

The build process attempts to make the build process easy for typical cases but
allow site specific overrides. areaDetector uses many other products:

### External Products
* TIFF, ZLIB, JPEG, SZIP, and HDF5 are all required. GRAPHICSMAGIC is optional. These
  are described in a later section.
* EPICS Base
  Any relatively recent 3.14 release of EPICS Base should work.
* EPICS modules
    - Each detector builds both a library and an EPICS IOC application.  To build the library
      only EPICS base and asynDriver are required.  To build the IOC application the  
      synApps components CALC, BUSY, SSCAN, and AUTOSAVE are required. 
      These are discussed in a later section.
    - Display Managers
      A display manager is needed to view the areaDetector control screens. Control screens
      are provided for the following display managers: MEDM, EDM, CSS, and caQtDM. The native 
      screens are for MEDM, these are manually created.  The other screens are converted
      from the MEDM screens using conversion utilities.
      These are discussed in a later section.   
* Image Viewers
areaDetector comes with tools to display images over EPICS Channel Access using ImageJ and IDL.
HDFView can be used to view files saved with the HDF5 file writing plugins.
ImageJ can be used to view files saved with the TIFF, JPEG, and netCDF plugins.

After all the required products have been installed and a release of areaDetector
has been downloaded then do the following in the top level directory:
    
    
### Copy files:
      
```
cp EXAMPLE_RELEASE_PATHS.local RELEASE_PATHS.local
cp EXAMPLE_RELEASE_LIBS.local RELEASE_LIBS.local
cp EXAMPLE_RELEASE_PRODS.local RELEASE_PRODS.local
```
      
### Edit RELEASE_PATHS.local
The definitions for SUPPORT, AREA_DETECTOR, and EPICS_BASE must all be changed.
All definitions must include the full path name. The definitions for HDF5 and SZ
may need to be changed. If GRAPHICS_MAGIC is installed then the definitions for
it may also need to be changed.

### Optionally create RELEASE_PATHS.local.EPICS_Target_ARCH
Some installations chose to build for multiple target architectures using different
development machines in the same directory tree on a file server.   In this case the
path to SUPPORT, AREA_DETECTOR and BASE may be different for each architecture. For
example BASE on Linux might be /usr/local/epics/base-3.14.12.4, while on a Windows
machine using the same copy of BASE the path might be H:/epics/base-3.14.12.4.  In
this case RELEASE_PATHS.local could specify the path for Linux while
RELEASE_PATHS.win32-x86 could specify the path for the win32-x86 build host. 
RELEASE_PATHS.local is read first, and then any definitions there will be replaced by
RELEASE_PATHS.EPICS_Target_ARCH if it exists.
      
### Edit RELEASE_LIBS.local
The location of ASYN must be specified.  It is normally placed in the SUPPORT
directory defined in RELEASE_PATHS.local. If you have built the version that appears
no changes are necessary.  As described above RELEASE_LIBS.local.EPICS_Target_ARCH 
can be used if the ASYN version or path is different for a specific target
architecture.  This is usually not necessary even for building Linux and Windows in
the same tree, because only the definition of SUPPORT in
RELEASE_PATHS.local.EPICS_Target_ARCH needs to be changed.

### Edit RELEASE_PRODS.local
The definitions for CALC, BUSY, SSCAN, and AUTOSAVE must be specified. If you have
built the versions that appear no changes are necessary. As described above
RELEASE_LIBS.local.EPICS_Target_ARCH  can be used if the ASYN version or path is
different for a specific target architecture.  This is usually not necessary even for
building Linux and Windows in the same tree, because only the definition of SUPPORT in
RELEASE_PATHS.local.EPICS_Target_ARCH needs to be changed.

### Optionally edit areaDetector/Makefile 
You can edit this file to change which detectors will be built. Some detectors
are commented out in the distribution because they cannot be built on all systems.
For example the Roper driver can only be built on Windows systems with the Princeton
Instruments WinView or WinSpec programs installed, and the Point Grey driver can
currently only be built on Linux systems if the version of libc.so is 2.14 or greater.
You may also want to comment out detectors that you don't need.
      
### make
Just type:
```
make
```
If this fails then some required products has probably not been installed. Read
more of this manual and install what is missing. When areaDetector builds successfully
go to the next step.

### Run SimDetector
      
```
cd ADCore/iocs/simDetectorIOC/iocBoot/iocSimDetector
### Edit Makefile to set ARCH to your EPICS target architecture
make
cp envPaths envPaths.linux
 ../../bin/linux-x86_64/simDetectorApp st.cmd.linux
```
If this works successfully then go to next step.
      
Congratulations!!!
      
You can ignore the rest of this document.
    
    
<b>NOTE MARK</b> I suggest the following changes for EXAMPLE_RELEASE_PATHS.local:
    
XXX_DIR
      
Change this to <b>XXX_LIB</b>. This requires a change in ADApp/commonDriverMakefile.
      
XX_INCLUDES= -I
      
Change this to <b>XX_INCLUDE=</b>. This requires a change in ADApp/pluginSrc/Makefile.
      
Change statement order
      
Put related things together.
      
ZLIB, TIFF, and JPEG
      
What about these? Should they also appear in RELEASE_PATHS.local OR maybe they should
not appear and SZLIB should also not appear. Note that HDF5 and GRAPHICS_MAGICK
MUST appear since they do not follow normal rules for /usr/local.
      
    
Thus the EXAMPLE_RELEASE_PATHS.local becomes:
```
# The following 3 definitions must be changed for each site
SUPPORT=/home/install/epics/support
AREA_DETECTOR=/home/git/areaDetector
EPICS_BASE=${SUPPORT}/base-3.14.12.3

# Define the location of HDF5
HDF5=/usr/local/hdf5
HDF5_LIB=${HDF5}/lib
HDF5_INCLUDE=${HDF5}/include

# Define the location of Graphics Magic
GRAPHICS_MAGICK=/usr/local
GRAPHICS_MAGICK_LIB=${GRAPHICS_MAGICK}/lib
GRAPHICS_MAGICK_INCLUDE=${GRAPHICS_MAGICK}/include/GraphicsMagick

#MARK either remove SZLIB or also have defs for TIFF, JPEG, and ZLIB
# Define the location of SZLIB
SZLIB=/usr/local
SZ_LIB=${SZLIB}/lib
SZ_INCLUDE=${SZLIB}/include

#Define TIFF, JPEG, and ZLIB ???
```

## Downloading a release of areaDetector
### From github
To download areaDetector via git just execute:
```
git clone --recursive https://github.com:areaDetector/areaDetector.git
```

###Release Site
See [areaDetector](http://cars.uchicago.edu/software/epics/areaDetector.html

**NOTE:** no 2.0 release of areaDetector is currently available. Thus the only
way to get it is through github.
    
##External Products Required for Building areaDetector
    
The product descrbes in this section are not part of epics (base, modules, or extensions)
but are used by areaDetector plugins. Some may already be installed or your system
so before reading this section just try to build areaDetector, especially ADCore.
If it builds this section can be ignored. If it does not then this section can be
used to locate and install any missing components.

The instructions in this section were the result of installing all the components
on a new laptop on which I installed fedora 20. Before installing I had already
built iocCore, asynDriver, and the required synApps components.

For each product the source was downloaded from a product download site and then
built. It may be easier to download pre-built modules, but downloading the sources
and building should work on more varieties of linux and darwin.

Each site provides some combination of .zip, .tar, .tar.gz, etc. If the file is
a zip file then just execute:
```
unzip file.zip
```
If the file is any flavor of tar just execute:
```
tar xf file
``

All of the following products put the result of unzip or tar into a sub-directory.
cd to that directory and follow the instructions for the appropriate sub-section.


If the instructions in the following subsections are used then RELEASE_PATHS.local,
except for the first three definitions, should be:
```
# The following 3 definitions must be changed for each site

SUPPORT=/home/install/epics/support
AREA_DETECTOR=/home/git/areaDetector
EPICS_BASE=${SUPPORT}/base-3.14.12.3

# Define the location of HDF5
HDF5=/usr/local/hdf5
HDF5_DIR=${HDF5}/lib
HDF5_INCLUDES= -I${HDF5}/include

# Define the location of SZLIB
SZLIB=/usr/local
SZ_DIR=${SZLIB}/lib
SZ_INCLUDES= -I${SZLIB}/include

# Define the location of Graphics Magic
GRAPHICS_MAGICK=/usr/local
GRAPHICS_MAGICK_DIR=${GRAPHICS_MAGICK}/lib
GRAPHICS_MAGICK_INCLUDES= -I${GRAPHICS_MAGICK}/include/GraphicsMagick
```
###TIFF
This can be download from [libTIFF](http://www.remotesensing.org/libtiff)
Then click on the Master Download Site and download the latest stable release.
After the latest release is unziped cd to the release and execute the commands:
```
./configure
make
sudo make install
```
The include and library files are installed by default into **/usr/local**

###ZLIB
    
This can be download from [zlib](http://www.zlib.net).

Then look for latest release download.

After untaring the release
```
./configure
make
sudo make install
```
    
The include and library files are installed by default into <b>/usr/local</b>

###JPEG
    
This can be download from [libjpeg](http://www.linuxfromscratch.org/blfs/view/svn/general/libjpeg.html).
        
Read the instructions from the web site about installing. I suggest that it be installed
into **/usr/local** instead of**/usr**. Thus to install:
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
###SZIP
    
This can be download from <a href="http://www.hdfgroup.org/doc_resource/SZIP">SZIP
      </a>
    
    
      Then click on <b>SZIP Source</b> and download the release that appears.
    
      After the latest release is untared, cd to the release and execute the commands:
    <pre>
./configure --prefix=/usr/local
make
make check
sudo make install
</pre>
    <h3>
      HDF5</h3>
    
      Start at: <a href="http://www.hdfgroup.org/HDF5/">HDF Group </a>
    
    
      The click on Downloads, then Current Release, then HDF5 Software, then Source Code,
      then latest release.
    
      After the latest release is untared, cd to the release and execute that command:
    <pre>
./configure --prefix=/usr/local/hdf5 
make
make check   
sudo make install
make check-install
</pre>
    <h3>
      GRAPHICSMAGICK</h3>
    
      To build this from mercurial repository:
    <pre>
hg clone http://hg.code.sf.net/p/graphicsmagick/code graphicsmagick-code
cd graphicsmagick-code/
./configure
make
sudo make install
</pre>
    
      However when I tried to build areaDetector with GrahicsMagick enabled the build
      failed with:
    <pre>
...
/usr/bin/ld: /usr/local/lib/libGraphicsMagick++.a(Image.o):
relocation R_X86_64_32S against `_ZTVN6Magick5ImageE'
can not be used when making a shared object; recompile with -fPIC
/usr/local/lib/libGraphicsMagick++.a: could not read symbols: Bad value
collect2: error: ld returned 1 exit status
</pre>
    <h2>
      EPICS Products Required for Building areaDetector</h2>
    
      areaDetector is based on <a href="http://www.aps.anl.gov/epics/modules/soft/asyn/">
        asyn</a> which is based on <a href="http://www.aps.anl.gov/epics/base/index.php">EPICS
          base</a> and also uses several components from <a href="http://www.aps.anl.gov/bcda/synApps">
            synApps</a>.
    
      The normal way of installing the EPICS components is to install and build ASYN and
      all synApp components under a single directory located by <b>SUPPORT</b> in file
      <b>RELEASE_PATHS.local</b>. All definitions in this file must have full path names.
    
    
      The expected version of ASYN appears in file <b>RELEASE_LIBS.local</b> If You want
      a different version then this file must be edited.
    
    
      The required synApps components appear is file <b>RELEASE_PRODS.local</b>. This
      file also shows which version of each component is expected. If You want a different
      version then this file must be edited.
    
    <h2>
      Configuration Products</h2>
    
      In order to configure the plugins You must install one or more of the following:
      MEDM, EDM, CSS, pyQT.
    <h3>
      MEDM</h3>
    
      At present X11 is not avaliable for fedora 20. Thus I can not build MEDM.
    <h3>
      EDM</h3>
    
      At present X11 is not avaliable for fedora 20. Thus I can not build EDM.
    <h3>
      CSS</h3>
    <h3>
      pyQt</h3>
    <h2>
      Image Viewer Products</h2>
    <h3>
      ImageJ</h3>
    <h3>
      IDL</h3>
    <h2>
      Installation Details</h2>
    <h3>
      RELEASE_PATHS.local</h3>
    <h3>
      RELEASE_LIBS.local</h3>
    <h3>
      RELEASE_PRODS.local</h3>
    <h3>
      iocBoot/ioc*</h3>
  </div>
</body>
</html>
