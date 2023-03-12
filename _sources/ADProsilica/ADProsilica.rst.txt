ADProsilica
===========

:author: Mark Rivers, University of Chicago

.. contents:: Contents

Overview
--------

This is an :doc:`../index` driver for Gigabit Ethernet and
Firewire cameras from `Allied Vision
Technologies <https://www.alliedvision.com>`__, who purchased Prosilica.
The driver is supported under Windows, Linux and Mac OS X using the
vendor library provided for those operating systems.

This driver inherits from :doc:`../ADCore/ADDriver`.
It implements nearly all of the parameters in
`asynNDArrayDriver.h <../areaDetectorDoxygenHTML/asyn_n_d_array_driver_8h.html>`__
and in
`ADArrayDriver.h <../areaDetectorDoxygenHTML/_a_d_driver_8h.html>`__. It
also implements a number of parameters that are specific to the
Prosilica cameras. The `prosilica class
documentation <../areaDetectorDoxygenHTML/classprosilica.html>`__ describes
this class in detail.

.. _StandardNotes:

Implementation of standard driver parameters
--------------------------------------------

The driver redefines the choices for several of the parameters defined
in ``ADDriver.h``. The ADTriggerMode choices for the Prosilica are:

-  Free Run (collects images as fast as allowed by the camera readout
   and the exposure time)
-  Sync In 1 (external trigger on line 1)
-  Sync In 2 (external trigger on line 2)
-  Sync In 3 (external trigger on line 3)
-  Sync In 4 (external trigger on line 4)
-  Fixed Rate (collects images at a fixed rate)
-  Software (single software trigger)

The Prosilica supports hardware timing input and output signals that are
supported in the driver.

The NDDataType choices for the Prosilica are:

-  NDUInt8 (8-bit data)
-  NDUInt16 (12 or 16 bit data)

The NDColorMode choices for the Prosilica are:

-  NDColorModeMono (monochromatic data)
-  NDColorModeBayer (raw Bayer data)
-  NDColorModeRGB1 (RGB1 data)

The color Prosilica cameras are also capable of various YUV color
formats but these are not supported in the driver. They may be added in
a future release.

Prosilica specific parameters
-----------------------------

The Prosilica driver implements the following parameters in addition to
those in asynNDArrayDriver.h and ADDriver.h:

.. cssclass:: table-bordered table-striped table-hover
.. flat-table::
  :header-rows: 2
  :widths: 60 20 20

  * - Parameter Definitions in prosilica.cpp and EPICS Record Definitions in prosilica.template
  * - Description
    - EPICS record name
    - EPICS record type
  * - **Bayer Color Conversion**
  * - Color conversion when NDColorMode is Bayer.  Choices are:

      - None: Raw Bayer images are passed to the plugins
      - RGB1: Bayer images are converted to RGB1
      - RGB2: Bayer images are converted to RGB2
      - RGB3: Bayer images are converted to RGB3
      
      Having the camera send Bayer images uses 3 times less network bandwidth than 
      sending RGB1 images.  It does place more CPU load on the host to convert
      from Bayer to RGB, but this is often an acceptable tradeoff.
    - $(P)$(R)BayerConvert, $(P)$(R)BayerConvert_RBV
    - mbbo, mbbi
  * - **Trigger and I/O Control**
  * - The edge or level for the selected trigger signal when ADTriggerMode=Sync In 1 to
      SyncIn 4. Allowed values are:, Rising edge, Falling edge, Any edge, High level, Low level
    - $(P)$(R)TriggerEvent, $(P)$(R)TriggerEvent_RBV
    - mbbo, mbbi
  * - The delay in seconds between the trigger signal and when the frame is actually acquired.
      Minimum value is 1 microsecond.
    - $(P)$(R)TriggerDelay, $(P)$(R)TriggerDelay_RBV
    - ao, ai
  * - Controls the behavior when an external trigger signal arrives before the camera
      is ready for the next trigger. Allowed values are:, Off - the external trigger is ignored, Previous frame - the external trigger is latched and triggers the next frame when
      the current frame completes
    - $(P)$(R)TriggerOverlap, $(P)$(R)TriggerOverlap_RBV
    - mbbo, mbbi
  * - Processing this record performs a software trigger if ADTriggerMode=Software.
    - $(P)$(R)TriggerSoftware
    - bo
  * - The level of the Sync In 1 signal
    - $(P)$(R)SyncIn1Level_RBV
    - bi
  * - The level of the Sync In 2 signal
    - $(P)$(R)SyncIn2Level_RBV
    - bi
  * - The mode of the Sync Out 1 signal. Allowed values are:

      - GPO (general purpose output)
      - AcqTrigReady
      - FrameTrigReady
      - FrameTrigger
      - Exposing
      - FrameReadout
      - Imaging
      - Acquiring
      - SyncIn1
      - SyncIn2
      - SyncIn3
      - SyncIn4
      - Strobe1
      - Strobe2
      - Strobe3
      - Strobe4
    - $(P)$(R)SyncOut1Mode, $(P)$(R)SyncOut1Mode_RBV
    - mbbo, mbbi
  * - The level of the Sync Out 1 signal. This is only programmable when SyncOut1Mode=GPO.
    - $(P)$(R)SyncOut1Level, $(P)$(R)SyncOut1Level_RBV
    - bo, bi
  * - Flag to invert the Sync Out 1 signal.
    - $(P)$(R)SyncOut1Invert, $(P)$(R)SyncOut1Invert_RBV
    - bo, bi
  * - The mode of the Sync Out 2 signal. Allowed values are the same as for PSSyncOut1Mode.
    - $(P)$(R)SyncOut2Mode, $(P)$(R)SyncOut2Mode_RBV
    - mbbo, mbbi
  * - The level of the Sync Out 2 signal. This is only programmable when SyncOut2Mode=GPO.
    - $(P)$(R)SyncOut2Level, $(P)$(R)SyncOut1Level_RBV
    - bo, bi
  * - Flag to invert the Sync Out 2 signal.
    - $(P)$(R)SyncOut2Invert, $(P)$(R)SyncOut2Invert_RBV
    - bo, bi
  * - The mode of the Sync Out 3 signal. Allowed values are the same as for PSSyncOut1Mode.
    - $(P)$(R)SyncOut3Mode, $(P)$(R)SyncOut3Mode_RBV
    - mbbo, mbbi
  * - The level of the Sync Out 3 signal. This is only programmable when SyncOut3Mode=GPO.
    - $(P)$(R)SyncOut3Level, $(P)$(R)SyncOut3Level_RBV
    - bo, bi
  * - Flag to invert the Sync Out 3 signal.
    - $(P)$(R)SyncOut3Invert, $(P)$(R)SyncOut3Invert_RBV
    - bo, bi
  * - The mode of the Strobe 1 signal. The Strobe signals are based on the following values,
      but allow for changing the delay and width relative to the underlying value. Any
      of the outputs can be set to the Stobe1 value, rather than the raw values of these
      signals. Allowed values are:

      - AcqTrigReady
      - FrameTrigReady
      - FrameTrigger
      - Exposing
      - FrameReadout
      - Acquiring
      - SyncIn1
      - SyncIn2
      - SyncIn3
      - SyncIn4
    - $(P)$(R)Strobe1Mode, $(P)$(R)Strobe1Mode_RBV
    - mbbo, mbbi
  * - Flag to allow controlling the strobe duration.
    - $(P)$(R)Strobe1CtlDuration, $(P)$(R)Strobe1CtlDuration_RBV
    - bo, bi
  * - The strobe duration if PSStrobe1CtlDuration is On.
    - $(P)$(R)Strobe1Duration, $(P)$(R)Strobe1Duration_RBV
    - ao, ai
  * - The strobe delay relative to the underlying signal that the strobe is based on.
    - $(P)$(R)Strobe1Delay, $(P)$(R)Strobe1Delay_RBV
    - ao, ai
  * - **Timestamp Control**
  * - Resets the timestamp timer in the camera. If PSTimestampType is POSIX or EPICS then
      it also stores the current POSIX or EPICS time in the driver.
    - $(P)$(R)PSResetTimer
    - longout
  * - Controls the type of timestamp in the timeStamp field of each NDArray. Choices are:
      
      - NativeTicks: The number of internal camera clock ticks which have elapsed since the last timer reset.
      - NativeSeconds: The number of seconds which have elapsed since the last timer reset.
        This is NativeTicks divided by the internal camera clock frequency.
      - POSIX: The number of seconds since the POSIX Epoch (00:00:00 UTC, January 1, 1970).
      - EPICS: The number of seconds since the EPICS Epoch (January 1, 1990).
      - IOC: The number of seconds since the EPICS Epoch (January 1, 1990).

      The POSIX and EPICS timestamps are calculated as follows: when the timer is reset
      the current POSIX or EPICS time is stored, and the internal camera timer is reset.
      The timestamps are then computed by adding the camera ticks (converted to seconds)
      to the stored POSIX or EPICS time. Thus, the relative times of each frame are accurately
      controlled by the internal camera clock. The accuracy of the absolute time is determined
      by the accuracy of the time of day clock in the IOC computer.
      
      The IOC timestamp is simply the time returned by epicsTimeGetCurrent(), and does not
      use the camera tick clock at all.

    - $(P)$(R)PSTimestampType, $(P)$(R)PSTimestampType_RBV
    - mbbo, mbbi
  * - **Statistics Information**
  * - Read the Gigabit Ethernet statistics when 1
    - $(P)$(R)PSReadStatistics
    - longout
  * - Driver type
    - $(P)$(R)PSDriverType_RBV
    - stringin
  * - Packet filter version
    - $(P)$(R)PSFilterVersion_RBV
    - stringin
  * - Frame rate (Hz)
    - $(P)$(R)PSFrameRate_RBV
    - ai
  * - Stream bytes per second in the PvAPI driver. This allows limiting the bandwidth
      that a camera uses. It also allows operation of GigE cameras on non-Gigabit Ethernet
      networks by decreasing the value to maximum that the network supports. The default
      of 115000000 allows full-speed operation on GigE networks.
    - $(P)$(R)PSByteRate, $(P)$(R)PSByteRate_RBV
    - longout, longin
  * - Actual packet size of Ethernet packets. When connecting to the camera the driver
      always automatically negotiates the largest packet size that the camera and IOC
      computer support.
    - $(P)$(R)PSPacketSize_RBV
    - longin
  * - Number of frames completed
    - $(P)$(R)PSFramesCompleted_RBV
    - longin
  * - Number of frames dropped
    - $(P)$(R)PSFramesDropped_RBV
    - longin
  * - Number of erroneous packets
    - $(P)$(R)PSPacketsErroneous_RBV
    - longin
  * - Number of missed packets
    - $(P)$(R)PSPacketsMissed_RBV
    - longin
  * - Number of received packets
    - $(P)$(R)PSPacketsReceived_RBV
    - longin
  * - Number of packets requested
    - $(P)$(R)PSPacketsRequested_RBV
    - longin
  * - Number of packets resent
    - $(P)$(R)PSPacketsResent_RBV
    - longin
  * - Number of bad frames
    - $(P)$(R)PSBadFrameCounter_RBV
    - longin

Configuration
-------------

The Prosilica driver is created with the ``prosilicaConfig`` command, either
from C/C++ or from the EPICS IOC shell.

.. code-block:: c

   int prosilicaConfig(char *portName,
                       const char* cameraId,
                       int maxBuffers, size_t maxMemory,
                       int priority, int stackSize, int maxPvAPIFrames)
     

The **cameraId** string can be any of the following:

-  The camera's UniqueId, which is a number assigned by the vendor to
   each Prosilica camera, e.g. 50110.
-  The camera's IP address, e.g. 164.54.160.48.
-  The camera's IP DNS name, e.g. gse-prosilica1.cars.aps.anl.gov.

Using the UniqueId has the advantage that the cameras can be configured
to use DHCP, and hence have non-predictable TCP/IP addresses. However,
if the UniqueId is used then the areaDetector IOC must be on the same
subnet as the camera, since cameras cannot be found by UniqueID through
routers. The simplest way to determine the uniqueId of a camera is to
run the Prosilica GigEViewer application, select the camera, and press
the "i" icon on the bottom of the main window to show the camera
information for this camera. The Unique ID will be displayed on the
first line in the information window.

The IP address or IP DNS name can be used for cameras with fixed IP
addresses, and **must** be used for cameras that are not on the local
subnet.

The maxPvAPIFrames parameter controls how many frame buffers will be
used by the PvAPI library. This is the last parameter in the
prosilicaConfig command, and if it is absent the default value of 2 is
used, which is sufficient in most circumstances. However, with very high
frame rates or busy IOCs increasing this value can reduce dropped
frames.

For details on the meaning of the other parameters to this function
refer to the detailed documentation on the prosilicaConfig function in
the `prosilica.cpp
documentation <../areaDetectorDoxygenHTML/prosilica_8cpp.html>`__ and in
the documentation for the constructor for the `prosilica
class <../areaDetectorDoxygenHTML/classprosilica.html>`__.

Example st.cmd startup file
---------------------------

.. toctree::
   :hidden:
   
   st_cmd.rst

There is an example IOC boot directory and startup script
:ref:`st.cmd <ADProsilica_st_cmd_example>` provided with
areaDetector.

MEDM screens
------------

The following is the MEDM screen prosilica.adl.

.. figure:: prosilica.png
    :align: center

The following is the MEDM screen that provides access to the specific
parameters for the Prosilica detector.

.. figure:: prosilicaMore.png
    :align: center

The following is an IDL
`epics_ad_display <https://cars.uchicago.edu/software/idl/imaging_routines.html#epics_ad_display>`__
screen displaying the Prosilica detector images.

.. figure:: prosilica_tvscl.jpg
    :align: center

Connection management
---------------------

The Prosilica driver implements connection management. Cameras do not
need to be accessible when the IOC starts up, and can be power-cycled or
disconnected and reconnected from the Ethernet without restarting the
IOC.

Whenever a new camera is detected on the network the Prosilica library
issues a callback to the driver. If the driver is not currently
connected to a camera it will attempt to connect when receiving the
callback. This mechanism should work no matter how the camera is
identified in the startup script, i.e. by Unique ID, IP address, or IP
name. It is also possible to manually connect and disconnect the camera
by using the ``$(P)$(R)AsynIO.CNCT`` PV, which is labeled "Connect" and
"Disconnect" on the medm screen.

If the camera is not accessible when the IOC boots, or is power-cycled
then the EPICS output records may not match the actual camera settings
and readbacks. They can be made to agree by processing the output
record, e.g. by pressing Enter or Return in the medm output widget. In
the future this may be improved by sending all of the EPICS settings to
the camera when it connects, though it is not clear if this would always
be the desired behavior.


