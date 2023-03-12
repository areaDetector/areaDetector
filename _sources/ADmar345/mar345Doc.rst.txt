========
ADmar345
========

:author: Mark Rivers, University of Chicago

.. contents:: Contents

.. _marXperts:   https://marxperts.com

Overview
--------

This is an :doc:`../index` driver for the marXperts_ mar345 detector.


Introduction
------------

The interface to the detector is via a TCP/IP socket interface to the
**mar345dtb** program that marXperts_ provides. The mar345dtb program
must be started before the areaDetector software is started.

mar345dtb must be configured to accepts commands on a TCP/IP socket
port. This is done by editing the file /home/mar345/tables/config.xxx
(where xxx is the serial number of that detector) and editing the
COMMAND line to the following format::

    COMMAND PORT 5001
        
where 5001 is the TCP/IP port to use. Any high port number can be used,
but it must agree with the one specified in the areaDetector
`mar345Config` command described below.

The mar345dtb program saves the data to disk as compressed binary files.
The areaDetector software reads these disk files in order to read the
data, because mar345dtb does not provide another mechanism to access the
data.


This driver inherits from :doc:`../ADCore/ADDriver`.
It implements nearly all of the parameters in
`asynNDArrayDriver.h <../areaDetectorDoxygenHTML/asyn_n_d_array_driver_8h.html>`__
and in
`ADArrayDriver.h <../areaDetectorDoxygenHTML/_a_d_driver_8h.html>`__. It
also implements a number of parameters that are specific to the
mar345 detector. The `mar345 class
documentation <../areaDetectorDoxygenHTML/classmar345.html>`__ describes this class in detail.

Implementation of standard driver parameters
--------------------------------------------

The following table describes how the mar345 driver implements some of
the standard driver parameters.

.. cssclass:: table-bordered table-striped table-hover
.. flat-table::
  :header-rows: 2
  :widths: 10 10 80

  * - **Implementation of Parameters in asynNDArrayDriver.h and ADDriver.h, and EPICS Record
      Definitions in ADBase.template and NDFile.template**
  * - Parameter index variable
    - EPICS record name
    - Description
  * - ADAcquire
    - $(P)$(R)Acquire
    - Setting this to 1 starts an acquisition sequence. If ADNumImages is greater than
      1 then it acquires multiple frames. For each frame it does the following:

      1. Erases the detector if mar345EraseMode is "Before expose". 
      2. Opens the shutter if either the mar345 shutter or EPICS shutter controls are enabled. 
      3. Waits for the desired exposure time. 
      4. Closes the shutter if either the mar345 shutter or EPICS shutter controls are enabled. 
      5. Scans the detector and saves the file. 
      6. Erases the detector if mar345EraseMode is "After scan". 

      If ADAcquire is set to 0 during exposure (step 3 above) then it proceeds immediately
      to step 4, finishes collecting the current frame and stops the acquisition sequence
      if ADNumImages is greater than 1. If mar345Abort is set to 0 then the acquisition
      is terminated as soon as possible without saving the data. Note however that commands
      to the mar345 server to erase, change mode, or scan cannot be aborted, so the driver
      must wait for these commands to complete.
  * - ADNumImages
    - $(P)$(R)NumImages
    - Controls the number of images to acquire when ADImageMode is ADImageMultiple.
  * - ADAcquirePeriod
    - $(P)$(R)AcquirePeriod
    - Controls the period between images when ADImageMode is ADImageMultiple or ADImageContinuous.
      If this is greater than the acquisition time plus readout overhead then the driver
      will wait until the period has elapsed before starting the next acquisition.
  * - NDFilePath
    - $(P)$(R)FilePath
    - Controls the path for saving images. It must be a valid path for mar345dtb *and*
      for the areaDetector driver, which is normally running in an EPICS IOC. If mar345dtb
      and the EPICS IOC are not running on the same machine then soft links will typically
      be used to make the paths look identical.
  * - NDFileTemplate
    - $(P)$(R)FileTemplate
    - The mar345dtb program uses a file extension which describes the scan size and resolution,
      e.g. 3450, etc. Thus the FileTemplate should not include an extension. Furthermore
      mar345dtb always uses exactly 3 digits for the FileNumber (with leading zeros if
      required), just before the extension. Thus the final 3 characters of the FileTemplate
      must be "%3.3d". If they are not then mar345dtb will successfully save the file,
      but with a different name than the areaDetector driver was expecting it to use,
      and the driver will generate an error when it tries to open the file to read the
      data. A good value for FileTemplate is thus "%s%s_%3.3d", which will concatentate
      the FilePath and FileName, followed by an underscore and then a 3 digit number with
      leading zeros if required. The underscore is optional, and other characters can
      be placed between the FileName and the FileNumber if desired.
  * - NDFileFormat
    - $(P)$(R)FileFormat
    - mar345 only supports mar345 format binary files.
  * - NDStatus
    - $(P)$(R)DetectorState_RBV
    - mar345 replaces the state strings with the following: Exposing, Scanning, Erasing,
      Changing Mode, Aborting, Error, and Waiting.


mar345 specific parameters
--------------------------

The mar345 driver implements the following parameters in addition to
those in asynNDArrayDriver.h and ADDriver.h. Note that to reduce the
width of this table the parameter index variable names have been split
into 2 lines, but these are just a single name, for example
``mar345ScanSize``.

.. cssclass:: table-bordered table-striped table-hover
.. flat-table::
  :header-rows: 2
  :widths: 80 10 10

  * - **Parameter Definitions in mar345.cpp and EPICS Record Definitions in mar345.template**
  * - Description
    - EPICS record name
    - EPICS record type
  * - **Readout parameters**
  * - The detector diameter to read out. Choices are 180mm, 240mm, 300mm, and 345mm.
    - $(P)$(R)ScanSize, $(P)$(R)ScanSize_RBV
    - mbbo, mbbi
  * - The pixel size to use when reading the detector out. Choices are 0.10 and 0.15mm.
    - $(P)$(R)ScanResolution, $(P)$(R)ScanResolution_RBV
    - mbbo, mbbi
  * - Writing 1 to this parameter causes the ScanSize and ScanResolution values to be
      sent to the server, changing the scan mode. This is not strictly necessary, because
      the size and resolution is also encoded in the file extension used in the scan command.
      However, changing the mode before doing a scan reduces the time for the scan, because
      the detector is already configured for the correct mode.
    - $(P)$(R)ChangeMode, $(P)$(R)ChangedMode_RBV
    - busy, bi
  * - **Erase parameters**
  * - Controls whether an erase cycle should be automatically performed during acquisition.
      Choices are None, Before expose, and After scan.
    - $(P)$(R)EraseMode, $(P)$(R)EraseMode_RBV
    - mbbo, mbbi
  * - The number of erase cycles to perform each time the detector is erased, either because
      the mar345Erase parameter is set to 1, or because of an automatic erase as part
      of an acquisition.
    - $(P)$(R)NumErase, $(P)$(R)NumErase_RBV
    - longout, longin
  * - Write 1 to this parameter to initiate erasing the detector. The detector will be
      erased multiple times if mar345NumErase is greater than 1.
    - $(P)$(R)Erase, $(P)$(R)Erase_RBV
    - busy, bi
  * - **Abort parameters**
  * - Writing 1 to this parameter aborts the current operation as soon as possible and
      returns the driver to the idle state. Note however that commands to the mar345 server
      cannot be aborted, so the driver must wait for the current command to complete.
    - $(P)$(R)Abort, $(P)$(R)Abort_RBV
    - bo, bi
  * - **Debugging**
  * - asyn record to control debugging communication with mar345dtb program
    - $(P)$(R)marSserverAsyn
    - asyn


Unsupported standard driver parameters
--------------------------------------

The mar345 driver does not support the following standard driver
parameters because they are not supported in the mar345dtb program:

-  Number of exposures per image (ADNumExposures)
-  Trigger mode (ADTriggerMode)
-  Gain (ADGain)
-  Region to read out (ADMinX, ADMinY, ADSizeX, ADSizeY, ADReverseX,
   ADReverseY)
-  Binning (ADBinX, ADBinY)
-  Frame type (ADFrameType)
-  Data type (NDDataType)
-  Reading previous files (NDReadFile)
-  Capture or stream file saving (NDFileWriteMode, NDFileCapture,
   NDNumCapture, NDNumCaptured)

Configuration
-------------

The mar345 driver is created with the mar345Config command, either from
C/C++ or from the EPICS IOC shell.

::

    int mar345Config(const char *portName, const char *serverPort,
                     int maxBuffers, size_t maxMemory,
                     int priority, int stackSize)
      
For details on the meaning of the parameters to this function refer to
the detailed documentation on the mar345Config function in the
`mar345.cpp documentation <../areaDetectorDoxygenHTML/mar345_8cpp.html>`__ and
in the documentation for the constructor for the 
`mar345 class <../areaDetectorDoxygenHTML/classmar345.html>`__.

Example st.cmd startup file
---------------------------

.. toctree::
   :hidden:
   
   st_cmd.rst

There is an example IOC boot directory and startup script
:ref:`st.cmd <ADmar345_st_cmd_example>` provided with
areaDetector.

MEDM screens
------------

The following show the MEDM screens that are used to control the mar345
detector. Note that the general purpose screen ADBase.adl can be used,
but it exposes many controls that are not applicable to the mar345, and
lacks some fields that are important for the mar345.

``mar345.adl`` is the main screen used to control the mar345 driver.

.. figure:: mar345.png
    :align: center

Performance measurements
------------------------

The mar345 is definitely not a fast detector! The following measurements
show the time to perform various erase and scan operations. Note that
because the mar345 file format is compressed the file sizes are
typically much less than the image sizes listed.

.. cssclass:: table-bordered table-striped table-hover
.. flat-table::
  :header-rows: 1
  :widths: 20 20 20 20 20 20

  * - Scan diameter
    - Pixel size
    - Image dimensions
    - Image size (MB)
    - Time to scan
    - Time to erase
  * - 180 mm
    - 0.15 mm
    - 1200x1200
    - 2.7
    - 38.6
    - 37.8
  * - 240 mm
    - 0.15 mm
    - 1600x1600
    - 4.9
    - 50.4
    - 50.8
  * - 300 mm
    - 0.15 mm
    - 2000x2000
    - 7.6
    - 74.7
    - 66.9
  * - 345 mm
    - 0.15 mm
    - 2300x2300
    - 10.1
    - 88.6
    - 82.7
  * - 180 mm
    - 0.10 mm
    - 1800x1800
    - 6.2
    - 46.4
    - 45.9
  * - 240 mm
    - 0.10 mm
    - 2400x2400
    - 11.0
    - 71.9
    - 63.8
  * - 300 mm
    - 0.10 mm
    - 3000x3000
    - 17.2
    - 89.1
    - 87.0
  * - 345 mm
    - 0.10 mm
    - 3450x3450
    - 22.7
    - 107.5
    - 107.1

Restrictions
------------

The following are some current restrictions of the mar345 driver:

-  The areaDetector driver can in principle be run on machines other
   than the Linux machine running mar345dtb, since the connection is via
   a socket. It has only been tested on Linux.
-  The following items are hardcoded in the driver. They can be changed
   by recompiling compiling if necessary.

   -  MAX\_MESSAGE\_SIZE=256 The maximum size of message to/from
      mar345dtb server.
   -  MAX\_FILENAME\_LEN=256 The maximum size of a complete file name
      including path and extension.
   -  MAR345\_COMMAND\_TIMEOUT=180.0 Timeout for a complete erase or
      scan command to complete.

