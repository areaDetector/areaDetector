NDPluginPvxs
===========
:author: James Souter, Diamond Light Source

.. contents:: Contents

Overview
--------

This plugin reimplements the functionality of :ref:`NDPluginPva`, but requires the linking
of the pvxs support module, with no requirement for the pvAccessCPP, pvDatabaseCPP, pvDataCPP or
normativeTypesCPP support modules. The plugin wraps a pvxs SharedPV, which is added to
the singleton PVXS server that is started when the IOC starts. 

NDPluginPvxs defines the following parameters.

.. cssclass:: table-bordered table-striped table-hover
.. flat-table::
  :header-rows: 2
  :widths: 5 5 5 70 5 5 5

  * -
    -
    - **Parameter Definitions in NDPluginPvxs.h and EPICS Record Definitions in NDPvxs.template**
  * - Parameter index variable
    - asyn interface
    - Access
    - Description
    - drvInfo string
    - EPICS record name
    - EPICS record type
  * - NDPluginPvxsPvName
    - asynOctet
    - r/o
    - Name of the EPICSv4 PV being served
    - PV_NAME
    - $(P)$(R)PvName_RBV
    - waveform


Configuration
-------------

The NDPluginPvxs plugin is created with the ``NDPvxsConfigure`` command,
either from C/C++ or from the EPICS IOC shell.

::

   NDPvxsConfigure (const char *portName, int queueSize, int blockingCallbacks,
                         const char *NDArrayPort, int NDArrayAddr, const char *pvName,
                         size_t maxMemory, int priority, int stackSize)
     

For details on the meaning of the parameters to this function refer to
the detailed documentation on the NDPvxsConfigure function in the
`NDPluginPvxs.cpp
documentation <../areaDetectorDoxygenHTML/_n_d_plugin_pvxs_8cpp.html>`__ and
in the documentation for the constructor for the `NDPluginPvxs
class <../areaDetectorDoxygenHTML/class_n_d_plugin_pvxs.html>`__.

ADCore must be built with `WITH_PVXS=YES` in a CONFIG or CONFIG_SITE file to use `NDPluginPva.` 
While it is possible to load an `NDPluginPva` and `NDPluginPvxs` in the same IOC, users should be avoid
building ADCore with `WITH_QSRV=YES` when using `NDPluginPvxs`, as an ioc with `qsrv.dbd` loaded
`may automatically disable QSRV2. <https://epics-base.github.io/pvxs/ioc.html#qsrv-2>`

Starting the pvxs server
----------------------------

Unlike NDPluginPva, the EPICSv4 PV will be automatically served in an IOC if the plugin
is configured, however the 
`IOC must be built with pvxsIoc.dbd and the pvxs and pvxsIoc libraries. <https://epics-base.github.io/pvxs/building.html#including-pvxs-in-your-application>`