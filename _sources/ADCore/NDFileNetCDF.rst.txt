.. _NDFileNetCDF:

NDFileNetCDF
============

:author: Mark Rivers, University of Chicago

.. contents:: Contents

Overview
--------

NDFileNetCDF inherits from NDPluginFile. This plugin saves data in the
`netCDF <http://www.unidata.ucar.edu/software/netcdf/>`__ file format,
which is a portable self-describing binary file format supported by
`UniData <http://www.unidata.ucar.edu/>`__ at `UCAR (University
Corporation for Atmospheric Research). <http://www.ucar.edu/>`__ There
are netCDF libraries for C, C++, Fortran, and Java. Other languages,
including Matlab and IDL have built-in support for netCDF. There are
also add-on interfaces available for Python, Ruby and other languages.

The netCDF plugin supports all NDArray data types and any number of
array dimensions. It also has full support for NDArray attributes. It
will write all attributes associated with the NDArray to the file. If
multiple arrays are written to a single netCDF file (stream or capture
mode) then each attribute will be an array, with the attribute value for
each NDArray in the file being stored. Note that the number and data
types of attributes must not be changed while file capture or file
streaming are in progress because that would change the structure of the
attribute array. Also the colorMode attribute must not be changed while
capture or streaming is in progress, because that would change the
structure of the NDArray data.

The netCDF classic file format does not handle 64-bit integer data, so cannot
actually store NDArrays with types NDInt64 or NDUInt64, or NDAttributes of type
NDAttrInt64 or NDAttrUInt64. This plugin works around this restriction by casting the data to float64,
so the netCDF library thinks it is writing float64 (double) data, and marks the netCDF data in in the file as double.
However the netCDF global attribute "datatype" (enum NDDataType_t) will correctly indicate the actual datatype.
File readers will need to cast the data to the actual datatype after reading the data with the netCDF
library functions.

The `NDFileNetCDF class
documentation <../areaDetectorDoxygenHTML/class_n_d_file_net_c_d_f.html>`__
describes this class in detail.

Configuration
-------------

The NDFileNetCDF plugin is created with the ``NDFileNetCDFConfigure``
command, either from C/C++ or from the EPICS IOC shell.

.. code-block:: c

   NDFileNetCDFConfigure (const char *portName, int queueSize, int blockingCallbacks, 
                          const char *NDArrayPort, int NDArrayAddr, size_t maxMemory, 
                          int priority, int stackSize)
     

For details on the meaning of the parameters to this function refer to
the detailed documentation on the NDFileNetCDFConfigure function in the
`NDFileNetCDF.cpp
documentation <../areaDetectorDoxygenHTML/_n_d_file_net_c_d_f_8cpp.html>`__
and in the documentation for the constructor for the `NDFileNetCDF
class <../areaDetectorDoxygenHTML/class_n_d_file_net_c_d_f.html>`__.

The following is the header contents of a netCDF file produced by this
plugin. This information was produced with the following command:

::

   ncdump -h test_netCDF_68.nc

   netcdf test_netCDF_68 {
   dimensions:
       numArrays = UNLIMITED ; // (10 currently)
       dim0 = 240 ;
       dim1 = 320 ;
       dim2 = 1 ;
       attrStringSize = 256 ;
   variables:
       int uniqueId(numArrays) ;
       double timeStamp(numArrays) ;
       float array_data(numArrays, dim0, dim1, dim2) ;
       int Attr_colorMode(numArrays) ;
       double Attr_AcquireTime(numArrays) ;
       double Attr_RingCurrent(numArrays) ;
       int Attr_ImageCounter(numArrays) ;
       char Attr_CameraModel(numArrays, attrStringSize) ;
       int Attr_BinX(numArrays) ;
       int Attr_BinY(numArrays) ;
       double Attr_AttrTimeStamp(numArrays) ;
       double Attr_ROI0Mean(numArrays) ;
       double Attr_ROI1Mean(numArrays) ;
       char Attr_FilePath(numArrays, attrStringSize) ;
       char Attr_FileName(numArrays, attrStringSize) ;

   // global attributes:
           :dataType = 6 ;
           :NDNetCDFFileVersion = 3. ;
           :numArrayDims = 3 ;
           :dimSize = 1, 320, 240 ;
           :dimOffset = 0, 0, 0 ;
           :dimBinning = 1, 2, 2 ;
           :dimReverse = 0, 0, 0 ;
           :Attr_colorMode_DataType = "Int32" ;
           :Attr_colorMode_Description = "Color mode" ;
           :Attr_colorMode_Source =  ;
           :Attr_colorMode_SourceType = "Driver" ;
           :Attr_AcquireTime_DataType = "Float64" ;
           :Attr_AcquireTime_Description = "Camera acquire time" ;
           :Attr_AcquireTime_Source = "13SIM1:cam1:AcquireTime" ;
           :Attr_AcquireTime_SourceType = "EPICS_PV" ;
           :Attr_RingCurrent_DataType = "Float64" ;
           :Attr_RingCurrent_Description = "Storage ring current" ;
           :Attr_RingCurrent_Source = "S:SRcurrentAI" ;
           :Attr_RingCurrent_SourceType = "EPICS_PV" ;
           :Attr_ImageCounter_DataType = "Int32" ;
           :Attr_ImageCounter_Description = "Image counter" ;
           :Attr_ImageCounter_Source = "ARRAY_COUNTER" ;
           :Attr_ImageCounter_SourceType = "Param" ;
           :Attr_CameraModel_DataType = "String" ;
           :Attr_CameraModel_Description = "Camera model" ;
           :Attr_CameraModel_Source = "MODEL" ;
           :Attr_CameraModel_SourceType = "Param" ;
           :Attr_BinX_DataType = "Int32" ;
           :Attr_BinX_Description = "X binning" ;
           :Attr_BinX_Source = "13SIM1:ROI1:0:BinX_RBV" ;
           :Attr_BinX_SourceType = "EPICS_PV" ;
           :Attr_BinY_DataType = "Int32" ;
           :Attr_BinY_Description = "Y binning" ;
           :Attr_BinY_Source = "13SIM1:ROI1:0:BinY_RBV" ;
           :Attr_BinY_SourceType = "EPICS_PV" ;
           :Attr_AttrTimeStamp_DataType = "Float64" ;
           :Attr_AttrTimeStamp_Description = "Time stamp" ;
           :Attr_AttrTimeStamp_Source = "TIME_STAMP" ;
           :Attr_AttrTimeStamp_SourceType = "Param" ;
           :Attr_ROI0Mean_DataType = "Float64" ;
           :Attr_ROI0Mean_Description = "Mean value ROI 0" ;
           :Attr_ROI0Mean_Source = "MEAN_VALUE" ;
           :Attr_ROI0Mean_SourceType = "Param" ;
           :Attr_ROI1Mean_DataType = "Float64" ;
           :Attr_ROI1Mean_Description = "Mean value ROI 0" ;
           :Attr_ROI1Mean_Source = "MEAN_VALUE" ;
           :Attr_ROI1Mean_SourceType = "Param" ;
           :Attr_FilePath_DataType = "String" ;
           :Attr_FilePath_Description = "File path" ;
           :Attr_FilePath_Source = "13SIM1:netCDF1:FilePath_RBV" ;
           :Attr_FilePath_SourceType = "EPICS_PV" ;
           :Attr_FileName_DataType = "String" ;
           :Attr_FileName_Description = "File name" ;
           :Attr_FileName_Source = "13SIM1:netCDF1:FileName_RBV" ;
           :Attr_FileName_SourceType = "EPICS_PV" ;
   }  

ncdump is one of a number of very useful command line utilities that
come with the netCDF package. The -h option to ncdump means to dump only
the header information, not the variable data. This is an explanation of
this output:

-  dimensions: numArrays is the number of arrays in the file. It will be
   1 for files collected in Single mode, and is normally >1 for files
   collected in Capture or Stream mode. For each array dim0 is the
   slowest varying dimension, dim1 the next slowest, etc. attrStringSize
   is the maximum string length for string attributes.
-  variables: There are 15 variables in this netCDF file. uniqueId is
   the unique ID number of each array. timeStamp is the timestamp in
   seconds for each array. array_data is the array data. Its data type
   depends on the data type of the NDArray data passed in the callbacks.
   It dimensions are [numArrays, dim0, dim1, ...dimN]. This notation is
   in the Fortran syntax where the slowest varying dimension comes first
   in the list. The remaining variables all have the prefix Attr\_ and
   are the attributes for the arrays. Each can have its own data type,
   and all have the numArrays elements.
-  global attributes. dataType is the NDDataType_t enum value for the
   array data type. numArrayDims is the number of dimensions in each
   array. array_data has 1 more dimension than this, numArrays, because
   it contains all of the array callback data. dimSize is an
   array[numArrayDims] containing the size of each dimension, with the
   fastest varying dimension first. dimOffset, dimBinning, and
   dimReverse are the values of the offset, binning and reverse fields
   in the NDDimension_t structure for each dimension. The remaining
   netCDF global attributes all have the prefix Attr\_, and describe the
   NDArray attribute values. For each NDArray attribute there is
   information on the data type, a description, source string and source
   type.

There is an IDL function,
`read_nd_netcdf <https://cars.uchicago.edu/software/idl/detector_routines.html#read_nd_netcdf>`__
that can be used to read the netCDF files created by this plugin. This
routine is contained in the `CARS IDL detector
package <https://github.com/CARS-UChicago/IDL_Detectors/blob/master/read_nd_netcdf.pro>`__.
This function is also contained in the areaDetector distribution in the
Viewers/IDL directory.

There is a plugin for the popular `ImageJ <http://rsbweb.nih.gov/ij/>`__
program that can be used to read the netCDF files created by this
plugin. This ImageJ plugin can be downloaded
`here <http://lmb.informatik.uni-freiburg.de/lmbsoft/imagej_plugins/netcdf.html>`__.
This plugin is also contained in the areaDetector distribution in the
Viewers/ImageJ/EPICS_areaDetector directory.

Screen Shots
------------

.. figure:: NDFileNetCDF.png
    :align: center

