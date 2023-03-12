NDPluginStdArrays
=================

:author: Mark Rivers, University of Chicago

.. contents:: Contents

Overview
--------

This plugin is the tool for converting the NDArray data produced by
asynNDArrayDriver drivers into a form that can be accessed by EPICS Channel Access clients.

NDPluginStdArrays inherits from NDPluginDriver. NDPluginStdArrays
converts the NDArray data from a callback into the 1-dimensional arrays
supported by the standard asyn array interfaces, i.e. asyn[Int8, Int16,
Int32, Int64, Float32, Float64]Array. These interfaces are supported by the
EPICS waveform record using standard asyn device support. Because this
plugin inherits from :doc:`NDPluginDriver`
it also provides additional information on the array data (e.g. number
of dimensions and dimension data) that are made available as EPICS PVs
so that clients can correctly interpret the array data. The
`NDPluginStdArrays class
documentation <../areaDetectorDoxygenHTML/class_n_d_plugin_std_arrays.html>`__
describes this class in detail.

NDPluginStdArrays defines the following parameters. It also implements
all of the standard plugin parameters from
:doc:`NDPluginDriver`. The EPICS database
``NDStdArrays.template`` provides access to these parameters, listed in the
following table.

.. cssclass:: table-bordered table-striped table-hover
.. flat-table::
  :header-rows: 2
  :widths: 5 5 5 70 5 5 5

  * -
    - **Parameter Definitions in NDPluginStdArrays.h and EPICS Record Definitions in NDStdArrays.template**
  * - Parameter index variable
    - asyn interface
    - Access
    - Description
    - drvInfo string
    - EPICS record name
    - EPICS record type
  * - NDPluginStdArraysData
    - asyn[Int8, Int16, Int32, Int64, Float32, Float64]Array
    - r/o
    - Array data as a 1-D array, possibly converted in data type from that in the NDArray
      object to the specific asyn interface.
    - STD_ARRAY_DATA
    - $(P)$(R)ArrayData
    - waveform

If the array data contains more than 16,000 bytes then in order for
EPICS clients to receive this data the environment variable ``EPICS_CA_MAX_ARRAY_BYTES`` on
both the EPICS IOC computer and EPICS client computer must be set to a
value at least as large as the array size in bytes.

Configuration
-------------

The NDPluginStdArrays plugin is created with the ``NDStdArraysConfigure``
command, either from C/C++ or from the EPICS IOC shell.

.. code-block:: c

   NDStdArraysConfigure (const char *portName, int queueSize, int blockingCallbacks, 
                         const char *NDArrayPort, int NDArrayAddr, size_t maxMemory, 
                         int priority, int stackSize)
     

For details on the meaning of the parameters to this function refer to
the detailed documentation on the NDStdArraysConfigure function in the
`NDPluginStdArrays.cpp
documentation <../areaDetectorDoxygenHTML/_n_d_plugin_std_arrays_8cpp.html>`__
and in the documentation for the constructor for the `NDPluginStdArrays
class <../areaDetectorDoxygenHTML/class_n_d_plugin_std_arrays.html>`__.

Screen shots
------------

The following is the MEDM screen that provides access to the parameters
in NDPluginDriver.h and NDPluginStdArrays.h through records in
NDPluginBase.template and NDStdArrays.template. This is the MEDM screen
that is normally used to control the display of images via EPICS channel
access.

.. figure:: NDStdArrays.png
    :align: center

