=============
ADPerkinElmer
=============

:author: Mark Rivers (University of Chicago), John Hammonds, Brian Tieman (Argonne National Laboratory)

.. contents:: Contents

Introduction
------------

This is an :doc:`../index` driver for the flat-panel amorphous
silicon detectors from
`Varex Imaging (previously Perkin Elmer) <https://www.vareximaging.com/products/security-industrial/security-industrial-flat-panel-detectors>`__.
 
The driver is based upon the XISL library provided by Perkin Elmer. It
only runs on Microsoft Windows computers. It supports both 32-bit and
64-bit versions of Windows.

This driver has been tested with the following detectors

-  XRD0820 with XRD-FG PCI frame grabber
-  XRD0822 with Gigabit Ethernet interface
-  XRD1621 with XRD-FGX PCI frame grabber
-  XRD1621 with XRD-FGe Opto PCI-Express frame grabber
-  XRPAD with Gigabit Ethernet interface

This driver inherits from :doc:`../ADCore/ADDriver`.
It implements nearly all of the parameters in
`asynNDArrayDriver.h <../areaDetectorDoxygenHTML/asyn_n_d_array_driver_8h.html>`__
and in
`ADArrayDriver.h <../areaDetectorDoxygenHTML/_a_d_driver_8h.html>`__. It
also implements a number of parameters that are specific to the
Perkin Elmer cameras. The `PerkinElmer class
documentation <../areaDetectorDoxygenHTML/class_perkin_elmer.html>`__
describes this class in detail.

Implementation of standard driver parameters
--------------------------------------------

The following table describes how the PerkinElmer driver implements some
of the standard driver parameters.

.. cssclass:: table-bordered table-striped table-hover
.. flat-table::
  :header-rows: 2
  :widths: 20 80

  * - **Implementation of Parameters in asynNDArrayDriver.h and ADDriver.h, and EPICS Record
      Definitions in ADBase.template and NDFile.template**
  * - EPICS record names
    - Description
  * - NumImages
    - Controls the number of images to acquire when ADImageMode is ``Multiple`` or ``Average``
  * - AcquireTime, AcquireTime_RBV
    - When TriggerMode= ``Internal`` or ``Soft Trigger`` this value controls the exposure time,
      both for SynchMode= ``Framewise`` and ``DDD No Clear``.

      When TriggerMode= ``External`` and SynchMode= ``DDD No Clear`` this value also controls
      the exposure time.

      When TriggerMode= ``External`` and SynchMode= ``Framewise`` the exposure time is determined
      by the time between trigger pulses, not by AcquireTime.

      When TriggerMode= ``Free Running`` the AcquireTime is ignored, and the detector runs
      at the maximum possible frame rate and a fixed exposure time.
  * - TriggerMode, TriggerMode_RBV
    - Sets the trigger mode for the detector. Options are:

      - Internal
      - External
      - Free Running
      - Soft Trigger

      See the `Synchronization and triggering`_ section below
      for more details about synchronization and triggering.
  * - ImageMode, ImageMode_RBV
    - Sets the trigger mode for the detector. Options are:

      - Single
      - Multiple
      - Continuous
      - Average This mode is new for the Perkin Elmer. It averages NumImages frames in
        the hardware frame grabber. It is useful when the total acquisition time is longer
        than the 5 second maximum allowed in Internal trigger mode, etc.

PerkinElmer specific parameters
-------------------------------

The PerkinElmer driver implements the following parameters in addition
to those in asynNDArrayDriver.h and ADDriver.h.

.. cssclass:: table-bordered table-striped table-hover
.. flat-table::
  :header-rows: 2
  :widths: 70 10 10

  * - Parameter Definitions in PerkinElmer.h and EPICS Record Definitions in PerkinElmer.template
  * - Description
    - EPICS record names (preceeded with $(P)$(R))
    - EPICS record types
  * - Number of software frame buffers to use
    - PENumFrameBuffers, PENumFrameBuffers_RBV
    - longout, longin
  * - Initialize the detector
    - PEInitialize
    - bo
  * - Trigger record for soft trigger mode
    - PETrigger
    - bo
  * - Frame number from the hardware frame buffer for this image. Values go from 1 to 8.
    - PEFrameBuffIndex
    - longin
  * - Image number for this image in the software frame buffer array.
    - PEImageNumber
    - longin
  * - Gain selection in units of pF capacitance. Choices are 0.25, 0.5, 1, 2, 4, and 8.
    - PEGain, PEGain_RBV
    - mbbo, mbbi
  * - Synchronization mode. Choices are:

      - DDD Clear This mode has not been tested and clear documentation on it seems to be missing.
      - DDD No Clear This is Data Delivered on Demand mode. It discards the next frame
        and then starts acquiring a new frame for the specified acquisition time.
      - Linewise  This is a specialized mode that requires an external timing source.
        It has not been tested with the EPICS driver. See the manual for details.
      - Framewise This is the normal default mode. The next frame delivered from the
        detector is stored.
      - AutoTrigger This mode is only available on the XRPAD. It automatically triggers
        a readout when the number of x-rays striking the detecotor reaches a defined threashold.

        See the `Synchronization and triggering`_ section below for more details about synchronization and triggering.
    - PESyncMode, PESyncMode_RBV
    - mbbo, mbbi
  * -
    - **Offset corrections**
  * - Number of frames to collect and average when collecting offset frames
    - PENumOffsetFrames
    - longout
  * - Current offset frame being collected when collecting offset frames
    - PECurrentOffsetFrame
    - longin
  * - Acquire offset frames
    - PEAcquireOffset
    - busy
  * - Set whether offset image is to be used. Choices are ``Disable`` (0) and ``Enable`` (1).
    - PEUseOffset
    - bo
  * - Report whether offset image has been collected and is available for use. Choices
      are ``Not available`` (0) and ``Available`` (1).
    - PEOffsetAvailable
    - bi
  * - An offset that is added to the image when the offset correction is performed. CorrectedImage
      = RawImage - OffsetImage + OffsetConstant. This constant should be used to prevent
      the CorrectedImage from having any negative pixel values, which would otherwise
      be clipped to 0. For efficiency this value is actually subtracted from the offset
      image as soon as it is collected, so it is then effectively added to each image
      when offset correction is done. This results in 2 restrictions:

      1) PEOffsetConstant must be specified before the offset images are collected. If
         it is changed then the offset images must be collected again.
      2) The value must be less than the minimum value of the offset image, or else negative
         values will result. In practice this should not be a problem, since an offset of
         100 should be sufficient to prevent negative values during offset correction, and
         the offset images are always greater than 100.
    - PEOffsetContant, PEOffsetContant_RBV
    - longout, longin

  * -
    - **Gain corrections**
  * - Number of frames to collect and average when collecting gain frames
    - PENumGainFrames
    - longout
  * - Current gain frame being collected when collecting gain frames
    - PECurrentGainFrame
    - longin
  * - Acquire gain frames
    - PEAcquireGain
    - busy
  * - Set whether gain image is to be used. Choices are ``Disable`` (0) and ``Enable`` (1).
    - PEUseGain
    - bo
  * - Report whether offset image has been collected and is available for use. Choices
      are:

      - Not available (0)
      - Available`` (1)
    - PEGainAvailable
    - bi
  * - File name for gain correction file
    - PEGainFile
    - waveform
  * - Save gain corrections to a file
    - PESaveGainFile
    - bo
  * - Load gain corrections from a file
    - PELoadGainFile
    - bo
  * -
    - **Bad pixel corrections**
  * - Set whether bad pixel correction is to be used
    - PEUsePixelCorrection
    - bo
  * - Report whether pixel correction file has been set and is available for use
    - PEPixelCorrectionAvailable
    - bi
  * - File name for pixel correction file
    - PEPixelFile
    - waveform
  * - Directory where gain and pixel correction files should be stored
    - PECorrectionsDir
    - waveform
  * - Load pixel corrections from a file for use
    - PELoadPixelCorrections
    - bo
  * -
    - **Skip frames settings**
  * - Set whether to skip initial PENumFramesToSkip frames from the detector on each acquisition.
      This can be used to discard the first frame which may contain data from before acquisition
      was started.
    - PESkipFrames, PESkipFrames_RBV
    - bo, bi
  * - Sets the number of initial frames from the detector to skip on each acquisition
      if PESkipFrames=1.
    - PENumFramesToSkip, PENumFramesToSkip_RBV
    - longout, longin
  * -
    - **Trigger output control**
  * - Controls which signal appears on the trigger output connector.  Choices are:

      0) FRM_EN_PWM
      1) FRM_EN_PWM_INV
      2) EP
      3) EP_INV
      4) DDD_PULSE
      5) DDD_PULSE_INV
      6) GND
      7) VCC
    - PETrigOutSignal, PETrigOutSignal_RBV
    - mbbo, mbbi
  * - Trigger output EP length
    - PETrigOutEPLength, PETrigOutEPLength_RBV
    - longout, longin
  * - Trigger output EP first frame
    - PETrigOutEPFirstFrame, PETrigOutEPFirstFrame_RBV
    - longout, longin
  * - Trigger output EP last frame
    - PETrigOutEPLastFrame, PETrigOutEPLastFrame_RBV
    - longout, longin
  * - Trigger output EP delay 1
    - PETrigOutEPDelay1, PETrigOutEPDelay1_RBV
    - longout, longin
  * - Trigger output EP delay 2
    - PETrigOutEPDelay2, PETrigOutEPDelay2_RBV
    - longout, longin
  * - Trigger output DDD delay
    - PETrigOutDDDDelay, PETrigOutDDDDelay_RBV
    - longout, longin
  * - Trigger output edge.  Choices are:

      0) Falling
      1) Rising
    - PETrigOutEdge, PETrigOutEdge_RBV
    - mbbo, mbbi

Unsupported standard driver parameters
--------------------------------------

The PerkinElmer driver does not support the following standard driver
parameters because they are not supported in the PerkinElmer library:

-  Readout region (ADMinX, ADMinY, ADSizeX, ADSizeY)
-  Type/Color (NDDataType, NDColorMode)
-  File (None of the file parameters in NDFile)
-  No Hardware shutter control

.. _refname:

Synchronization and triggering
------------------------------

The Perkin Elmer detectors support a number of acquisition modes which
are controlled by the TriggerMode, SyncMode, and AcquireTime records.
This is a simplified discussion, and the reader should consult the `XIS
software manual <1000952_Manual_XIS_Rev7.pdf>`__ and the detector
hardware manuals (e.g. `1621 hardware
manual <Manual_PKIaSi_XRD1621%202008-07-30.pdf>`__) for details.

The number of frames to be acquired is always controlled by the
ImageMode=(Single, Multiple, or Continuous) and NumImages (if
ImageMode=Multiple) records, regardless of the values of the
TriggerMode, SyncMode, and AcquireTime records.

It is important to realize that the detector hardware is constantly
acquiring a stream of frames even when the EPICS Acquire record is 0.
The rate of this frame stream depends on the value of the TriggerMode:

-  TriggerMode= ``Free Running`` The frame rate is the maximum that the
   detector supports.
-  TriggerMode= ``Internal`` The frame rate is controlled by an internal
   clock whose period is set to AcquireTime.
-  TriggerMode= ``External`` The frame rate is controlled by the frequency
   of the external trigger pulses.
-  TriggerMode= ``Soft Trigger`` The frame rate is controlled by the rate
   at which the PETrigger record is processed.

When SyncMode=Framewise and acquisition is started by setting Acquire=1
then the first frame that the EPICS driver collects is the next frame in
that frame stream. **NOTE:** this means that this frame probably started
exposing before Acquire was set to 1. This may not be desirable since
motors may have still been moving, or a shutter not yet opened. When
acquiring a sequence of images (ImageMode=Multiple) this would only
affect the first image in the sequence, which might be acceptable.
However, if doing a step-scan with ImageMode=Single it would affect
every image in the scan. There are two ways to handle this. One is to
use SyncMode=Framewise and set PENumFramesToSkip=1. This is quite
inefficient since on average it will have an overhead of AcquireTime/2,
and a worst case overhead of AcquireTime. A better approach is to set
FrameSync= ``DDD No Clear``, which adds a small overhead to each frame
(~0.1-0.2 seconds), but ensures that the exposure is started after
Acquire is set to 1.

TriggerMode=Free Running
~~~~~~~~~~~~~~~~~~~~~~~~

In this mode the AcquireTime is ignored, and the detector runs at the
maximum possible frame rate and a fixed minimum exposure time. SyncMode
should be set to Framewise in this mode.

TriggerMode=Internal
~~~~~~~~~~~~~~~~~~~~

In this mode the behavior depends on the setting of SyncMode.

-  If SyncMode= ``Framewise`` then the detector hardware is constantly
   acquiring a stream of images with a frame rate determined by the
   AcquireTime record, up to the maximum frame rate the detector is
   capable of. When Acquire is set to 1 then this driver begins
   acquiring the images and doing callbacks to plugins. This is the
   fastest way to run the detector with a user-programmable acquisition
   time. However, it suffers from the limitation described above that
   the first frame after Acquire is set to 1 actually was being exposed
   before Acquire was set to 1. This may not be desired, particularly
   when ImageMode=Single. **NOTE:** the XRPAD detectors cannot use this
   mode because they are limited to a single internal frame rate and the
   AcquireTime record has no effect in this mode.
-  If SyncMode= ``DDD No Clear`` then the detector hardware is constantly
   acquiring a stream of images at the maximum possible frame rate.
   However, when Acquire is set to 1 this driver sends a software
   trigger to the detector. This causes the hardware to discard the next
   frame when it arrives and to start a new exposure with an exposure
   time of AcquireTime. When the frame completes then the driver sends
   the next software trigger in the frame callback function. When
   running in this mode there is no limitation on the first frame
   containing data from before Acquire was set to 1. It does have the
   limitation that there is additional overhead per frame, so the
   maximum frame rate is reduced. On the XRD1621 this overhead is ~0.2
   second in 2048x2048 mode, and ~0.1 seconds in 1024x1024 mode. This
   mode is recommended when running with ImageMode=Single. If running
   with ImageMode=Multiple then this mode should be used if the first
   frame limitation described above is a problem. If the limitation of
   ``DDD No Clear`` mode on frame rate is more important than the
   limitation of ``Framewise`` mode on the first frame then one should use
   SyncMode= ``Framewise``.

TriggerMode=External
~~~~~~~~~~~~~~~~~~~~

In this mode the behavior also depends on the setting of SyncMode.

-  If SyncMode= ``Framewise`` then the detector hardware is constantly
   acquiring a stream of images with a frame rate determined by the time
   between external trigger pulses, up to the maximum frame rate the
   detector is capable of. When Acquire is set to 1 then this driver
   begins acquiring the images and doing callbacks to plugins. This is
   the fastest way to run the detector with an external trigger.
   However, it has some potential problems. First, if the time between
   external triggers is not constant then the exposure time of the
   frames will be different. This can lead to serious data quality
   issues because the offset correction will not be correct. If the time
   between trigger pulses is constant then there is still a potential
   data quality problem if the offset images were not collected with
   exactly the time acquisition time, for example if they will collected
   with an TriggerMode=Internal and a slightly different acquisition
   time. Of course this is not an issue if the offset images are
   acquired with the same external trigger period used for the data
   images, but this may be difficult to arrange in practice.
-  If SyncMode= ``DDD No Clear`` then the detector hardware is constantly
   acquiring a stream of images at the maximum possible frame rate.
   However, when an external trigger pulse is received this causes the
   hardware to discard the next frame when it arrives and to start a new
   exposure with an exposure time of AcquireTime. This mode thus allows
   acquiring for a fixed exposure time on each external trigger, rather
   than the time until the next trigger is received. This means that
   offset images can be simply collected with TriggerMode=Internal and
   the same AcquireTime. This mode does have the limitation that there
   is additional overhead per frame, so the maximum frame rate is
   reduced. The overhead is the same as described above for
   TriggerMode= ``Internal``, SyncMode= ``Framewise``.

TriggerMode=Soft Trigger
~~~~~~~~~~~~~~~~~~~~~~~~

In this mode the behavior also depends on the setting of SyncMode.

-  If SyncMode= ``Framewise`` then the detector hardware is constantly
   acquiring a stream of images with a frame rate determined by the time
   between software triggers from the PETrigger record, up to the
   maximum frame rate the detector is capable of. When Acquire is set to
   1 then this driver begins acquiring the images and doing callbacks to
   plugins. This is the fastest way to run the detector with a software
   trigger. However, it has the same potential problems as
   TriggerMode= ``External`` and SyncMode= ``Framewise`` described above. In the
   case of software triggers the problem may be even more important if
   the time between software triggers has significant variation.
-  If SyncMode= ``DDD No Clear`` then the detector hardware is constantly
   acquiring a stream of images at the maximum possible frame rate.
   However, when a software trigger is received this causes the hardware
   to discard the next frame when it arrives and to start a new exposure
   with an exposure time of AcquireTime. This mode thus allows acquiring
   for a fixed exposure time on each software trigger, rather than the
   time until the next software trigger is received. This means that
   offset images can be simply collected with TriggerMode= ``Internal`` and
   the same AcquireTime. This mode does have the limitation that there
   is additional overhead per frame, so the maximum frame rate is
   reduced. The overhead is the same as described above for
   TriggerMode= ``Internal``, SyncMode= ``Framewise``.

It is important that the offset images are collected with the same
SyncMode as the data. If the offset images are collected with
SyncMode=Framewise and the data are collected with SyncMode= ``DDD No
Clear`` then the offset correction will not be correct, even if both are
collected with the same AcquireTime. When using SyncMode= ``DDD No Clear``
it is fine to collect the offset and data images with different values
of TriggerMode. For example the offset images could be collected with
TriggerMode= ``Internal`` and the data images collected with TriggerMode= ``External``.

Configuration
-------------

The PerkinElmer driver is created with the perkinElmerConfig command,
either from C/C++ or from the EPICS IOC shell.

::

    int PerkinElmerConfig(const char *portName, int IDType, const char* IDValue,
                          int maxBuffers, size_t maxMemory,
                          int priority, int stackSize )
      

For details on the meaning of the parameters to this function refer to
the detailed documentation on the PerkinElmerConfig function in the
`PerkinElmer.cpp
documentation <../areaDetectorDoxygenHTML/_perkin_elmer_8cpp.html>`__ and
in the documentation for the constructor for the `PerkinElmer
class <../areaDetectorDoxygenHTML/class_perkin_elmer.html>`__.

Example st.cmd startup file
---------------------------
The following startup script is provided with ADPerkinElmer.

.. literalinclude:: ../../../ADPerkinElmer/iocs/perkinElmerIOC/iocBoot/iocPerkinElmer/st.cmd

MEDM screens
------------

The following show the MEDM screens that are used to control the
PerkinElmer detector. Note that the general purpose screen ADBase.adl
can be used, but it exposes many controls that are not applicable to the
PerkinElmer, and lacks some fields that are important for the
PerkinElmer driver.

``PerkinElmer.adl`` is the main screen used to control the PerkinElmer
driver.

.. figure:: PerkinElmer.png
    :align: center

Performance measurements
------------------------

The following measurements were done to demonstrate the performance that
can be obtained with the areaDetector PerkinElmer driver.

-  XRD1621 detector with the XRD-FGe Opto PCI-Express frame grabber

   -  Can stream data continuously to local disk at 15 frames/s at
      2048x2048 (120 MB/s) and 30 frames/s at 1024x1024 (60 MB/s). These
      tests were done on a Windows 7 64-bit machine with 2 disk drives
      (SAS, 15K RPM, RAID 0).

-  XRD0822 detector with a GigE interface

   -  Can stream data continuously to local disk at 25 frames/s at
      1024x1024 (50 MB/s) and 50 frames/s at 512x512 (25 MB/s). These
      tests were done on a Windows 7 64-bit laptop with a local disk
      drive.

Restrictions
------------

The following are some current restrictions of the PerkinElmer driver:

-  Prior to R2-4 the driver only supported a single frame-grabber or
   directly connected GigE detector. This restriction was removed in
   R2-4.

