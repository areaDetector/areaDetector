NDPluginROIStat
===============
:author: Matt Pearson, Mark Rivers
:affiliation: Oak Ridge National Lab, University of Chicago

.. contents:: Contents

Overview
--------

This plugin provides multiple regions-of-interest (ROIs) on 1-D and 2-D
data and calculates basic statistics (min, max, mean, total, and net
counts). It also can accumulate time-series arrays of these statistics.
The asyn address parameter is used to identify the ROIs, and so a large
number of ROIs can be dealt with in a single thread. The purpose of this
plugin is to make it easy for IOC application developers to do simple
ROI calculations, without having to use multiple traditional ROI and
separate statistics plugins. The plugin can optionally do callbacks on
an NDArray object, appending an attribute list. This makes it possible
to append the ROI statistic data to the output NDArray.

.. note:: 
    This plugin only supports 1-D and 2-D arrays. The NDPluginStats plugin
    can compute statistics on N-dimensional arrays, but it is less efficient
    for these simple statistics on 1-D and 2-D arrays.


Three database template files are provided:

.. cssclass:: table-bordered table-striped table-hover
.. flat-table::
  :header-rows: 1
  :widths: 20 80

  * - Template File Name
    - Description  
  * - ``NDROIStat.template``
    - This needs to be instantiated once for each instance of the plugin.
      It provides records that apply to the entire plugin or to all ROIs.
  * - ``NDROIStatN.template``
    - This needs to be instantiated once for each ROI in the plugin
      (the number must equal the number of ROIs specified in the IOC shell function). 
      The template provides records that apply to each ROI.
  * - ``NDROIStat8.template`` 
    - This is provided as a convenience. 
      It instantiates 8 ROIs by including ``NDROIStat.template`` once 
      and ``NDROIStatN.template`` 8 times.

`NDPluginROIStat` inherits from `NDPluginDriver`.

`NDPluginROIStat` defines the following parameters. It also implements all
of the standard plugin parameters from
:doc:`NDPluginDriver`. The template files
listed above provide access to these parameters, listed in the following
tables.

.. |br| raw:: html

    <br>

.. cssclass:: table-bordered table-striped table-hover
.. flat-table::
  :header-rows: 2
  :widths: 5 5 5 70 5 5 5

  * -
    - Parameter Definitions in NDPluginROIStat.h and EPICS Record Definitions in NDROIStat.template
  * - Parameter index variable
    - asyn interface
    - Access
    - Description
    - drvInfo string
    - EPICS record name
    - EPICS record type
  * - NDPluginROIStatResetAll
    - asynInt32
    - r/w
    - Reset the statistics data for all the configured ROIs.
    - ROISTAT_RESETALL
    - $(P)$(R)ResetAll
    - bo
  * -
    -
    - **Time-Series data**
  * - NDPluginROIStat, TSControl
    - asynInt32
    - r/w
    - Controls time-series data collection. The enum choices are: |br|
      **Erase/Start**: Clears all time-series arrays, sets ROISTAT_TS_CURRENT_POINT=0, and
      starts time-series data collection. |br|
      **Start**: Starts time-series data collection without clearing arrays or modifying
      ROISTAT_TS_CURRENT_POINT. Used to restart collection after a Stop operation. |br|
      **Stop**: Stops times-series data collection. Performs callbacks on all time-series
      waveform records. |br|
      **Read**: Performs callbacks on all time-series waveform records, updating the values. |br|
      **Erase**: Clears all time-series arrays, sets ROISTAT_TS_CURRENT_POINT=0, and performs 
      callbacks on all time-series waveform records.
    - ROISTAT_TS_CONTROL
    - $(P)$(R)TSControl
    - mbbo
  * - N.A.
    - N.A.
    - r/w
    - Sends the "Read" command to the TSControl record above. This record can be periodically
      processed to update the time-series waveform records. It is scan disabled if TSAcquiring=Done,
      so that updates are only performed when time-series acquisition is in progress.
    - N.A.
    - $(P)$(R)TSRead
    - longout
  * - NDPluginROIStat, TSNumPoints
    - asynInt32
    - r/w
    - Controls the number of time-series points to collect. There is no maximum value,
      the time-series arrays in the plugin are freed and reallocated each time this value
      is changed. However, the size of the waveform records is fixed when the IOC is started,
      so NELM in those records must be large enough for the largest time-series needed.
    - ROISTAT_TS_NUM_POINTS
    - $(P)$(R)TSNumPoints
    - longout
  * - NDPluginROIStat, TSCurrentPoint
    - asynInt32
    - r/o
    - The current time-series point. If TSCurrentPoint reaches TSNumPoints then time-series
      acquisition is automatically stopped, and callbacks are done on all time-series
      waveform records, updating the values. This means that even if TSRead has SCAN=Passive
      that the waveform records will update when time-series acquisition is complete.
    - ROISTAT_TS_CURRENT_POINT
    - $(P)$(R)TSCurrentPoint
    - longin
  * - NDPluginROIStat, TSAcquiring
    - asynInt32
    - r/o
    - Indicates status of time-series data acquisition. Values are 0=Done and 1=Acquiring.
    - ROISTAT_TS_ACQUIRING
    - $(P)$(R)TSAcquiring
    - bi

.. cssclass:: table-bordered table-striped table-hover
.. flat-table::
  :header-rows: 3
  :widths: 5 5 5 70 5 5 5

  * -
    -
    - ROI Specific Parameters
  * -
    - Parameter Definitions in NDPluginROIStat.h and EPICS Record Definitions in NDROIStatN.template
  * - Parameter index variable
    - asyn interface
    - Access
    - Description
    - drvInfo string
    - EPICS record name
    - EPICS record type
  * - NDPluginROIStatName
    - asynOctet
    - r/w
    - The name of the plugin.
    - ROISTAT_NAME
    - $(P)$(R)Name, $(P)$(R)Name_RBV
    - stringout
  * - NDPluginROIStatUse
    - asynInt32
    - r/w
    - Set this to 1 to use this ROI, which will mean the statistics will be calculated
      for this ROI.
    - ROISTAT_USE
    - $(P)$(R)Use, $(P)$(R)Use_RBV
    - bo, bi
  * - NDPluginROIStatReset
    - asynInt32
    - r/w
    - Reset the statistics data for this ROI.
    - ROISTAT_RESET
    - $(P)$(R)Reset
    - bo
  * - NDPluginROIStatBgdWidth
    - asynInt32
    - r/w
    - The background width for the net counts calculation. The average background counts
      in a border of this width around the ROI are computed. The border begins at the
      outer edge of the defined ROI and progresses inward by the BgdWidth.
    - ROISTAT_BGD_WIDTH
    - $(P)$(R)BgdWidth, $(P)$(R)BgdWidth_RBV
    - longout
      , longin
  * - NDPluginROIStatDim0Min
    - asynInt32
    - r/w
    - Start element in the X dimension.
    - ROISTAT_DIM0_MIN
    - $(P)$(R)MinX, $(P)$(R)MinX_RBV
    - longout, longin
  * - NDPluginROIStatDim0Size
    - asynInt32
    - r/w
    - The ROI size in the X dimension.
    - ROISTAT_DIM0_SIZE
    - $(P)$(R)SizeX, $(P)$(R)SizeX_RBV
    - longout, longin
  * - NDPluginROIStatDim0MaxSize
    - asynInt32
    - r/o
    - Max size of the ROI in the X dimension.
    - ROISTAT_DIM0_MAX_SIZE
    - $(P)$(R)MaxSizeX, $(P)$(R)MaxSizeX_RBV
    - longin
  * - NDPluginROIStatDim1Min
    - asynInt32
    - r/w
    - Start element in the Y dimension.
    - ROISTAT_DIM1_MIN
    - $(P)$(R)MinY, $(P)$(R)MinY_RBV
    - longout, longin
  * - NDPluginROIStatDim1Size
    - asynInt32
    - r/w
    - The ROI size in the Y dimension.
    - ROISTAT_DIM1_SIZE
    - $(P)$(R)SizeY, $(P)$(R)SizeY_RBV
    - longout, longin
  * - NDPluginROIStatDim1MaxSize
    - asynInt32
    - r/o
    - Max size of the ROI in the Y dimension.
    - ROISTAT_DIM1_MAX_SIZE
    - $(P)$(R)MaxSizeY, $(P)$(R)MaxSizeY_RBV
    - longin
  * - NDPluginROIStatMinValue
    - asynFloat64
    - r/o
    - Minimum count value in the ROI.
    - ROISTAT_MIN_VALUE
    - $(P)$(R)MinValue_RBV
    - ai
  * - NDPluginROIStatMaxValue
    - asynFloat64
    - r/o
    - Maximum count value in the ROI.
    - ROISTAT_MAX_VALUE
    - $(P)$(R)MaxValue_RBV
    - ai
  * - NDPluginROIStatMeanValue
    - asynFloat64
    - r/o
    - Mean counts value in the ROI.
    - ROISTAT_MEAN_VALUE
    - $(P)$(R)MeanValue_RBV
    - ai
  * - NDPluginROIStatTotal
    - asynFloat64
    - r/o
    - Total counts in the ROI.
    - ROISTAT_TOTAL
    - $(P)$(R)Total_RBV
    - ai
  * - NDPluginROIStatNet
    - asynFloat64
    - r/o
    - Net (background subtracted) counts in the ROI.
    - ROISTAT_NET
    - $(P)$(R)Net_RBV
    - ai
  * -
    -
    - **Time-Series data**
  * - NDPluginROIStat, TSXXX
    - asynFloat64Array
    - r/o
    - The time series data arrays. XXX is one of the following, corresponding to each
      of the basic statistics and centroid and sigma statistics described above: |br|
      MinValue |br|
      MaxValue |br|
      MeanValue |br|
      Total |br|
      Net |br|
    - ROISTAT_TS_MIN_VALUE |br|
      ROISTAT_TS_MAX_VALUE |br|
      ROISTAT_TS_MEAN_VALUE |br|
      ROISTAT_TS_TOTAL |br|
      ROISTAT_TS_NET |br|
    - $(P)$(R)TSXXX
    - waveform


Configuration
-------------

The NDPluginROIStat plugin is created with the ``NDROIStatConfigure``
function, either from C/C++ or from the EPICS IOC shell.

::

   NDROIStatConfigure(const char *portName, int queueSize, int blockingCallbacks, 
                             const char *NDArrayPort, int NDArrayAddr, int maxROIs, int maxBuffers,  
                             size_t maxMemory, int priority, int stackSize)
     

All but the ``maxROIs`` parameter are common to all plugins. This
defines how many ROIs this plugin will deal with. Usually this will
match the number of NDROIStatN templates have been instantiated. For
example:

:: 

    NDROIStatConfigure("DET1.ROI", 100, 0, "DET1", 0, 8, -1, -1, 0, 0)


Screen shots
------------

The following MEDM screen provides access to the parameters in
NDPluginDriver.h and NDPluginROIStat.h through records in
NDPluginBase.template, and NDROIStat.template.

.. figure:: NDROIStat.png
    :align: center

The following MEDM screen provides access to the parameters in
NDPluginROIStat.h through records in NDROIStatN.template.

.. figure:: NDROIStatN.png
    :align: center

The following MEDM screen provides access to 8 ROIs at once.

.. figure:: NDROIStat8.png
    :align: center

.. figure:: NDROIStatTimeSeriesMean.png
    :align: center

