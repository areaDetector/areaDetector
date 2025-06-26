ADTLBC2
=======


.. _ADDriver: https://areadetector.github.io/areaDetector/ADCore/ADDriver.html
.. _areaDetector: https://areadetector.github.io/areaDetector/index.html
.. _EPICS: http://www.aps.anl.gov/epics
.. _asynNDArrayDriver: https://areadetector.github.io/areaDetector/ADCore/NDArray.html#asynndarraydriver
.. _SDK: https://www.thorlabs.com/software_pages/ViewSoftwarePage.cfm?code=Beam
.. _BC210CU/M: https://www.thorlabs.com/thorproduct.cfm?partnumber=BC210CU/M
.. _profilers: https://www.thorlabs.com/newgrouppage9.cfm?objectgroup_id=3483

.. contents:: Contents

Overview
------------

This is an `areaDetector`_ driver for the Thorlabs ThorLabs BC207 and BC210
beam `profilers`_ using the TLBC2 library provided along with the
manufacturer's `SDK`_. It has been tested on the `BC210CU/M`_ camera, but
should work for the entire model family.

The BC207 and BC210 profilers are CMOS camera beam profilers designed for beam
analysis over the 245-400nm wavelength range (UV series) or 350-1100nm
wavelength range (VIS series). The main difference between the BC207 and BC210
families is the resolution: 5.0 MP and 12.3 MP, respectively. All detectors
include a front-mounted filter wheel with six neutral density (ND) filters to
attenuate the beam.

These detectors also have built-in calculations for profiling the beam, such
as: width and centroid along the X and Y axes and saturation, among others; as
well as preprocessing steps such as: bad pixel correction, ambient light
correction, etc. These calculations can be accessed and controlled via
driver-speficis PVs listed in the `ADTLBC2 specific parameters`_ section.

Implementation of standard driver parameters
--------------------------------------------

The following list contains all the EPICS record names of parameters defined in
`ADDriver`_ and `asynNDArrayDriver`_ which are currently supported by the ADTLBC2
driver.

* $(P)$(R)TemperatureActual
* $(P)$(R)Gain
* $(P)$(R)MaxSizeX_RBV
* $(P)$(R)MaxSizeY_RBV
* $(P)$(R)MinX
* $(P)$(R)MinY
* $(P)$(R)SizeX
* $(P)$(R)SizeY
* $(P)$(R)ImageMode
* $(P)$(R)AcquireTime
* $(P)$(R)AcquirePeriod
* $(P)$(R)NumImages
* $(P)$(R)NumImagesCounter_RBV
* $(P)$(R)DetectorState_RBV
* $(P)$(R)StatusMessage_RBV
* $(P)$(R)Manufacturer_RBV
* $(P)$(R)Model_RBV
* $(P)$(R)SerialNumber_RBV
* $(P)$(R)FirmwareVersion_RBV
* $(P)$(R)SDKVersion_RBV
* $(P)$(R)Acquire
* $(P)$(R)ArrayCounter_RBV

The following table describes standard driver parameters which have specific
implementations and behaviors inside ADTLBC2.

.. cssclass:: table-bordered table-striped table-hover
.. list-table::
  :header-rows: 1
  :widths: 10 120

  * - EPICS record name
    - Description
  * - $(P)$(R)Gain
    - Detector gain in dB. Max value is 12 and min is 0.
  * - $(P)$(R)MinX
    - Sets the first pixel to be read in the X direction and updates the ROI.
  * - $(P)$(R)MinY
    - Sets the first pixel to be read in the Y direction and updates the ROI.
  * - $(P)$(R)SizeX
    - Sets the size of the region to be read in the X direction and updates the ROI.
  * - $(P)$(R)SizeY
    - Sets the size of the region to be read in the Y direction and updates the ROI.
  * - $(P)$(R)StatusMessage_RBV
    - Contains the last error's message.

.. _ADTLBC2 specific parameters:

ADTLBC2 specific parameters
---------------------------

The TLBC2 driver implements the following parameters in addition to
those in `ADDriver`_ and `asynNDArrayDriver`_.

.. cssclass:: table-bordered table-striped table-hover
.. list-table::
  :header-rows: 1
  :widths: 15 55 15 15

  * - drvInfo string
    - Description
    - EPICS record name
    - EPICS record type
  * - COMPUTE_AMBIENT_LIGHT_CORRECTION
    - Compute ambient light correction. Toggled back to 0 once it has finished computing.
    - $(P)$(R)ComputeAmbientLightCorrection, $(P)$(R)ComputeAmbientLightCorrection_RBV
    - bo, bi
  * - AMBIENT_LIGHT_CORRECTION
    - Ambient light correction mode toggle. Can only be enabled if AMBIENT_LIGHT_CORRECTION_STATUS is "Computed", in other words, after ambient light correction has been computed at least once. Disabled by default.
    - $(P)$(R)AmbientLightCorrection, $(P)$(R)AmbientLightCorrection_RBV
    - bo, bi
  * - AMBIENT_LIGHT_CORRECTION_STATUS
    - Ambient light correction status. Can either be "Never run" or "Computed". Starting value is "Never run" and becomes "Computed" once ambient light correction has been computed for the first time. This status is used to determine whether ambient light correction can be applied to the next acquisition or not.
    - $(P)$(R)AmbientLightCorrectionStatus_RBV
    - bi
  * - ATTENUATION
    - Attenuation in dB. Allowed range is 0-100. Default value is 0.
    - $(P)$(R)Attenuation, $(P)$(R)Attenuation_RBV
    - ao, ai
  * - AUTO_EXPOSURE
    - Auto exposure toggle. Uses latest data to set automatic exposure. Disabled by default.
    - $(P)$(R)AutoExposure, $(P)$(R)AutoExposure_RBV
    - bo, bi
  * - AUTO_CALC_AREA_CLIP_LEVEL
    - Auto calc area clip level. Default is 0.01.
    - $(P)$(R)AutoCalcAreaClipLevel, $(P)$(R)AutoCalcAreaClipLevel_RBV
    - ao, ai
  * - AMBIENT_LIGHT_CORRECTION
    - Ambient light correction mode toggle. Can either be enabled or disabled. Disabled by default.
    - $(P)$(R)AmbientLightCorrection
    - bo, bi
  * - BEAM_WIDTH_X
    - Beam width at clip level in X asis.
    - $(P)$(R)BeamWidthX_RBV
    - ai
  * - BEAM_WIDTH_Y
    - Beam width at clip level in Y axis.
    - $(P)$(R)BeamWidthY_RBV
    - ai
  * - CENTROID_X
    - Centroid position in X axis.
    - $(P)$(R)CentroidX_RBV
    - ai
  * - CENTROID_Y
    - Centroid position in Y axis.
    - $(P)$(R)CentroidY_RBV
    - ai
  * - CLIP_LEVEL
    - Clip level used for beam statistics. Allowed range is 0-1. Default value is 0.135.
    - $(P)$(R)ClipLevel, $(P)$(R)ClipLevel_RBV
    - ai, ao
  * - COMPUTE_AMBIENT_LIGHT_CORRECTION
    - Change ambient light correction mode (enabled or disabled).
    - $(P)$(R)AmbientLightCorrection, $(P)$(R)AmbientLightCorrection_RBV
    - bi, bo
  * - SATURATION
    - Ratio of the maximum intensity used.
    - $(P)$(R)Saturation_RBV
    - ai
  * - WAVELENGTH
    - Set wavelength in nanometers. Allowed range is 245-400nm. Default value is 245nm.
    - $(P)$(R)Wavelength, $(P)$(R)Wavelength_RBV
    - ao, ai


IOC Startup Script
------------------

The command to configure an TLBC2 camera in the startup script is::

  TLBC2Config(const char *portName, int maxSizeX, int maxSizeY, int maxMemory, int reset)

``portName`` is the name for the TLBC2 port driver

``maxSizeX`` is the maximum sensor dimension in the X axis.

``maxSizeY`` is the maximum sensor dimension in the Y axis.

``maxMemory`` is the maximum amount of memory the NDArrayPool is allowed to allocate.  0 means unlimited.

``reset`` whether to reset device or not.

Restrictions
------------

Since the libraries are provided by the vendor and are only implemented for
Windows, this driver does not work on Linux.

This driver always connects to the first device it finds, therefore it's not
currently possible to have two cameras and one IOC for each in the same
machine.

This driver should work with all detectors from the BC210 and BC207 series,
however it has only been tested on the `BC210CU/M`_ model so far.

Compiling on Windows
--------------------

The driver and firmware versions that have been tested and are known to work
are 2.1 and 1.0.25, respectively.

The Makefiles use the build variables `THORLABS_INC` and `THORLABS_LIB` to find
the relevant libraries and headers. Once the SDK has been installed, set these
variables to point to their respective directories in a `RELEASE.local` file.

* `THORLABS_INC` must point to the SDK include directory (called `Include`).
* `THORLABS_LIB` must point to the 64-bit library directory (called `msc`).

Note that GNU Make does not deal with whitespace in file/folder names well,
therefore the driver may have to be moved from its standard install location
(inside `Program Files`) to another directory which contains no whitespace in
order for GNU make to work.
