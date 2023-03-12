======================================
ADmarCCD
======================================

:author: Mark Rivers, University of Chicago

.. contents:: Contents

.. _Rayonix:   https://www.mar-usa.com

Overview
--------

This is an :doc:`../index` driver for Rayonix_ marCCD detectors.

The interface to the detector is via a TCP/IP socket interface to the
``marccd\_server\_socket`` server that Rayonix provides. The
``marccd\_server\_socket`` program must be started before the areaDetector
software is started, by running the marccd program and executing
``Acquire/Remote Control/Start``.

marccd must be using Version 1 (for non-HS detectors) or 2 (for HS
detectors) of the remote protocol. This is normally done done by editing
the file ``marccd/configuration/marccd.conf`` and replacing the line::

  include marccd_server_v0.conf

with::

  include marccd_server_v1.conf
        

The file ``marccd_server_v1.conf`` should contain the lines::

  remote_mode_server_command /home/marccd/contrib/marccd_server/marccd_server_socket
  remote_mode_server_arguments 2222
        

The first line points to the location of the `marccd\_server\_socket`
program that is used to implement remote control. In order to work with
the areaDetector driver this must be a version of this program created
after November 11, 2008 when support for the `get_frameshift` command
was added. A recent version of this program can be downloaded from the
`Rayonix FTP site <ftp://ftp.rayonix.com/pub/marccd/example_marccd_server.tgz>`__.

The marccd program saves the data to disk as TIFF files. The
areaDetector software reads these disk files in order to read the data,
because marccd does not provide another mechanism to access the data.

This driver inherits from :doc:`../ADCore/ADDriver`.
It implements nearly all of the parameters in
`asynNDArrayDriver.h <../areaDetectorDoxygenHTML/asyn_n_d_array_driver_8h.html>`__
and in
`ADArrayDriver.h <../areaDetectorDoxygenHTML/_a_d_driver_8h.html>`__. It
also implements a number of parameters that are specific to the
marCCD detectors. The `marCCD class
documentation <../areaDetectorDoxygenHTML/class_mar_C_D_C.html>`__
describes this class in detail.

Implementation of standard driver parameters
--------------------------------------------

The following table describes how the MarCCD driver implements some of
the standard driver parameters.

.. |br| raw:: html

    <br>

.. cssclass:: table-bordered table-striped table-hover
.. flat-table::
  :header-rows: 2
  :widths: 20 20 60

  * - **Implementation of Parameters in asynNDArrayDriver.h and ADDriver.h, and EPICS Record
      Definitions in ADBase.template and NDFile.template**
  * - Parameter index variable
    - EPICS record name
    - Description
  * - ADFrameType
    - $(P)$(R)FrameType
    - The driver redefines the choices for the ADFrameType parameter (record $(P)$(R)FrameType)
      from ADDriver.h. The choices for the MarCCD are: |br|
      ``Normal`` (corrected data frame without double correlation) |br|
      ``Background`` (background frame with 0 exposure time, done with double correlation
      to remove zingers) |br|
      ``Raw`` (data frame without correction for background or spatial distortion) |br|
      ``DblCorrelation`` (two images each collected for half the nominal acquisition time,
      zingers removed by double correlation) |br|
  * - ADImageMode
    - $(P)$(R)ImageMode
    - The driver redefines the choices for the ADImageMode parameter (record $(P)$(R)ImageMode)
      from ADDriver.h.
      ``Single`` (aingle frame acquisition) |br|
      ``Multiple`` (acquire ADNumImages images) |br|
      ``Continuous`` (acquire images continuously until ADAcquire is set to 0) |br|
      ``Series triggered`` (acquire a series of images using an external trigger signal) |br|
      ``Series timed`` (acquire a series of images using the internal clock) |br|
      ``Series triggered`` and ``Series timed`` are only supported for ServerMode=2 servers.
  * - ADTriggerMode
    - $(P)$(R)TriggerMode
    - The driver redefines the choices for the ADTriggerMode parameter (record $(P)$(R)TriggerMode)
      from ADDriver.h.
      ``Internal`` (Single frame acquisition) |br|
      ``Frame`` (rising edge causes frame transfer/readout and start of next acquisition) |br|
      ``Bulb`` (rising edge of trigger starts acquisition, falling edge causes frame transfer/readout) |br|
      ``Timed`` (rising edge of trigger causes frame transfer/readout and each exposure lasts for time
      ADAcquireTime) |br|
      ServerMode=1 servers only support ``Internal``, ServerMode=2 servers support all 4 modes.
  * - ADNumImages
    - $(P)$(R)NumImages
    - Controls the number of images to acquire when ADImageMode is ADImageMultiple.
  * - ADAcquirePeriod
    - $(P)$(R)AcquirePeriod
    - Controls the period between images when ADImageMode is ADImageMultiple or ADImageContinuous.
      If this is greater than the acquisition time plus readout overhead then the driver
      will wait until the period has elapsed before starting the next acquisition.
  * - ADReadStatus
    - $(P)$(R)ReadStatus
    - Writing 1 to this parameter causes the status to be read from the marccd server.
      By processing or periodically scanning this record the status information can be
      refreshed. This is normally not necessary, but if ADArrayCallbacks is 0 and marCCDOverlap
      is 1 then the status will not indicate that the system is idle when acquisition
      is complete, because the driver polling stops before the file is written. This record
      can be used to eliminate the confusion that might cause.
  * - NDFilePath
    - $(P)$(R)FilePath
    - Controls the path for saving images. It must be a valid path for marccd *and*
      for the areaDetector driver, which is normally running in an EPICS IOC. If marccd
      and the EPICS IOC are not running on the same machine then soft links will typically
      be used to make the paths look identical.
  * - NDFileFormat
    - $(P)$(R)FileFormat
    - marccd only supports TIFF files.

It is useful to use an NDPluginStats plugin set to use the entire marCCD
detector. The MaxValue\_RBV PV can be monitored to make sure that the
16-bit limit of 65,535 is not being approached in any pixel.

MarCCD specific parameters
--------------------------

The MarCCD driver implements the following parameters in addition to
those in asynNDArrayDriver.h and ADDriver.h. Note that to reduce the
width of this table the parameter index variable names have been split
into 2 lines, but these are just a single name, for example
``marCCDState``.

.. cssclass:: table-bordered table-striped table-hover
.. flat-table::
  :header-rows: 2
  :widths: 70 10 10 10

  * - **Parameter Definitions in marccd.cpp and EPICS Record Definitions in marccd.template**
  * - Description
    - drvInfo string
    - EPICS record name
    - EPICS record type
  * - **Status parameters**
  * - The version number of the server software running on the detector.  This is 1 for the
      servers running on non-HS detectors, and 2 for the servers running on HS detectors.
    - MAR_SERVER_MODE
    - $(P)$(R)ServerMode_RBV
    - longin
  * - State word returned by marccd server. The low-order 4-bits of this word are the
      state of the marccd server, and will be Idle (0x0), Error (0x7), or Busy (0x8).
      The next 24 bits encode the state of the 6 server tasks (Acquire, Readout, Correct,
      Save, Dezinger, Series) with 4-bits per task. Each task can be in the state Idle (0x0),
      Queued (0x1), Executing (0x2), Error (0x4), or Reserved (0x8).
    - MAR_STATE
    - $(P)$(R)MarState_RBV
    - longin
  * - Status of the marccd server task (Idle, Error, or Busy)
    - MAR_STATUS
    - $(P)$(R)MarStatus_RBV
    - mbbi
  * - Status of the marccd server acquire task (Idle, Queued, Executing, Error, or Reserved)
    - MAR_ACQUIRE_STATUS
    - $(P)$(R)MarAcquireStatus_RBV
    - mbbi
  * - Status of the marccd server readout task (Idle, Queued, Executing, Error, or Reserved)
    - MAR_READOUT_STATUS
    - $(P)$(R)MarReadoutStatus_RBV
    - mbbi
  * - Status of the marccd server correct task (Idle, Queued, Executing, Error, or Reserved)
    - MAR_CORRECT_STATUS
    - $(P)$(R)MarCorrectStatus_RBV
    - mbbi
  * - Status of the marccd server file writing task (Idle, Queued, Executing, Error, or
      Reserved)
    - MAR_WRITING_STATUS
    - $(P)$(R)MarWritingStatus_RBV
    - mbbi
  * - Status of the marccd server dezinger task (Idle, Queued, Executing, Error, or Reserved)
    - MAR_DEZINGER_STATUS
    - $(P)$(R)MarDezingerStatus_RBV
    - mbbi
  * - Status of the marccd server series acquisition task (Idle, Queued, Executing, Error, or Reserved).
    - MAR_SERIES_STATUS
    - $(P)$(R)MarSeriesStatus_RBV
    - mbbi
  * - **Optimization parameters**
  * - The marccd server has 5 tasks (Acquire, Readout, Correct, Write, Dezinger) that
      can overlap their operation. The areaDetector driver can exploit this to improve
      performance in some circumstances. If this parameter is set to 1 (Overlap) then
      the ADAcquire parameter will go to 0 (Done) when the Readout task is done executing,
      but before the Correct and Write tasks have finished correcting and saving the file
      to disk. This improves performance because the next image can begin as soon as ADAcquire
      goes to done, and hence before the previous image is written to disk. Note, however
      that this parameter must be set to 0 (Sequential) if callbacks are being used to
      compute statistics that are being used in data collection, e.g. in a scan. If this is
      not done then the statistics information will be grabbed before it is updated and incorrect
      scan data will result.
    - MAR_OVERLAP
    - $(P)$(R)OverlapMode, $(P)$(R)OverlapMode_RBV
    - bo, bi
  * - **Frameshift parameters**
  * - marccd can be used for time-resolved studies by collecting multiple data sets before
      reading out the detector. This is done by placing a mask in front of the detector
      that restricts the x-rays to horizontal stripe. An exposure is made, and then an
      external signal causes the detector to shift the image by the number of lines given
      by this parameter. A number of images separated by times of a few milliseconds can
      be collected, and then the detector is read out. Set this parameter to 0 to disable
      frameshift mode.
    - MAR_FRAME_SHIFT
    - $(P)$(R)FrameShift, $(P)$(R)FrameShift_RBV
    - longout, longin
  * - **Series acquisition parameters (ServerMode=2 only)**
  * - The template for the file names written in trigger or timed series acquisition modes.
      The FilePath, FileName and FileNumber are combined into a string using this
      C format string.  This is used to construct a base file name.
      The actual file names in the series are constructed using this base file name plus
      the SeriesFileFirst and SeriesFileDigits records. Example: "%s%s_%3.3d"
    - MAR_SERIES_FILE_TEMPLATE
    - $(P)$(R)SeriesFileTemplate, $(P)$(R)SeriesFileTemplate_RBV
    - waveform, waveform
  * - The number of the first file in a triggered or timed series acquisition.
    - MAR_SERIES_FILE_FIRST
    - $(P)$(R)SeriesFileFirst, $(P)$(R)SeriesFileFirst_RBV
    - longout, longin
  * - The number of digits to use for the file numbers in triggered or timed series acquisition.
    - MAR_SERIES_FILE_DIGITS
    - $(P)$(R)SeriesFileDigits, $(P)$(R)SeriesFileDigits_RBV
    - longout, longin
  * - **Gating and readout parameters (ServerMode=2 only)**
  * - The gating mode for the detector. Choices are: , "None" , "Gated"
    - MAR_GATE_MODE
    - $(P)$(R)GateMode, $(P)$(R)GateMode_RBV
    - mbbo, mbbi
  * - The readout mode for the detector. Choices are: ``Standard``, ``High gain`` 
      ``Low noise`` , ``HDR`` (High Dynamic Range)
    - MAR_GATE_MODE
    - $(P)$(R)GateMode, $(P)$(R)GateMode_RBV
    - mbbo, mbbi
  * - **Baseline stabilization parameters**
  * - The following text is from a document describing baseline stabilization from Rayonix.
      "Baseline stabilization is an optional addition to the marccd data collection software.
      This software option stabilizes the baseline offset level of each CCD image to a
      more accurate value than only the analog electronics provide. This feature is important
      in any type of measurement that requires comparisons between successive data frames
      that include, for example, subtracting (or adding) two data frames, such as one
      often must do in small angle scattering experiments. Baseline instability can make
      it appear that there are slightly more or slightly less X-rays across the entire
      detector (or readout channel) in a data frame. That is different than the read noise,
      which has no net effect on the average. A stable baseline is less critical for data
      analysis in which a background value is calculated by measuring the background around
      each individual spot on the same data frame (typically done in single crystal crystallography
      experiments). The baseline level of a CCD is usually established by measuring an
      analog voltage of the readout amplifier, and the "zero" level can drift over time
      due to ambient temperature changes or other electronic instability. The time scale
      on which the drift occurs is usually greater than about 20 minutes or so; therefore,
      the marccd software default for recollecting background images is every 20 minutes
      or once every data segment in a dataset. Expected baseline stability improvement
      Whereas the older MarCCD detectors had a baseline stability that was only good to
      about &plusmn;1-2 ADU, the SX Series and MX Series detectors have improved electronic
      baseline stability, closer to +-0.5 ADU. When this Baseline Stabilization
      software option is used, the baseline can be improved much further, with baseline
      stability as low as about +-0.01 ADU. CCD overscan The method of improving
      the baseline is by an overscan technique. When this option is "on", extra blank
      pixels are read out from the CCD after each line of the CCD is read out from the
      serial register. In the marccd program memory, a temporary data frame which is larger
      than the normal data frame is recorded, and the pixels outside the imaging area
      are used to compute the baseline. These blank pixels do not correspond to any real
      region of the CCD; they are just a result of telling the readout electronics to
      readout with no charge present., The user must also enter a target baseline stability value. 
      This number represents
      the accuracy to which the program will try to stabilize the baseline, in ADU (analog-to-digital
      units). The resulting data frame will have a baseline value that is approximately
      the corrected_frame_bias, typically 10 or 100, plus or minus the target baseline
      stability value. For example, if the user enters 0.1, and the corrected_frame_bias
      is 10 (i.e. images with no X-rays normally have a baseline around 10 ADU), then
      a data frame with X-rays will result in a baseline value of approximately 10 &plusmn;0.1
      ADU (and each individual pixel will also have contributions due to X-rays and read
      noise). The accuracy limit of this software feature is about 0.01 ADU, so any target
      value entered between 0 and 0.01 is automatically converted to the limit, 0.01 ADU.
      Entering a target value of 0 is equivalent to turning off the baseline stabilization."
    - MAR_STABILITY
    - $(P)$(R)Stability, $(P)$(R)Stability_RBV
    - ao, ai
  * -
    - **Timeout parameters**
  * - Timeout in seconds when reading a TIFF file. It should be set to several seconds,
      because there it can take some time for the marccd server to write the file.
    - MAR_TIFF_TIMEOUT
    - $(P)$(R)ReadTiffTimeout
    - ao
  * - **Ancillary parameters. These parameters are written to the header of the marccd
      TIFF file.**
  * - Distance from the sample to the detector (mm)
    - MAR_DETECTOR_DISTANCE
    - $(P)$(R)DetectorDistance
    - ao
  * - X position of the direct beam on the detector (mm)
    - MAR_BEAM_X
    - $(P)$(R)BeamX
    - ao
  * - Y position of the direct beam on the detector (mm)
    - MAR_BEAM_Y
    - $(P)$(R)BeamY
    - ao
  * - Starting value of phi rotation (deg)
    - MAR_START_PHI
    - $(P)$(R)StartPhi
    - ao
  * - Rotation axis being used (phi, omega, etc.)
    - MAR_ROTATION_AXIS
    - $(P)$(R)RotationAxis
    - stringout
  * - Rotation range of the rotation axis.
    - MAR_ROTATION_RANGE
    - $(P)$(R)RotationRange
    - ao
  * - Detector two-theta angle (deg); requires theta axis definition with display name
      "TwoTheta" in a marccd configuration file (i.e. ``theta_display_name TwoTheta``).
      This configuration file is typically goniostat_none.conf or goniostat_sw.conf, but
      any configuration file that gets loaded can be used.
    - MAR_TWO_THETA
    - $(P)$(R)TwoTheta
    - ao
  * - Wavelength in Angstroms.
    - MAR_WAVELENGTH
    - $(P)$(R)Wavelength
    - ao
  * - Comments for this file.
    - MAR_FILE_COMMENTS
    - $(P)$(R)FileComments
    - waveform
  * - Comments for this dataset.
    - MAR_DATASET_COMMENTS
    - $(P)$(R)DatasetComments
    - waveform
  * -
    - **Debugging**
  * - asyn record to control debugging communication with marccd_server_socket program
    - N/A
    - $(P)$(R)marSserverAsyn
    - asyn

Unsupported standard driver parameters
--------------------------------------

The MarCCD driver does not support the following standard driver
parameters because they are not supported in the marccd program:

-  Number of exposures per image (ADNumExposures)
-  Gain (ADGain)
-  Region to read out (ADMinX, ADMinY, ADSizeX, ADSizeY, ADReverseX,
   ADReverseY)
-  Data type (NDDataType)
-  Reading previous files (NDReadFile)
-  Capture or stream file saving (NDFileWriteMode, NDFileCapture,
   NDNumCapture, NDNumCaptured)

Configuration
-------------

The marCCD driver is created with the marCCDConfig command, either from
C/C++ or from the EPICS IOC shell.

::

    int marCCDConfig(const char *portName, const char *serverPort,
                     int maxBuffers, size_t maxMemory,
                     int priority, int stackSize)
      

For details on the meaning of the parameters to this function refer to
the detailed documentation on the marCCDConfig function in the
`marCCD.cpp documentation <../areaDetectorDoxygenHTML/mar_c_c_d_8cpp.html>`__ and
in the documentation for the constructor for the 
`marCCD class <../areaDetectorDoxygenHTML/classmar_c_c_d.html>`__.

Example st.cmd startup file
---------------------------

.. toctree::
   :hidden:
   
   st_cmd.rst

There is an example IOC boot directory and startup script
:ref:`st.cmd <ADmarCCD_st_cmd_example>` provided with
areaDetector.

MEDM screens
------------

The following show the MEDM screens that are used to control the MarCCD
detector. Note that the general purpose screen ADBase.adl can be used,
but it exposes many controls that are not applicable to the MarCCD, and
lacks some fields that are important for the MarCCD.

``marCCD.adl`` is the main screen used to control the marCCD driver.

.. figure:: marCCD.png
    :align: center


``marccdAncillary.adl`` is the screen used to input ancillary
information that is written to the MarCCD TIFF files.

.. figure:: MarCCDAncillary.png
    :align: center

``asynRecord.adl`` is used to control the debugging information printed
by the asyn TCP/IP driver (asynTraceIODriver) and the EPICS device
support (asynTraceIODevice).

.. figure:: MarCCDAsynRecord.png
    :align: center

``asynOctet.adl`` can be used to send any command to the marccd remote
server and display the response. It can be loaded from the More menu in
asynRecord.adl above.

.. figure:: MarCCDAsynOctet.png
    :align: center

Performance measurements
------------------------

The following measurements were done to demonstrate the performance that
can be obtained with the areaDetector MarCCD driver. These measurements
were made with a MAR-165 CCD. The EPICS IOC was running on the same
Linux machine as the marccd program. The acquisition time was 1 second.

.. cssclass:: table-bordered table-striped table-hover
.. flat-table::
  :header-rows: 2

  * - Binning
    - Image size
    - marCCDOverlap
    - Time for 10 images
    - Overhead per image
    - Time per task
  * - 2x2
    - 2048x2048
    - Sequential
    - 50.0
    - 4.00
    - Readout: 3.02
      , Correct: 0.56
      , Save: 0.20
  * - 2x2
    - 2048x2048
    - Overlap
    - 46.2
    - 3.62
    - Same
  * - 4x4
    - 1024x1024
    - Sequential
    - 29.0
    - 1.90
    - Readout: 1.30
      , Correct: 0.28
      , Save: 0.06
  * - 4x4
    - 1024x1024
    - Overlap
    - 28.7
    - 1.87
    - Same
  * - 8x8
    - 512x512
    - Sequential
    - 24.0
    - 1.40
    - Readout: 0.78
      , Correct: 0.29
      , Save: 0.06
  * - 8x8
    - 512x512
    - Overlap
    - 23.6
    - 1.36
    - Same

Restrictions
------------

The following are some current restrictions of the MarCCD driver:

-  The marccd program can save files in many different formats, with
   TIFF being the default. The areaDetector driver can only read TIFF
   files, not other formats.
-  The areaDetector driver can in principle be run on machines other
   than the Linux machine running marccd, since the connection is via a
   socket. It has only been tested on Linux.
-  The MarCCD driver keeps retrying to read each TIFF file until the
   modification date of the TIFF file is *after* the time that the
   exposure command was issued. If it did not do this check then it
   could be reading and displaying old files that happen to have the
   same name as the current files being collected. This check requires
   that the computer that is running the soft IOC must have its clock
   well synchronized with the clock on the computer on which the files
   are being written (i.e. the computer generating the file modification
   time). If the clocks are not synchronized then the files may appear
   to be stale when they are not, and the driver will time out. The
   driver actually tolerates up to 10 second clock skew betweeen the
   computers but any more than this may lead to problems.
-  The following items are hardcoded in the driver. They can be changed
   by recompiling compiling if necessary.

   -  MAX_MESSAGE_SIZE=256 The maximum size of message to/from
      marccd\_socket\_server.
   -  MAX_FILENAME_LEN=256 The maximum size of a complete file name
      including path and extension.
   -  MARCCD_SERVER_TIMEOUT=1.0 Timeout when communicating with
      marccd\_socket\_server.
   -  FILE_READ_DELAY=.01 seconds. The time between polling to see if
      the TIFF file exists or if it is the expected size.
   -  MARCCD_POLL_DELAY=.01 seconds. The time between polling the
      marccd_socket_server status to see when a task has completed.
