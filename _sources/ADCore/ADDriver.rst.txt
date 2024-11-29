ADDriver
~~~~~~~~

ADDriver inherits from asynNDArrayDriver. This is the class from which
area detector drivers are directly derived. It provides parameters and
methods that are specific to area detectors, while asynNDArrayDriver is
a general NDArray driver. The `ADDriver class
documentation <../areaDetectorDoxygenHTML/class_a_d_driver.html>`__ describes
this class in detail.

The file `ADDriver.h <../areaDetectorDoxygenHTML/_a_d_driver_8h.html>`__
defines the parameters that all areaDetector drivers should implement if
possible.

.. cssclass:: table-bordered table-striped table-hover
.. flat-table::
  :header-rows: 2


  * -
    -
    - Parameter Definitions in ADDriver.h and EPICS Record Definitions in ADBase.template  
  * - Parameter index variable
    - asyn interface
    - Access
    - Description
    - drvInfo string
    - EPICS record name
    - EPICS record type
  * -
    -
    -
    - **Information about the detector**
  * - ADMaxSizeX 
    - asynInt32 
    - r/o 
    - Maximum (sensor) size in the X direction 
    - MAX_SIZE_X 
    - $(P)$(R)MaxSizeX_RBV 
    - longin 
  * - ADMaxSizeY 
    - asynInt32 
    - r/o 
    - Maximum (sensor) size in the Y direction 
    - MAX_SIZE_Y 
    - $(P)$(R)MaxSizeY_RBV 
    - longin 
  * - ADTemperature 
    - asynFloat64 
    - r/w 
    - Detector temperature 
    - TEMPERATURE 
    - $(P)$(R)Temperature, $(P)$(R)Temperature_RBV
    - ao, ai 
  * - ADTemperatureActual 
    - asynFloat64 
    - r/o 
    - Actual detector temperature 
    - TEMPERATURE_ACTUAL 
    - $(P)$(R)Temperature_Actual 
    - ai 
  * -
    -
    - **Detector readout control including gain, binning, region start and size, reversal**
  * - ADGain 
    - asynFloat64 
    - r/w 
    - Detector gain 
    - GAIN 
    - $(P)$(R)Gain, $(P)$(R)Gain_RBV 
    - ao, ai 
  * - ADBinX 
    - asynInt32 
    - r/w 
    - Binning in the X direction 
    - BIN_X 
    - $(P)$(R)BinX, $(P)$(R)BinX_RBV 
    - longout, longin 
  * - ADBinY 
    - asynInt32 
    - r/w 
    - Binning in the Y direction 
    - BIN_Y 
    - $(P)$(R)BinY, $(P)$(R)BinY_RBV 
    - longout, longin 
  * - ADMinX 
    - asynInt32 
    - r/w 
    - First pixel to read in the X direction. 0 is the first pixel on the detector. 
    - MIN_X 
    - $(P)$(R)MinX, $(P)$(R)MinX_RBV 
    - longout, longin 
  * - ADMinY 
    - asynInt32 
    - r/w 
    - First pixel to read in the Y direction. 0 is the first pixel on the detector. 
    - MIN_Y 
    - $(P)$(R)MinY, $(P)$(R)MinY_RBV 
    - longout, longin
  * - ADSizeX 
    - asynInt32 
    - r/w 
    - Size of the region to read in the X direction 
    - SIZE_X 
    - $(P)$(R)SizeX, $(P)$(R)SizeX_RBV 
    - longout, longin 
  * - ADSizeY 
    - asynInt32 
    - r/w 
    - Size of the region to read in the Y direction 
    - SIZE_Y 
    - $(P)$(R)SizeY, $(P)$(R)SizeY_RBV 
    - longout, longin 
  * - ADReverseX 
    - asynInt32 
    - r/w 
    - Reverse array in the X direction, (0=No, 1=Yes) 
    - REVERSE_X 
    - $(P)$(R)ReverseX, $(P)$(R)ReverseX_RBV 
    - longout, longin 
  * - ADReverseY 
    - asynInt32 
    - r/w 
    - Reverse array in the Y direction, (0=No, 1=Yes) 
    - REVERSE_Y 
    - $(P)$(R)ReverseY, $(P)$(R)ReverseY_RBV 
    - longout, longin 
  * -
    -
    -
    - **Image and trigger modes** 
  * - ADImageMode 
    - asynInt32 
    - r/w 
    - Image mode (ADImageMode_t). 
    - IMAGE_MODE 
    - $(P)$(R)ImageMode, $(P)$(R)ImageMode_RBV 
    - mbbo, mbbi 
  * - ADTriggerMode 
    - asynInt32 
    - r/w 
    - Trigger mode (ADTriggerMode_t). 
    - TRIGGER_MODE 
    - $(P)$(R)TriggerMode, $(P)$(R)TriggerMode_RBV 
    - mbbo, mbbi 
  * -
    -
    -
    - **Frame type** 
  * - ADFrameType 
    - asynInt32 
    - r/w 
    - Frame type (ADFrameType_t). 
    - FRAME_TYPE 
    - $(P)$(R)FrameType, $(P)$(R)FrameType_RBV 
    - mbbo, mbbi 
  * -
    -
    -
    - **Acquisition time and period**
  * - ADAcquireTime 
    - asynFloat64 
    - r/w 
    - Acquisition time per image 
    - ACQ_TIME 
    - $(P)$(R)AcquireTime, $(P)$(R)AcquireTime_RBV 
    - ao, ai 
  * - ADAcquirePeriod 
    - asynFloat64 
    - r/w 
    - Acquisition period between images 
    - ACQ_PERIOD 
    - $(P)$(R)AcquirePeriod, $(P)$(R)AcquirePeriod_RBV 
    - ao, ai 
  * -
    -
    -
    - **Number of exposures and number of images**
  * - ADNumExposures 
    - asynInt32 
    - r/w 
    - Number of exposures per image to acquire 
    - NEXPOSURES 
    - $(P)$(R)NumExposures, $(P)$(R)NumExposures_RBV 
    - longout, longin 
  * - ADNumImages 
    - asynInt32 
    - r/w 
    - Number of images to acquire in one acquisition sequence 
    - NIMAGES 
    - $(P)$(R)NumImages, $(P)$(R)NumImages_RBV 
    - longout, longin 
  * -
    -
    -
    - **Status information** 
  * - ADStatus 
    - asynInt32 
    - r/o 
    - Acquisition status (ADStatus_t) 
    - STATUS 
    - $(P)$(R)DetectorState_RBV 
    - mbbi 
  * - ADStatusMessage 
    - asynOctet 
    - r/o 
    - Status message string 
    - STATUS_MESSAGE 
    - $(P)$(R)StatusMessage_RBV 
    - waveform 
  * - ADStringToServer 
    - asynOctet 
    - r/o 
    - String from driver to string-based vendor server 
    - STRING_TO_SERVER 
    - $(P)$(R)StringToServer_RBV 
    - waveform 
  * - ADStringFromServer 
    - asynOctet 
    - r/o 
    - String from string-based vendor server to driver 
    - STRING_FROM_SERVER 
    - $(P)$(R)StringFromServer_RBV 
    - waveform 
  * - ADNumExposuresCounter 
    - asynInt32 
    - r/o 
    - Counter that increments by 1 each time an exposure is acquired for the current image.
      Driver resets to 0 when acquisition is started. 
    - NUM_EXPOSURES_COUNTER 
    - $(P)$(R)NumExposuresCounter_RBV 
    - longin 
  * - ADNumImagesCounter 
    - asynInt32 
    - r/o 
    - Counter that increments by 1 each time an image is acquired in the current acquisition
      sequence. Driver resets to 0 when acquisition is started. Drivers can use this as
      the loop counter when ADImageMode=ADImageMultiple. 
    - NUM_IMAGES_COUNTER 
    - $(P)$(R)NumImagesCounter_RBV 
    - longin 
  * - ADTimeRemaining 
    - asynFloat64 
    - r/o 
    - Time remaining for current image. Drivers should update this value if they are doing
      the exposure timing internally, rather than in the detector hardware. 
    - TIME_REMAINING 
    - $(P)$(R)TimeRemaining_RBV 
    - ai 
  * - ADReadStatus 
    - asynInt32 
    - r/w 
    - Write a 1 to this parameter to force a read of the detector status. Detector drivers
      normally read the status as required, so this is usually not necessary, but there
      may be some circumstances under which forcing a status read may be needed. 
    - READ_STATUS 
    - $(P)$(R)ReadStatus 
    - bo 
  * -
    -
    -
    - **Shutter control** 
  * - ADShutterMode 
    - asynInt32 
    - r/w 
    - Shutter mode (None, detector-controlled or EPICS-controlled) (ADShutterMode_t)
    - SHUTTER_MODE 
    - $(P)$(R)ShutterMode, $(P)$(R)ShutterMode_RBV 
    - mbbo, mbbi 
  * - ADShutterControl 
    - asynInt32 
    - r/w 
    - Shutter control for the selected (detector or EPICS) shutter (ADShutterStatus_t)
    - SHUTTER_CONTROL 
    - $(P)$(R)ShutterControl, $(P)$(R)ShutterControl_RBV 
    - bo, bi
  * - ADShutterControlEPICS 
    - asynInt32 
    - r/w 
    - This record processes when it receives a callback from the driver to open or close
      the EPICS shutter. It triggers the records below to actually open or close the EPICS
      shutter. 
    - SHUTTER_CONTROL_EPICS 
    - $(P)$(R)ShutterControlEPICS 
    - bi 
  * - N/A 
    - N/A 
    - r/w 
    - This record writes its OVAL field to its OUT field when the EPICS shutter is told
      to open. The OCAL (and hence OVAL) and OUT fields are user-configurable, so any
      EPICS-controllable shutter can be used. 
    - N/A 
    - $(P)$(R)ShutterOpenEPICS 
    - calcout 
  * - N/A 
    - N/A 
    - r/w 
    - This record writes its OVAL field to its OUT field when the EPICS shutter is told
      to close. The OCAL (and hence OVAL) and OUT fields are user-configurable, so any
      EPICS-controllable shutter can be used. 
    - N/A 
    - $(P)$(R)ShutterCloseEPICS 
    - calcout 
  * - ADShutterStatus 
    - asynInt32 
    - r/o 
    - Status of the detector-controlled shutter (ADShutterStatus_t) 
    - SHUTTER_STATUS 
    - $(P)$(R)ShutterStatus_RBV 
    - bi 
  * - N/A 
    - N/A 
    - r/o 
    - Status of the EPICS-controlled shutter. This record should have its input link (INP)
      set to a record that contains the open/close status information for the shutter.
      The link should have the "CP" attribute, so this record processes when the input
      changes. The ZRVL field should be set to the value of the input link when the shutter
      is closed, and the ONVL field should be set to the value of the input link when
      the shutter is open. 
    - N/A 
    - $(P)$(R)ShutterStatusEPICS_RBV 
    - mbbi 
  * - ADShutterOpenDelay 
    - asynFloat64 
    - r/w 
    - Time required for the shutter to actually open 
    - SHUTTER_OPEN_DELAY 
    - $(P)$(R)ShutterOpenDelay, $(P)$(R)ShutterOpenDelay_RBV 
    - ao, ai 
  * - ADShutterCloseDelay 
    - asynFloat64 
    - r/w 
    - Time required for the shutter to actually close 
    - SHUTTER_CLOSE_DELAY 
    - $(P)$(R)ShutterCloseDelay, $(P)$(R)ShutterCloseDelay_RBV 
    - ao, ai 
