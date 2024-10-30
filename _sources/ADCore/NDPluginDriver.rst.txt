NDPluginDriver
==============

NDPluginDriver inherits from
`asynNDArrayDriver <areaDetectorDoc.html#asynNDArrayDriver>`__.
NDPluginDriver is the class from which actual plugins are directly
derived. The EPICS database NDArrayBase.template provides access to each
of the parameters defined in asynNDArrayDriver, and the
`asynNDArrayDriver <areaDetectorDoc.html#asynNDArrayDriver>`__
documentation describes that database. The NDPluginDriver class handles
most of the details of processing NDArray callbacks from the driver.
Plugins derived from this class typically need to implement the
processCallbacks method, and one or more of the write(Int32, Float64,
Octet) methods. The `NDPluginDriver class
documentation <../areaDetectorDoxygenHTML/class_n_d_plugin_driver.html>`__ 
describes this class in detail.

NDPluginDriver defines parameters that all plugin drivers should
implement if possible. These parameters are defined by strings (drvInfo
strings in asyn) with an associated asyn interface, and access
(read-only or read-write). The EPICS database NDPluginBase.template
provides access to these standard plugin parameters, listed in the
following table.

.. cssclass:: table-bordered table-striped table-hover
.. flat-table::
  :header-rows: 2
  :widths: 5 5 60 10 10 10

  * -
    -
    - **Parameter Definitions in NDPluginDriver.h and EPICS Record Definitions in NDPluginBase.template**
  * - asyn interface
    - Access
    - Description
    - drvInfo string
    - EPICS record name
    - EPICS record type
  * -
    -
    - **Information about this plugin**
  * - asynOctet
    - r/o
    - A string describing the plugin type.
    - PLUGIN_TYPE
    - $(P)$(R)PluginType_RBV
    - stringin
  * -
    -
    - **asyn NDArray driver doing callbacks to this plugin**
  * - asynOctet
    - r/w
    - asyn port name for NDArray driver that will make callbacks to this plugin. This
      port can be changed at run time, connecting the plugin to a different NDArray driver.
    - NDARRAY_PORT
    - $(P)$(R)NDArrayPort, (P)$(R)NDArrayPort_RBV
    - stringout, stringin
  * - asynInt32
    - r/w
    - asyn port address for NDArray driver that will make callbacks to this plugin. This
      address can be changed at run time, connecting the plugin to a different address
      in the NDArray driver.
    - NDARRAY_ADDR
    - $(P)$(R)NDArrayAddress, $(P)$(R)NDArrayAddress_RBV
    - longout, longin
  * -
    -
    - **Queue size and status**
  * - asynInt32
    - r/w
    - The total queue size for callbacks when BlockingCallbacks=0. This can be changed
      at run time to increase or decrease the size of the queue and thus the buffering
      in this plugin. This changes the memory requirements of the plugin. When the queue
      size is changed the plugin temporarily stops the callbacks from the input driver
      and waits for all NDArrays currently in the queue to process.
    - QUEUE_SIZE
    - $(P)$(R)QueueSize, $(P)$(R)QueueSize_RBV
    - longout, longin
  * - asynInt32
    - r/o
    - The number of free queue elements. This record goes into minor alarm when the queue
      is 75% full and major alarm when the queue is 100% full.
    - QUEUE_FREE
    - $(P)$(R)QueueFree
    - longin
  * - N/A
    - r/o
    - The number of used queue elements.
    - N/A
    - $(P)$(R)QueueUse
    - calc
  * -
    -
    - **Number of threads**
  * - asynInt32
    - r/o
    - The maximum number of threads that this plugin is allowed to use. This is defined
      when the plugin is created, and cannot be changed at run-time. Note that some plugins
      are not thread-safe for multiple threads running in the same plugin object, and
      these must force MaxThreads=1.
    - MAX_THREADS
    - $(P)$(R)MaxThreads_RBV
    - longin
  * - asynInt32
    - r/w
    - The number of threads to use for this plugin. The value must be between 1 and MaxThreads.
    - NUM_THREADS
    - $(P)$(R)NumThreads, $(P)$(R)NumThreads_RBV
    - longout, longin
  * - asynInt32
    - r/w
    - Selects whether the plugin outputs NDArrays in the order in which they arrive (Unsorted=0)
      or sorted by UniqueId (Sorted=1).
    - SORT_MODE
    - $(P)$(R)SortMode, $(P)$(R)SortMode_RBV
    - mbbo, mbbi
  * - asynFloat64
    - r/w
    - Sets the minimum time that the plugin will wait for preceeding arrays to arrive
      before outputting array N when SortMode=Sorted.
    - SORT_TIME
    - $(P)$(R)SortTime, $(P)$(R)SortTime_RBV
    - ao, ai
  * - asynInt32
    - r/w
    - The maximum allowed size of the std::multiset. This can be changed at run time to
      increase or decrease the size of the queue and thus the buffering in this plugin.
      This changes the memory requirements of the plugin.
    - SORT_SIZE
    - $(P)$(R)SortSize, $(P)$(R)SortSize_RBV
    - longout, longin
  * - asynInt32
    - r/o
    - The number of NDArrays remaining before the std::multiset will not be allowed to
      grow larger and the plugin may begin to drop output frames.
    - SORT_FREE
    - $(P)$(R)SortFree
    - longin
  * - asynInt32
    - r/w
    - The number of NDArrays that have been output in the "wrong" order. The definition
      of the wrong order for NDArray[N] is that NDArray[N].uniqueId=NDArray[N-1].uniqueId
      or NDArray[N].uniqueId=NDArray[N-1].uniqueId+1. The reason for the equality test
      is explained above.
    - DISORDERED_ARRAYS
    - $(P)$(R)DisorderedArrays, $(P)$(R)DisorderedArrays_RBV
    - longout, longin
  * - asynInt32
    - r/w
    - Counter that increments by 1 each time an NDArray callback occurs when SortMode=1
      and the std::multiset is full (SortFree=0), so the NDArray cannot be added to the
      std::multiset.
    - DROPPED_OUTPUT_ARRAYS
    - $(P)$(R)DroppedOutputArrays, $(P)$(R)DroppedOutputArrays_RBV
    - longout, longin
  * -
    -
    - **Callback enable, throttling, and statistics**
  * - asynInt32
    - r/w
    - Enable (1) or disable (0) callbacks from the driver to this plugin. If callbacks
      are disabled then the plugin will normally be idle and consume no CPU resources.
      When disabling the plugin it will continue to process any NDArrays that are already
      in the queue.
    - ENABLE_CALLBACKS
    - $(P)$(R)EnableCallbacks, $(P)$(R)EnableCallbacks_RBV
    - bo, bi
  * - asynInt32
    - r/w
    - 0 = callbacks from the driver do not block; the NDArray data is put on a queue and
      the callback processes in one of the plugin threads.
      , 1 = callbacks from the driver block; the callback processes in the driver callback
      thread.
    - BLOCKING_CALLBACKS
    - $(P)$(R)BlockingCallbacks, $(P)$(R)BlockingCallbacks_RBV
    - bo, bi
  * - asynInt32
    - r/w
    - NDPluginDriver maintains a pointer to the last NDArray that the plugin received.
      If the ProcessPlugin record is processed then the plugin runs again using this same
      NDArray. This can be used to change the plugin parameters and observe the effects
      on downstream plugins and image viewers without requiring the underlying detector
      to collect another NDArray. When the plugin is disabled the cached NDArray is released
      back to the NDArrayPool.
    - PROCESS_PLUGIN
    - $(P)$(R)ProcessPlugin
    - bo
  * - asynFloat64
    - r/o
    - The execution time when the plugin processes. This is useful for measuring the performance
      of the plugin
    - EXECUTION_TIME
    - $(P)$(R)ExecutionTime_RBV
    - ai
  * - asynFloat64
    - r/w
    - The minimum time in seconds between calls to processCallbacks. Any callbacks occuring
      before this minimum time has elapsed will be ignored. 0 means no minimum time, i.e.
      process all callbacks.
    - MIN_CALLBACK_TIME
    - $(P)$(R)MinCallbackTime, $(P)$(R)MinCallbackTime_RBV
    - ao, ai
  * - asynFloat64
    - r/w
    - The maximum data output rate in bytes/s. If the output rate would exceed this then
      the output array is dropped and DroppedOutputArrays is incremented. This can be
      useful, for example, to limit the network bandwidth from a plugin. For most plugins
      this logic is implemented in NDPluginDriver::endProcessCallbacks() when the plugin
      is finishing its operation and is doing callbacks to any downstream plugins. However,
      the NDPluginPva and NDPluginStdArrays plugins are treated differently because the
      output we generally want to throttle is not the NDArray passed to downstream plugins,
      but rather the size of the output for the pvaServer (NDPluginPva) or the size of
      the arrays passed back to device support for waveform records (NDPluginStdArrays).
      For these plugins the throttling logic is thus also implemented inside the plugin.
      If these plugins are throttled then they really do no useful work, and so ArrayCounter
      is not incremented. This makes the ArrayRate reflect the rate at which the plugin
      is actually doing useful work. For NDPluginStdArrays this is also important because
      clients (e.g. ImageJ) may monitor the ArrayCounter_RBV field to decide when to read
      the array and update the display.
    - MAX_BYTE_RATE
    - $(P)$(R)MaxByteRate, $(P)$(R)MaxByteRate_RBV
    - ao, ai
  * - asynInt32
    - r/w
    - Counter that increments by 1 each time an NDArray callback occurs when NDPluginDriverBlockingCallbacks=0
      and the plugin driver queue is full, so the callback cannot be processed.
    - DROPPED_ARRAYS
    - $(P)$(R)DroppedArrays, $(P)$(R)DroppedArrays_RBV
    - longout, longin
  * -
    -
    - **Debugging control**
  * - N/A
    - N/A
    - N/A
    - $(P)$(R)AsynIO
    - asyn

Sorting of output NDArrays
--------------------------
When using a plugin with multiple threads, or when the input plugin is NDPluginGather
it is likely that the NDArray output will be slightly out of order, i.e. NDArray::uniqueId
fields will not be monotonically increasing. This is because the threads are running
asynchronously and at slightly different speeds. As a consequence a file plugin
downstream of this plugin would write NDArrays to the file in the "wrong" order.
Plugins have an option to sort the NDArrays by uniqueId to attempt to output them
in the correct order. This sorting option is enabled by setting SortMode=Sorted,
and works using the following algorithm:

- An std::multiset object is created to store the NDArray output pointers as they
  are received in NDArrayDriver::doNDArrayCallbacks. This is the method that all derived
  classes must call to output NDArrays to downstream plugins. This std::multiset also
  stores the time at which each NDArray was received by the NDArrayDriver::doNDArrayCallbacks
  method. This multiset is automatically sorted by the uniqueId of each NDArray.

- A worker thread is created which processes at the time interval specified by SortTime.
  This thread outputs the next array (NDArray[N]) in the multiset if any of the following
  are true:

  - NDArray[N].uniqueId = NDArray[N-1].uniqueId. This allows for the case where multiple
    upstream plugins are processing the same NDArray. This may happen, for example,
    if NDPluginGather is being used and not all of its inputs are getting their NDArrays
    from from NDPluginScatter.

  - NDArray[N].uniqueId = NDArray[N-1].uniqueId + 1. This is the normal case.

  - NDArray[N] has been in the multiset for longer than SortTime. This will be the
    case if the next array that <i>should</i> have been output has not arrived, perhaps
    because it has been dropped by some upstream plugin and will never arrive. Increasing
    the SortTime will allow longer for out of order arrays to arrive, at the expense
    of more memory because the multiset will grow larger before outputting the arrays.

When NDArrays are added to the multiset they have their reference count increased,
and so will still be consuming memory. The multiset is limited in size to SortSize.
If the multiset would grow larger than this because arrays are arriving faster than
they are being removed with the specified SortTime, then they will be dropped in
the same manner as when NDArrays are dropped from the normal input queue. In this
case DroppedOutputArrays will be incremented. Note that because NDArrays can be
stored in both the normal input queue and the multiset the total memory potentially
used by the plugin is determined by both QueueSize and SortSize.
If the plugin is receiving 500 NDArrays/s (2 ms period), and the maximum time the
plugin threads require to execute is 20 msec, then the minimum value of SortTime
should be 0.02 sec, and the minimum value of SortSize would be 10. It is a good
idea to add a safety margin to these values, so perhaps SortSize=50 and SortTime=0.04
sec.
