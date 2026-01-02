======================================
ADMythen
======================================

:author: Joe Sullivan, M. Moore, Mark Rivers

.. _ADDriver: https://areadetector.github.io/areaDetector/ADCore/ADDriver.html
.. _areaDetector: https://areadetector.github.io/areaDetector/index.html
.. _EPICS: http://www.aps.anl.gov/epics
.. _Dectris: http://www.dectris.com
.. _LNLS: http://lnls.cnpem.br/
.. _ADPilatus: https://github.com/areaDetector/ADPilatus
.. _ADLambda: https://github.com/areaDetector/ADLambda

.. contents:: Contents

Introduction
------------

This is an `EPICS`_ `areaDetector`_ driver for the Mythen strip detector from `Dectris`_.
The Mythen is a one-dimensional silicon strip detector designed for X-ray diffraction
and spectroscopy applications.

ADMythen inherits from the `ADDriver`_ base class, which provides standard areaDetector
functionality. The driver uses asynPort for TCP/IP communication with the detector,
NDArray for the standard areaDetector data format, and EPICS Channel Access for control
and monitoring.

The driver supports all standard areaDetector plugins including NDStdArrays, NDPluginFile,
NDPluginStats, etc.

Features
--------

* **TCP/IP Communication**: Communicates with the detector over TCP/IP (UDP or TCP protocols)
* **Multiple Module Support**: Supports up to 2 detector modules, each with 1280 channels
* **Flexible Acquisition Modes**: Single image, multiple images, and continuous acquisition
* **Trigger Modes**: None (software), Single (external), and Continuous (external)
* **Data Corrections**:
  * Flat field correction
  * Count rate correction with configurable deadtime constant
  * Bad channel interpolation
* **Configurable Bit Depths**: 4, 8, 16, or 24 bits per channel
* **Read Modes**: Raw (uncorrected) or Corrected data
* **Energy Settings**: Predefined settings for common X-ray sources (Cu, Mo, Ag, Cr) or custom energy values
* **Gate Mode**: Support for gated acquisitions
* **Firmware Compatibility**: Works with multiple firmware versions

History and Credits
-------------------

* **Original Development**: Based on asyn driver from `LNLS`_
* **areaDetector Integration**: Modified by Joe Sullivan (ANL-APS/XSD/BCDA) to work with areaDetector
* **Enhancements**:
  * M. Moore (ANL-APS/XSD/DET): Firmware compatibility updates, ReadMode support, trigger timeout handling, ImageMode improvements
* **Documentation**: Comprehensive documentation created by K. Gofron (ORNL, Oak Ridge National Laboratory, December 6, 2025)

For detailed release notes, see `RELEASE.md <https://github.com/areaDetector/ADMythen/blob/master/RELEASE.md>`__.

Installation
------------

Prerequisites
~~~~~~~~~~~~

Required EPICS Modules
^^^^^^^^^^^^^^^^^^^^^

* **EPICS Base**: Version 3.14.12 or later (or EPICS 7.x)
* **areaDetector Modules**:
  * **ADCore**: R3-3 or later
  * **ADSupport**: Compatible version
  * **asyn**: R4-26 or later

System Requirements
^^^^^^^^^^^^^^^^^^^

* **Operating System**: Linux or Windows
* **Network**: TCP/IP connectivity to the Mythen detector
* **Compiler**:
  * Linux: GCC 4.8 or later
  * Windows: Visual Studio 2015 or later
* **Build Tools**: GNU Make

Obtaining the Source Code
~~~~~~~~~~~~~~~~~~~~~~~~~~

The ADMythen source code should be placed in the areaDetector support directory structure::

    /epics/support/areaDetector/
      ├── ADCore/
      ├── ADSupport/
      ├── ADMythen/          <-- Place ADMythen here
      └── ...

Clone or download the repository::

    cd /epics/support/areaDetector
    git clone <repository-url> ADMythen

Building from Source
~~~~~~~~~~~~~~~~~~~~

1. Configure RELEASE Files
^^^^^^^^^^^^^^^^^^^^^^^^^^

Edit ``configure/RELEASE`` to point to your EPICS modules::

    # Location of external products
    EPICS_BASE=/epics/base-3.15.5
    ADCORE=$(AREA_DETECTOR)/ADCore
    ADSUPPORT=$(AREA_DETECTOR)/ADSupport
    ASYN=$(AREA_DETECTOR)/asyn

2. Build the Driver
^^^^^^^^^^^^^^^^^^^

From the ADMythen top-level directory::

    make

This will build:
* The driver library (``mythenApp``)
* The IOC application (``iocs/mythenIOC``)

3. Install
^^^^^^^^^^

::

    make install

Platform-Specific Notes
^^^^^^^^^^^^^^^^^^^^^^^^

**Linux:**
* Standard build process as described above
* Supports both little-endian and big-endian architectures

**Windows:**
* Use Visual Studio solution files if provided
* Ensure proper path configuration for EPICS modules

**Big-Endian Support:**
* The driver includes automatic byte-swapping for big-endian systems
* No special configuration required

Verification
~~~~~~~~~~~~

To verify the installation:

#. Check that the libraries were built::

       ls -la lib/linux-x86_64/

#. Check that the IOC was built::

       ls -la iocs/mythenIOC/bin/linux-x86_64/

#. Start a test IOC (see Configuration section for details)

Configuration
-------------

IOC Setup
~~~~~~~~~

The IOC is located in ``iocs/mythenIOC/``. The main startup script is ``iocBoot/iocMythen/st.cmd``.

Directory Structure
^^^^^^^^^^^^^^^^^^^

::

    iocs/mythenIOC/
    ├── configure/
    │   ├── RELEASE          # EPICS module paths
    │   └── ...
    ├── iocBoot/
    │   └── iocMythen/
    │       ├── st.cmd       # IOC startup script
    │       └── auto_settings.req
    └── mythenApp/
        └── src/
            └── mythenAppMain.cpp

Network Configuration
~~~~~~~~~~~~~~~~~~~~~

The driver communicates with the Mythen detector over TCP/IP. Configure the network connection in ``st.cmd``::

    # Configure asyn IP port
    # Format: drvAsynIPPortConfigure("portName", "IP:Port Protocol", priority, noAutoConnect, noProcessEos)
    drvAsynIPPortConfigure("IP_M1K", "192.168.0.90:1030 UDP", 0, 0, 1)

**Parameters:**
* **portName**: Name for the asyn port (e.g., "IP_M1K")
* **IP:Port**: Detector IP address and port (default port is 1030)
* **Protocol**: "UDP" or "TCP" (UDP is typical)
* **priority**: Thread priority (0 = default)
* **noAutoConnect**: 0 = auto-connect, 1 = manual connect
* **noProcessEos**: 1 = don't process end-of-string

**End-of-String Configuration:**

::

    # Set output end-of-string (carriage return)
    asynOctetSetOutputEos("IP_M1K", 0, "\r")

**Tracing (for debugging):**

::

    # Enable I/O tracing (mask 6 = read/write)
    asynSetTraceIOMask("IP_M1K", 0, 6)
    # Enable general tracing (mask 3 = traceIO, traceError, traceWarning)
    asynSetTraceMask("IP_M1K", 0, 3)

Driver Configuration
~~~~~~~~~~~~~~~~~~~~~

Configure the Mythen driver using the ``mythenConfig`` function::

    # mythenConfig(portName, IPPortName, maxBuffers, maxMemory, priority, stackSize)
    mythenConfig("SD1", "IP_M1K", -1, -1)

**Parameters:**
* **portName**: Name for the driver port (e.g., "SD1")
* **IPPortName**: Name of the asyn IP port (e.g., "IP_M1K")
* **maxBuffers**: Maximum NDArray buffers (-1 = unlimited)
* **maxMemory**: Maximum memory in bytes (-1 = unlimited)
* **priority**: Thread priority (optional, default from ADDriver)
* **stackSize**: Thread stack size (optional, default from ADDriver)

**Example with limits:**

::

    mythenConfig("SD1", "IP_M1K", 100, 100000000)  # 100 buffers, 100MB memory

Database Configuration
~~~~~~~~~~~~~~~~~~~~~~~

Load the database template with appropriate macros::

    # Set environment variables
    epicsEnvSet("PREFIX", "dp_mythen1K:")
    epicsEnvSet("PORT", "SD1")
    epicsEnvSet("ADDR", "0")
    epicsEnvSet("TIMEOUT", "1")

    # Load database template
    dbLoadRecords("$(ADMYTHEN)/mythenApp/Db/mythen.template", 
                  "P=$(PREFIX),R=cam1:,PORT=$(PORT),ADDR=0,TIMEOUT=1")

**Macro Substitutions:**
* **P**: EPICS prefix (e.g., "dp_mythen1K:")
* **R**: Record name prefix (e.g., "cam1:")
* **PORT**: Driver port name (e.g., "SD1")
* **ADDR**: asyn address (typically 0)
* **TIMEOUT**: Timeout in seconds

**Array Size Configuration:**

::

    epicsEnvSet("EPICS_CA_MAX_ARRAY_BYTES", "64008")  # For 1280 channels * 4 bytes
    epicsEnvSet("XSIZE", "1280")  # Number of channels
    epicsEnvSet("YSIZE", "1")     # Always 1 for strip detector
    epicsEnvSet("NCHANS", "1280") # Total channels

Plugin Configuration
~~~~~~~~~~~~~~~~~~~~~

NDStdArrays Plugin
^^^^^^^^^^^^^^^^^^

Configure the standard arrays plugin to make data available via Channel Access::

    NDStdArraysConfigure("Image1", 3, 0, "$(PORT)", 0, 0)
    dbLoadRecords("$(ADCORE)/db/NDStdArrays.template", 
                  "P=$(PREFIX),R=image1:,PORT=Image1,ADDR=0,TIMEOUT=1,NDARRAY_PORT=$(PORT),NDARRAY_ADDR=0,TYPE=Float64,FTVL=DOUBLE,NELEMENTS=$(NCHANS)")

Other Plugins
^^^^^^^^^^^^^

Load additional plugins using the common plugins script::

    < $(ADCORE)/iocBoot/commonPlugins.cmd

This loads standard plugins like:
* NDPluginFile (file saving)
* NDPluginStats (statistics)
* NDPluginROI (regions of interest)
* NDPluginColorConvert
* NDPluginOverlay
* NDPluginTransform
* NDPluginCircularBuff
* NDPluginAttribute
* NDPluginFFT

Autosave Configuration
~~~~~~~~~~~~~~~~~~~~~~~

Configure autosave to restore detector settings::

    set_requestfile_path("$(TOP)/mythenApp/Db")
    create_monitor_set("auto_settings.req", 30, "P=$(PREFIX),D=cam1:")

This saves detector settings every 30 seconds and restores them on IOC startup.

Complete Example: st.cmd
~~~~~~~~~~~~~~~~~~~~~~~~~

::

    < envPaths

    # Set array size for Channel Access
    epicsEnvSet EPICS_CA_MAX_ARRAY_BYTES 64008

    # Load database definitions
    dbLoadDatabase("$(TOP)/dbd/mythenApp.dbd")
    mythenApp_registerRecordDeviceDriver(pdbbase)

    # Configure network connection
    drvAsynIPPortConfigure("IP_M1K", "192.168.0.90:1030 UDP", 0, 0, 1)
    asynOctetSetOutputEos("IP_M1K", 0, "\r")

    # Set environment variables
    epicsEnvSet("PREFIX", "dp_mythen1K:")
    epicsEnvSet("PORT", "SD1")
    epicsEnvSet("XSIZE", "1280")
    epicsEnvSet("YSIZE", "1")
    epicsEnvSet("NCHANS", "1280")

    # Configure driver
    mythenConfig("SD1", "IP_M1K", -1, -1)

    # Load database
    dbLoadRecords("$(ADMYTHEN)/mythenApp/Db/mythen.template", 
                  "P=$(PREFIX),R=cam1:,PORT=$(PORT),ADDR=0,TIMEOUT=1")

    # Configure plugins
    NDStdArraysConfigure("Image1", 3, 0, "$(PORT)", 0, 0)
    dbLoadRecords("$(ADCORE)/db/NDStdArrays.template", 
                  "P=$(PREFIX),R=image1:,PORT=Image1,ADDR=0,TIMEOUT=1,NDARRAY_PORT=$(PORT),NDARRAY_ADDR=0,TYPE=Float64,FTVL=DOUBLE,NELEMENTS=$(NCHANS)")
    < $(ADCORE)/iocBoot/commonPlugins.cmd

    # Configure autosave
    set_requestfile_path("$(TOP)/mythenApp/Db")
    create_monitor_set("auto_settings.req", 30, "P=$(PREFIX),D=cam1:")

    # Initialize IOC
    iocInit()

Driver Parameters
-----------------

Standard areaDetector Parameters
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ADMythen inherits all standard parameters from `ADDriver`_. Key parameters include:

.. cssclass:: table-bordered table-striped table-hover
.. list-table::
  :header-rows: 1
  :widths: 20 15 65

  * - Parameter
    - Type
    - Description
  * - ``Acquire``
    - bo
    - Start/stop acquisition (0=Stop, 1=Start)
  * - ``AcquireTime``
    - ao
    - Exposure time per frame (seconds)
  * - ``AcquirePeriod``
    - ao
    - Time between frames (seconds)
  * - ``ImageMode``
    - mbbo
    - Single, Multiple, Continuous, or Stream
  * - ``NumImages``
    - longout
    - Number of images to acquire
  * - ``NumImagesCounter``
    - longin
    - Current image number
  * - ``ArraySizeX``
    - longin
    - Array size in X (channels)
  * - ``ArraySizeY``
    - longin
    - Array size in Y (always 1)
  * - ``DataType``
    - mbbo
    - Data type (NDInt32)
  * - ``Status``
    - mbbi
    - Detector status
  * - ``StatusMessage``
    - stringin
    - Status message

See `areaDetector documentation <https://areadetector.github.io/areaDetector/>`__ for complete list of standard parameters.

Mythen-Specific Parameters
~~~~~~~~~~~~~~~~~~~~~~~~~~

Detector Settings
^^^^^^^^^^^^^^^^^

**SD_SETTING** (mbbo/mbbi)
* **Description**: Detector setting for predefined X-ray source configurations
* **Values**:
  * 0 = Cu (Copper)
  * 1 = Mo (Molybdenum)
  * 2 = Ag (Silver)
  * 3 = Cr (Chromium)
* **Access**: Read/Write
* **EPICS Records**: ``$(P)$(R)Setting``, ``$(P)$(R)Setting_RBV``

**SD_FIRMWARE_VERSION** (stringin)
* **Description**: Detector firmware version (read-only)
* **Access**: Read-only
* **EPICS Record**: ``$(P)$(R)FirmwareVersion``

Acquisition Parameters
^^^^^^^^^^^^^^^^^^^^^^

**SD_DELAY_TIME** (ao/ai)
* **Description**: Delay after trigger before acquisition starts
* **Units**: seconds
* **Range**: 0.0 to 655.35 seconds (in 100ns steps)
* **Default**: 0.1 seconds
* **Access**: Read/Write
* **EPICS Records**: ``$(P)$(R)DelayTime``, ``$(P)$(R)DelayTime_RBV``

**SD_NUM_FRAMES** (longout/longin)
* **Description**: Number of frames to acquire in one acquisition sequence
* **Range**: 1 to 500
* **Default**: 1
* **Access**: Read/Write
* **EPICS Records**: ``$(P)$(R)NumFrames``, ``$(P)$(R)NumFrames_RBV``
* **Note**: In Single image mode, this is automatically set to 1

**SD_TRIGGER** (mbbo/mbbi)
* **Description**: Trigger mode
* **Values**:
  * 0 = None (software trigger only)
  * 1 = Single (external trigger for single frame)
  * 2 = Continuous (continuous acquisition on external triggers)
* **Default**: 0 (None)
* **Access**: Read/Write
* **EPICS Records**: ``$(P)$(R)TriggerMode``, ``$(P)$(R)TriggerMode_RBV``

**SD_READ_MODE** (mbbo/mbbi)
* **Description**: Data readout mode
* **Values**:
  * 0 = Raw (uncorrected data from detector)
  * 1 = Corrected (data with corrections applied)
* **Default**: 0 (Raw)
* **Access**: Read/Write
* **EPICS Records**: ``$(P)$(R)ReadMode``, ``$(P)$(R)ReadMode_RBV``

Energy and Threshold
^^^^^^^^^^^^^^^^^^^^

**SD_THRESHOLD** (ao/ai)
* **Description**: Energy threshold for photon counting
* **Units**: keV
* **Range**: 0.0 to 50.0 keV (typical)
* **Default**: 8.05 keV
* **Access**: Read/Write
* **EPICS Records**: ``$(P)$(R)ThresholdEnergy``, ``$(P)$(R)ThresholdEnergy_RBV``

**SD_ENERGY** (ao/ai)
* **Description**: Beam energy (used for corrections)
* **Units**: keV
* **Range**: 0.0 to 50.0 keV (typical)
* **Default**: 8.1 keV
* **Access**: Read/Write
* **EPICS Records**: ``$(P)$(R)BeamEnergy``, ``$(P)$(R)BeamEnergy_RBV``
* **Note**: Only available in firmware version 3.0 or later

Correction Parameters
^^^^^^^^^^^^^^^^^^^^^

**SD_USE_FLATFIELD** (bo/bi)
* **Description**: Enable/disable flat field correction
* **Values**: 0 = Disable, 1 = Enable
* **Default**: 1 (Enabled)
* **Access**: Read/Write
* **EPICS Records**: ``$(P)$(R)UseFlatField``, ``$(P)$(R)UseFlatField_RBV``

**SD_USE_COUNTRATE** (bo/bi)
* **Description**: Enable/disable count rate correction
* **Values**: 0 = Disable, 1 = Enable
* **Default**: 1 (Enabled)
* **Access**: Read/Write
* **EPICS Records**: ``$(P)$(R)UseCountRate``, ``$(P)$(R)UseCountRate_RBV``

**SD_TAU** (ao/ai)
* **Description**: Deadtime constant for count rate correction
* **Units**: nanoseconds
* **Range**: -1.0 (use detector default) or > 0
* **Default**: -1.0
* **Access**: Read/Write
* **EPICS Records**: ``$(P)$(R)Tau``, ``$(P)$(R)Tau_RBV``
* **Note**: Set to -1 to use detector's default value

**SD_USE_BADCHANNEL_INTRPL** (bo/bi)
* **Description**: Enable/disable bad channel interpolation
* **Values**: 0 = Disable, 1 = Enable
* **Default**: 1 (Enabled)
* **Access**: Read/Write
* **EPICS Records**: ``$(P)$(R)UseBadChanIntrpl``, ``$(P)$(R)UseBadChanIntrpl_RBV``

Data Format
^^^^^^^^^^^

**SD_BIT_DEPTH** (mbbo/longin)
* **Description**: Bit depth for data readout
* **Values**:
  * 0 = 24 bits
  * 1 = 16 bits
  * 2 = 8 bits
  * 3 = 4 bits
* **Default**: 0 (24 bits)
* **Access**: Read/Write
* **EPICS Records**: ``$(P)$(R)BitDepth``, ``$(P)$(R)BitDepth_RBV``
* **Note**: Lower bit depths reduce data size but also reduce dynamic range

Gate Mode
^^^^^^^^^

**SD_USE_GATES** (bo/bi)
* **Description**: Enable/disable gate mode
* **Values**: 0 = Disable, 1 = Enable
* **Default**: 0 (Disabled)
* **Access**: Read/Write
* **EPICS Records**: ``$(P)$(R)UseGates``, ``$(P)$(R)UseGates_RBV``

**SD_NUM_GATES** (longout/longin)
* **Description**: Number of gates per frame
* **Range**: 1 to maximum supported by detector
* **Default**: 1
* **Access**: Read/Write
* **EPICS Records**: ``$(P)$(R)NumGates``, ``$(P)$(R)NumGates_RBV``
* **Note**: Only used when gate mode is enabled

System Parameters
^^^^^^^^^^^^^^^^^

**SD_NMODULES** (longin)
* **Description**: Number of detector modules (read-only)
* **Range**: 1 to 2
* **Access**: Read-only
* **EPICS Record**: ``$(P)$(R)NumModules_RBV``

**SD_RESET** (bo)
* **Description**: Reset detector (write-only)
* **Access**: Write-only
* **EPICS Record**: ``$(P)$(R)Reset``
* **Note**: Writing 1 to this record resets the detector

Parameter Reference Table
~~~~~~~~~~~~~~~~~~~~~~~~~

.. cssclass:: table-bordered table-striped table-hover
.. list-table::
  :header-rows: 1
  :widths: 15 12 10 8 15 8 30

  * - Parameter Name
    - EPICS Type
    - Data Type
    - Units
    - Range/Values
    - Access
    - Description
  * - SD_SETTING
    - mbbo/mbbi
    - Int32
    - -
    - 0-3 (Cu/Mo/Ag/Cr)
    - R/W
    - Detector setting
  * - SD_DELAY_TIME
    - ao/ai
    - Float64
    - s
    - 0.0-655.35
    - R/W
    - Delay after trigger
  * - SD_THRESHOLD
    - ao/ai
    - Float64
    - keV
    - 0.0-50.0
    - R/W
    - Energy threshold
  * - SD_ENERGY
    - ao/ai
    - Float64
    - keV
    - 0.0-50.0
    - R/W
    - Beam energy
  * - SD_USE_FLATFIELD
    - bo/bi
    - Int32
    - -
    - 0/1
    - R/W
    - Enable flat field
  * - SD_USE_COUNTRATE
    - bo/bi
    - Int32
    - -
    - 0/1
    - R/W
    - Enable count rate correction
  * - SD_TAU
    - ao/ai
    - Float64
    - ns
    - -1.0 or >0
    - R/W
    - Deadtime constant
  * - SD_USE_BADCHANNEL_INTRPL
    - bo/bi
    - Int32
    - -
    - 0/1
    - R/W
    - Enable bad channel interpolation
  * - SD_BIT_DEPTH
    - mbbo/longin
    - Int32
    - -
    - 0-3 (24/16/8/4 bits)
    - R/W
    - Bit depth
  * - SD_USE_GATES
    - bo/bi
    - Int32
    - -
    - 0/1
    - R/W
    - Enable gates
  * - SD_NUM_GATES
    - longout/longin
    - Int32
    - -
    - 1-max
    - R/W
    - Number of gates
  * - SD_NUM_FRAMES
    - longout/longin
    - Int32
    - -
    - 1-500
    - R/W
    - Number of frames
  * - SD_TRIGGER
    - mbbo/mbbi
    - Int32
    - -
    - 0-2 (None/Single/Continuous)
    - R/W
    - Trigger mode
  * - SD_READ_MODE
    - mbbo/mbbi
    - Int32
    - -
    - 0-1 (Raw/Corrected)
    - R/W
    - Read mode
  * - SD_NMODULES
    - longin
    - Int32
    - -
    - 1-2
    - R
    - Number of modules
  * - SD_FIRMWARE_VERSION
    - stringin
    - String
    - -
    - -
    - R
    - Firmware version
  * - SD_RESET
    - bo
    - Int32
    - -
    - 0/1
    - W
    - Reset detector

Operation
---------

Starting the IOC
~~~~~~~~~~~~~~~~

#. **Navigate to IOC directory:**

   ::

       cd /epics/support/areaDetector/ADMythen/iocs/mythenIOC/iocBoot/iocMythen

#. **Edit st.cmd** to configure detector IP address and other settings

#. **Start the IOC:**

   ::

       ./st.cmd

   Or use the run script::

       ./run

#. **Verify startup:**
   * Check for error messages
   * Verify detector connection
   * Check that records are loaded

Basic Acquisition
~~~~~~~~~~~~~~~~~

Single Image Acquisition
^^^^^^^^^^^^^^^^^^^^^^^^

#. **Set exposure time:**

   ::

       caput dp_mythen1K:cam1:AcquireTime 1.0  # 1 second exposure

#. **Set image mode to Single:**

   ::

       caput dp_mythen1K:cam1:ImageMode 0  # Single

#. **Start acquisition:**

   ::

       caput dp_mythen1K:cam1:Acquire 1

#. **Monitor status:**

   ::

       caget dp_mythen1K:cam1:Acquire
       caget dp_mythen1K:cam1:Status

#. **Read data** (via NDStdArrays plugin):

   ::

       caget dp_mythen1K:image1:ArrayData

Multiple Image Acquisition
^^^^^^^^^^^^^^^^^^^^^^^^^^

#. **Set number of frames:**

   ::

       caput dp_mythen1K:cam1:NumFrames 10

#. **Set image mode to Multiple:**

   ::

       caput dp_mythen1K:cam1:ImageMode 1  # Multiple

#. **Set acquire period** (time between frames):

   ::

       caput dp_mythen1K:cam1:AcquirePeriod 2.0  # 2 seconds between frames

#. **Start acquisition:**

   ::

       caput dp_mythen1K:cam1:Acquire 1

#. **Monitor progress:**

   ::

       caget dp_mythen1K:cam1:NumImagesCounter

Trigger Modes
~~~~~~~~~~~~~

None (Software Trigger)
^^^^^^^^^^^^^^^^^^^^^^^^

Default mode. Acquisition is started by software only::

    caput dp_mythen1K:cam1:TriggerMode 0  # None
    caput dp_mythen1K:cam1:Acquire 1       # Start acquisition

Single (External Trigger)
^^^^^^^^^^^^^^^^^^^^^^^^^^

One frame per external trigger::

    caput dp_mythen1K:cam1:TriggerMode 1  # Single
    caput dp_mythen1K:cam1:Acquire 1      # Arm detector
    # Detector waits for external trigger

Continuous (External Trigger)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Continuous acquisition on external triggers::

    caput dp_mythen1K:cam1:TriggerMode 2  # Continuous
    caput dp_mythen1K:cam1:ImageMode 2    # Continuous
    caput dp_mythen1K:cam1:Acquire 1      # Start continuous mode
    # Each external trigger acquires one frame

Data Corrections
~~~~~~~~~~~~~~~~

Flat Field Correction
^^^^^^^^^^^^^^^^^^^^^

Flat field correction compensates for channel-to-channel variations::

    # Enable flat field correction
    caput dp_mythen1K:cam1:UseFlatField 1

    # Disable flat field correction
    caput dp_mythen1K:cam1:UseFlatField 0

**Note**: Flat field calibration must be performed on the detector before use.

Count Rate Correction
^^^^^^^^^^^^^^^^^^^^^

Count rate correction compensates for deadtime effects at high count rates::

    # Enable count rate correction
    caput dp_mythen1K:cam1:UseCountRate 1

    # Set deadtime constant (nanoseconds)
    # Use -1 for detector default, or set specific value
    caput dp_mythen1K:cam1:Tau 1000.0  # 1000 ns deadtime

**When to use**: At high count rates (>100 kHz per channel typically)

Bad Channel Interpolation
^^^^^^^^^^^^^^^^^^^^^^^^^^

Interpolates values for known bad channels::

    # Enable bad channel interpolation
    caput dp_mythen1K:cam1:UseBadChanIntrpl 1

    # Disable (use raw values for bad channels)
    caput dp_mythen1K:cam1:UseBadChanIntrpl 0

Read Modes
~~~~~~~~~~

Raw Mode
^^^^^^^^

Uncorrected data directly from detector::

    caput dp_mythen1K:cam1:ReadMode 0  # Raw

**Use when**:
* You want to apply your own corrections
* Debugging detector issues
* Maximum data fidelity needed

Corrected Mode
^^^^^^^^^^^^^^

Data with detector corrections applied::

    caput dp_mythen1K:cam1:ReadMode 1  # Corrected

**Use when**:
* Standard operation
* Corrections are properly calibrated
* Simplified data processing

Energy Settings
~~~~~~~~~~~~~~~

Predefined Settings
^^^^^^^^^^^^^^^^^^^

Use predefined settings for common X-ray sources::

    # Copper K-alpha (8.05 keV)
    caput dp_mythen1K:cam1:Setting 0  # Cu

    # Molybdenum K-alpha (17.48 keV)
    caput dp_mythen1K:cam1:Setting 1  # Mo

    # Silver K-alpha (22.16 keV)
    caput dp_mythen1K:cam1:Setting 2  # Ag

    # Chromium K-alpha (5.41 keV)
    caput dp_mythen1K:cam1:Setting 3  # Cr

Custom Energy
^^^^^^^^^^^^^

Set custom energy values::

    # Set threshold energy
    caput dp_mythen1K:cam1:ThresholdEnergy 10.0  # 10 keV

    # Set beam energy (firmware 3.0+)
    caput dp_mythen1K:cam1:BeamEnergy 10.5  # 10.5 keV

Gate Mode Operation
~~~~~~~~~~~~~~~~~~~

Gate mode allows time-resolved measurements::

    # Enable gate mode
    caput dp_mythen1K:cam1:UseGates 1

    # Set number of gates per frame
    caput dp_mythen1K:cam1:NumGates 4  # 4 gates per frame

    # Start acquisition
    caput dp_mythen1K:cam1:Acquire 1

**Use cases**:
* Time-resolved experiments
* Pump-probe measurements
* Synchronized with external events

Bit Depth Selection
~~~~~~~~~~~~~~~~~~~

Choose bit depth based on dynamic range needs::

    # 24 bits (maximum dynamic range)
    caput dp_mythen1K:cam1:BitDepth 0

    # 16 bits (reduced data size)
    caput dp_mythen1K:cam1:BitDepth 1

    # 8 bits (minimal data size)
    caput dp_mythen1K:cam1:BitDepth 2

    # 4 bits (very limited dynamic range)
    caput dp_mythen1K:cam1:BitDepth 3

**Trade-offs**:
* Higher bit depth = larger data files, more dynamic range
* Lower bit depth = smaller files, less dynamic range

User Interface
--------------

MEDM Screens
~~~~~~~~~~~~

The driver includes MEDM (Motif Editor and Display Manager) screens:

**Main Control Screen**: ``mythenApp/op/adl/mythen.adl``

To launch::

    medm -x -macro "P=dp_mythen1K:,R=cam1:" mythenApp/op/adl/mythen.adl

Or use the provided script::

    ./start_medm_mythen

The screen includes:
* Acquisition controls
* Detector settings
* Energy and threshold controls
* Correction enable/disable
* Status display
* Data display

Other UI Formats
~~~~~~~~~~~~~~~~

The driver includes screens in multiple formats:

* **EDM**: ``mythenApp/op/edl/autoconvert/mythen.edl``
* **CSS/Bob**: ``mythenApp/op/bob/autoconvert/mythen.bob``
* **OPI**: ``mythenApp/op/opi/autoconvert/mythen.opi``
* **UI**: ``mythenApp/op/ui/autoconvert/mythen.ui``

Command Line Interface
~~~~~~~~~~~~~~~~~~~~~~

Use EPICS Channel Access command-line tools:

**Get parameter values:**

::

    caget dp_mythen1K:cam1:AcquireTime
    caget dp_mythen1K:cam1:Status
    caget dp_mythen1K:cam1:FirmwareVersion

**Set parameter values:**

::

    caput dp_mythen1K:cam1:AcquireTime 2.0
    caput dp_mythen1K:cam1:TriggerMode 1

**Monitor parameters:**

::

    camonitor dp_mythen1K:cam1:Status
    camonitor dp_mythen1K:cam1:NumImagesCounter

**Get array data:**

::

    caget dp_mythen1K:image1:ArrayData

Examples
--------

Example 1: Basic Single Image Acquisition
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Complete IOC configuration for basic operation:

**st.cmd:**

::

    < envPaths
    epicsEnvSet EPICS_CA_MAX_ARRAY_BYTES 64008

    dbLoadDatabase("$(TOP)/dbd/mythenApp.dbd")
    mythenApp_registerRecordDeviceDriver(pdbbase)

    drvAsynIPPortConfigure("IP_M1K", "192.168.0.90:1030 UDP", 0, 0, 1)
    asynOctetSetOutputEos("IP_M1K", 0, "\r")

    epicsEnvSet("PREFIX", "mythen:")
    epicsEnvSet("PORT", "SD1")
    epicsEnvSet("NCHANS", "1280")

    mythenConfig("SD1", "IP_M1K", -1, -1)
    dbLoadRecords("$(ADMYTHEN)/mythenApp/Db/mythen.template", 
                  "P=$(PREFIX),R=cam1:,PORT=$(PORT),ADDR=0,TIMEOUT=1")

    NDStdArraysConfigure("Image1", 3, 0, "$(PORT)", 0, 0)
    dbLoadRecords("$(ADCORE)/db/NDStdArrays.template", 
                  "P=$(PREFIX),R=image1:,PORT=Image1,ADDR=0,TIMEOUT=1,NDARRAY_PORT=$(PORT),NDARRAY_ADDR=0,TYPE=Float64,FTVL=DOUBLE,NELEMENTS=$(NCHANS)")

    iocInit()

**Acquisition sequence:**

::

    # Set 1 second exposure
    caput mythen:cam1:AcquireTime 1.0

    # Single image mode
    caput mythen:cam1:ImageMode 0

    # Start acquisition
    caput mythen:cam1:Acquire 1

    # Wait for completion, then read data
    caget mythen:image1:ArrayData

Example 2: Multiple Frames with External Trigger
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Configuration for triggered multi-frame acquisition::

    # Set trigger mode to Single
    caput mythen:cam1:TriggerMode 1

    # Set number of frames
    caput mythen:cam1:NumFrames 10

    # Set image mode to Multiple
    caput mythen:cam1:ImageMode 1

    # Set exposure time
    caput mythen:cam1:AcquireTime 0.5

    # Set delay after trigger
    caput mythen:cam1:DelayTime 0.1

    # Arm detector (waits for triggers)
    caput mythen:cam1:Acquire 1

    # Monitor progress
    camonitor mythen:cam1:NumImagesCounter

Example 3: Continuous Acquisition with Corrections
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Configuration for continuous acquisition with all corrections enabled::

    # Enable all corrections
    caput mythen:cam1:UseFlatField 1
    caput mythen:cam1:UseCountRate 1
    caput mythen:cam1:UseBadChanIntrpl 1

    # Set count rate correction parameters
    caput mythen:cam1:Tau 1000.0

    # Use corrected read mode
    caput mythen:cam1:ReadMode 1

    # Set energy for Cu K-alpha
    caput mythen:cam1:Setting 0  # Cu

    # Continuous mode
    caput mythen:cam1:ImageMode 2
    caput mythen:cam1:TriggerMode 2  # Continuous trigger

    # Start continuous acquisition
    caput mythen:cam1:Acquire 1

Example 4: Integration with File Plugin
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Save data automatically using NDPluginFile::

    # In st.cmd, load file plugin:
    NDFileHDF5Configure("HDF1", 50, 0, "$(PORT)", 0, 0, 0)
    dbLoadRecords("$(ADCORE)/db/NDFileHDF5.template", 
                  "P=$(PREFIX),R=HDF1:,PORT=HDF1,ADDR=0,TIMEOUT=1,NDARRAY_PORT=$(PORT),NDARRAY_ADDR=0")

    # Enable file saving
    caput mythen:HDF1:EnableCallbacks 1
    caput mythen:HDF1:FileWriteMode 2  # Stream mode

    # Set file path and name
    caput mythen:HDF1:FilePath "/data/mythen"
    caput mythen:HDF1:FileName "scan_001"
    caput mythen:HDF1:AutoIncrement 1
    caput mythen:HDF1:AutoSave 1

    # Start acquisition (files saved automatically)
    caput mythen:cam1:Acquire 1

Troubleshooting
---------------

Common Issues
~~~~~~~~~~~~~

Connection Problems
^^^^^^^^^^^^^^^^^^^

**Problem**: Cannot connect to detector

**Solutions**:

#. **Verify network connectivity:**

   ::

       ping 192.168.0.90

#. **Check IP address and port in st.cmd:**

   ::

       drvAsynIPPortConfigure("IP_M1K", "192.168.0.90:1030 UDP", 0, 0, 1)

#. **Verify detector is powered on and network configured**

#. **Check firewall settings** (port 1030 UDP should be open)

#. **Enable asyn tracing:**

   ::

       asynSetTraceIOMask("IP_M1K", 0, 6)  # Trace I/O
       asynSetTraceMask("IP_M1K", 0, 3)     # Trace errors

**Problem**: Connection timeout

**Solutions**:

#. Increase timeout in database template
#. Check network latency
#. Verify detector firmware is compatible

Acquisition Problems
^^^^^^^^^^^^^^^^^^^^

**Problem**: Acquisition times out

**Solutions**:

#. **Check exposure time:**

   ::

       caget mythen:cam1:AcquireTime

   Timeout is automatically set to ``M1K_TIMEOUT + AcquireTime``

#. **Verify trigger mode** (if using external trigger):

   ::

       caget mythen:cam1:TriggerMode

#. **Check detector status:**

   ::

       caget mythen:cam1:Status

#. **Increase timeout** if needed (modify ``M1K_TIMEOUT`` in source code)

**Problem**: No data received

**Solutions**:

#. **Check read mode:**

   ::

       caget mythen:cam1:ReadMode

#. **Verify number of modules:**

   ::

       caget mythen:cam1:NumModules_RBV

#. **Check array size configuration:**

   ::

       caget mythen:cam1:ArraySizeX

#. **Enable I/O tracing** to see communication

**Problem**: Wrong data values

**Solutions**:

#. **Check bit depth setting:**

   ::

       caget mythen:cam1:BitDepth

#. **Verify corrections are appropriate:**

   ::

       caget mythen:cam1:UseFlatField
       caget mythen:cam1:UseCountRate

#. **Check energy/threshold settings**

Configuration Errors
^^^^^^^^^^^^^^^^^^^^

**Problem**: Records not loading

**Solutions**:

#. **Verify database path:**

   ::

       dbLoadRecords("$(ADMYTHEN)/mythenApp/Db/mythen.template", ...)

#. **Check macro substitutions:**

   ::

       epicsEnvSet("PREFIX", "mythen:")
       epicsEnvSet("PORT", "SD1")

#. **Verify ADCore path** in RELEASE file

**Problem**: Plugin not working

**Solutions**:

#. **Check plugin configuration:**

   ::

       NDStdArraysConfigure("Image1", 3, 0, "$(PORT)", 0, 0)

#. **Verify NDARRAY_PORT** in plugin database:

   ::

       NDARRAY_PORT=$(PORT)

#. **Check EPICS_CA_MAX_ARRAY_BYTES:**

   ::

       epicsEnvSet EPICS_CA_MAX_ARRAY_BYTES 64008

Diagnostic Tools
~~~~~~~~~~~~~~~~

asyn Tracing
^^^^^^^^^^^^

Enable detailed I/O tracing::

    # In st.cmd or iocsh:
    asynSetTraceIOMask("IP_M1K", 0, 0xFFFF)  # Trace all I/O
    asynSetTraceMask("IP_M1K", 0, 0xFFFF)     # Trace all messages

Trace masks:
* **I/O Mask**: 1=read, 2=write, 4=flush, 8=connect/disconnect
* **General Mask**: 1=traceIO, 2=traceError, 4=traceWarning, 8=traceInfo, 16=traceFlow

EPICS Channel Access Tools
^^^^^^^^^^^^^^^^^^^^^^^^^^

**Monitor status:**

::

    camonitor mythen:cam1:Status
    camonitor mythen:cam1:Acquire

**Get all parameters:**

::

    caget mythen:cam1:*

**Check detector settings:**

::

    caget mythen:cam1:FirmwareVersion
    caget mythen:cam1:NumModules_RBV
    caget mythen:cam1:Setting_RBV

Network Debugging
^^^^^^^^^^^^^^^^^

**Test network connection:**

::

    # Test UDP connection
    nc -u 192.168.0.90 1030

    # Check if port is open
    nmap -sU -p 1030 192.168.0.90

Error Messages
~~~~~~~~~~~~~~

Common Status Codes
^^^^^^^^^^^^^^^^^^^

* **ADStatusIdle (0)**: Detector idle, ready for acquisition
* **ADStatusAcquire (1)**: Acquiring data
* **ADStatusReadout (2)**: Reading out data
* **ADStatusError (3)**: Error occurred

Firmware Compatibility
^^^^^^^^^^^^^^^^^^^^^^

The driver checks firmware version on startup. Some features (like ``SD_ENERGY``) require firmware 3.0 or later.

**Check firmware:**

::

    caget mythen:cam1:FirmwareVersion

Performance Optimization
~~~~~~~~~~~~~~~~~~~~~~~~

Memory Management
^^^^^^^^^^^^^^^^^

Configure buffer limits based on available memory::

    # Limit to 100 buffers, 100MB
    mythenConfig("SD1", "IP_M1K", 100, 100000000)

Network Optimization
^^^^^^^^^^^^^^^^^^^^^

* Use UDP for lower latency (default)
* Ensure low network latency (< 1 ms typical)
* Avoid network congestion

Thread Priority
^^^^^^^^^^^^^^^

Adjust thread priorities if needed (advanced)::

    mythenConfig("SD1", "IP_M1K", -1, -1, 50, 200000)
    # priority=50, stackSize=200000

Technical Reference
-------------------

Communication Protocol
~~~~~~~~~~~~~~~~~~~~~~~

The driver communicates with the Mythen detector using a text-based command protocol over TCP/IP.

Command Format
^^^^^^^^^^^^^^

Commands are sent as ASCII strings terminated with carriage return (``\r``)::

    -command [arguments]\r

Response Format
^^^^^^^^^^^^^^^

Responses are binary data:
* Integer responses: 4-byte signed integer (big-endian or little-endian)
* Float responses: 4-byte float (for tau command)
* String responses: ASCII string (for version command)
* Data readout: Binary array of integers

Supported Commands
^^^^^^^^^^^^^^^^^^

.. cssclass:: table-bordered table-striped table-hover
.. list-table::
  :header-rows: 1
  :widths: 25 50 25

  * - Command
    - Description
    - Response
  * - ``-start``
    - Start acquisition
    - Int32 (0=success)
  * - ``-stop``
    - Stop acquisition
    - Int32 (0=success)
  * - ``-time <hns>``
    - Set exposure time (100ns units)
    - Int32
  * - ``-delafter <hns>``
    - Set delay after trigger (100ns units)
    - Int32
  * - ``-frames <n>``
    - Set number of frames
    - Int32
  * - ``-trigen <0|1>``
    - Enable/disable single trigger
    - Int32
  * - ``-conttrigen <0|1>``
    - Enable/disable continuous trigger
    - Int32
  * - ``-readout``
    - Read corrected data
    - Binary array
  * - ``-readoutraw``
    - Read raw data
    - Binary array
  * - ``-kthresh <keV>``
    - Set threshold energy
    - Int32
  * - ``-energy <keV>``
    - Set beam energy (firmware 3.0+)
    - Int32
  * - ``-tau <ns>``
    - Set deadtime constant
    - Float32
  * - ``-flatfieldcorrection <0|1>``
    - Enable/disable flat field
    - Int32
  * - ``-ratecorrection <0|1>``
    - Enable/disable rate correction
    - Int32
  * - ``-badchannelinterpolation <0|1>``
    - Enable/disable bad channel interpolation
    - Int32
  * - ``-gateen <0|1>``
    - Enable/disable gates
    - Int32
  * - ``-gates <n>``
    - Set number of gates
    - Int32
  * - ``-module <n>``
    - Select module (0 or 1)
    - Int32
  * - ``-get nmodules``
    - Get number of modules
    - Int32
  * - ``-get version``
    - Get firmware version
    - String
  * - ``-get tau``
    - Get tau value
    - Float32
  * - ``-setting <0|1|2|3>``
    - Set predefined setting (Cu/Mo/Ag/Cr)
    - Int32
  * - ``-bitdepth <0|1|2|3>``
    - Set bit depth (24/16/8/4 bits)
    - Int32

Endianness Handling
^^^^^^^^^^^^^^^^^^^

The driver automatically handles byte order:
* Detector uses big-endian format
* Driver swaps bytes on little-endian systems
* Handled transparently by ``swap4()`` and ``swap8()`` functions

Data Format
~~~~~~~~~~~

NDArray Structure
^^^^^^^^^^^^^^^^^

* **Data Type**: ``NDInt32`` (32-bit signed integers)
* **Dimensions**: 1D array (1280 channels × number of modules)
* **Color Mode**: Monochrome (``NDColorModeMono``)
* **Array Size**:
  * Single module: 1280 channels
  * Two modules: 2560 channels

Data Encoding
^^^^^^^^^^^^^

* **Raw Mode**: Data encoded according to bit depth setting
* **Corrected Mode**: 24-bit data with corrections applied
* **Byte Order**: Native endianness (swapped if needed)

Array Attributes
^^^^^^^^^^^^^^^^

Each NDArray includes standard attributes:
* ``ColorMode``: Color mode (monochrome)
* ``UniqueId``: Unique frame identifier
* ``TimeStamp``: Acquisition timestamp
* Standard areaDetector attributes

Firmware Compatibility
~~~~~~~~~~~~~~~~~~~~~~~

Supported Versions
^^^^^^^^^^^^^^^^^^

* Firmware 2.x: Basic functionality
* Firmware 3.0+: Full functionality including energy command

Version Detection
^^^^^^^^^^^^^^^^^

Firmware version is read on driver initialization::

    -get version

Stored in ``SD_FIRMWARE_VERSION`` parameter.

Version-Specific Features
^^^^^^^^^^^^^^^^^^^^^^^^^

* **Energy command**: Requires firmware 3.0 or later
* **Read mode**: Available in all versions
* **Corrections**: Available in all versions

Limitations
~~~~~~~~~~~

Hardware Limits
^^^^^^^^^^^^^^^

* **Maximum modules**: 2 modules
* **Channels per module**: 1280 channels
* **Maximum frames**: 500 frames per acquisition
* **Bit depth**: 4, 8, 16, or 24 bits

Software Limits
^^^^^^^^^^^^^^^

* **Timeout**: 5.0 seconds base timeout (``M1K_TIMEOUT``)
* **Maximum trigger timeout count**: 50 retries
* **Array size**: Fixed at 1280 × number of modules

Platform Limitations
^^^^^^^^^^^^^^^^^^^^

* **Windows**: Full support
* **Linux**: Full support (little-endian and big-endian)
* **Other platforms**: May require porting

API Reference
~~~~~~~~~~~~~

mythenConfig()
^^^^^^^^^^^^^^

Configure the Mythen driver::

    int mythenConfig(const char *portName, 
                     const char *IPPortName,
                     int maxBuffers,
                     int maxMemory,
                     int priority = 0,
                     int stackSize = 0)

**Parameters:**
* ``portName``: Driver port name
* ``IPPortName``: asyn IP port name
* ``maxBuffers``: Maximum NDArray buffers (-1 = unlimited)
* ``maxMemory``: Maximum memory in bytes (-1 = unlimited)
* ``priority``: Thread priority (optional)
* ``stackSize``: Thread stack size (optional)

**Returns:** 0 on success

Class Methods (for developers)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Key methods in the ``mythen`` class:
* ``setAcquire()``: Start/stop acquisition
* ``setExposureTime()``: Set exposure time
* ``setTrigger()``: Set trigger mode
* ``setFrames()``: Set number of frames
* ``dataCallback()``: Process acquired data
* ``acquisitionTask()``: Acquisition thread
* ``pollTask()``: Status polling thread

Appendices
----------

Appendix A: File Structure
~~~~~~~~~~~~~~~~~~~~~~~~~~~

::

    ADMythen/
    ├── configure/              # Build configuration
    │   ├── RELEASE            # EPICS module paths
    │   ├── CONFIG            # Build configuration
    │   └── ...
    ├── iocs/                  # IOC applications
    │   └── mythenIOC/
    │       ├── configure/     # IOC configuration
    │       ├── iocBoot/       # IOC startup scripts
    │       │   └── iocMythen/
    │       │       ├── st.cmd              # Main startup script
    │       │       └── auto_settings.req    # Autosave request file
    │       └── mythenApp/
    │           └── src/
    │               └── mythenAppMain.cpp    # IOC main
    ├── mythenApp/             # Driver application
    │   ├── Db/                # Database templates
    │   │   ├── mythen.template             # Main database template
    │   │   └── mythen_settings.req          # Autosave settings
    │   ├── op/                # Operator interface files
    │   │   ├── adl/           # MEDM screens
    │   │   │   └── mythen.adl
    │   │   ├── edl/           # EDM screens
    │   │   ├── bob/           # CSS/Bob screens
    │   │   ├── opi/           # OPI screens
    │   │   └── ui/            # UI screens
    │   └── src/               # Source code
    │       ├── mythen.cpp                    # Main driver implementation
    │       └── mythenSupport.dbd             # Database definitions
    ├── LICENSE                # License file
    ├── README.md              # This file
    └── RELEASE.md             # Release notes

Appendix B: Database Template Reference
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The main database template is ``mythenApp/Db/mythen.template``. It includes:

* Standard ADBase records (from ``ADBase.template``)
* Mythen-specific records for all driver parameters
* Readback records (RBV) for all writable parameters

**Key Records:**

.. cssclass:: table-bordered table-striped table-hover
.. list-table::
  :header-rows: 1
  :widths: 30 15 55

  * - Record Name
    - Type
    - Description
  * - ``$(P)$(R)Setting``
    - mbbo
    - Detector setting
  * - ``$(P)$(R)DelayTime``
    - ao
    - Delay after trigger
  * - ``$(P)$(R)ThresholdEnergy``
    - ao
    - Energy threshold
  * - ``$(P)$(R)BeamEnergy``
    - ao
    - Beam energy
  * - ``$(P)$(R)UseFlatField``
    - bo
    - Enable flat field
  * - ``$(P)$(R)UseCountRate``
    - bo
    - Enable count rate correction
  * - ``$(P)$(R)Tau``
    - ao
    - Deadtime constant
  * - ``$(P)$(R)UseBadChanIntrpl``
    - bo
    - Enable bad channel interpolation
  * - ``$(P)$(R)BitDepth``
    - mbbo
    - Bit depth
  * - ``$(P)$(R)UseGates``
    - bo
    - Enable gates
  * - ``$(P)$(R)NumGates``
    - longout
    - Number of gates
  * - ``$(P)$(R)NumFrames``
    - longout
    - Number of frames
  * - ``$(P)$(R)TriggerMode``
    - mbbo
    - Trigger mode
  * - ``$(P)$(R)ReadMode``
    - mbbo
    - Read mode
  * - ``$(P)$(R)NumModules_RBV``
    - longin
    - Number of modules (read-only)
  * - ``$(P)$(R)FirmwareVersion``
    - stringin
    - Firmware version (read-only)

All writable records have corresponding ``_RBV`` readback records.

Appendix C: Build System
~~~~~~~~~~~~~~~~~~~~~~~~

Makefile Structure
^^^^^^^^^^^^^^^^^^

The build system uses EPICS standard Makefiles:

* **Top-level Makefile**: Builds driver and IOC
* **mythenApp/Makefile**: Builds driver library
* **iocs/mythenIOC/Makefile**: Builds IOC application

Build Targets
^^^^^^^^^^^^^

::

    make                    # Build everything
    make install           # Install libraries and binaries
    make clean             # Clean build files
    make uninstall         # Remove installed files

Configuration Files
^^^^^^^^^^^^^^^^^^^

* **configure/RELEASE**: EPICS module paths
* **configure/CONFIG**: Build configuration
* **configure/CONFIG_SITE**: Site-specific configuration

Appendix D: Glossary
~~~~~~~~~~~~~~~~~~~~

* **ADDriver**: areaDetector base driver class
* **asyn**: EPICS asynchronous I/O library
* **EPICS**: Experimental Physics and Industrial Control System
* **IOC**: Input/Output Controller (EPICS server)
* **NDArray**: areaDetector data array format
* **NDPlugin**: areaDetector plugin for data processing
* **PV**: Process Variable (EPICS record)
* **RBV**: Readback Value (read-only copy of parameter)
* **Strip Detector**: One-dimensional detector with linear array of pixels

Appendix E: References
~~~~~~~~~~~~~~~~~~~~~~

areaDetector Documentation
^^^^^^^^^^^^^^^^^^^^^^^^^^

* `areaDetector Homepage <https://areadetector.github.io/areaDetector/>`__
* `ADCore Documentation <https://areadetector.github.io/areaDetector/ADCore/ADCore.html>`__
* `Installation Guide <https://areadetector.github.io/areaDetector/install_guide.html>`__

Related Drivers
^^^^^^^^^^^^^^^

* `ADPilatus <https://github.com/areaDetector/ADPilatus>`__: Dectris Pilatus detector driver
* `ADLambda <https://github.com/areaDetector/ADLambda>`__: X-Spectrum Lambda detector driver

EPICS Documentation
^^^^^^^^^^^^^^^^^^^

* `EPICS Homepage <https://epics.anl.gov/>`__
* `EPICS Application Developer's Guide <https://epics.anl.gov/base/R3-16/0-docs/AppDevGuide/index.html>`__
* `asyn Driver Support <https://epics-modules.github.io/asyn/>`__

Detector Documentation
^^^^^^^^^^^^^^^^^^^^^^

* `Dectris Website <http://www.dectris.com>`__
* Mythen detector user manual (contact Dectris)

Appendix F: Release Notes
~~~~~~~~~~~~~~~~~~~~~~~~~

See `RELEASE.md <https://github.com/areaDetector/ADMythen/blob/master/RELEASE.md>`__ for detailed release notes and version history.

Version R2-1 (December 11, 2019)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

* Updated for areaDetector R3-3 compatibility
* Minor debugging improvements

Version R2-0 (July 4, 2017)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

* Windows compilation support
* Big-endian machine support
* Fixed error in writeFloat64
* Updated medm screen for ADCore R3-0

Version 1-0 (August 10, 2015)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

* Initial release
* Based on LNLS asyn driver
* areaDetector integration by Joe Sullivan (ANL-APS)

