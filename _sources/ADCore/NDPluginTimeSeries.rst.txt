NDPluginTimeSeries
==================
:author: Mark Rivers, University of Chicago

.. contents:: Contents

Overview
--------

This plugin accepts 1-D arrays of dimension [NumSignals] or 2-D arrays
of dimension [NumSignals, NewTimePoints]. The plugin creates NumSignals
NDArrays of dimension [NumTimePoints], each of which is the time-series
for one signal. It also creates an NDArray of dimension [NumTimePoints,
NumSignals] containing the data for all of the signals. It also exports
waveform records containing the time-series data for each signal, and a
time-axis array containing the relative times of each point in the
time-series. These waveform records are useful for plotting in OPI
screens.

On each callback the new time points are appended to the existing time
series arrays. The plugin can operate either two modes. In fixed time
mode the time-series arrays are cleared when acquisition is started, and
new time points are appended until NumTimePoints points have been
received, at which point acquisition stops and further callbacks are
ignored. In circular buffer mode once NumTimePoints samples are received
then acquisition continues with the new time points replacing the oldest
ones in the circular buffer.

The plugin requires knowing the time interval between samples from the
driver (TimePerPoint). This information normally comes from a database
link to a record in the detector driver, but it can be manually
specified as well. The plugin exports a 1-D waveform record containing
the relative time values of each sample, which can be used for the
horizontal axis in an OPI display.

The plugin optionally does time averaging of the input signal. It can
average any integer number of input samples(NumAverage), so that the
time between points in the output waveforms is NumAverage*TimePerPoint =
AveragingTime.

NDPluginTimeSeries inherits from NDPluginDriver. The `NDPluginTimeSeries
class
documentation <../areaDetectorDoxygenHTML/class_n_d_plugin_time_series.html>`__
describes this class in detail.

Two database template files are provided:

.. cssclass:: table-bordered table-striped table-hover
.. flat-table::
  :header-rows: 1
  :widths: 20 80

  * - Template File Name
    - Description  
  * - ``NDTimeSeries.template``
    - This needs to be instantiated once for each instance of the plugin. 
      It provides records that apply to the entire plugin or to all signals.
  * - ``NDTimeSeriesN.template``  
    - This needs to be instantiated once for each signal in the plugin
      (the number must equal the number of signals specified in the IOC shell function). 
      The template provides records that apply to each signal.   

NDPluginTimeSeries inherits from NDPluginDriver.

NDPluginTimeSeries defines the following parameters. It also implements
all of the standard plugin parameters from
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
    - Parameters for entire plugin. Definitions in NDPluginTimeSeries.h and NDTimeSeries.template.
  * - Parameter index variable
    - asyn interface
    - Access
    - Description
    - drvInfo string
    - EPICS record name
    - EPICS record type
  * - TSAcquire
    - asynInt32
    - r/w
    - Starts and stops time-series data collection. Value are: |br|
      **0**: "Done": Stops times-series data collection. Performs callbacks on all time-series
      waveform records. The record automatically goes to Done when acquisition completes
      in Fixed Length acquire mode. |br|
      **1**: "Acquire": Clears all time-series arrays, sets TS_CURRENT_POINT=0, and starts
      time-series data collection.
    - TS_ACQUIRE
    - $(P)$(R)TSAcquire, $(P)$(R)TSAcquiring
    - busy, bi
  * - TSRead
    - asynInt32
    - r/w
    - Forces the plugin to do callbacks with the time series NDArrays and waveform records.
      This record SCAN field can be set to periodically update the waveforms. The callbacks
      are automatically done when TSAcquire goes to done at the end of acquisition in
      Fixed Length mode.
    - TS_READ
    - $(P)$(R)TSRead
    - bo
  * - TSNumPoints
    - asynInt32
    - r/w
    - Controls the number of time-series points to collect. There is no maximum value,
      the time-series arrays in the plugin are freed and reallocated each time this value
      is changed. However, the size of the waveform records is fixed when the IOC is started,
      so NELM in those records must be large enough for the largest time-series needed.
    - TS_NUM_POINTS
    - $(P)$(R)TSNumPoints
    - longout
  * - TSCurrentPoint
    - asynInt32
    - r/o
    - The current time-series point. In Fixed Length mode when TSCurrentPoint reaches
      TSNumPoints then time-series acquisition is automatically stopped, and callbacks
      are done on all time-series NDArrays and waveform records, updating the values.
      This means that even if TSRead.SCAN is Passive that the NDArrays and waveform records
      will update when time-series acquisition is complete.
    - TS_CURRENT_POINT
    - $(P)$(R)TSCurrentPoint
    - longin
  * - TSAcquireMode
    - asynInt32
    - r/w
    - The time series acquisition mode. Choices are:, 0: "Fixed length"
      , 1: "Circ. buffer"
    - TS_ACQUIRE_MODE
    - $(P)$(R)TSAcquireMode, $(P)$(R)TSAcquireMode_RBV
    - mbbo
      , mbbi
  * - TSTimePerPoint
    - asynFloat64
    - r/w
    - The time interval between samples in the waveforms from the driver. This value is
      normally updated automatically using the TSTimePerPointLink record described below.
      It can also be manually changed if there is no EPICS record available to provide
      this value automatically. This value is used to compute NumAverage, and to construct
      the TSTimeAxis array.
    - TS_TIME_PER_POINT
    - $(P)$(R)TSTimePerPoint, $(P)$(R)TSTimePerPoint_RBV
    - ao, ai
  * - N.A
    - N.A.
    - r/w
    - This record has OMSL="closed_loop" and DOL set to a record that contains the time
      between points from the driver. The link will normally have the CP attribute, so
      this record processes whenever the input record changes. The OUT field of this record
      is TSTimePerPoint.
    - N.A.
    - $(P)$(R)TSTimePerPointLink
    - ao
  * - TSAveragingTime
    - asynFloat64
    - r/w
    - The requested value of the time interval over which input time-series points are
      averaged. If the TSAveragingTime is not an integer multiple of TSTimePerPoint then
      TSAveragingTime_RBV will be different from TSAveragingTime and will be the actual
      averaging time.
    - TS_AVERAGING_TIME
    - $(P)$(R)TSAveragingTime, $(P)$(R)TSAveragingTime_RBV
    - ao, ai
  * - TSNumAverage
    - asynInt32
    - r/o
    - The number of time points from the driver that will be averaged in the plugin. This
      is computed from TSAveragingTime/TSTimePerPoint. It is constrained to be the nearest
      positive integer to this ratio.
    - TS_NUM_AVERAGE
    - $(P)$(R)TSNumAverage
    - longin
  * - TSElapsedTime
    - asynFloat64
    - r/w
    - The elapsed time since TSAcquire was set to 1. Stops updating when TSAcquire goes
      to 0.
    - TS_ELAPSED_TIME
    - $(P)$(R)TSElapsedTime
    - ai
  * - TSTimeAxis
    - asynFloat64ArrayIn
    - r/o
    - A waveform record containing the time value of each point in the TimeSeries waveforms.
      When AcquireMode="Fixed length"; then time=0 is the time when acquisition
      started and `TSTimeAxis[i] = TSAveragingTime_RBV * i.`
      When AcquireMode="Circ. buffer"; then time=0 is the most recent time and
      `TSTimeAxis[i] = -TSAveragingTime_RBV * (NumTimePoints-1-i)`, so the oldest time point is the most negative.
    - TS_TIME_AXIS
    - $(P)$(R)TSTimeAxis
    - waveform
  * - TSTimeStamp
    - asynFloat64ArrayIn
    - r/o
    - A waveform record containing the NDArray timestamp for each sample from the input
      NDArray. Note that since multiple time points can be contained in a single NDArray
      there can be repeated values in this waveform. Assuming the driver provides a reliable
      value for TSTimePerPoint described above, then the TSTimeAxis waveform provides
      a more accurate relative time array to use in plotting. The values in this waveform
      are absolute timestamps which can also be useful.
    - TS_TIMESTAMP
    - $(P)$(R)TSTimestamp
    - waveform


.. cssclass:: table-bordered table-striped table-hover
.. flat-table::
  :header-rows: 2
  :widths: 5 5 5 70 5 5 5

  * -
    - Parameters for each signal N. Definitions in NDPluginTimeSeries.h and NDTimeSeriesN.template
  * - Parameter index variable
    - asyn interface
    - Access
    - Description
    - drvInfo string
    - EPICS record name
    - EPICS record type
  * - TSSignalName
    - N.A.
    - r/w
    - The name for this signal.
    - N.A.
    - $(P)$(R)SignalName
    - stringout
  * - TSTimeSeries
    - asynFloat64ArrayIn
    - r/o
    - The time series data arrays.
    - TS_TIME_SERIES
    - $(P)$(R)TimeSeries
    - waveform

Configuration
-------------

The NDPluginTimeSeries plugin is created with the ``NDTimeSeriesConfigure``
function, either from C/C++ or from the EPICS IOC shell.

::

   NDTimeSeriesConfigure(const char *portName, int queueSize, int blockingCallbacks, 
                             const char *NDArrayPort, int NDArrayAddr, int maxSignals,
                             int maxBuffers,  size_t maxMemory, int priority, int stackSize)
     

All but the ``maxSignals`` parameter are common to all plugins.

maxSignals defines how many signals this plugin will deal with. Usually
this will match the number of NDTimeSeriesN templates have been loaded.

For example:

::

    NDTimeSeriesConfigure("TS1", 100, 0, "DET1", 0, 8, 0, 0, 0, 0)

Screen shots
------------

The following MEDM screen provides access to the parameters in
NDPluginDriver.h and NDPluginTimeSeries.h through records in
NDPluginBase.template, and NDTimeSeries.template.

.. figure:: NDPluginTimeSeries.png
    :align: center

The following MEDM screen shows the time series plot for an array in
NDPluginTimeSeriesN.template.

.. figure:: NDTimeSeriesPlot.png
    :align: center

