ADLambda
==========================

:author:  John Hammonds (Argonne National Laboratory)


Table Of Contents
-----------------

.. contents:: Contents

Introduction
------------

This is an `EPICS <https://www.aps.anl.gov/epics>`__
`areaDetector <https://cars.uchicago.edu/software/epics/areaDetector.html>`__
driver for the Lambda detector from
`X-spectrum <http://www.x-spectrum.de>`__

The interface to the detector is provided by a TCP/UDP/IP socket
interface to the detector. X-Spectrum provides a library to handle
communications and acquisition in the camera. This driver provides an
interface layer between the vendor library and EPICS areaDetector.

The hardware interface between the camera and the controlling computer
consists of

-  A single 1GB copper network link used for command/control
-  A pair of 10GB Fiber network links for sending image data

This detector is capable of acquiring up to 2000 images/sec. It may be
necessary, therefore, to adapt standard areaDetector plugins for faster
collection to ensure processing of all images.

Implementation of standard driver parameters
--------------------------------------------

The following table describes how the Lambda driver implements some of
the standard driver parameters.

.. cssclass:: table-bordered table-striped table-hover
.. list-table::
  :header-rows: 1
   
  * - EPICS record name
    - Description
  * - AcquireTime
    - Controls the exposure time of the camera in seconds.
  * - AcquirePeriod
    - NOT YET IMPLEMENTED IN FIRMWARE.  Controls the period
      between exposures in internal trigger mode.  Currently this
      feature is not supported by the camera firmware but this
      is a planned feature.  Once the firmware supports this feature,
      the software should support this.
  * - TriggerMode
    - Default descriptions are overloaded.  Allows four modes:      
      
      * Internal
      * External_SequencePer - each external pulse runs an **image
        sequence** per pulse
      * External_ImagePer - each external pulse runs an **image** per pulse
      * Gating_Mode - Trigger an image that is active while the pulse is high
  * - Acquire
    -  Controls starting and stopping camera images.
  * - NumImages
    - In Multiple Image mode, this controls the number of images
      to be collected.
  * - ImageMode
    - Sets selection of Single or multiple Images when acquire
      button is pressed.  Acquisition on the Lambda is controlled
      by setting a defined number of images.  "Continuous" has been
      removed from the options.

Lambda specific parameters
--------------------------------------------------------------------------------

Parameter Definition in ADLambda.cpp and record definitions in ADLambda.template

.. cssclass:: table-bordered table-striped table-hover
.. list-table::
  :header-rows: 1
  
  * - Parameter index variable
    - asyn interface
    - Access
    - Description
    - drvInfo string
    - EPICS record name
    - EPICS record type
  * - LAMBDA_ConfigFilePath
    - asynOctet
    - r/w
    - Displays the full path to the detector's configueationFile
    - LAMBDA_CONFIG_FILE_PATH
    - ConfigFilePath
    - waveform
  * - LAMBDA_EnergyThreshold
    - asynFloat64
    - r/w
    - Sets the minimum energy that should register a valid event.
    - LAMBDA_ENERGY_THRESHOLD
    - EnergyThreshold
    - ao
  * - LAMBDA_DecodedQueuePath
    - asynInt32
    - r
    - Displays how far into the decoded queue we have reached.
    - LAMBDA_DECODED_QUEUE_DEPTH
    - DecodedQueueDepth
    - longin
  * - LAMBDA_BadFrameCounter
    - asynInt32
    - r
    - Counts the number of bad frames detected during decoding. 
      images are sent as a number of packets, rotating over 4 network ports. 
      If the packets for the next image are received on all 4 ports, before
      the finish of an image, then the frame is counted as a bad frame.
    - LAMBDA_BAD_FRAME_COUNTER
    - BadFrameCounter
    - longin
  * - LAMBDA_OperatingMode
    - asynInt32
    - r/w
    - Switches between 12 and 24 bit counting mode.  These are 
      referenced in pulldown as
 
      * ContinuousReadWrite - Switches to 12-bit mode and sets image 
        data type to UInt16.
      * TwentyFourBit - Switches to 24-bit mode also sets image
        data type to UInt32.
      
      Note that it is possible in each mode to choose any Integer data 
      type
    - LAMBDA_OPERATING_MODE
    - OperatingMode
      
      OperatingMode_RBV
    - mbbo
      
      mbbi
  * - LAMBDA_DetectorState
    - asynInt32
    - r
    - Displays the current state of the image processing of the
      detector.
      
      * ON - Normal idle state. 
      * DISABLE 
      * BUSY - Performing non-imaging task such as loading config file. 
      * FAULT 
      * RECEIVING_IMAGES - Detector is actively taking images 
        and placing them in the raw buffer to be decoded.
      * PROCSSING_IMAGES - The detector is finished collecting images
        and has placed the in the raw buffer to be decoded.  The decoding 
        process is still working on reconstructing packets for images in
        the raw buffer and placing them in the decoded buffer 
      * FINISHED - All images have been received and decoded. The raw
        buffer is empty.  This signal will remain in this state until 
        StopAcquisition is called.  Once StopAcquisition is called, then 
        this signal will return to ON.
      
    - LAMBDA_DETECTOR_STATE
    - LambdaState
    - mbbi


Configuration
-------------

The Lambda detector instance is created using the LambdaConfig command,
either from C/C++ or from the EPICS IOC shell.

::

     int LambdaConfig(const char *portName, const char* configPath, 
             int maxBuffers, size_t maxMemory, 
             int priority, int stackSize)

For details on the meaning of the parameters to this function refer to
the documentation on the LambdaConfig function in the `Lambda.cpp
documentation <areaDetectorDoxygenHTML/ADLambda_8cpp.html>`__

and in the documentation for the constructor in the `ADLambda
class <areaDetectorDoxygenHTML/class_ADLambda.html>`__)

MEDM screens
------------

Performance measurements
------------------------

The Lambda detector itself is rated at 2000 frames/sec. This rate has
been achieved simply processing the detector images into NDArrays. We
have been able to save up to 2000 images/sec for a period of time using
a PipeWriterPlugin that writes to a Unix Pipe which feeds an external
MPI program which compresses the image data and writes to a binary file
with occasional bad frame errors. Sometimes the detector seems to
persist giving bad frames once it gets started and the detector has to
be reset. Sometimes we lose communication with the detector through the
1GB command interface. When this happens we have to power down the
detector for about 10 minutes before it will come on again.

Hardware Notes
--------------

This detector is capable of supporting a number of features which are
not currently available in the detector driver & firmware. These
features will be added as they are made available by the vendor.

Restrictions
------------

To be added later...
