======================================
ADHamamatsuDCAM
======================================

:author: Hamamatsu Phtonics K.K. and Mark Rivers, University of Chicago

.. contents:: Contents

.. _ADHamamatsuDCAM:  https://github.com/areaDetector/ADHamamatsuDCAM
.. _ADSupport:        https://github.com/areaDetector/ADSupport
.. _DCAM:             https://www.hamamatsu.com/us/en/product/cameras/software/driver-software.html
.. _hamamatsu class:  ../areaDetectorDoxygenHTML/classhamamatsu.html
.. _Hamamatsu:        https://www.hamamatsu.com/us/en/product/cameras.html

Overview
--------

This is an :doc:`../index` driver for Hamamatsu_ cameras using the DCAM_ library.
It runs on Windows, and on some versions of Linux.  

`hamamatsu class`_ describes this class in detail.

Installation
------------
Prior to building ADHamamatsuDCAM_ the DCAM_ software must be downloaded. 
This consists of 2 packages:

  - DCAM-API, which contains the required Windows or Linux drivers.
  - DCAM-SDK4, which contains the SDK library that ADHamamatsuDCAM_ calls.

DCAM-API on Linux is officially supported on Ubuntu 22.04 LTS and Debian 11.
The Active Silicon CoaXPress driver does not build on RHEL 9/CentOS 9.
The driver fails to compile, because g++ 11.5 is stricter than the g++ on supported OS versions.
Hopefully this will be fixed in the future.

DCAM-SDK4 is downloaded as a .zip file for Windows, and a tar.gz file for Linux.
These do not contain an installer, so there is no standard location for the dcamsdk4 directory,
the user must chose a location.
The ADHamamatsuDCAM_ driver needs to find these files when compiling.
This is done by defining the path to dcamsdk4 in these files::

  ADHamamatsuDCAM/configure/CONFIG_SITE.$(EPICS_HOST_ARCH).Common
  ADHamamatsuDCAM/iocs/hamamatsuIOC/configure/CONFIG_SITE.$(EPICS_HOST_ARCH).Common

Examples are provided in the distribution.

On Windows the file dcamsdk4/samples/cpp/misc/config4.h needs to be edited to comment out #pragma lines as shown here::

  /*
  #if defined(_WIN64)
  #pragma comment(lib,"../../../lib/win64/dcamapi.lib")
  #elif defined(WIN32)
  #pragma comment(lib,"../../../lib/win32/dcamapi.lib")
  #endif
  */


ADHamamatsuDCAM driver
----------------------
ADHamamatsuDCAM inherits from :doc:`../ADCore/ADDriver`.  It adds the following parameters and EPICS records that are
specific to ADHamamatsuDCAM.


.. cssclass:: table-bordered table-striped table-hover
.. list-table::
   :header-rows: 1
   :widths: auto

   * - EPICS record names
     - Record types
     - drvInfo string
     - Description
   * - HamaRegionReset
     - bo
     - HamaRegionReset
     - Resets the readout region to full chip
   * - HamaTriggerSource, HamaTriggerSource_RBV"
     - mbbo, mbbo
     - HamaTriggerSource
     - Trigger source.  Choices are "Internal" (0), "External" (1), and "Software" (2)
   * - HamaTriggerMode, HamaTriggerMode_RBV"
     - mbbo, mbbo
     - HamaTriggerMode
     - Trigger mode.  Choices are "Normal" (0), and "Start" (1)
   * - HamaTriggerActive, HamaTriggerActive_RBV"
     - mbbo, mbbo
     - HamaTriggerActive
     - Trigger source.  Choices are "Edge" (0), "Level" (1), and "Syncreadout" (2)
   * - HamaTriggerPolarity, HamaTriggerPolarity_RBV"
     - mbbo, mbbo
     - HamaTriggerPolarity
     - Trigger mode.  Choices are "Negative" (0), and "Positive" (1)
   * - HamaFireTrigger
     - bo
     - HamaFireTrigger
     - Fires the trigger in Software mode
   * - HamaTriggerTimes, HamaTriggerTimes_RBV
     - longout, longin
     - HamaTriggerTimes
     - Number of triggers
   * - HamaTriggerDelay, HamaTriggerDelay_RBV
     - ao, ai
     - HamaTriggerDelay
     - Trigger delay time
   * - HamaSensorCoolerStatus_RBV
     - waveform
     - HamaSensorCoolerStatus
     - Cooler status
   * - HamaReadoutSpeed, HamaReadoutSpeed_RBV"
     - mbbo, mbbo
     - HamaReadoutSpeed
     - Readout speed.  Choices are "Slowest" (0), and "Fastest" (1)

IOC startup script
------------------
The command to configure an ADHamamatsuDCAM camera in the startup script is::

  hamamatsuConfig(const char *portName, const char *cameraId,
                  size_t maxMemory, int priority, int stackSize)

``portName`` is the name for the ADHamamatsuDCAM port driver

``cameraId`` is index of the camera in the available camera list. 

``maxMemory`` is the maximum amount of memory the NDArrayPool is allowed to allocate.  0 means unlimited.

``priority`` is the priority of the port thread.  0 means medium priority.

``stackSize`` is the stack size.  0 means medium size.

MEDM screen
-----------
The following is the MEDM screen ADHamamatsuDCAM.adl when controlling an Orca Fire camera.

Note that each frame is 20.0 MB, and it is collecting 115 frames/s, which is 2.3 GB/s. 

.. figure:: ADHamamatsuDCAM.png
    :align: center

Issues
------
The following issues have been identified:

- AcquirePeriod is not implemented.  It should be if possible.
- Is color mode needed, i.e. does DCAM support any color cameras?
- The driver is not setting the ArraySize PVs
- Hardware triggering needs to be tested
- RHEL 9 should be supported if possible.
