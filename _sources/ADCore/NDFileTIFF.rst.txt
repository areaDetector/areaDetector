NDFileTIFF
==========

:author: Mark Rivers, University of Chicago

.. contents:: Contents

Overview
--------

NDFileTIFF inherits from NDPluginFile. This plugin saves data in the
`TIFF <http://en.wikipedia.org/wiki/Tagged_Image_File_Format>`__ file
format, which is a popular file format for storing images. There is TIFF
support for almost all languages and programs such as IDL and Matlab.

The TIFF plugin supports all 8 NDArray data types (signed and unsigned
8, 16, 32, 64 bit integers, 32 and 64 bit floating point. It supports all
color modes (Mono, RGB1, RGB2, and RGB3). Note that many TIFF readers do
not support 16, 32 or 64 bit integer TIFF files, floating point TIFF files,
and 16 or 32 bit color files. NDFileTIFF is limited to a single array
per file, but capture and stream mode are supported by writing multiple
TIFF files.

Tests were done with IDL, ImageJ, and the Python Imaging Library (PIL)
to read TIFF files with all 10 data types. IDL can read all 10 types,
although it does not support signed 8-bit integers so it treats them as
unsigned. ImageJ can read all types except NDFloat64, NDInt64, and NDUInt64.
PIL cannot read NDInt8, NDUInt32, or NDFloat64.

The TIFF plugin writes four NDArray class members as TIFF tags starting
with tag number 65000. For historical reasons these tags do not have labels.
They are the following NDArray class members converted to ASCII:

::

    -  65000: timeStamp
    -  65001: uniqueID
    -  65002: epicsTS (seconds)
    -  65003: epicsTS (nano-seconds)

The TIFF plugin also writes all NDAttributes attached to the NDArray as TIFF
ASCII file tags, up to a maximum of 536 attributes. These tags start with
number 65010. The NDAttributes are converted to ASCII with attribute name,
followed by a colon and the attribute value. This information can be displayed,
for example, with the tiffinfo program:

::

     >tiffinfo ~/scratch/test_001.tif
     TIFF Directory at offset 0x400008 (4194312)
     Image Width: 1024 Image Length: 1024
     Bits/Sample: 32
     Sample Format: IEEE floating point
     Compression Scheme: None
     Photometric Interpretation: min-is-black
     Samples/Pixel: 1
     Rows/Strip: 1024
     Planar Configuration: single image plane
     Make: Unknown
     Model: Unknown
     Software: EPICS areaDetector
     Tag 65000: 854478417.972736
     Tag 65001: 3059
     Tag 65002: 854478418
     Tag 65003: 1977554
     Tag 65010: ColorMode:0
     Tag 65011: RingCurrent:100.010157
     Tag 65012: RingCurrent_EGU:mA
     Tag 65013: ID_Energy:42.181675
     Tag 65014: ID_Energy_EGU:keV
     Tag 65015: ImageCounter:3059
     Tag 65016: MaxSizeX:1024
     Tag 65017: MaxSizeY:1024
     Tag 65018: CameraModel:Basic simulator
     Tag 65019: AttributesFileParam:simDetectorAttributes.xml
     Tag 65020: AttributesFileNative:1139
     Tag 65021: AttributesFileString:simDetectorAttributes.xml
     Tag 65022: CameraManufacturer:Simulated detector
     Tag 65023: Pi:3.141593
     Tag 65024: E:2.718282
     Tag 65025: Ten:10
     Tag 65026: Gettysburg:Four score and seven years ago our fathers

       

If there is an NDAttribute of type NDAttrString named
TIFFImageDescription then this attribute is written to the
TIFFTAG_IMAGEDESCRIPTION tag in the TIFF file. This is the output of
tiffinfo on a file written by the TIFF plugin. The data is from a
Pilatus detector. The areaDetector ADPilatus driver reads the
TIFFTAG_IMAGEDESCRIPTION data from the original TIFF file that camserver
writes, and adds it to the NDArray as the TIFFImageDescription
attribute. Note that the ImageDescription data is present twice, both in
the standard TIFF ImageDesciption tag and in user tag 65010 in the TIFF
file with all other NDAttributes. This is OK because some data
processing code may expect to find the information in one location or
the other.

::

   >tiffinfo ~/scratch/pilatus_test_000.tif
   TIFF Directory at offset 0x5cbdc (379868)
     Image Width: 487 Image Length: 195
     Bits/Sample: 32
     Sample Format: signed integer
     Compression Scheme: None
     Photometric Interpretation: min-is-black
     Samples/Pixel: 1
     Rows/Strip: 195
     Planar Configuration: single image plane
     ImageDescription: # Pixel_size 172e-6 m x 172e-6 m
   # Silicon sensor, thickness 0.000320 m
   # Exposure_time 1.0000000 s
   # Exposure_period 1.0000000 s
   # Tau = 203.0e-09 s
   # Count_cutoff 1390572 counts
   # Threshold_setting: 7000 eV
   # Gain_setting: mid gain (vrf = -0.200)
   # N_excluded_pixels = 0
   # Excluded_pixels: (nil)
   # Flat_field: (nil)
   # Trim_file: p100k0118_E10000_T7000_vrf_m0p20.bin
   # Image_path: .../Data/gpd_user/data/idc/2017/run1/setup_603/setup/images/
   # Wavelength 1.54000 A
   # Energy_range (0, 0) eV
   # Detector_distance 1.00000 m
   # Detector_Voffset 0.00000 m
   # Beam_xy (0.00, 0.00) pixels
   # Flux 0.000000
   # Filter_transmission 1.0000
   # Start_angle 0.0000 deg.
   # Angle_increment 0.1000 deg.
   # Detector_2theta 0.0000 deg.
   # Polarization 0.990
   # Alpha 0.0000 deg.
   # Kappa 0.0000 deg.
   # Phi 0.0000 deg.
   # Phi_increment 0.1000 deg.
   # Omega 0.0000 deg.
   # Omega_increment 0.1000 deg.
   # Chi 0.0000 deg.
   # Chi_increment 0.1000 deg.
   # Oscillation_axis X, CW
   # N_oscillations 1

     Make: Unknown
     Model: Unknown
     Software: EPICS areaDetector
     Tag 65000: 854479056.813133
     Tag 65001: 1
     Tag 65002: 854479058
     Tag 65003: 208583261
     Tag 65010: TIFFImageDescription:# Pixel_size 172e-6 m x 172e-6 m
   # Silicon sensor, thickness 0.000320 m
   # Exposure_time 1.0000000 s
   # Exposure_period 1.0000000 s
   # Tau = 203.0e-09 s
   # Count_cutoff 1390572 counts
   # Threshold_setting: 7000 eV
   # Gain_setting: mid gain (vrf = -0.200)
   # N_excluded_pixels = 0
   # Excluded_pixels: (nil)
   # Flat_field: (nil)
   # Trim_file: p100k0118_E10000_T7000_vrf_m0p20.bin
   # Image_path: .../Data/gpd_user/data/idc/2017/run1/setup_603/setup/images/
   # Wavelength 1.54000 A
   # Energy_range (0, 0) eV
   # Detector_distance 1.00000 m
   # Detector_Voffset 0.00000 m
   # Beam_xy (0.00, 0.00) pixels
   # Flux 0.000000
   # Filter_transmission 1.0000
   # Start_angle 0.0000 deg.
   # Angle_increment 0.1000 deg.
   # Detector_2theta 0.0000 deg.
   # Polarization 0.990
   # Alpha 0.0000 deg.
   # Kappa 0.0000 deg.
   # Phi 0.0000 deg.
   # Phi_increment 0.1000 deg.
   # Omega 0.0000 deg.
   # Omega_increment 0.1000 deg.
   # Chi 0.0000 deg.
   # Chi_increment 0.1000 deg.
   # Oscillation_axis X, CW
   # N_oscillations 1
     

The `NDFileNetTIFF class
documentation <../areaDetectorDoxygenHTML/class_n_d_file_t_i_f_f.html>`__
describes this class in detail.

Configuration
-------------

The NDFileTIFF plugin is created with the ``NDFileTIFFConfigure`` command,
either from C/C++ or from the EPICS IOC shell.

.. code-block:: c

   NDFileTIFFConfigure (const char *portName, int queueSize, int blockingCallbacks, 
                        const char *NDArrayPort, int NDArrayAddr, size_t maxMemory, 
                        int priority, int stackSize)
     

For details on the meaning of the parameters to this function refer to
the detailed documentation on the NDFileTIFFConfigure function in the
`NDFileTIFF.cpp
documentation <../areaDetectorDoxygenHTML/_n_d_file_t_i_f_f_8cpp.html>`__
and in the documentation for the constructor for the `NDFileTIFF
class <../areaDetectorDoxygenHTML/class_n_d_file_t_i_f_f.html>`__.

Screen Shots
------------

.. figure:: NDFileTIFF.png
    :align: center

