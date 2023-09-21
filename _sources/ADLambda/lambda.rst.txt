=======
ADLambda
=======

:author: John Hammonds, Keenan Lang, Mark Rivers


.. _ADLambda: https://areadetector.github.io/master/areaDetectorDoxygenHTML/ADLambda.html
.. _ADLambda class: https://areadetector.github.io/master/areaDetectorDoxygenHTML/classADLambda.html
.. _ADDriver: https://areadetector.github.io/master/ADCore/ADDriver.html
.. _X-Spectrum: https://x-spectrum.de/
.. _areaDetector: https://areadetector.github.io/master/index.html
.. _EPICS: http://www.aps.anl.gov/epics
.. _asynNDArrayDriver: https://areadetector.github.io/master/ADCore/NDArray.html#asynndarraydriver


.. contents:: Contents

Introduction
------------

This is an `EPICS`_ `areaDetector`_ driver for the XSP Lambda detectors
from `X-Spectrum`_. It has been tested on the Lambda 250K, 750K, and 2M.
The driver communicates with the detector via the xsp library.


This driver inherits from `ADDriver`_. It implements many of the
parameters in `asynNDArrayDriver`_ and in `ADDriver`_. It
also implements a number of parameters that are specific to the Lambda
detector. The `ADLambda class`_ describes this class in detail.

This driver was built upon a multithreaded architecture. Each detector
is composed of any number of individual camera modules. An aquisition
thread is generated for each of the detector's modules. Using offsets
listed in the Lambda detector's configuration, the individual modules'
images are stitched into a combined NDArray, which an additional thread
is in charge of exporting to the areaDetector plugin pipeline.


Trigger Modes
-------------

There are three trigger modes available:

* **Internal**: one internal trigger pulse generates one image
* **External_SequencePer**: rising edge on trigger input starts acquisition of all frames
* **External_ImagePer**: 
    for 24 bit depth, a rising edge on trigger input starts acquisition of one or more frames 
    with the programmed shutter time, trigger input is ignored until counter has been read out; 
	
    for all other bit depths, a rising edge on trigger input stops acquisition of current frame 
    and immediately starts acquisition of next frame, the programmed shutter time is ignored in 
    this case


Implementation of standard driver parameters
--------------------------------------------

The following table describes how the Eiger driver implements some of
the standard driver parameters defined in asynNDArrayDriver.h and ADDriver.h,
ADBase.template and NDFile.template

.. cssclass:: table-bordered table-striped table-hover
.. list-table::
  :header-rows: 1
  :widths: 10 80

  * - EPICS record name
    - Description
  * - $(P)$(R)TriggerMode, $(P)$(R)TriggerMode_RBV
    - Sets the trigger mode for the detector. Options are:

      - Internal
      - External_SequencePer
      - External_ImagePer

  * - $(P)$(R)NumImages, $(P)$(R)NumImages_RBV
    - Sets the number of images to take 
  * - $(P)$(R)NumExposures, $(P)$(R)NumExposures_RBV
    - Not Used
  * - $(P)$(R)AcquireTime, $(P)$(R)AcquireTime_RBV
    - Sets the exposure time for a single frame.
  * - $(P)$(R)AcquirePeriod, $(P)$(R)AcquirePeriod_RBV
    - Not Used
  * - $(P)$(R)DataType_RBV
    - DataType is set based on the Lambda's operating mode parameter. When operating
      with a 1- or 6- bit bit-depth, the DataType will be set to UInt8. For 12-bit,
      the DataType is UInt16, and 24-bit is UInt32.
  * - $(P)$(R)TemperatureActual
    - Not Used
  * - $(P)$(R)MaxSizeX_RBV
    - The size of the final, stitched frame in the X direction
  * - $(P)$(R)MaxSizeY_RBV
    - The size of the final, stitched frame in the Y direction
  * - $(P)$(R)ArraySizeX_RBV
    - The size of the final, stitched frame in the X direction
  * - $(P)$(R)ArraySizeY_RBV
    - The size of the final, stitched frame in the Y direction
  * - $(P)$(R)Manufacturer_RBV, $(P)$(R)Model_RBV
    - Set to "X-Spectrum GmbH"
  * - $(P)$(R)SerialNumber_RBV
    - Not Used
  * - $(P)$(R)FirmwareVersion_RBV
    - Detector firmware version
  * - $(P)$(R)SDKVersion_RBV
    - XSP Library version
  * - $(P)$(R)DriverVersion_RBV
    - Not Used

Lambda specific parameters
-------------------------

The Lambda driver implements the following parameters in addition to those in asynNDArrayDriver.h and ADDriver.h.
Most records are in ADLambda.template, but there is a single record listed in LambdaModule.template which should be
loaded for each module that makes up the full detector.

Detector Status
~~~~~~~~~~~~~~~
.. cssclass:: table-bordered table-striped table-hover
.. list-table::
  :header-rows: 1
  :widths: 70 10 10


  * - Description
    - EPICS record name
    - EPICS record type
  * - Bad Frame Counter
      The number of frames recorded as invalid by the detector during the last acquisition
    - BadFrameCounter, BadFrameCounter_RBV
    - ao, ai
  * - Readout Threads
      The current number of threads aquiring from modules
	- ReadoutThreads
	- ai

Acquisition Setup
~~~~~~~~~~~~~~~~~
.. cssclass:: table-bordered table-striped table-hover
.. list-table::
  :header-rows: 1
  :widths: 70 10 10

  * - Description
    - EPICS record name
    - EPICS record type
  * - Dual Threshold Mode, determines whether the detector will use the dual threshold mode.
      In dual threshold mode, the DualThreshold parameter is an upper threshold and the
	  detector will combine the low threshold and high threshold images into a single
	  NDArray frame, the high threshold image being below the low threshold image in the 
	  Y dimension.
	- DualMode, DualMode_RBV
	- mbbo, mbbi
  * - Energy threshold for the acquisition. This is the low energy threshold.
    - EnergyThreshold, EnergyThreshold_RBV
    - ao, ai
  * - Energy threshold 2 for the acquisition. This is the high energy threshold.
    - DualThreshold, DualThreshold_RBV
    - ao, ai
  * - Charge Summing Mode, determines whether the detector will use charge summing
    - ChargeSumming, ChargeSumming_RBV
	- mbbo, mbbi
  * - Gating Mode, determines whether gating is switched on or off
    - GatingMode, GatingMode_RBV
	- mbbo, mbbi

Trigger Setup
~~~~~~~~~~~~~
.. cssclass:: table-bordered table-striped table-hover
.. list-table::
  :header-rows: 1
  :widths: 70 10 10

  * - Description
    - EPICS record name
    - EPICS record type
  * - Bit Depth of image
    - OperatingMode, OperatingMode_RBV
    - mbbo, mbbi

Unsupported standard driver parameters
--------------------------------------

All standard parameters not mentioned in this document are not supported.

Configuration
-------------

The Lambda driver is created with the LambdaConfig command,
either from C/C++ or from the EPICS IOC shell.::

    int LambdaConfig(const char *portName, const char *config, int modules)

For details on the meaning of the parameters to this function refer to
the detailed documentation on the LambdaConfig function in the
`ADLambda`_ and in the documentation for the
constructor for the `ADLambda class`_

There an example IOC boot directory and startup script provided with
areaDetector.

medm screens
------------

The following shows the medm screens that are used to control the Lambda
detector. Note that the general purpose screen ADBase.adl can be used,
but it exposes a few PVs that are not applicable to the Lambda,
and lacks many PVs that are important for the Lambda.

LambdaBase.adl is the screen used to control the Lambda detector.

