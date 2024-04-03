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
     - The number CRC errors on the CoVisionaXPress links to the camera.
   * - ResetErrorCounts
     - bo
     - ES_RESET_ERROR_COUNTS
     - Resets RejectedFrames and CRCErVisionrors to 0.
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
     - Mikrotron EoSens 2.0MCX12 
     - Adimec Q-12A
     - ViewWorks VNP604
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
     - 6.2
   * - GBytes/s. Mono10
     - 7.46
     - 3.83
     - 2.0
   * - Frames/s, Mono12
     - N.A
     - N.A
     - 6.2
   * - GBytes/s. Mono12
     - N.A
     - N.A
     - 2.0

The ADEuresys driver can collect the images from these cameras at the full specified frame rates
in the table above, with one exception.  The Mikrotron camera in 10-bit mode has a maximum
from rate of 1798 frames/s.  However, ADEuresys only receives 1668 frames/s on Linux and
1703 frames/s on Windows. I have contacted Euresys to ask about this difference.

.. figure:: Mikrotron_photo.jpg
    :align: center
    :width: 600

    Mikrotron EoSens 2.0MCX12 
 
.. figure:: Adimec_photo.jpg
    :align: center
    :width: 600
    
    Adimec Q-12A

.. figure:: ViewWorks_photo.jpg
    :align: center
    :width: 600

    ViewWorks VNP604

File saving benchmarks
----------------------

The following table shows the results of writing data to disk with the NDFileHDF5 plugin.
These tests were done on a computer with the following specifications:

  - Model: Dell Precision 5860
  - Processor: Intel Xeon w5-2445, 3100 MHz
  - RAM: 256GB
  - Disk: 1 TB MVMe in M.2 slot on motherboard 
  - OS: Windows 10 and CentOS 9 Stream, dual boot
  - CoaXPress card: Euresys CoaxLink CXP-12

The tests were done in both Mono8, and Mono10 or Mono12 modes.
In Mono8 mode the NDArrays are UInt8.  In Mono10 or Mono12 modes the NDArrays are UInt16.
The same tests were done on Windows 10 and CentOS 9 Stream.

In each mode tests were done with the HDF5 plugin in both Stream mode and Capture mode.

In Stream mode the HDF5 files are written to disk as they are being collected.
The camera frame rate was adjusted to the value that led to no dropped frames
when saving the specified number of frames, while using a 2000 frame queue.

In Capture mode the specified number of frames were saved into the capture buffer
at the full frame rate for that mode.
The HDF5 file was then written to disk after the capture was complete,
and the total time to write the file was measured.  
This allows calculating the file saving frame rate and data rate.

.. cssclass:: table-bordered table-striped table-hover
.. list-table:: **Mikroton camera (Mono8=50,000 frames, Mono10=30,000 frames)**
   :header-rows: 1
   :widths: auto

   * - Format
     - OS
     - Camera frames/s
     - File mode
     - File frames/s
     - GB/s
   * - Mono8
     - Linux
     - 2247
     - Stream
     - 1400
     - 2.90
   * - Mono8
     - Windows
     - 2247
     - Stream
     - 1700
     - 3.53
   * - Mono8
     - Linux
     - 2247
     - Capture
     - 1429
     - 2.96
   * - Mono8
     - Windows
     - 2247
     - Capture
     - 1786
     - 3.70
   * - Mono10
     - Linux
     - 1668
     - Stream
     - 650
     - 2.70
   * - Mono10
     - Windows
     - 1703
     - Stream
     - 800
     - 3.32
   * - Mono10
     - Linux
     - 1668
     - Capture
     - 666
     - 2.76
   * - Mono10
     - Windows
     - 1703
     - Capture
     - 909
     - 3.77

.. cssclass:: table-bordered table-striped table-hover
.. list-table:: **Adimec camera (Mono8=10,000 frames, Mono10=10,000 frames)**
   :header-rows: 1
   :widths: auto

   * - Format
     - OS
     - Camera frames/s
     - File mode
     - File frames/s
     - GB/s
   * - Mono8
     - Linux
     - 187
     - Stream
     - 187
     - 2.35
   * - Mono8
     - Windows
     - 187
     - Stream
     - 187
     - 2.35
   * - Mono8
     - Linux
     - 187
     - Capture
     - 286
     - 3.60
   * - Mono8
     - Windows
     - 187
     - Capture
     - 312
     - 3.93
   * - Mono10
     - Linux
     - 152
     - Stream
     - 90
     - 2.27
   * - Mono10
     - Windows
     - 152
     - Stream
     - 90
     - 2.27
   * - Mono10
     - Linux
     - 152
     - Capture
     - 92
     - 2.32
   * - Mono10
     - Windows
     - 152
     - Capture
     - 84
     - 2.11

.. cssclass:: table-bordered table-striped table-hover
.. list-table:: **ViewWorks camera (Mono8=1,000 frames, Mono12=500 frames)**
   :header-rows: 1
   :widths: auto

   * - Format
     - OS
     - Camera frames/s
     - File mode
     - File frames/s
     - GB/s
   * - Mono8
     - Linux
     - 6.2
     - Stream
     - 6.2
     - 0.94
   * - Mono8
     - Windows
     - 6.2
     - Stream
     - 6.2
     - 0.94
   * - Mono8
     - Linux
     - 6.2
     - Capture
     - 16.7
     - 2.52
   * - Mono8
     - Windows
     - 6.2
     - Capture
     - 9.7
     - 1.46
   * - Mono12
     - Linux
     - 6.2
     - Stream
     - 6.2
     - 1.87
   * - Mono12
     - Windows
     - 6.2
     - Stream
     - 5.0
     - 1.51
   * - Mono12
     - Linux
     - 6.2
     - Capture
     - 8.8
     - 2.66
   * - Mono12
     - Windows
     - 6.2
     - Capture
     - 3.7
     - 1.11

I tried three schemes to see if I could increase the file saving rate to keep up with the Mikrotron camera.

The first was to load 2 NDFileHDF5 plugins, and use NDPluginScatter to send every other frame to each
plugin.  If file rate was limited by CPU load on the HDF5 plugin this should improve performance.
I found that this did not help at all, each plugin ran at 50% of the frame rate of a single HDF5 plugin.
I believe this indicates that the rate is limited by a global mutex in the HDF5 library, since neither
plugin was CPU bound, and I don't think they were limited by the NVMe disk write rate.

The second scheme was to create 2 pvaDriver IOCs on the same computer.  Each of these runs an HDF5
file writer.  The ADEuresys IOC loads 2 NDPluginPva plugins, each connected to one of the
pvaDriver IOCs.  NDPluginScatter on the ADEuresys IOC sends every other frame to each NDPluginPva.
This did not work because the PVA links could not keep up.  I also found that the PVA links dropped
frames even at 100 frames/s.  This needs to be investigated, because PVA should not be dropping frames
under low loads.

The third scheme was to write a new file plugin called NDFileJRaw.  This plugin uses a simple raw format,
using the low-level OS-specific calls to write the NDArrays.  The "J" in JRaw indicates a future plan
to put JSON metadata in the file to indicate the data format and size, and possibly to store NDAttributes.
I found that writing files with JRaw, both in Stream mode and Capture mode, did not result in a
statistically significant improvement over HDF5.  In some cases it was faster, but in some it was
slower.  The results are in the benchmarks.txt file in the docs/ADEuresys directory.  For brevity
I have not included them in the table above.

MEDM screens
------------
The following is the MEDM screen ADEuresys.adl when controlling a Miktron EoSens 2.0MCX12 camera.
ADEuresys.adl is very similar to ADGenICam.adl, with a few additional widgets for the PVs that are 
specific to ADEuresys.

Note that each frame is 2.1 MB, and it is collecting 2,247 frames/s, which is 4.66 GB/s. 

.. figure:: ADEuresys_Mikrotron.png
    :align: center

The following are the camera-specfic features MEDM screens for the Mikrotron camera.
These screens were autogenerated by the Python script in :doc:`../ADGenICam/ADGenICam`, and are specific to this camera model.
They are loaded from the "Camera-specific features" related display widget in the above screen.

.. figure:: ADEuresys_Mikrotron_Features1.png
    :align: center

.. figure:: ADEuresys_Mikrotron_Features2.png
    :align: center
