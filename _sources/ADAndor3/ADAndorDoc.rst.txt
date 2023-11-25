ADAndor3 
==========================

:author: Mark Rivers (University of Chicago), Phillip Sorensen (Cornell University)

Table of Contents
-----------------

.. contents:: Contents

Introduction
------------

This is an `EPICS <http://www.aps.anl.gov/epics>`__
`areaDetector <areaDetector.html>`__ driver for CMOS detectors from
`Andor Technology <http://www.andor.com>`__. It has been tested on the
Andor Neo sCMOS camera with 3-tap Camera Link interface, but should work
with other cameras (e.g. Zyla) as well. The driver is supported on
32-bit and 64- bit Linux and 32-bit and 64-bit Windows. The driver is
called "Andor3" because it is built with Version 3 of the Andor Software
Development Kit (SDK). This version of the SDK is required to work with
the Andor sCMOS cameras, and currently does **not** work with the Andor
CCD cameras.

The driver provides access to essentially all of the features of the
Andor sCMOS cameras:

-  Fixed number of frames or continuous acquisition.
-  Multiple accumulations per frame.
-  Readout frequency
-  Readout mode (11-bit low noise, 11-bit high-well, 16 bit
   combination).
-  Support for all of the Andor trigger modes
-  Binning and Area Of Interest (AOI) readout
-  Set and monitor the camera temperature
-  Set the camera fan speed.
-  Gating and Image Intensifier settings

This driver inherits from `ADDriver <areaDetectorDoc.html#ADDriver>`__.
It implements many of the parameters in
`asynNDArrayDriver.h <../areaDetectorDoxygenHTML/asyn_n_d_array_driver_8h.html>`__
and in
`ADArrayDriver.h <../areaDetectorDoxygenHTML/_a_d_driver_8h.html>`__. It
also implements a number of parameters that are specific to the Andor
detectors. The `andor3 class
documentation <../areaDetectorDoxygenHTML/classandor3.html>`__ describes
this class in detail.

This document does not attempt to explain the meaning of the
Andor-specific parameters. The Andor Software Development Kit
documentation provides this detailed information. Andor does not allow
me to redistribute the SDK documentation as part of areaDetector. It
must be obtained from `Andor's Web
site <http://www.andor.com/scientific-software/software-development-kit/andor-sdk>`__.

The Andor3 SDK is very well designed. Camera parameters (e.g. exposure
time, binning) are called "features". Features can be integer, float,
bool, string, or enum. Each feature can be queried to determine if it is
implemented on the current detector. In addition:

-  For integer and float features:

   -  What is the valid range of values for the current camera under the
      current conditions?

-  For enum features

   -  How many enum choices are there?
   -  For each enum choice:

      -  What is the string associated with that enum choice?
      -  Is that choice implemented for the current camera?
      -  If it is implemented, is it valid under the current conditions?

-  Ability to register a user-defined C callback function that will be
   called whenever a feature value changes. These changes can be the
   indirect result of changing another feature. For example, changing
   the binning might force the exposure time to change, etc.

The areaDetector driver uses these features. All of the enum menus are
built dynamically at iocInit, they are not preset in the template file.
This ensures that the enum choices match the actual capabilities of the
current camera. Whenever an integer or float parameter is changed it is
checked to ensure it is within the current valid bounds for that
feature. The feature callback is used to ensure that the current EPICS
readback value of that parameter matches the actual camera value,
without requiring the driver to poll.

areaDetector includes the header and library files required to build the
andor3 driver on any Linux or Windows computer. However, it does not
include the shareable libraries, DLLs or drivers to actually run a
detector. Those must be obtained from Andor, either by purchasing their
SDK or their Solis application software. On Windows the path to the
directory containing the Andor DLLs from the SDK or Solis must be added
to the PATH environment variable when running the areaDetector IOC. On
Linux the path to the directory containing the Andor shareable libraries
from the SDK must be added to the LD_LIBRARY_PATH environment variable
when running the areaDetector IOC.

Note: Linux drivers and Bitflow based camera may require the removal of
files /usr/local/lib/libatusb*. These files sometime interfere with
Bitflow based cameras on Linux (per Andor).

Implementation of standard driver parameters
--------------------------------------------

The following table describes how the Andor driver implements some of
the standard driver parameters.

.. |br| raw:: html

    <br>

.. cssclass:: table-bordered table-striped table-hover
.. flat-table::
  :header-rows: 2
  :widths: 10 10 80

  * - **Implementation of Parameters in asynNDArrayDriver.h and ADDriver.h, and EPICS Record 
      Definitions in ADBase.template and NDFile.template**
  * - Parameter index variable
    - EPICS record name
    - Description
  * - ADTriggerMode
    - $(P)$(R)TriggerMode, $(P)$(R)TriggerMode_RBV
    - Sets the trigger mode for the detector. Options for the Neo are:
      |br| Internal |br|
      Software |br|
      External |br|
      External Start |br|
      External Exposure |br|
      Advanced |br|
  * - ADImageMode
    - $(P)$(R)ImageMode, $(P)$(R)ImageMode_RBV
    - Sets the image mode for the detector. Options are:
      |br| Fixed |br|
      Continuous |br|
      Fixed counts collects NumImages images, continuous acquires continuously until acquisition is stopped.
  * - ADNumExposures
    - $(P)$(R)NumExposures, $(P)$(R)NumExposures_RBV
    - Sets the number of accumulations per image. This is performed in Andor's driver, not on the chip.
  * - ADNumImages
    - $(P)$(R)NumImages, $(P)$(R)NumImages_RBV
    - Sets the number of images to take when ImageMode=Fixed.
  * - ADAcquirePeriod
    - $(P)$(R)AcquirePeriod, $(P)$(R)AcquirePeriod_RBV
    - Sets the FrameRate (=1./AcquirePeriod). The FrameRate can also be set directly.
      |br| The minimum AcquirePeriod (maximum FrameRate) depends on a number of other parameters,
      including the A3Binning, SizeX, SizeY, ReadoutRate, A3ShutterMode, PixelEncoding,
      and Overlap.
  * - NDDataType
    - $(P)$(R)DataType, $(P)$(R)DataType_RBV
    - The data type for the image data. Allowed values are:
      |br| UInt16 |br|
      UInt32 |br|
      The DataType is selected automatically, based upon the PixelEncoding record. The
      data type will be UInt32 if PixelEncoding is Mono32, and UInt16 for all other values
      of PixelEncoding. PixelEncoding=Mono32 can be used when A3Binning &gt; 1x1, or when
      multiple accumulations (NumExposures &gt; 1) could cause 16-bit overflow.
  * - ADTemperature
    - $(P)$(R)Temperature, $(P)$(R)Temperature_RBV
    - Sets the setpoint temperature of the camera.
  * - ADTemperatureActual
    - $(P)$(R)TemperatureActual
    - Reads the actual temperature of the camera.

Andor specific parameters
-------------------------

.. cssclass:: table-bordered table-striped table-hover
.. flat-table::
  :header-rows: 2
  :widths: 10 10 5 80 10 10 5

  * - **Parameter Definitions in andor3.cpp and EPICS Record Definitions in andor3.template**
  * - Parameter index variable
    - asyn interface
    - Access
    - Description
    - drvInfo string
    - EPICS record name
    - EPICS record type
  * - **Version information**
  * - Andor3SerialNumber
    - asynOctet
    - R/O
    - Camera serial number
    - A3_SERIAL_NUMBER
    - SerialNumber
    - stringin
  * - Andor3FirmwareVersion
    - asynOctet
    - R/O
    - Camera firmware version
    - A3_FIRMWARE_VERSION
    - FirmwareVersion
    - stringin
  * - Andor3SoftwareVersion
    - asynOctet
    - R/O
    - SDK software version
    - A3_SOFTWARE_VERSION
    - SoftwareVersion
    - stringin
  * - Andor3ControllerID
    - asynOctet
    - R/O
    - Camera controller ID
    - A3_CONTROLLER_ID
    - ControllerID
    - stringin
  * - **Cooling control and status**
  * - Andor3SensorCooling
    - asynInt32
    - R/W
    - Turn the sensor cooler on and off
    - A3_SENSOR_COOLING
    - SensorCooling, SensorCooling_RBV
    - bo, bi
  * - Andor3TempStatus
    - asynInt32
    - R/O
    - Temperature status. Values and strings on the Neo are:
      |br| 0 = Cooler Off |br|
      1 = Stabilized |br|
      2 = Cooling |br|
      3 = Drift |br|
      4 = Not Stabilized |br|
      5 = Fault |br|
    - A3_TEMP_STATUS
    - TempStatus_RBV
    - mbbi
  * - Andor3TempControl
    - asynInt32
    - R/W
    - Temperature setpoint value in degrees C. On the cooled cameras (e.g. Neo) the temperature
      is set to discrete values, rather than continuously with the Temperature record,
      because the firmware does pixel corrections for specific temperatures. Choices on
      the Neo are:
      |br| 0 = -15 |br|
      1 = -20 |br|
      2 = -25 |br|
      3 = -30 |br|
      4 = -35 |br|
      5 = -40 |br|
    - A3_TEMP_CONTROL
    - TempControl, TempControl_RBV
    - mbbo, mbbi
  * - Andor3FanSpeed
    - asynInt32
    - R/W
    - Fan speed. Choices on the Neo are:
      |br| 0 = Off |br|
      1 = Low |br|
      2 = On |br|
    - A3_FAN_SPEED
    - FanSpeed, FanSpeed_RBV
    - mbbo, mbbi
  * - **Readout control and status**
  * - Andor3ShutterMode
    - asynInt32
    - R/W
    - Selects the shutter mode. Choices for the Neo are:
      |br| 0 = Rolling |br|
      1 = Global |br|
      Rolling shutter mode interleaves exposure and readout. This is the fastest readout
      mode, but means that each row of the sensor acquires data for a different time period.
      In Rolling mode the exposure time is determined by the row readout time, and cannot
      be independently controlled. In Global mode all pixels acquire for the same time
      period and are then read out. The exposure time is independent of the readout time,
      and can be as short as 9 microseconds on the Neo.
    - A3_SHUTTER_MODE
    - A3ShutterMode, A3ShutterMode_RBV
    - mbbo, mbbi
  * - Andor3FrameRate
    - asynFloat64
    - R/W
    - Frame rate. This is the reciprocal of ADAcquirePeriod, and is the units used to
      control the Andor cameras. There are two limitations on the frame rate:
      |br| The maximum rate that the sensor can be read out into the RAM on the camera head.
      The driver will limit the FrameRate to this value in all cases. This value can be
      obtained for the current acquisition settings by typing "asynReport 1" to get a
      detailed report from the Andor3 driver. |br|
      The maximum sustained frame transfer rate that the interface (e.g. CameraLink
      card) can support. This value is contained in the TransferRate PV. It is permissible
      to exceed this value for a limited number of frames, allowing the frames to accumulate
      in the camera head RAM. The number of frames that will fit in the camera head RAM
      is controlled by the PixelEncoding and the AOI parameters (A3Binning, SizeX, SizeY).
      If the camera head RAM fills up the acquisition will hang, and must be stopped and
      restarted. |br|
    - A3_FRAME_RATE
    - FrameRate, FrameRate_RBV
    - ao, ai
  * - Andor3TransferRate
    - asynFloat64
    - R/O
    - The maximum transfer rate in frames/s of the interface (e.g. CameraLink card). This
      is the maximum sustained FrameRate which can be achieved.
    - A3_TRANSFER_RATE
    - TransferRate
    - ai
  * - Andor3PreAmpGain
    - asynInt32
    - R/W
    - Controls the pre-amp gain and readout mode. Choices for the Neo are:
      |br| 0 = 11-bit (high well capacity) |br|
      1 = 11-bit (low noise) |br|
      2 = 16-bit (low noise &amp; high well capacity) |br|
    - A3_PRE_AMP_GAIN
    - PreAmpGain, PreAmpGain_RBV
    - mbbo, mbbi
  * - Andor3PixelEncoding
    - asynInt32
    - R/W
    - Controls the pixel encoding. Choices for the Neo are:
      |br| 0 = Mono12 |br|
      1 = Mono12Packed |br|
      2 = Mono16 |br|
      9 = Mono32 |br|
      Mono12 and Mono12Packed are only available when PreAmpGain is one of the 11-bit
      modes.
    - A3_PIXEL_ENCODING
    - PixelEncoding, PixelEncoding_RBV
    - mbbo, mbbi
  * - Andor3ReadoutRate
    - asynInt32
    - R/W
    - Controls the ADC readout rate. Choices for the Neo are:
      |br| 0 = 100 MHz |br|
      1 = 200 MHz |br|
      2 = 280 MHz |br|
    - A3_READOUT_RATE
    - ReadoutRate, ReadoutRate_RBV
    - mbbo, mbbi
  * - Andor3ReadoutTime
    - asynFloat64
    - R/O
    - The readout time from the sensor into camera RAM.
    - A3_READOUT_TIME
    - ReadoutTime
    - ai
  * - Andor3Overlap
    - asynInt32
    - R/W
    - Controls whether acquisition and readout are overlapped. Choices are 0 (No) and
      1 (Yes). On the Neo if Overlap=Yes then the minimum exposure time is limited to
      about 10 ms. If Overlap=No then the minimum exposure time is 9 microseconds.
    - A3_OVERLAP
    - Overlap, Overlap_RBV
    - bo, bi
  * - Andor3NoiseFilter
    - asynInt32
    - R/W
    - Controls whether the firmware applies the Spurious Noise Filter. Choices are 0 (No)
      and 1 (Yes).
    - A3_NOISE_FILTER
    - NoiseFilter, NoiseFilter_RBV
    - bo, bi
  * - **Trigger control**
  * - Andor3SoftwareTrigger
    - asynInt32
    - R/W
    - Writing 1 to this record generates a software trigger if TriggerMode=Software.
    - A3_SOFTWARE_TRIGGER
    - SoftwareTrigger
    - bo
  * - **Area-Of-Interest (AOI) Control**
  * - Andor3FullAOIControl
    - asynInt32
    - R/O
    - Indicates whether this camera supports full AOI control. This is Yes for the Neo.
    - A3_FULL_AOI_CONTROL
    - FullAOIControl
    - bi
  * - Andor3Binning
    - asynInt32
    - R/W
    - Controls the binning. The binning on the sCMOS camera is done in firmware and only
      certain preset values are allowed. Choices for the Neo are:
      |br| 0 = 1x1 |br|
      1 = 2x2 |br|
      2 = 3x3 |br|
      3 = 4x4 |br|
      4 = 8x8 |br|
    - A3_BINNING
    - A3Binning, A3Binning_RBV
    - mbbo, mbbi
  * - **Gating and Image Intensifier Control**
  * - Andor3GateMode
    - asynInt32
    - R/W
    - Selects the photocathode gating mode. Choices for the iStar are:
      |br| 0 = CWOn |br|
      1 = CWOff |br|
      2 = FireOnly |br|
      3 = GateOnly |br|
      4 = FireAndGate |br|
      5 = DDG |br|
    - A3_GATE_MODE
    - GateMode, GateMode_RBV
    - mbbo, mbbi
  * - Andor3InsertionDelay
    - asynInt32
    - R/W
    - Configures the duration of the gate insertion delay. Choices for the iStar are:
      |br| 0 = Normal |br|
      1 = Fast |br|
      Fast mode is not available in CW gate modes.
    - A3_INSERTION_DELAY
    - InsertionDelay, InsertionDelay_RBV
    - mbbo, mbbi
  * - Andor3MCPGain
    - asynInt32
    - R/W
    - Controls the voltage applied across the microchannel plate. Range for the iStar is 0-4095.
    - A3_MCP_GAIN
    - MCPGain, MCPGain_RBV
    - longout, longin
  * - Andor3MCPIntelligate
    - asynInt32
    - R/W
    - Enables or disables the MCP Intelligate mode. Only available when Normal
      insertion delay is selected. Choices are 0 (Off) and 1 (On).
    - A3_MCP_INTELLIGATE
    - MCPIntelligate, MCPIntelligate_RBV
    - bo, bi

The Andor driver implements the following parameters in addition to
those in asynNDArrayDriver.h and ADDriver.h.

Unsupported standard driver parameters
--------------------------------------

ColorMode, ReverseX, and ReverseY are currently not supported.

Usage
-----

If any of the parameters set are out of range the driver will set them
to the minimum or maximum allowed value for that parameter. The \_RBV
value will indicate the actual value in effect.

Configuration
-------------

The Andor3 driver is created with the andor3Config command, either from
C/C++ or from the EPICS IOC shell.

::

   int andor3Config(const char *portName, const char *cameraSerial,
                   int maxBuffers, size_t maxMemory,
                   int priority, int stackSize, int maxFrames)
     

For details on the meaning of the parameters to this function refer to
the detailed documentation on the andor3Config function in the
`andor3.cpp documentation <../areaDetectorDoxygenHTML/andor3_8cpp.html>`__
and in the documentation for the constructor for the `andor3
class <../areaDetectorDoxygenHTML/classandor3.html>`__. The maxFrames
parameter controls the number of frame buffers the driver queues to the
SDK when acquiring data. The default value is 10. Increasing this number
will allow the SDK to transfer images from the camera at the full
interface speed even when the driver is not reading them that quickly.
This will help to prevent frames from filling the camera RAM when
operating close to the maximum interface transfer rate (=TransferRate).

There an example IOC boot directory and startup script
(`iocBoot/iocAndor3/st.cmd) <andor3_st_cmd.html>`__ provided with
areaDetector.

MEDM screen
-----------

The following shows the MEDM screen that is used to control the Andor
detector. Note that the general purpose screen ADBase.adl can be used,
but it exposes a few controls that are not applicable to the Andor, and
lacks some fields that are important for the Andor.

``Andor3.adl`` is the main screen used to control the Andor driver.

.. container::

   .. rubric:: Andor3.adl
      :name: andor3.adl

   |Andor3.png|

Restrictions
------------

There is currently no way to query the driver to know how much memory
remains in the on-board RAM. When the RAM fills up the camera stops
acquiring images.

.. |Andor3.png| image:: Andor3.png
