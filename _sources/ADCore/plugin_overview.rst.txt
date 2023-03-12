Overview
========

A powerful feature of the areaDetector module is the concept of
plugins. A plugin is code that is called by a driver that passes NDArray
data in a callback. Plugins can be used to process array data in real
time. Existing plugins do operations such as compute statistics
(NDPluginStats), convert data to standard asyn arrays
(NDPluginStdArrays), save data to disk (NDPluginFile), select
regions-of-interest (NDPluginROI), as well as many more. New plugins can
be written to perform specialized functions. Once a plugin is written it
will work with any areaDetector driver. Plugins have the the following
properties:

-  They can execute either in a blocking mode or a non-blocking mode. In
   the blocking mode the callback is executed by the driver callback
   thread. In this mode the callback is guaranteed to execute for each
   NDArray callback. However, it can slow down the driver, and does not
   utilize the multi-core capability of modern CPUs. In the non-blocking
   mode the driver callback simply places the NDArray data in a queue
   that is part of the plugin. The plugin then executes the callback
   code in one or more of its own threads. It removes NDArray data from
   the queue, processes it, and releases the data back to the
   NDArrayPool when it is done. In the non-blocking mode some additional
   memory is required for the NDArray objects that are in the queue. It
   is also possible to drop NDArray data if the queue is full when a
   callback occurs, i.e. some callback data will not be processed. The
   non-blocking mode can utilize the multi-core capabilities of modern
   CPUs because each plugin is executing in its own threads. The
   operation of the queue and the NDArrayPool class means that data
   never needs to be copied unless it is modified, each plugin has a
   pointer to the data which will continue to be valid until the last
   plugin is done with it.
-  They can be enabled or disabled at run time.
-  They can be throttled to only execute at a limited rate. This means,
   for example, that a detector can be saving data to disk at full
   speed, but images can be posted to EPICS at a reduced rate.
-  They can be throttled to limit their output data rate. This means,
   for example, that a detector can be saving data to disk at full
   speed, but images sent across the netword at a fixed network
   bandwidth. The number of images/s may vary with detector ROI,
   binning, and data compression.
-  They can be unplugged from one driver, and plugged into another
   driver at run time. For example, the NDPluginROI driver is itself a
   source of NDArray data callbacks, so a file saving plugin could be
   unplugged from a detector driver (where it would be saving the entire
   detector), and plugged into a particular ROI, where it would just
   save a portion of the detector. Similarly the NDPluginColorConvert
   plugin is also a source of NDArray data callbacks. A pipeline of
   plugins can be constructed, for example
   NDPluginColorConvert->NDPluginROI->NDPluginStdArrays. Each stage of
   this pipeline can be executing in its own threads, and on modern
   multi-core processors each can be executing on its own cores.
-  In non-blocking mode the maximum allowed number of threads is fixed
   when the plugin is created, but the actual number of threads to use
   can be changed from 1 to this upper limit at run time.
-  When there are multiple threads in use it is likely that the output
   NDArrays will not be in the correct order of ascending values of
   NDArray::UniqueId because each thread is processing asynchronously.
   All plugins therefore have an option to sort the output arrays before
   sending them. This comes at the expense of an increase in memory use
   and latency between the input and output arrays.
-  Plugins store the last input array so they can be processed again
   with different settings without waiting for a new input array to
   arrive.

