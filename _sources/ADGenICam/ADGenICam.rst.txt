======================================
ADGenICam
======================================

:author: Mark Rivers, University of Chicago

.. contents:: Contents

.. _GenICam:      https://www.emva.org/standards-technology/genicam
.. _aravis:       https://github.com/AravisProject/aravis
.. _ADGenICam:    https://github.com/areaDetector/ADGenICam
.. _ADAravis:     https://github.com/areaDetector/ADAravis
.. _aravisGigE:   https://github.com/areaDetector/aravisGigE
.. _ADSpinnaker:  https://github.com/areaDetector/ADSpinnaker
.. _ADVimba:      https://github.com/areaDetector/ADVimba
.. _ADSupport:    https://github.com/areaDetector/ADSupport
.. _ADGenICam class: ../areaDetectorDoxygenHTML/class_a_d_gen_i_cam.html
.. _meson:        https://mesonbuild.com
.. _ninja:        https://ninja-build.org

Overview
--------

This is an :doc:`../index` base class driver for GenICam cameras.

GenICam_ is a Generic Interface for Cameras from the European Machine Vision Association (EMVA). 
The stated goal of GenICam is:

- The goal of GenICam (Generic Interface for Cameras) is to provide a generic programming interface for 
  all kinds of devices (mainly cameras), no matter what interface technology (GigE Vision, USB3 Vision, CoaXPress, 
  Camera Link HS, Camera Link etc.) they are using or what features they are implementing. 
  The result is that the application programming interface (API) will be identical regardless 
  of interface technology.

GenICam supports the following interface standards:

- GigE Vision 
- USB3 Vision
- Camera Link
- Camera Link HS
- IIDC2 (Firewire)

GenICam cameras are controlled via *features* such as ExposureTime, Gain, TriggerMode, etc.  Each feature has a
name, data type, flags indicating whether it can be read or written, etc.
Many features are defined in the GenICam specification, and vendors must use these names
for those features.  Vendors are also free to add new features that are specific to their cameras.

All GenICam cameras contain an XML file that provides a complete description of all of the features available
in that camera. ADGenICam provides Python scripts that read and parse the XML file and automatically produce
the EPICS database (.template) file and medm display (.adl) files.  These medm files can then be autoconverted
to edm, CSS-Boy, and caQtDM files.

This is a small snippet from an XML file for the feature called PixelFormat::

  <Enumeration Name="PixelFormat" NameSpace="Standard">
    <ToolTip>Format of the pixel data.</ToolTip>
    <Description>Format of the pixel data.</Description>
    <DisplayName>Pixel Format</DisplayName>
    <Visibility> Beginner</Visibility>
    <pIsLocked>TLParamsLocked</pIsLocked>
    <ImposedAccessMode>RW</ImposedAccessMode>
    <EnumEntry Name="Mono8" NameSpace="Standard">
      <ToolTip>Pixel format set to Mono 8.</ToolTip>
      <Description>Pixel format set to Mono 8.</Description>
      <DisplayName>Mono 8</DisplayName>
      <pIsImplemented>Mono8Inq_Reg</pIsImplemented>
      <Value>0x01080001</Value>
    </EnumEntry>

Derived classes
---------------

ADGenICam is a base class from which actual detector driver classes are derived.  areaDetector includes the following
derived classes:

- ADAravis_: Driver that uses the aravis_ library.  Runs on most Linux systems, and can control any GenICam_ camera.
- ADSpinnaker_: Driver that uses the FLIR Spinnaker SDK.  Runs on Windows and Linux Ubuntu 18. Controls only FLIR/Point Grey cameras.
- ADVimba_: Driver that uses the AVT Vimba SDK.  Runs on Windows and most Linux systems.  Controls only AVT/Prosilica cameras.

`ADGenICam class`_ describes this class in detail.

ADGenICam and aravis
--------------------
A problem with the GenICam standard is that while it provides a reference implementation, it is not
open-source.  Source code derived from the reference implementation cannot be freely distributed.

One solution to this is the aravis_ project, which is a reverse-engineered library and utilities for GenICam cameras.
aravis_ is available only on Linux.
areaDetector has long had an aravisGigE_ driver based on aravis_.  
Although it is called aravisGigE, the driver actually supports USB cameras as well as GigE and 10 GigE cameras.  

There is a new ADAravis_ driver.  This driver also uses
aravis_, but it differs from aravisGigE_ by using this ADGenICam_ base class to significantly reduce the amount of code.
Many of the ideas in ADGenICam_ were borrowed from aravisGigE_.

.. _ADGenICam_Installing_aravis:

Installing aravis
-----------------
The aravis_ package is required for aravisGigE_ and ADAravis_ drivers.  It is also required to extract
the XML file from the camera, even when using drivers that are not based on aravis_, e.g. the
ADSpinnaker_ driver or the ADVimba_ driver.  ADSpinnaker_ and ADVimba_ can run on Windows, but the XML
file extraction must be done once on Linux.

aravis 0.7.x and higher use the meson_ and ninja_ build systems, rather than the autoconf and gnumake 
systems used in previous versions.
 
The following shows the steps to build and install aravis 0.8.1 on a Centos 7 machine running as root::

  yum install ninja-build
  yum install meson
  yum install glib2-devel
  yum install gtk-doc
  yum install libxml2-devel  
  yum install gtk3-devel
  yum install gstreamer1
  yum install gstreamer1-devel
  yum install gstreamer1-plugins-base-devel
  yum install libnotify-devel
  yum install gtk-doc
  yum install gobject-introspection-devel
  yum install zlib-devel
  cd /usr/local
  git clone https://github.com/AravisProject/aravis
  cd aravis/
  git checkout ARAVIS_0_8_1
  meson build
  cd build
  ninja-build
  ninja-build install

The steps above will be different if you do not have root access and need to install elsewhere,
or if you are running another OS like Ubuntu where `apt install` is used in place of `yum install`.
The names of the required packages will also be different on another OS like Ubuntu.

.. _ADGenICam_Download_XML:

Downloading the XML file
------------------------
To use the ADGenICam driver it is first necessary to download the XML file from the camera.
To my knowledge the only code available to download the XML file from any GenICam camera
is the **arv-tool** program that is included in the aravis package.  This means that the aravis
package is required to create the XML file, and so the camera must be available from a Linux
system at least once initially.

However, if using the ADSpinnaker_ driver or the ADVimba_ driver aravis_ is not needed to build the
driver or at run-time.

To extract the XML file from the camera first run the arv-tool program to make a list of all the 
GenICam cameras that are visible from the Linux system, for example::

  TahoeU18:/corvette/home/epics/devel/areaDetector/ADGenICam> arv-tool-0.8
  Allied Vision Technologies-GC1380H (02-2142A)-02-2142A-06178 (164.54.160.58)
  Allied Vision Technologies-GT1380 (02-2604A)-02-2604A-07008 (164.54.160.104)
  Allied Vision Technologies-Manta G-507B (E0022704)-50-0503479161 (164.54.160.4)
  Allied Vision Technologies-Manta G-507C (E0022705)-50-0503419258 (164.54.160.21)
  Allied Vision Technologies-Manta_G-146C (E0020011)-50-0503317598 (164.54.160.62)
  FLIR-Oryx ORX-10G-51S5M-18011754 (192.168.0.2)


Then download the XML file with the command `arv-tool -n cameraName genicam > XML_file_name`, for example::

  TahoeU18:/corvette/home/epics/devel/areaDetector/ADGenICam> arv-tool-0.8 -n "Allied Vision Technologies-Manta G-507B (E0022704)-50-0503479161" genicam > xml/AVT_Manta_G507B.xml
  TahoeU18:/corvette/home/epics/devel/areaDetector/ADGenICam> ls -ltr xml
  total 19496
  -rw-rw-r-- 1 epics domain users  332287 Oct  7  2018 PGR_Blackfly_50S5C.xml
  -rw-r--r-- 1 epics domain users  317859 Jun  2  2019 PGR_Blackfly_20E4C.xml
  ...
  -rwxrwxr-x 1 epics domain users  202059 May 14 14:59 AVT_Mako_G158C.xml
  -rw-rw-r-- 1 epics domain users  483863 Sep 15 18:32 Basler_piA640_210gm.xml
  -rw-rw-r-- 1 epics domain users  924352 Sep 15 18:32 FLIR_BFS_70S7M.xml
  -rw-r--r-- 1 epics domain users  232180 Sep 29 12:12 AVT_Manta_G507B.xml

.. _ADGenICam_Python_scripts:

Python scripts
--------------

Python script to create EPICS database
======================================
**ADGenICam/scripts/makeDb.py** is a Python program to read the XML file and produce an EPICS database (.template) file.
The first argument is the name of the XML file and the second argument is the name of the .template file.

For example::

  TahoeU18:/corvette/home/epics/devel/areaDetector/ADGenICam> python scripts/makeDb.py xml/PGR_Blackfly_20E4C.xml GenICamApp/Db/PGR_Blackfly_20E4C.template
  More than 16 enum entries for PixelFormat mbbi record, discarding additional options.
     If needed, edit the Enumeration tag for PixelFormat to select the 16 you want.
  Don't know what to do with Register
  More than 16 enum entries for GevSupportedOptionSelector mbbi record, discarding additional options.
     If needed, edit the Enumeration tag for GevSupportedOptionSelector to select the 16 you want.
  More than 16 enum entries for ChunkPixelFormat mbbi record, discarding additional options.
     If needed, edit the Enumeration tag for ChunkPixelFormat to select the 16 you want.
    
  TahoeU18:/corvette/home/epics/devel/areaDetector/ADGenICam> ls -ltr GenICamApp/Db/*.template
  -rw-rw-r-- 1 epics domain users 142961 Oct  8  2018 GenICamApp/Db/PGR_Blackfly_50S5C.template
  -rw-rw-r-- 1 epics domain users 117222 Oct 30  2018 GenICamApp/Db/AVT_Manta_G507C.template
  -rw-r--r-- 1 epics domain users 220518 May 31 10:43 GenICamApp/Db/FLIR_ORX_10G_51S5.template
  -rw-r--r-- 1 epics domain users 137902 Jun  2 09:38 GenICamApp/Db/PGR_Blackfly_20E4C.template

Note that there were some warnings from makeDb.py about more than 16 enumeration options.
Some GenICam cameras support more than 16 choices for some enumeration features, but EPICS mbbo/mbbi records are limited to 16. 
However, this warning is usually not significant, because the XML file contains all possible values for the enum choices,
but at run-time ADGenICam limits the choices to those actually available for the current camera and operating mode,
and in practice appears never to be more than 16.
Ignore the warning about `Don't know what to do with Register`, the register entries are not needed.

This is a portion of the PGR_Blackfly_20E4C.template created above for the ExposureAuto mbbi and mbbo records::

  record(mbbi, "$(P)$(R)GC_ExposureAuto_RBV") {
    field(DTYP, "asynInt32")
    field(INP,  "@asyn($(PORT),$(ADDR=0),$(TIMEOUT=1))GC_E_ExposureAuto")
    field(ZRST, "Off")
    field(ZRVL, "0")
    field(ONST, "Once")
    field(ONVL, "1")
    field(TWST, "Continuous")
    field(TWVL, "2")
    field(SCAN, "I/O Intr")
    field(DISA, "0")
    info(autosaveFields, "DESC ZRSV ONSV TWSV THSV FRSV FVSV SXSV SVSV EISV NISV TESV ELSV TVSV TTSV FTSV FFSV TSE")
  }
  
  record(mbbo, "$(P)$(R)GC_ExposureAuto") {
    field(DTYP, "asynInt32")
    field(OUT,  "@asyn($(PORT),$(ADDR=0),$(TIMEOUT=1))GC_E_ExposureAuto")
    field(DOL,  "0")
    field(ZRST, "Off")
    field(ZRVL, "0")
    field(ONST, "Once")
    field(ONVL, "1")
    field(TWST, "Continuous")
    field(TWVL, "2")
    field(DISA, "0")
    info(autosaveFields, "DESC ZRSV ONSV TWSV THSV FRSV FVSV SXSV SVSV EISV NISV TESV ELSV TVSV TTSV FTSV FFSV TSE PINI VAL")
  }

GenICam integer features are 64 bit integers.  Prior to R1-2 of ADGenICam EPICS longin and longout records were used
for these features.  However, longin and longout records are limited to 32 bits, and thus cannot represent the features
when the value is larger than 32 bits.  In EPICS base 3.16.1 or 7.0 and higher int64in and int64out records can be used for
these 64-bit integer features.  Prior to base 3.6.1 ai and ao record can be used.  These are 64-bit floats, which can exactly
represent integers up to 52 bits, which is a significant improvement over the 32 bit limitation of longin and longout records.  

In R1-2 makeDb.py makeDb.py was changed to accept a `--devInt64` flag.  If this flag is present then the database will use
int64in and int64out records.  This is recommended when using base 3.16.1 or 7.0 or higher.  For previous versions of base
the `--devInt64` flag must not be used, and makeDb.py will create ai and ao records instead.

This Python script, and the one to create medm screens described next, attempt to name the EPICS records as the name of the
GenICam feature, preceded by the string `GC_` to prevent conflict with any record names already defined in areaDetector.
However, many GenICam feature names are quite long and this would lead to record names that are too long, particularly
since they will have IOC-specific prefixes added to them.  The Python scripts limit the record names to being 20 characters.
This truncation is done by trimming *words* in the name to 3 characters, where words are defined to begin with an upper-case
character.  The words are trimmed from left to right until the record name is no more than 20 characters.
For example, the GenICam feature name `AutoExposureTimeLowerLimit` has the record name `GC_AutExpTimLowLimit`.
This trimming algorithm is different from the one in aravisGigE_, and has the advantage that the names are more readable,
and a given GenICam feature name will almost always result in the same record name, which was not true for aravisGigE_.
In aravisGigE_ the feature names were truncated to 16, and a final digit was assigned in case of duplicate names.

For example, for the GenICam property `AcquisitionFrameRate` on a Point Grey BlackFly 20E4C aravisGigE_ produces the record name
`$(P)$(R)AcquisitionFram1`, while ADGenICam produces `$(P)$(R)GC_AcqFrameRate`, which is more understandable because it 
includes the word `Rate`.  Furthermore ADGenICam produces this same record name for this feature on the FLIR Oryx 51S5, while
aravisGigE produces `$(P)$(R)AcquisitionFram6`, which is different from what is produces for the BlackFly camera.
This is because the number of duplicate names after truncation is different for the 2 cameras.

Python script to create medm screens
====================================
**ADGenICam/scripts/makeAdl.py** is a Python program to read the XML file and produce a medm (.adl) files.
The first argument is the name of the XML file and the second argument is the base name of the adl files.
The medm files are **[baseFile]-features1.adl**, **[baseFile]-features2.adl**, etc.

For example::

  TahoeU18:/corvette/home/epics/devel/areaDetector/ADGenICam> python scripts/makeAdl.py xml/PGR_Blackfly_20E4C.xml GenICamApp/op/adl/PGR_Blackfly_20E4C
  Don't know what to do with Register
  TahoeU18:/corvette/home/epics/devel/areaDetector/ADGenICam> ls -ltr GenICamApp/op/adl/PGR_Blackfly_20E4C-features_*
  -rw-r--r-- 1 epics domain users 50125 Jun  3 14:40 GenICamApp/op/adl/PGR_Blackfly_20E4C-features_1.adl
  -rw-r--r-- 1 epics domain users 40041 Jun  3 14:40 GenICamApp/op/adl/PGR_Blackfly_20E4C-features_2.adl
  -rw-r--r-- 1 epics domain users 49498 Jun  3 14:40 GenICamApp/op/adl/PGR_Blackfly_20E4C-features_3.adl
  -rw-r--r-- 1 epics domain users  5760 Jun  3 14:40 GenICamApp/op/adl/PGR_Blackfly_20E4C-features_4.adl


The number of camera specific screens generated is determined by the number of features in a specific camera
and by the maximum screen size specified in the Python program.  In this case 4 .adl files were created.
These are shown in the `MEDM screens`_ section below.

To change the maximum size of the adl files these 2 lines
in makeAdl.py can be edited::

  maxScreenWidth = 1600
  maxScreenHeight = 850

Shell scripts
-------------
`addCamera.sh` is a simple script that runs both makeDb.py and makeAdl.py.
It is run from the top-level ADGenICam directory and is passed the name of the camera,
i.e. the name of the XML file without the path and without the .xml extension.
It can be edited to enable or disable the --devInt64 flag for makeDb.py.

`updateCameras.sh` is a simple script that runs `addCamera.sh` for all cameras in the 
xml/ directory.  It is useful for updating all databases and OPI screens when the Python
scripts are changed.

ADGenICam Classes
-----------------
ADGenICam provides three classes that are used by derived classes for real cameras.

GenICamFeature class
====================
This class defines a GenICam feature and associates it with an asyn parameter.  
Real drivers must define a class derived from ADGenICam that implements
a number of pure virtual functions, for example to write/read features to/from the camera.  
This is the complete list of all pure virtual functions that the derived class must implement::

    // These are the pure virtual functions that derived classes must implement
    virtual bool isImplemented(void) = 0;
    virtual bool isAvailable(void) = 0;
    virtual bool isReadable(void) = 0;
    virtual bool isWritable(void) = 0;
    virtual int readInteger(void) = 0;
    virtual int readIntegerMin(void) = 0;
    virtual int readIntegerMax(void) = 0;
    virtual int readIncrement(void) = 0;
    virtual void writeInteger(int value) = 0;
    virtual bool readBoolean(void) = 0;
    virtual void writeBoolean (bool value) = 0;
    virtual double readDouble(void) = 0;
    virtual double readDoubleMin(void) = 0;
    virtual double readDoubleMax(void) = 0;
    virtual void writeDouble(double value) = 0;
    virtual int readEnumIndex(void) = 0;
    virtual void writeEnumIndex(int value) = 0;
    virtual std::string readEnumString(void) = 0;
    virtual void writeEnumString(std::string const & value) = 0;
    virtual void readEnumChoices(std::vector<std::string>& enumStrings, std::vector<int>& enumValues) = 0;
    virtual std::string readString(void) = 0;
    virtual void writeString(std::string const & value) = 0;
    virtual void writeCommand(void) = 0;

This class includes the following properties::

    std::string mAsynName;        // asyn parameter name
    asynParamType mAsynType;      // asyn parameter type
    int mAsynIndex;               // asyn parameter index
    std::string mFeatureName;     // GenICam feature name
    GCFeatureType_t mFeatureType; // GenICam feature type

The class contains methods to convert the units used by EPICS drivers to/from those used by
GenICam.  These conversions currently include:

- GenICam **ExposureTime**, **ExposureTimeAbs**, and **TriggerDelay** features: EPICS uses seconds, GenICam uses microseconds
- EPICS **AcquirePeriod**: EPICS uses period in seconds, GenICam uses frame rate in Hz
- EPICS **ImageMode**: EPICS uses [Single, Multiple, Continuous] enums, GenICam uses [Continuous, SingleFrame, Multiframe],
  and we want to use the EPICS values.
 
The class contains methods to write the features value to the camera, and to read the current value of the
feature from the camera.
 
ADGenICamFeatureSet
===================
This class is a collection of all of the GenICamFeature objects for the camera, implemented as a C++ std::multiset.
It provides methods to insert and find the GenICamFeature objects, to read all of the feature values from the camera,
and to print a report of all of the feature attributes and values.  This report() method is called when asynReport is
called for the owner ADGenICam class.


ADGenICam class
===============
This is the base class from which the actual camera driver will derive.  ADGenICam inherits from :doc:`../ADCore/ADDriver`.

It contains the following methods:

- **createFeature()** This is a pure virtual method that the derived class must implement.  It creates a detector-specific
  feature object derived from GenICamFeature.
- **drvUserCreate()** This is the method that is called when EPICS base is initializing the records during iocInit.
  Most areaDetector drivers do not implement this method.  This is because they create their detector-specific parameters
  in their constructor, and the asynPortDriver base class implements **drvUserCreate**.  
  ADGenICam is different.  It creates the detector-specific parameters dynamically as the database file created by
  the Python script described above is read in during iocInit.  For each GenICam feature that is passed in the drvUser
  argument to this function it calls createFeature() described above.
- **addADDriverFeatures()**  This function creates GenICam features for all of the standard ADDriver parameters that map to GenICam
  features.  This allows the standard records such as ADManufacturer, ADModel, ADMaxSizeX, ADMaxSizeY, etc. to get their values
  with no feature-specific code in this class or the derived class.
- **writeInt32()** This function handles writes to integer parameters.  If the parameter is associated with a GenICam feature
  then it writes the value to the camera.
  Because writing to one feature can affect the values of other features, the values of all GenICam features are read back after
  the write is complete.  This includes the enum strings and values for all enum features, since these may have changed as well.
- **writeFloat64()** This function handles writes to double precision parameters.  If the parameter is associated with a GenICam feature
  then it writes the value to the camera.
  Because writing to one feature can affect the values of other features, the values of all GenICam features are read back after
  the write is complete.  This includes the enum strings and values for all enum features, since these may have changed as well.
- **readEnum()** This reads the enum strings associated with a parameter.  If the parameter is associated with a GenICam feature
  then it reads the enum strings and values for that feature, so EPICS bo/bi/mbbo/mbbi record enum choices are constructed 
  at run-time with the choices the camera actually supports.  This function is called at iocInit for all enum features.
  These enum choices can also change at run-time as explained above, and this class does callbacks to device support with 
  the new choices.
- **startCapture()** This is a pure virtual function that the derived class must implement to start the camera acquisition.
- **stopCapture()**  This is a pure virtual function that the derived class must implement to stop the camera acquisition.

The following is part of the output of the **asynReport 1 ARV1** command for the ADAravis driver, which inherits from ADGenICam.
Note that it prints the list of features::


  epics> asynReport 1 ARV1
  ARV1 multiDevice:No canBlock:Yes autoConnect:Yes
      enabled:Yes connected:Yes numberConnects 1
      nDevices 0 nQueued 0 blocked:No
      asynManagerLock:No synchronousLock:No
      exceptionActive:No exceptionUsers 1 exceptionNotifys 0
      traceMask:0x1 traceIOMask:0x2 traceInfoMask:0x1
  Aravis GigE detector ARV1
    NX, NY:            1600  1200
    Data type:         1
  Feature list
  
        Node name: AcquisitionFrameCount
            value: 1
  
        Node name: AcquisitionFrameCount
            value: 1
  
        Node name: AcquisitionFrameRate
            value: 43.956043
  
        Node name: AcquisitionFrameRate
            value: 43.956043
  
        Node name: AcquisitionFrameRate
            value: 43.956043
  
        Node name: AcquisitionFrameRateAuto
            value: Continuous
  
        Node name: AcquisitionFrameRateEnabled
            value: false
  
        Node name: AcquisitionFrameRateEnabled
            value: false
  
        Node name: AcquisitionMode
            value: Continuous
  
  ...

This is the output of **asynReport** with **details** increased from 1 to 2 so additional information about each feature is printed::

  epics> asynReport 2 ARV1
  ARV1 multiDevice:No canBlock:Yes autoConnect:Yes
      enabled:Yes connected:Yes numberConnects 1
      nDevices 0 nQueued 0 blocked:No
      asynManagerLock:No synchronousLock:No
      exceptionActive:No exceptionUsers 1 exceptionNotifys 0
      traceMask:0x1 traceIOMask:0x2 traceInfoMask:0x1
      interposeInterfaceList
          asynOctet pinterface 0x55d81c1bbfa0 drvPvt 0x55d81ca033c0
      interfaceList
          asynCommon pinterface 0x55d81c1baed0 drvPvt 0x55d81c9ff750
          asynDrvUser pinterface 0x55d81c1bacc0 drvPvt 0x55d81c9ff750
          asynOctet pinterface 0x55d81c1bade0 drvPvt 0x55d81c9ff750
          asynInt32 pinterface 0x55d81c1baea0 drvPvt 0x55d81c9ff750
          asynFloat64 pinterface 0x55d81c1bae40 drvPvt 0x55d81c9ff750
          asynInt32Array pinterface 0x55d81c1bad80 drvPvt 0x55d81c9ff750
          asynGenericPointer pinterface 0x55d81c1bad20 drvPvt 0x55d81c9ff750
          asynEnum pinterface 0x55d81c1bace0 drvPvt 0x55d81c9ff750
  Aravis GigE detector ARV1
    NX, NY:            1600  1200
    Data type:         1
  Feature list
  
        Node name: AcquisitionFrameCount
            value: 1
        asynIndex: 72
         asynName: NIMAGES
         asynType: 1
    isImplemented: true
      isAvailable: false
       isReadable: true
       isWritable: true
  
        Node name: AcquisitionFrameCount
            value: 1
        asynIndex: 215
         asynName: GC_I_AcquisitionFrameCount
         asynType: 1
    isImplemented: true
      isAvailable: false
       isReadable: true
       isWritable: true
  
        Node name: AcquisitionFrameRate
            value: 43.956043
        asynIndex: 134
         asynName: GC_D_AcquisitionFrameRate
         asynType: 3
    isImplemented: true
      isAvailable: true
       isReadable: true
       isWritable: false
  
        Node name: AcquisitionFrameRate
            value: 43.956043
        asynIndex: 94
         asynName: GC_FRAMERATE
         asynType: 3
    isImplemented: true
      isAvailable: true
       isReadable: true
       isWritable: false
  
        Node name: AcquisitionFrameRate
            value: 43.956043
        asynIndex: 75
         asynName: ACQ_PERIOD
         asynType: 3
    isImplemented: true
      isAvailable: true
       isReadable: true
       isWritable: false
  
        Node name: AcquisitionFrameRateAuto
            value: Continuous
        asynIndex: 165
         asynName: GC_E_AcquisitionFrameRateAuto
         asynType: 1
    isImplemented: true
      isAvailable: true
       isReadable: true
       isWritable: false
            enums: 0: Off
                   2: Continuous
  
        Node name: AcquisitionFrameRateEnabled
            value: false
        asynIndex: 299
         asynName: GC_B_AcquisitionFrameRateEnabled
         asynType: 1
    isImplemented: true
      isAvailable: true
       isReadable: true
       isWritable: true
  
        Node name: AcquisitionFrameRateEnabled
            value: false
        asynIndex: 95
         asynName: GC_FRAMERATE_ENABLE
         asynType: 1
    isImplemented: true
      isAvailable: true
       isReadable: true
       isWritable: true
  
        Node name: AcquisitionMode
            value: Continuous
        asynIndex: 69
         asynName: IMAGE_MODE
         asynType: 1
    isImplemented: true
      isAvailable: true
       isReadable: true
       isWritable: true
  
        Node name: AcquisitionMode
            value: Continuous
        asynIndex: 164
         asynName: GC_E_AcquisitionMode
         asynType: 1
    isImplemented: true
      isAvailable: true
       isReadable: true
       isWritable: true
            enums: 0: Continuous
                   1: SingleFrame
                   2: MultiFrame

  ...  

Standard driver parameters
~~~~~~~~~~~~~~~~~~~~~~~~~~

ADGenICam implements many of the driver parameters from the ADDriver base class 
by mapping these parameters to GenICam features.  The following table lists the mapping between
the standard records and GenICam features.


.. cssclass:: table-bordered table-striped table-hover
.. list-table::
   :header-rows: 1
   :widths: auto

   * - EPICS record names
     - Record types
     - GenICam features
     - Description
   * - Manufacturer_RBV
     - stringin
     - DeviceVendorName
     - The camera manufacturer name
   * - Model_RBV
     - stringin
     - DeviceModelName
     - The camera model name
   * - FirmwareVersion_RBV
     - stringin
     - DeviceFirmwareVersion
     - The camera firmware version
   * - SerialNumber_RBV
     - stringin
     - DeviceSerialNumber
     - The camera serial number
   * - MaxSizeX_RBV
     - longin
     - WidthMax
     - Sensor width
   * - MaxSizeY_RBV
     - longin
     - HeightMax
     - Sensor height
   * - SizeX, SizeX_RBV
     - longout, longin
     - Width
     - Width of region to read out
   * - SizeY, SizeY_RBV
     - longout, longin
     - Height
     - Height of region to read out
   * - MinX, MinX_RBV
     - longout, longin
     - OffsetX
     - X start of region to read out
   * - MinY, MinY_RBV
     - longout, longin
     - OffsetY
     - Y start of region to read out
   * - BinX, BinX_RBV
     - longout, longin
     - BinningHorizontal
     - Horizontal binning factor
   * - BinY, BinY_RBV
     - longout, longin
     - BinningVertial
     - Vertical binning factor
   * - ImageMode, ImageMode_RBV
     - mbbo, mbbi
     - AcquisitionMode
     - The driver maps the standard areaDetector enums [Single, Multiple, Continuous] into the GenICam enums
       [Continuous, SingleFrame, MultiFrame].
   * - AcquireTime, AcquireTime_RBV
     - ao, ai
     - ExposureTime or ExposureTimeAbs
     - Exposure time in seconds. The units are converted to/from the microseconds used by GenICam.
   * - AcquirePeriod, AcquirePeriod_RBV
     - ao, ai
     - AcquisitionFrameRate or AcquisitionFrameRateAbs
     - Acquire period in seconds. The units are converted to/from the frames/s used by GenICam.
   * - AcquireTime, AcquireTime_RBV
     - ao, ai
     - ExposureTime or ExposureTimeAbs
     - Exposure time in seconds. The units are converted to/from the microseconds used by GenICam.
   * - NumImages,, NumImages_RBV
     - longout, longin
     - AcquisitionFrameCount
     - Number of images to acquire when ImageMode=Multiple
   * - Gain, Gain_RBV
     - ao, ai
     - Gain
     - Analog gain.  Units may vary, often in dB.

ADGenICam driver parameters
~~~~~~~~~~~~~~~~~~~~~~~~~~~

ADGenICam adds a number of driver parameters beyond those in the ADDriver base class 
and the camera-specific parameters described above.  
These parameters are intended to abstract the most commonly used GenICam features so that they:

- Have the same EPICS record names regardless of the actual GenICam feature name.
- Exist even if there is no corresponding GenICam feature for a specific camera, so that OPI screens don't show invalid widgets
  and scripts will not generate errors due to non-existent PVs.

.. cssclass:: table-bordered table-striped table-hover
.. list-table::
   :header-rows: 1
   :widths: auto

   * - EPICS record names
     - Record types
     - GenICam features
     - Description
   * - FrameRate, FrameRate_RBV
     - ao, ai
     - AcquisitionFrameRate or AcquisitionFrameRateAbs
     - Frame rate in frames/s
   * - FrameRateEnable, FrameRateEnable_RBV
     - bo, bi
     - AcquisitionFrameRateEnable or AcquisitionFrameRateEnabled
     - Enables/disables frame rate.  If disabled then exposure time or readout time determines frame rate.
   * - TriggerSource, TriggerSource_RBV
     - mbbo, mbbi
     - TriggerSource
     - Source of trigger signals.
   * - TriggerOverlap, TriggerOverlap_RBV
     - mbbo, mbbi
     - TriggerOverlap
     - Controls whether trigger signals can overlap the exposure or readout.
   * - TriggerSoftware
     - bo
     - TriggerSoftware
     - Generates a software trigger when processed.
   * - ExposureMode, ExposureMode_RBV
     - mbbo, mbbi
     - ExposureMode
     - Typical values are **Timed** and **TriggerWidth**.
   * - ExposureAuto, ExposureAuto_RBV
     - mbbo, mbbi
     - ExposureAuto
     - Enables automatically changing the **ExposureTime** for optimal image.
   * - GainAuto, GainAuto_RBV
     - mbbo, mbbi
     - GainAuto
     - Enables automatically changing the **Gain** for optimal image.
   * - PixelFormat, PixelFormat_RBV
     - mbbo, mbbi
     - PixelFormat
     - Typical values are **Mono8**, **Mono16**, **RGB8Packed**, etc. 
   
.. _ADGenICam_Linux_System_Settings:

Linux USB and GigE System Settings
----------------------------------

In order to run GigE and USB-3 cameras at their full frame rates on Linux it is necessary to change some
of the default Linux system settings.

For GigE cameras the default values of the net.core.rmem_max and net.core.rmem_default are typically much to low
(~200 KB) for optimal performance of GigE and 10 GigE cameras.  These should be increased to 8 MB.  To set the
values immediately execute the following commands as root::

  sysctl -w net.core.rmem_max=8388608 net.core.rmem_default=8388608


To make the settings permanent add these lines to /etc/sysctl.conf::

  net.core.rmem_max=8388608
  net.core.rmem_default=8388608


For USB-3 cameras by default Linux limits image capture to 2 MB. 
To capture images over 2 MB, extend the USBFS limit on how many buffers can be locked into the driver. 
This is done by updating the boot params in grub. You can set the memory limit until the next reboot, or set it permanently.

To set the maximum usbfs memory limit until the next reboot, run this command as root::
 
  modprobe usbcore usbfs_memory_mb=1000


This method does not work with Ubuntu 14.04 or newer. With Ubuntu 14.04, users must set the memory limit permanently.

To set the maximum usbfs memory limit permanently:

Open the /etc/default/grub file in any text editor. Find and replace::

  GRUB_CMDLINE_LINUX_DEFAULT="quiet splash"

with this::

  GRUB_CMDLINE_LINUX_DEFAULT="quiet splash usbcore.usbfs_memory_mb=1000"

Update grub with these settings::

  update-grub
  
On some Linux systems the update-grub command does not exist, so use this instead:

grub2-mkconfig -o /boot/grub2/grub.cfg

Reboot and test a USB 3 camera.

If this method fails to set the memory limit, run the following command::

  sh -c 'echo 1000 > /sys/module/usbcore/parameters/usbfs_memory_mb'

To confirm that you have successfully updated the memory limit, run the following command::

  cat /sys/module/usbcore/parameters/usbfs_memory_mb

MEDM screens
------------
The following is the MEDM screen ADGenICam.adl when controlling a Point Grey Blackfly BFLY-PGE-20E4C camera.
This screen is generic and can be used for any GemICam camera.

.. figure:: ADGenICam.png
    :align: center

The following is the MEDM screen PGR_Blackyfly_20E4C-features1.adl when controlling a Point Grey Blackfly BFLY-PGE-20E4C camera.
This screen was autogenerated by the Python script described above, and is specific to this camera model.
It is loaded from the "Camera-specific features" related display widget in the above screen.

.. figure:: ADGenICam_features1.png
    :align: center

The following are the additional three feature screens generated by the Python program described above.
The number of camera specific screens generated is determined by the number of features in a specific camera
and by the maximum screen size specified in the Python program.

.. figure:: ADGenICam_features2.png
    :align: center

.. figure:: ADGenICam_features3.png
    :align: center

.. figure:: ADGenICam_features4.png
    :align: center


Known issues
------------    
- New cameras use the Gain feature, which is a double.  Some old cameras use GainRaw or GainRawChannelB which is an integer.
  The ADGain parameter cannot be used for GainRawChannelB because the data types don't match, so one must use feature itself.

- Some cameras use an integer for the AcquireTime or AcquirePeriod features.  These won't work, the data types won't match, so one must use raw feature.

  - These problems could possibly be fixed by checking for the case when asynType is different from expected.

- BinningMode feature is only supported by raw feature.  I have no way to test this, I don't have any cameras with that feature.

  - aravisGige allowed setting the binning with ADBinX and ADBinY even if BinningMode only supported certain enums.


