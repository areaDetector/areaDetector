======================================
ADEuresys
======================================

:author: Mark Rivers, University of Chicago

.. contents:: Contents

.. _Euresys:      https://www.euresys.com/en/Homepage
.. _GenICam:      https://www.emva.org/standards-technology/genicam
.. _CoaXPress:    https://www.coaxpress.com
.. _ADGenICam:    https://github.com/areaDetector/ADGenICam
.. _ADEuresys:    https://github.com/areaDetector/ADEuresys
.. _ADSupport:    https://github.com/areaDetector/ADSupport
.. _EGrabber:     https://www.euresys.com/en/Products/Machine-Vision-Software/eGrabber
.. _ADEuresys class: ../areaDetectorDoxygenHTML/class_a_d_euresys.html

Overview
--------

This is an :doc:`../index` driver for CoaXPress_ cameras using the Euresys_ CoaxLink frame grabbers and EGrabber_ SDK.

CoaXPress is a high-speed camera interface using one or more parallel coaxial cables between the camera and a frame grabber
in a computer.  The following table shows the speeds and cable length limits for different standard CoaXPress bit rates.

.. cssclass:: table-bordered table-striped table-hover
.. list-table::
   :header-rows: 1
   :widths: auto

   * - Name
     - Bit rate
     - Maximum cable length with Belden 1694A RG-6 cables
   * - CXP-1
     - 1.25 Gbit/s
     - 130 m
   * - CXP-2
     - 2.5 Gbit/s
     - 110 m
   * - CXP-3
     - 3.125 Gbit/s
     - 100 m
   * - CXP-5
     - 5.0 Gbit/s
     - 60 m
   * - CXP-6
     - 6.25 Gbit/s
     - 40 m
   * - CXP-10
     - 10.0 Gbit/s
     - 40 m
   * - CXP-12
     - 12.5 Gbit/s
     - 30 m

A CXP-12 X4 camera has 4 parallel CXP-12 links, and thus runs at a total rate of 50 Gbit/s.

CoaXPress cameras use the GenICam_ standard. GenICam_ is a Generic Interface for Cameras from the
European Machine Vision Association (EMVA). 

ADEuresys is derived from the base class ADGenICam_, which handles many of the details of
mapping GenICam_ features to EPICS records.
`ADEuresys class`_ describes this class in detail.

ADEuresys_ uses the Euresys EGrabber_ SDK. It runs on most versions of Windows and Linux.
The driver is identical on Windows and Linux, there is no OS-specific code in the driver.

ADEuresys_ currently only supports CoaXPress frame grabbers.  
EGrabber_ also supports their CameraLink frame grabbers and GigE cameras.
Support for these could be added in the future, either as part of ADEuresys, or as a separate areaDetector driver.

The EGrabber_ SDK must be downloaded and installed prior to building the driver.
It can be found in the Download area on the Euresys_ website.
On Windows this is a standard installation procedure.
On Linux there is an install.sh script which builds the kernel driver for the frame grabber
and installs the include and binary files in /opt/euresys.

The ADEuresys/configure/CONFIG_SITE or ADEuresys/configure/CONFIG_SITE.$(EPICS_HOST_ARCH).Common files must be edited
to point to the location of the include files on the local system.

Prior to using ADEuresys with a specific camera model, the XML file must be read from the camera using the Euresys gentl utility.
Run the command::

  gentl xml

This will extract the XML files for both the camera and the GenTL components System, Interface, Device, and DataStream
and save as files in the current default directory.

The EPICS database file and OPI screens must then be generated from the camera XML file
by running the Python programs as described in the
:ref:`ADGenICam documentation <ADGenICam_Python_scripts>`.

ADEuresys driver
----------------
ADEuresys inherits from :doc:`../ADGenICam/ADGenICam`.  It adds the following parameters and EPICS records that are
specific to ADEuresys.


.. cssclass:: table-bordered table-striped table-hover
.. list-table::
   :header-rows: 1
   :widths: auto

   * - EPICS record names
     - Record types
     - drvInfo string
     - Description
   * - TimeStampMode, TimeStampMode_RBV
     - bo, bi
     - ES_TIME_STAMP_MODE
     - Controls whether the TimeStamp attribute comes from the camera internal time stamp or from the EPICS time.
       Choices are Camera (0), and EPICS (1).
   * - UniqueIdMode, UniqueIdMode_RBV
     - bo, bi
     - ES_UNIQUE_ID_MODE
     - Controls whether the UniqueId attribute comes from the camera internal value or from the driver.
       Choices are Camera (0), and Driver (1).
   * - UnpackingMode, UnpackingMode_RBV
     - mbbo, mbbi
     - ES_UNPACKING_MODE
     - Controls conversion of Mono10 and Mono12 formats to Mono16.
       Mono10 and Mono12 data are sent across the CXP cables in packed format.
       These formats can be unpacked by the DMA engine to Mono16 with the following choices:

       - LSB The frame grabber adds leading 0 bits, so Mono10 will range from 0 to 1023 and Mono12 from 0 to 4095.
       - MSB The frame grabber adds trailing 0 bit. Mono10 values can thus be 0, 64, 128, ... 65472,
         and Mono12 values can be 0, 16, 32, ... 65520.
       - Off No padding bits are added and the pixel data are not aligned on byte boundaries.
   * - ConvertPixelFormat, ConvertPixelFormat_RBV
     - mbbo, mbbi
     - ES_CONVERT_PIXEL_FORMAT
     - Controls conversion of the pixel format read from the camera to a different format.  For example this can be used
       to convert Bayer to RGB which allows the camera to send 8-bit data over the bus and then convert to 24-bit
       RGB on the host computer, reducing the required bandwidth and increasing the frame rate.
       NOTE: This is currently not implemented in the driver because I don't have a color camera
       to use for testing.
   * - ESBufferSize
     - longin
     - ES_BUFFER_SIZE
     - The number of EGrabber buffers allocated when the driver is created.
   * - OutputQueue
     - longin
     - ES_OUTPUT_QUEUE
     - The number of buffers in the EGrabber output queue waiting for the ADEuresys driver to process.
   * - RejectedFrames
     - longin
     - ES_REJECTED_FRAMES
     - The number of frames that the EGrabber dropped when the output queue overflowed because the
       ADEuresys driver could not keep up.
   * - CRCErrors
     - longin
     - ES_CRC_ERROR_COUNT
     - The number CRC errors on the CoaXPress links to the camera.
   * - ResetErrorCounts
     - bo
     - ES_RESET_ERROR_COUNTS
     - Resets RejectedFrames and CRCErrors to 0.
   * - ProcessTotalTime
     - ai
     - ES_PROCESS_TOTAL_TIME
     - The total execution time of the frame callback function in ms.
   * - ProcessCopyTime
     - ai
     - ES_PROCESS_COPY_TIME
     - The time in ms required to copy the data from the EGrabber buffer to the NDArray
       in the frame callback function.  This is included in ProcessTotalTime.


IOC startup script
------------------
The command to configure an ADEuresys camera in the startup script is::

  ADEuresysConfig(const char *portName, const char *cameraId, int numEGBuffers,
                  size_t maxMemory, int priority, int stackSize)

``portName`` is the name for the ADEuresys port driver

``cameraId`` is used to identify which camera to control.
In most cases there will be a single frame grabber card in the system, connected to a single camera.
This parameter is currently not implemented, but could be in the future if it is desired to use
multiple cameras from one computer.

``numEGBuffers`` is the number of buffers to allocate in EGrabber. If set to 0 or omitted the default of 100 will be used.

``maxMemory`` is the maximum amount of memory the NDArrayPool is allowed to allocate.  0 means unlimited.

``priority`` is the priority of the port thread.  0 means medium priority.

``stackSize`` is the stack size.  0 means medium size.

Cameras Tested
--------------
ADEuresys_ has been tested with 3 very different cameras, shown in the following table.

.. cssclass:: table-bordered table-striped table-hover
.. list-table::
   :header-rows: 1
   :widths: auto

   * - Model
     - Miktron EoSens 2.0MCX12 
     - Adimec Q-12A
     - VisionWorks VNP604
   * - Pixel dimensions
     - 1920 x 1080 
     - 4096 x 3072
     - 14192 x 10640
   * - MegaPixels
     - 2.1 
     - 12.6
     - 151.0
   * - CXP version
     - CXP-12 X4
     - CXP-6 X4
     - CXP-6 X4
   * - Frames/s, Mono8
     - 2247
     - 187
     - 6.5
   * - GBytes/s. Mono8
     - 4.66
     - 2.35
     - 1.0
   * - Frames/s, Mono10
     - 1798
     - 152
     - ?
   * - GBytes/s. Mono10
     - 7.46
     - 3.83
     - ?
   * - Frames/s, Mono12
     - N.A
     - N.A
     - ?
   * - GBytes/s. Mono12
     - N.A
     - N.A
     - ?

MEDM screens
------------
The following is the MEDM screen ADEuresys.adl when controlling a Miktron EoSens 2.0MCX12 camera.
ADEuresys.adl is very similar to ADGenICam.adl, with a few additional widgets for the PVs that are 
specific to ADEuresys.

Note that each frame is 2.1 MB, and it is collecting 2,247 frames/s, which is 4.66 GB/s. 

.. figure:: ADEuresys_Mikrotron.png
    :align: center

The following are the camera-specfic features MEDM screens for the Miktrotron camera.
These screens were autogenerated by the Python script in :doc:`../ADGenICam/ADGenICam`, and are specific to this camera model.
They are loaded from the "Camera-specific features" related display widget in the above screen.

.. figure:: ADEuresys_Mikrotron_Features1.png
    :align: center

.. figure:: ADEuresys_Mikrotron_Features2.png
    :align: center
