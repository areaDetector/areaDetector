NDArray
=======

NDArray
-------

The NDArray (N-Dimensional array) is the class that is used for passing
detector data from drivers to plugins. An NDArray is a general purpose
class for handling array data. An NDArray object is self-describing,
meaning it contains enough information to describe the data itself. It
can optionally contain "attributes" (class NDAttribute) which contain
meta-data describing how the data was collected, etc.

An NDArray can have up to ND_ARRAY_MAX_DIMS dimensions, currently 10. A
fixed maximum number of dimensions is used to significantly simplify the
code compared to unlimited number of dimensions. Each dimension of the
array is described by an `NDDimension
structure <../areaDetectorDoxygenHTML/struct_n_d_dimension.html>`__. The
`NDArray class
documentation <../areaDetectorDoxygenHTML/class_n_d_array.html>`__\ describes
this class in detail.

NDArrayPool
-----------

The NDArrayPool class manages a free list (pool) of NDArray objects.
Drivers allocate NDArray objects from the pool, and pass these objects
to plugins. Plugins increase the reference count on the object when they
place the object on their queue, and decrease the reference count when
they are done processing the array. When the reference count reaches 0
again the NDArray object is placed back on the free list. This mechanism
minimizes the copying of array data in plugins. The `NDArrayPool class
documentation <../areaDetectorDoxygenHTML/class_n_d_array_pool.html>`__\ describes
this class in detail.

NDAttribute
-----------

The NDAttribute is a class for linking metadata to an NDArray. An
NDattribute has a name, description, data type, value, source type and
source information. Attributes are identified by their names, which are
case-sensitive. There are methods to set and get the information for an
attribute.

It is useful to define some conventions for attribute names, so that
plugins or data analysis programs can look for a specific attribute. The
following are the attribute conventions used in current plugins:

.. cssclass:: table-bordered table-striped table-hover
.. flat-table::
  :header-rows: 2
  :widths: 20 60 20

  * -
    - **Conventions for standard attribute names**
  * - Attribute name
    - Description
    - Data type
  * - ColorMode
    - Color mode of the NDArray. Used by many plugins to determine how to process the
      array.
    - NDAttrInt32 (NDColorMode_t)
  * - BayerPattern
    - Bayer patter of an image collect by a color camera with a Bayer mask. Could be used
      to convert to a an RGB color image. This capability may be added to NDPluginColorConvert.
    - NDAttrInt32 (NDBayerPattern_t)
  * - DriverFilename
    - The name of the file originally collected by the driver. This is used by NDPluginFile
      to delete the original driver file if the DeleteDriverFile flag is set and the NDArray
      has been successfully saved in another file.
    - NDAttrString
  * - FilePluginDestination
    - This is used by NDPluginFile to determine whether to process this NDArray. If this
      attribute is present and is "all" or the name of this plugin then the NDArray is
      processed, otherwise it is ignored.
    - NDAttrString
  * - FilePluginFileName
    - This is used by NDPluginFile to set the file name when saving this NDArray.
    - NDAttrString
  * - FilePluginFileNumber
    - This is used by NDPluginFile to set the file number when saving this NDArray.
    - NDAttrInt32
  * - FilePluginFileClose
    - This is used by NDPluginFile to close the file after processing this NDArray.
    - NDAttrInt32
  * - [HDF dataset name]
    - This is used by NDFileHDF5 to determine which dataset in the file this NDArray should
      be written to. The attribute name is the name of the HDF5 dataset.
    - NDAttrString
  * - [posName]
    - This is used by NDFileHDF5 to determine which position in the dataset this NDArray
      should be written to. The attribute name is contained in a *PosName* record defined
      in NDFileHDF5.template. This is designed to allow, for example, "snake scan" data
      to be placed in the correct order in an HDF5 file.
    - NDAttrInt32

Attribute names are case-sensitive. For attributes not in this table a
good convention would be to use the corresponding driver parameter
without the leading ND or AD, and with the first character of every
"word" of the name starting with upper case. For example, the standard
attribute name for ADManufacturer should be "Manufacturer",
ADNumExposures should be "NumExposures", etc.

The `NDAttribute class
documentation <../areaDetectorDoxygenHTML/class_n_d_attribute.html>`__
describes this class in detail.

NDAttributeList
---------------

The NDAttributeList implements a linked list of NDAttribute objects.
NDArray objects contain an NDAttributeList which is how attributes are
associated with an NDArray. There are methods to add, delete and search
for NDAttribute objects in an NDAttributeList. Each attribute in the
list must have a unique name, which is case-sensitive.

When NDArrays are copied with the NDArrayPool methods the attribute list
is also copied.

IMPORTANT NOTE: When a new NDArray is allocated using
``NDArrayPool::alloc()`` the behavior of any existing attribute list on the
NDArray taken from the pool is determined by the value of the global
variable ``eraseNDAttributes``. By default the value of this variable is
0. This means that when a new NDArray is allocated from the pool its
attribute list is **not** cleared. This greatly improves efficiency in
the normal case where attributes for a given driver are defined once at
initialization and never deleted. (The attribute **values** may of
course be changing.) It eliminates allocating and deallocating attribute
memory each time an array is obtained from the pool. It is still
possible to add new attributes to the array, but any existing attributes
will continue to exist even if they are ostensibly cleared e.g.
``asynNDArrayDriver::readNDAttributesFile()`` is called again. If it is
desired to eliminate all existing attributes from NDArrays each time a
new one is allocated then the global variable ``eraseNDAttributes``
should be set to 1. This can be done at the iocsh prompt with the
command:

.. code:: c

   var eraseNDAttributes 1


The `NDAttributeList class
documentation <../areaDetectorDoxygenHTML/class_n_d_attribute_list.html>`__
describes this class in detail.

PVAttribute
-----------

The PVAttribute class is derived from NDAttribute. It obtains its value
by monitor callbacks from an EPICS PV, and is thus used to associate
current the value of any EPICS PV with an NDArray. The `PVAttribute
class
documentation <../areaDetectorDoxygenHTML/class_p_v_attribute.html>`__
describes this class in detail.

paramAttribute
--------------

The paramAttribute class is derived from NDAttribute. It obtains its
value from the current value of a driver or plugin parameter. The
paramAttribute class is typically used when it is important to have the
current value of the parameter and the value of a corresponding
PVAttribute might not be current because the EPICS PV has not yet
updated. The `paramAttribute class
documentation <../areaDetectorDoxygenHTML/classparam_attribute.html>`__
describes this class in detail.

functAttribute
--------------

The functAttribute class is derived from NDAttribute. It obtains its
value from a user-written C++ function. The functAttribute class is thus
very general, and can be used to add almost any information to an
NDArray. ADCore contains example code, myAttributeFunctions.cpp that
demonstates how to write such functions. The `functAttribute class
documentation <../areaDetectorDoxygenHTML/classfunct_attribute.html>`__
describes this class in detail.

constAttribute
--------------

This type of attribute obtains its value directly from the "source" value in the XML file.

asynNDArrayDriver
-----------------

asynNDArrayDriver inherits from asynPortDriver. It implements the
asynGenericPointer functions for NDArray objects. This is the class from
which both plugins and area detector drivers are indirectly derived. The
`asynNDArrayDriver class
documentation <../areaDetectorDoxygenHTML/classasyn_n_d_array_driver.html>`__\ describes
this class in detail.

The file
`asynNDArrayDriver.h <../areaDetectorDoxygenHTML/asyn_n_d_array_driver_8h.html>`__
defines a number of parameters that all NDArray drivers and plugins
should implement if possible. These parameters are defined by strings
(drvInfo strings in asyn) with an associated asyn interface, and access
(read-only or read-write). There is also an integer index to the
parameter which is assigned by asynPortDriver when the parameter is
created in the parameter library. The EPICS database
NDArrayBase.template provides access to these standard driver
parameters. The following table lists the standard driver parameters.
The columns are defined as follows:

-  **Parameter index variable:** The variable name for this parameter
   index in the driver. There are several EPICS records in
   ADBase.template that do not have corresponding parameter indices, and
   these are indicated as Not Applicable (N/A).
-  **asyn interface:** The asyn interface used to pass this parameter to
   the driver.
-  **Access:** Read-write (r/w) or read-only (r/o).
-  **drvInfo string:** The string used to look up the parameter in the
   driver through the drvUser interface. This string is used in the
   EPICS database file for generic asyn device support to associate a
   record with a particular parameter. It is also used to associate a
   `paramAttribute <../areaDetectorDoxygenHTML/classparam_attribute.html>`__
   with a driver parameter in the XML file that is read by
   asynNDArrayDriver::readNDAttributesFile
-  **EPICS record name:** The name of the record in ADBase.template.
   Each record name begins with the two macro parameters $(P) and $(R).
   In the case of read/write parameters there are normally two records,
   one for writing the value, and a second, ending in \_RBV, that
   contains the actual value (Read Back Value) of the parameter.
-  **EPICS record type:** The record type of the record. Waveform
   records are used to hold long strings, with length (NELM) = 256 bytes
   and EPICS data type (FTVL) = UCHAR. This removes the 40 character
   restriction string lengths that arise if an EPICS "string" PV is
   used. MEDM allows one to edit and display such records correctly.
   EPICS clients will typically need to convert such long strings from a
   string to an integer or byte array before sending the path name to
   EPICS. In IDL this is done as follows:

.. code::

    
    ; Convert a string to a null-terminated byte array and write with caput
    IDL> t = caput('13PS1:TIFF1:FilePath', [byte('/home/epics/scratch'),0B])
    ; Read a null terminated byte array 
    IDL> t = caget('13PS1:TIFF1:FilePath', v)
    ; Convert to a string 
    IDL> s = string(v) 


In SPEC this is done as follows:

.. code::

    array _temp[256]
    # Setting the array to "" will zero-fill it
    _temp = ""
    # Copy the string to the array.  Note, this does not null terminate, so if array already contains
    # a longer string it needs to first be zeroed by setting it to "".
    _temp = "/home/epics/scratch"
    epics_put("13PS1:TIFF1:FilePath", _temp)
        

Note that for parameters whose values are defined by enum values (e.g
NDDataType, NDColorMode, etc.), drivers can use a different set of enum
values for these parameters. They can override the enum menu in
ADBase.template with driver-specific choices by loading a
driver-specific template file that redefines that record field after
loading ADBase.template.

.. cssclass:: table-bordered table-striped table-hover
.. flat-table::
  :header-rows: 2
  :widths: 5 5 5 70 5 5 5

  * - **Parameter Definitions in asynNDArrayDriver.h and EPICS Record Definitions in NDArrayBase.template
      (file-related records are in NDFile.template)**
  * - Parameter index variable
    - asyn interface
    - Access
    - Description
    - drvInfo string
    - EPICS record name
    - EPICS record type
  * -
    -
    -
    - **Information about the version of ADCore and the plugin or driver**
  * - NDADCoreVersion
    - asynOctet
    - r/o
    - ADCore version number. This can be used by Channel Access clients to alter their
      behavior depending on the version of ADCore that was used to build this driver or
      plugin.
    - ADCORE_VERSION
    - $(P)$(R)ADCoreVersion_RBV
    - stringin
  * - NDDriverVersion
    - asynOctet
    - r/o
    - Driver or plugin version number. This can be used by Channel Access clients to alter
      their behavior depending on the version of the plugin or driver.
    - DRIVER_VERSION
    - $(P)$(R)DriverVersion_RBV
    - stringin
  * -
    -
    -
    - **Information about the device**
  * - ADManufacturer 
    - asynOctet 
    - r/o 
    - Detector manufacturer name 
    - MANUFACTURER 
    - $(P)$(R)Manufacturer_RBV 
    - stringin 
  * - ADModel 
    - asynOctet 
    - r/o 
    - Detector model name 
    - MODEL 
    - $(P)$(R)Model_RBV 
    - stringin 
  * - ADSerialNumber 
    - asynOctet 
    - r/o 
    - Detector serial number 
    - SERIAL_NUMBER 
    - $(P)$(R)SerialNumber_RBV 
    - stringin 
  * - ADFirmwareVersion 
    - asynOctet 
    - r/o 
    - Detector firmware version 
    - FIRMWARE_VERSION 
    - $(P)$(R)FirmwareVersion_RBV 
    - stringin 
  * - ADSDKVersion 
    - asynOctet 
    - r/o 
    - Detector vendor's Software Development Kit (SDK) version number. 
    - SDK_VERSION 
    - $(P)$(R)SDKVersion_RBV 
    - stringin 
  * - ADFirmwareVersion 
    - asynOctet 
    - r/o 
    - Detector firmware version number. 
    - FIRMWARE_VERSION 
    - $(P)$(R)FirmwareVersion_RBV 
    - stringin 
  * -
    - 
    - 
    - **Acquisition control** 
  * - ADAcquire 
    - asynInt32 
    - r/w 
    - Start (1) or stop (0) image acquisition. This record forward links to $(P)$(R)AcquireBusy
      which is an EPICS busy record that does not process its forward link until acquisition
      is complete. Clients should write 1 to the Acquire record to start acquisition,
      and wait for AcquireBusy to go to 0 to know that acquisition is complete. This can
      be done automatically with ca_put_callback. 
    - ACQUIRE 
    - $(P)$(R)Acquire, $(P)$(R)Acquire_RBV 
    - bo, bi 
  * - N.A. 
    - N.A. 
    - r/o 
    - This is an EPICS busy record that is set to 1 when Acquire is set to 1 and not process
      its forward link until acquisition is complete. 
    - N.A. 
    - $(P)$(R)AcquireBusy 
    - busy 
  * - N.A. 
    - N.A. 
    - r/o 
    - This record controls whether AcquireBusy goes to 0 when the detector is done (Acquire=0),
      or whether it waits until $(P)$(R)NumQueuedArrays also goes to 0, i.e. that all
      plugins are also done. Choices are No (0) and Yes(1). 
    - N.A. 
    - $(P)$(R)WaitForPlugins 
    - bo 
  * -
    -
    -
    - **Information about the asyn port**
  * - NDPortNameSelf
    - asynOctet
    - r/o
    - asyn port name
    - PORT_NAME_SELF
    - $(P)$(R)PortName_RBV
    - stringin
  * -
    -
    -
    - **Data type**
  * - NDDataType
    - asynInt32
    - r/w
    - Data type (NDDataType_t).
    - DATA_TYPE
    - $(P)$(R)DataType, $(P)$(R)DataType_RBV
    - mbbo, mbbi
  * -
    -
    -
    - **Color mode**
  * - NDColorMode
    - asynInt32
    - r/w
    - Color mode (NDColorMode_t).
    - COLOR_MODE
    - $(P)$(R)ColorMode, $(P)$(R)ColorMode_RBV
    - mbbo, mbbi
  * - NDBayerPattern
    - asynInt32
    - r/o
    - Bayer pattern (NDBayerPattern_t) of NDArray data.
    - BAYER_PATTERN
    - $(P)$(R)BayerPattern_RBV
    - mbbi
  * -
    -
    -
    - **Actual dimensions of array data**
  * - NDNDimensions
    - asynInt32
    - r/w
    - Number of dimensions in the array
    - ARRAY_NDIMENSIONS
    - $(P)$(R)NDimensions, $(P)$(R)NDimensions_RBV
    - longout, longin
  * - NDDimensions
    - asynInt32Array
    - r/w
    - Size of each dimension in the array
    - ARRAY_DIMENSIONS
    - $(P)$(R)Dimensions, $(P)$(R)Dimensions_RBV
    - waveform (out), waveform (in)
  * - N.A.
    - N.A
    - r/o
    - Size of each array dimension, extracted from the $(P)$(R)Dimensions and $(P)$(R)Dimensions_RBV
      waveform records. Note that these are both longin record, i.e. readonly values using
      subarray records. In the future longout records may be added to write to the individual
      values in $(P)$(R)Dimensions.
    - N.A.
    - $(P)$(R)ArraySize[N] N=0-9
      , (P)$(R)ArraySize[N]_RBV
    - longin, longin
  * - NDArraySizeX
    - asynInt32
    - r/o
    - Size of the array data in the X direction
    - ARRAY_SIZE_X
    - $(P)$(R)ArraySizeX_RBV
    - longin
  * - NDArraySizeY
    - asynInt32
    - r/o
    - Size of the array data in the Y direction
    - ARRAY_SIZE_Y
    - $(P)$(R)ArraySizeY_RBV
    - longin
  * - NDArraySizeZ
    - asynInt32
    - r/o
    - Size of the array data in the Z direction
    - ARRAY_SIZE_Z
    - $(P)$(R)ArraySizeZ_RBV
    - longin
  * - NDArraySize
    - asynInt32
    - r/o
    - Total size of the array data in bytes
    - ARRAY_SIZE
    - $(P)$(R)ArraySize_RBV
    - longin
  * - NDCodec
    - asynOctet
    - r/o
    - The codec used to compress this array
    - CODEC
    - $(P)$(R)Codec_RBV
    - stringin
  * - NDCompressedSize
    - asynInt32
    - r/o
    - Compressed size of the array data in bytes. Only meaningful if NDCodec is not empty
      string.
    - COMPRESSED_SIZE
    - $(P)$(R)Compressed_RBV
    - longin
  * -
    -
    -
    - **Array data**
  * - NDArrayCallbacks
    - asynInt32
    - r/w
    - Controls whether the driver or plugin does callbacks with the array data to registered
      plugins. 0=No, 1=Yes. Setting this to 0 in a driver can reduce overhead in the case
      that the driver is being used only to control the device, and not to make the data
      available to plugins or to EPICS clients. Setting this to 0 in a plugin can reduce
      overhead by eliminating the need to copy the NDArray if that plugin is not being
      used as a source of NDArrays to other plugins.
    - ARRAY_CALLBACKS
    - $(P)$(R)ArrayCallbacks, $(P)$(R)ArrayCallbacks_RBV
    - bo, bi
  * - NDArrayData
    - asynGenericPointer
    - r/w
    - The array data as an NDArray object
    - NDARRAY_DATA
    - N/A. EPICS access to array data is through NDStdArrays plugin.
    - N/A
  * - NDArrayCounter
    - asynInt32
    - r/w
    - Counter that increments by 1 each time an array is acquired. Can be reset by writing
      a value to it.
    - ARRAY_COUNTER
    - $(P)$(R)ArrayCounter, $(P)$(R)ArrayCounter_RBV
    - longout, longin
  * - N/A
    - N/A
    - r/o
    - Rate at which arrays are being acquired. Computed in the ADBase.template database.
    - N/A
    - $(P)$(R)ArrayRate_RBV
    - calc
  * -
    -
    -
    - **Array attributes**
  * - NDAttributesFile
    - asynOctet
    - r/w
    - The name of an XML file defining the NDAttributes to be added to each NDArray by
      this driver or plugin. The format of the XML file is described in the documentation
      for `asynNDArrayDriver::readNDAttributesFile() <../areaDetectorDoxygenHTML/classasyn_n_d_array_driver.html>`__ 
    - ND_ATTRIBUTES_FILE
    - $(P)$(R)NDAttributesFile
    - waveform
  * - NDAttributesMacros
    - asynOctet
    - r/w
    - A macro definition string that can be used to do macro substitution in the XML file.
      For example if this string is "CAMERA=13SIM1:cam1:,ID=ID13us:" then all $(CAMERA)
      and $(ID) strings in the XML file will be replaced with 13SIM1:cam1: and ID13us:
      respectively.
    - ND_ATTRIBUTES_MACROS
    - $(P)$(R)NDAttributesMacros
    - waveform
  * - NDAttributesStatus
    - asynInt32
    - r/o
    - The status of reading and parsing the XML attributes file. This is used to indicate
      if the file cannot be found, if there is an XML syntax error, or if there is a macro
      substitutions error.
    - ND_ATTRIBUTES_STATUS
    - $(P)$(R)NDAttributesStatus
    - mbbi
  * -
    -
    -
    - **Array pool status**
  * - NDPoolMaxMemory
    - asynFloat64
    - r/o
    - The maximum number of NDArrayPool memory bytes that can be allocated. 0=unlimited.
    - POOL_MAX_MEMORY
    - $(P)$(R)PoolMaxMem
    - ai
  * - NDPoolUsedMemory
    - asynFloat64
    - r/o
    - The number of NDArrayPool memory bytes currently allocated. The SCAN rate of this
      record controls the scanning of all of the dynamic NDArrayPool status records.
    - POOL_USED_MEMORY
    - $(P)$(R)PoolUsedMem
    - ai
  * - NDPoolAllocBuffers
    - asynInt32
    - r/o
    - The number of NDArrayPool buffers currently allocated.
    - POOL_ALLOC_BUFFERS
    - $(P)$(R)PoolAllocBuffers
    - longin
  * - NDPoolFreeBuffers
    - asynInt32
    - r/o
    - The number of NDArrayPool buffers currently allocated but free.
    - POOL_FREE_BUFFERS
    - $(P)$(R)PoolFreeBuffers
    - longin
  * - N.A.
    - N.A.
    - r/o
    - The number of NDArrayPool buffers currently in use. This is calculated as NDPoolAllocBuffers
      - NDPoolFreeBuffers.
    - N.A.
    - $(P)$(R)PoolUsedBuffers
    - calc
  * - NDPoolEmptyFreeList
    - asynInt32
    - r/w
    - Processing this record deletes all of the NDArrays on the freelist and sets the
      freelist size to 0. This provides a mechanism to free large amounts of memory and
      return it to the operating system, for example after a rapid acquisition with large
      plugin queues. On Windows the memory is returned to the operating system immediately.
      On Linux the freed memory may not actually be returned to the operating system even
      though it has been freed in the areaDetector process. On Centos7 (and presumably
      many other versions of Linux) setting the value of the environment variable `MALLOC_TRIM_THRESHOLD_`
      to a small value will allow the memory to actually be returned to the operating
      system.
    - POOL_EMPTY_FREELIST
    - $(P)$(R)EmptyFreeList
    - bo
  * - NDNumQueuedArrays
    - asynInt32
    - r/o
    - The number of NDArrays from this driver's NDArrayPool that are currently queued
      for processing by plugins. When this number goes to 0 the plugins have all completed
      processing.
    - NUM_QUEUED_ARRAYS
    - $(P)$(R)NumQueuedArrays
    - longin
  * -
    -
    -
    - **Debugging control**
  * - N/A
    - N/A
    - N/A
    - asyn record to control debugging (asynTrace)
    - N/A
    - $(P)$(R)AsynIO
    - asyn
  * -
    -
    -
    - **File saving parameters (records are defined in NDFile.template)**
  * - NDFilePath
    - asynOctet
    - r/w
    - File path
    - FILE_PATH
    - $(P)$(R)FilePath, $(P)$(R)FilePath_RBV
    - waveform, waveform
  * - NDFilePathExists
    - asynInt32
    - r/o
    - Flag indicating if file path exists
    - FILE_PATH_EXISTS
    - $(P)$(R)FilePathExists_RBV
    - bi
  * - NDFileName
    - asynOctet
    - r/w
    - File name
    - FILE_NAME
    - $(P)$(R)FileName, $(P)$(R)FileName_RBV
    - waveform, waveform
  * - NDFileNumber
    - asynInt32
    - r/w
    - File number
    - FILE_NUMBER
    - $(P)$(R)FileNumber, $(P)$(R)FileNumber_RBV
    - longout, longin
  * - NDFileTemplate
    - asynOctet
    - r/w
    - Format string for constructing NDFullFileName from NDFilePath, NDFileName, and NDFileNumber.
      The final file name (which is placed in NDFullFileName) is created with the following
      code:
      ``epicsSnprintf(FullFilename, sizeof(FullFilename), FileTemplate, FilePath, Filename, FileNumber);``
      FilePath, Filename, FileNumber are converted in that order with FileTemplate. An
      example file format is `"%s%s%4.4d.tif"`. The first %s converts the FilePath,
      followed immediately by another %s for Filename. FileNumber is formatted with %4.4d,
      which results in a fixed field with of 4 digits, with leading zeros as required.
      Finally, the .tif extension is added to the file name. This mechanism for creating
      file names is very flexible. Other characters, such as _ can be put in Filename
      or FileTemplate as desired. If one does not want to have FileNumber in the file
      name at all, then just omit the %d format specifier from FileTemplate. If the client
      wishes to construct the complete file name itself, then it can just put that file
      name into NDFileTemplate with no format specifiers at all, in which case NDFilePath,
      NDFileName, and NDFileNumber will be ignored.
    - FILE_TEMPLATE
    - $(P)$(R)FileTemplate, $(P)$(R)FileTemplate_RBV
    - waveform, waveform
  * - NDFullFileName
    - asynOctet
    - r/o
    - Full file name constructed using the algorithm described in NDFileTemplate
    - FULL_FILE_NAME
    - $(P)$(R)FullFileName_RBV
    - waveform, waveform
  * - NDAutoIncrement
    - asynInt32
    - r/w
    - Auto-increment flag. Controls whether FileNumber is automatically incremented by
      1 each time a file is saved (0=No, 1=Yes)
    - AUTO_INCREMENT
    - $(P)$(R)AutoIncrement, $(P)$(R)AutoIncrement_RBV
    - bo, bi
  * - NDAutoSave
    - asynInt32
    - r/w
    - Auto-save flag (0=No, 1=Yes) controlling whether a file is automatically saved each
      time acquisition completes.
    - AUTO_SAVE
    - $(P)$(R)AutoSave, $(P)$(R)AutoSave_RBV
    - bo, bi
  * - NDFileFormat
    - asynInt32
    - r/w
    - File format. The format to write/read data in (e.g. TIFF, netCDF, etc.)
    - FILE_FORMAT
    - $(P)$(R)FileFormat, $(P)$(R)FileFormat_RBV
    - mbbo, mbbi
  * - NDWriteFile
    - asynInt32
    - r/w
    - Manually save the most recent array to a file when value=1
    - WRITE_FILE
    - $(P)$(R)WriteFile, $(P)$(R)WriteFile_RBV
    - busy, bi
  * - NDReadFile
    - asynInt32
    - r/w
    - Manually read a file when value=1
    - READ_FILE
    - $(P)$(R)ReadFile, $(P)$(R)ReadFile_RBV
    - busy, bi
  * - NDFileWriteMode
    - asynInt32
    - r/w
    - File saving mode (Single, Capture, Stream)(NDFileMode_t)
    - WRITE_MODE
    - $(P)$(R)FileWriteMode, $(P)$(R)FileWriteMode_RBV
    - mbbo, mbbi
  * - NDFileWriteStatus
    - asynInt32
    - r/o
    - File write status. Gives status information on last file open or file write operation.
      Values are WriteOK (0) and WriteError (1).
    - WRITE_STATUS
    - $(P)$(R)FileWriteStatus
    - mbbi
  * - NDFileWriteMessage
    - asynOctet
    - r/o
    - File write error message. An error message string if the previous file open or file
      write operation resulted in an error.
    - WRITE_MESSAGE
    - $(P)$(R)FileWriteMessage
    - waveform
  * - NDFileCapture
    - asynInt32
    - r/w
    - Start (1) or stop (0) file capture or streaming
    - CAPTURE
    - $(P)$(R)Capture, $(P)$(R)Capture_RBV
    - busy, bi
  * - NDFileNumCapture
    - asynInt32
    - r/w
    - Number of frames to acquire in capture or streaming mode
    - NUM_CAPTURE
    - $(P)$(R)NumCapture, $(P)$(R)NumCapture_RBV
    - longout, longin
  * - NDFileNumCaptured
    - asynInt32
    - r/o
    - Number of arrays currently acquired capture or streaming mode
    - NUM_CAPTURED
    - $(P)$(R)NumCaptured_RBV
    - longin
  * - NDFileFreeCapture
    - asynInt32
    - r/w
    - Release all of the NDArrays in the capture buffer to the free pool.
    - FREE_CAPTURE
    - $(P)$(R)FreeCapture
    - bo
  * - NDFileDeleteDriverFile
    - asynInt32
    - r/w
    - Flag to enable deleting original driver file after a plugin has re-written the file
      in a different format. This can be useful for detectors that must write the data
      to disk in order for the areaDetector driver to read it back. Once a file-writing
      plugin has rewritten the data in another format it can be desireable to then delete
      the original file.
    - DELETE_DRIVER_FILE
    - $(P)$(R)DeleteDriverFile, $(P)$(R)DeleteDriverFile_RBV
    - bo, bi
  * - NDFileLazyOpen
    - asynInt32
    - r/w
    - Flag to defer the creation of a new file until the first NDArray to write has been
      received. This removes the need for passing an extra NDArray through the file writing
      plugin to initialise dimensions and possibly NDAttribute list before opening the
      file. The downside is that file creation can potentially be time-consuming so processing
      the first NDArray may be slower than subsequent ones.
      , Only makes sense to use with file plugins which support multiple frames per file
      and only in "Stream" mode.
    - FILE_LAZY_OPEN
    - $(P)$(R)LazyOpen, $(P)$(R)LazyOpen_RBV
    - bo, bi
  * - NDFileCreateDir
    - asynInt32
    - r/w
    - This parameter is used to automatically create directories if they don't exist.
      If it is zero (default), no directories are created. If it is negative, then the
      absolute value is the maximum of directories that will be created (i.e. -1 will
      create a maximum of one directory to complete the path, -2 will create a maximum
      of 2 directories). If it is positive, then at least that many directories in the
      path must exist (i.e. a value of 1 will create all directories below the root directory
      and 2 will not create a directory in the root directory).
    - CREATE_DIR
    - $(P)$(R)CreateDirectory, $(P)$(R)CreateDirectory_RBV
    - longout, longin
  * - NDFileTempSuffix
    - asynOctet
    - r/w
    - If this string is non-null, the file is opened with this suffix temporarily appended
      to the file name. When the file is closed it is then renamed to the correct file
      name without the suffix. This is useful for processing software watching for the
      file to appear since the file appears as an atomic operation when it is ready to
      be opened.
    - FILE_TEMP_SUFFIX
    - $(P)$(R)TempSuffix, $(P)$(R)TempSuffix_RBV
    - stringout, stringin


