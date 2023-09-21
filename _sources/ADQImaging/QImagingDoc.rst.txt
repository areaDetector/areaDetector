ADQImaging
==========

:author: Arthur Glowacki (Argonne National Lab)


Table of Contents
-----------------

.. contents:: Contents

Introduction
------------

This is an `EPICS <http://www.aps.anl.gov/epics>`__
`areaDetector <areaDetector.html>`__ driver for CMOS detectors from
`QImaging Technology <http://www.qimaging.com/>`__. It has been tested
on the RETIGA 4000DC camera, but should work with other cameras as well.
The driver is supported 32-bit and 64-bit Windows.

The driver provides access to essentially all of the features of the
QImaging cameras:

-  Fixed number of frames or continuous acquisition.
-  Multiple accumulations per frame.
-  Readout frequency
-  Readout mode
-  Support for all of the QImaging trigger modes
-  Binning and Area Of Interest (AOI) readout
-  Set and monitor the camera temperature

This driver inherits from `ADDriver <areaDetectorDoc.html#ADDriver>`__.
It implements many of the parameters in
`asynNDArrayDriver.h <../areaDetectorDoxygenHTML/asyn_n_d_array_driver_8h.html>`__
and in
`ADArrayDriver.h <../areaDetectorDoxygenHTML/_a_d_driver_8h.html>`__. It
also implements a number of parameters that are specific to the QImaging
detectors.

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

Implementation of standard driver parameters
--------------------------------------------

The following table describes how the QImaging driver implements some of
the standard driver parameters.

.. cssclass:: table-bordered table-striped table-hover
.. flat-table::
  :header-rows: 2
  
  * - **Implementation of Parameters in asynNDArrayDriver.h and ADDriver.h, and EPICS Record
      Definitions in ADBase.template and NDFile.template**
  * - Parameter index variable
    - EPICS record name
    - Description
  * - ADTriggerMode
    - $(P)$(R)TriggerMode, $(P)$(R)TriggerMode_RBV
    - Sets the trigger mode for the detector. Options for the Neo are:
      Internal

      * Software 
      * External 
      * External Start
      * External Exposure 
      * Advanced

  * - ADImageMode
    - $(P)$(R)ImageMode, $(P)$(R)ImageMode_RBV
    - Sets the image mode for the detector. Options are:
      
      * Fixed
      * Continuous

      Fixed counts collects NumImages images, continuous acquires continuously until acquisition
      is stopped.
  * - ADNumExposures
    - $(P)$(R)NumExposures, $(P)$(R)NumExposures_RBV
    - Sets the number of accumulations per image.
  * - ADNumImages
    - $(P)$(R)NumImages, $(P)$(R)NumImages_RBV
    - Sets the number of images to take when ImageMode=Fixed.
  * - ADAcquirePeriod
    - $(P)$(R)AcquirePeriod, $(P)$(R)AcquirePeriod_RBV
    - Sets the FrameRate (=1./AcquirePeriod). The FrameRate can also be set directly.
      The minimum AcquirePeriod (maximum FrameRate) depends on a number of other parameters,
      including the A3Binning, SizeX, SizeY, ReadoutRate, A3ShutterMode, PixelEncoding,
      and Overlap.
  * - NDDataType
    - $(P)$(R)DataType, $(P)$(R)DataType_RBV
    - The data type for the image data. Allowed values are:
      
      * UInt16
      * UInt32
      
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

QImaging specific parameters
----------------------------

The QImaging driver implements the following parameters in addition to
those in asynNDArrayDriver.h and ADDriver.h.

.. cssclass:: table-bordered table-striped table-hover
.. flat-table::
  :header-rows: 2
  
  * -  **Parameter Definitions in QImaging.h and EPICS Record Definitions in qimaging.template**
  * - Parameter index variable
    - asyn interface
    - Access
    - Description
    - drvInfo string
    - EPICS record name
    - EPICS record type
  * -  **Version information**
  * - qSerialNumberRBVString
    - asynOctet
    - R/O
    - Camera serial number
    - SERIAL_NUMBER_RBV
    - SerialNumber
    - stringin
  * - qMaxBitDepthRBVString
    - asynInt32
    - R/O
    - Max bit depth
    - MAX_BIT_DEPTH_RBV
    - MaxBitDepth
    - stringin
  * - qUniqueIdRBVString
    - asynOctet
    - R/O
    - Unique ID
    - UNIQUE_ID_RBV
    - UniqueId
    - stringin
  * - qCcdTypeRBVString
    - asynOctet
    - R/O
    - CCD Type
    - CCD_TYPE_RBV
    - CCDType
    - stringin
  * - **Cooling control and status**
  * - qCooledRBVString
    - asynInt32
    - R/W
    - Turn the sensor cooler on and off
    - COOLED_RBV
    - Cooled, Cooled_RBV
    - bo, bi
  * - qRegulatedCoolingRBVString
    - asynInt32
    - R/O
    - Regulated cooling
    - REGULATED_COOLING_RBV
    - RegulatedCooling_RBV
    - bo, bi
  * - qFanControlRBVString
    - asynInt32
    - R/W
    - Fan Control
    - FAN_CONTROL_RBV
    - FanControl
    - mbbo, mbbi
  * - qHighSensitivityModeRBVString
    - asynInt32
    - R/W
    - High sensitivity mode
    - HIGH_SENSITIVITY_MODE_RBV
    - HighSensitivityMode_RBV
    - mbbo, mbbi
  * - **Readout control and status**
  * - qBlackoutModeRBVString
    - asynOctet
    - R/O
    - Blackout mode
    - BLACK_OUT_MODE_RBV
    - BlackOutMode_RBV
    - stringin
  * - qAsymmetricalBinningRBVString
    - asynFloat64
    - R/W
    -
    - ASYMMETRICAL_BINNING_RBV
    - AsymetricalBinning_RBV
    - ao, ai
  * - qCoolerActiveString
    - asynFloat64
    - R/O
    -
    - COOLER_ACTIVE
    - CoolerActive
    - ai
  * - qReadoutSpeedString
    - asynInt32
    - R/W
    -
    - READOUT_SPEED
    - ReadoutSpeed, ReadoutSpeed_RBV
    - mbbo, mbbi
  * - qOffsetString
    - asynInt32
    - R/W
    -
    - OFFSET
    - Offset, Offset_RBV
    - mbbo, mbbi
  * - qImageFormatString
    - asynInt32
    - R/W
    -
    - IMAGE_FORMAT
    - ImageFormat, ImageFormat_RBV
    - mbbo, mbbi
  * - qAcquireTimeRBVString
    - asynFloat64
    - R/O
    -
    - ACQUIRE_TIME_RBV
    - AquireTime_RBV
    - ai
  * - qMinXRBVString
    - asynInt32
    - R/W
    -
    - MIN_X_RBV
    - MinX_RBV
    - bo, bi
  * - qMinYRBVString
    - asynInt32
    - R/W
    -
    - MIN_Y_RBV
    - MinY_RBV
    - bo, bi
  * - **Trigger control**
  * - qSizeXRBVString
    - asynInt32
    - R/W
    -
    - SIZE_X_RBV
    - SizeX_RBV
    - bo
  * - **Area-Of-Interest (AOI) Control**
  * - qSizeYRBVString
    - asynInt32
    - R/O
    -
    - SIZE_Y_RBV
    - SizeY_RBV
    - bi
  * - qTriggerModeRBVString
    - asynInt32
    - R/W
    -
    - TRIGGER_MODE_RBV
    - TriggerMode_RBV
    - mbbo, mbbi

Usage
-----

If any of the parameters set are out of range the driver will set them
to the minimum or maximum allowed value for that parameter. The \_RBV
value will indicate the actual value in effect.

Configuration
-------------

The QImaging driver is created with the QImageConfig command, either
from C/C++ or from the EPICS IOC shell.

::

    int QImageConfig(const char *portName, int cameraId,
                    int maxBuffers, size_t maxMemory,
                    int priority, int stackSize, int maxFrames)
      

Screenshots
-----------

qimage_base.adl
~~~~~~~~~~~~~~~

The following shows the MEDM screen that is used to control the QImage
detector.

``qimage_base.adl`` is the main screen used to control the QImaging
driver.

.. image:: QImage_MEDM.png
    :width: 75%
    :align: center

