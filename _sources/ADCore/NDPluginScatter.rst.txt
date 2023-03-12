NDPluginScatter
===============

:author: Mark Rivers,University of Chicago

.. contents:: Contents

Overview
--------

This plugin is used to distribute (scatter) the processing of NDArrays
to multiple downstream plugins. It allows multiple intances of a plugin
to process NDArrays in parallel, utilizing multiple cores to increase
throughput. It is commonly used in together with
:doc:`NDPluginGather`, which gathers the outputs from
multiple plugins back into a single stream. The :doc:`documentation for
NDPluginGather <NDPluginGather>` provides a detailed example of
using `NDPluginScatter` and `NDPluginGather` to do parallel processing of 5
statistics plugins.

This plugin works differently from other plugins that do callbacks to
downstream plugins. Other plugins pass each NDArray that they generate
of **all** downstream plugins that have registered for callbacks.
NDPluginScatter does not do this, rather it passes each NDArray to only
one downstream plugin. The mechanism for chosing which plugin to pass
the next NDArray to can be described as a modified round-robin. The
first NDArray is passed to the first registered callback client, the
second NDArray to the second client, etc. After the last client the next
NDArray goes to the first client, and so on. The modification to strict
round-robin is that if client N input queue is full then an attempt is
made to send the NDArray to client N+1, and if this would fail to client
N+2, etc. If no clients are able to accept the NDArray because their
queues are full then the last client that is tried (N-1) will drop the
NDArray. Because the "last client" rotates according the round-robin
schedule the load of dropped arrays will be uniform if all clients are
executing at the same speed and if their queues are the same size.

Their is an ``NDPluginScatterMethod`` parameter in the driver to allow
future implementations of other scheduling schemes, should this prove
desireable.

NDPluginScatter inherits from NDPluginDriver. NDPluginScatter does not
do any modification to the NDArrays that it receives except for possibly
adding new NDAttributes if an attribute file is specified. The
`NDPluginScatter class
documentation <../areaDetectorDoxygenHTML/class_n_d_plugin_scatter.html>`__
describes this class in detail.

Configuration
-------------

The NDPluginScatter plugin is created with the `NDScatterConfigure`
command, either from C/C++ or from the EPICS IOC shell.

::

   NDScatterConfigure (const char *portName, int queueSize, int blockingCallbacks, 
                         const char *NDArrayPort, int NDArrayAddr, size_t maxMemory, 
                         int priority, int stackSize)
     

For details on the meaning of the parameters to this function refer to
the detailed documentation on the NDScatterConfigure function in the
`NDPluginScatter.cpp
documentation <../areaDetectorDoxygenHTML/_n_d_plugin_scatter_8cpp.html>`__
and in the documentation for the constructor for the `NDPluginScatter
class <../areaDetectorDoxygenHTML/class_n_d_plugin_scatter.html>`__.

Screen shots
------------

The following is the MEDM screen that provides control of the
NDNDPluginScatter plugin. Note that it currently only contains the
controls for the base class NDPluginDriver.

.. figure:: NDScatter.png
    :align: center

