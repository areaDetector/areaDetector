NDPluginAttrPlot
================

:author: Bla Kranjc, Cosylab

.. contents:: Contents

Overview
--------

This plugin is aimed for drivers that expose data as NDAttributes. The
main use case for the plugin is to retrieve NDAttribute values, cache
them and expose them as a waveform. This enables easy plotting of
real-time values of the attributes in OPIs. Graphical representation of
the use case is depicted in Figure 1.

The plugin is similar to the
:doc:`NDPluginAttribute`, the differences are
described in the section `Comparison to
NDPluginAttribute`_. All configuration options and
instructions are contained in the section `Configuration`_. A
real world example of the plugin usage and EDM GUI can be found in the
section `Screenshots`_. Details of added parameters for this
plugin and other development details can be found in the section
`Developer Notes`_. 

.. figure:: NDPluginAttrPlot-usecase.png
    :align: center

    Typical use case that can make use of the NDPluginAttrPlot.

The plugin is initialized when the first NDArray is passed to it and
reinitialized whenever the ``$(P)$(R)Reset`` is processed or the
unique ID of the received NDArray has decreased. The names of the
attributes that are saved are not part of the plugin configuration but
are extracted from the NDArray. Unique ID of the NDArray is always
tracked as if it was an attribute. During initialization the list of
NDAttributes in the NDAttributeList of the NDArray is traversed and a
preconfigured number of NDAttribute names and their values are saved on
the first come first served basis.

.. note::
    Please note that only numerical
    attributes are supported by the plugin. String attributes are skipped
    and all numerical attribute values are converted to ``double``.

The values are saved internally in the plugin in a circular buffer. The
length of this buffer is configured at the plugin load and cannot be
configured during runtime. Also all attributes use the same value for
the circular buffer length.

The names of the NDAttributes that are saved by the plugin are exposed
to the EPICS layer in ``$(P)$(R)Attr$(ATTR_IND)`` records. To reduce
the strain on the channel access interface not all circular buffers are
exposed at all times, but only a preconfigured number of waveform
records are available. The records are named
``$(P)$(R)$(AXIS):Data$(DATA_IND)``. Additional macro ``AXIS`` is
added to clearly name the plot's X or Y axis if required. As the main
purpose of the plugin is live plotting the data records are processed
periodically with 1 Hz and is not tied to the acquisition period. The
data that is exposed to the waveforms is selected by writing the index
(``ATTR_IND``) of desired attribute to the
``$(P)$(R)$(AXIS):DataSelect$(DATA_IND)`` record. Magic numbers are
used for selection of Unique ID field (value of -1) and a selection of
"None" - array of all zeros (value of -2). The readback of index and the
name of the attribute name are then exposed on
``$(P)$(R)$(AXIS):DataSelect$(DATA_IND)_RBV`` and
``$(P)$(R)$(AXIS):DataLabel$(DATA_IND)`` respectively. Current length
of the internal circular buffer is provided to the user via
``$(P)$(R)NPts`` record.

Additional notes about this plugin:

-  If the value for an attribute that is being saved is not present in
   the NDArray a 0 is written for that value in the circular buffer;
-  Exposed attributes are preserved among re-initializations if
   possible.

Comparison to NDPluginAttribute
-------------------------------

NDPluginAttrPlot is different from NDPluginAttribute in the following
ways:

-  Attributes are not selected by name but rather a list of first N
   attributes is extracted from the first NDArray of an acquisition;
-  Circular buffer is used for attribute values;
-  The attribute values are written to the waveforms periodically (1Hz),
   decoupled from the callbacks;
-  Only exposes raw attribute values;
-  All numerical attributes are supported but the values are converted
   to ``double``.

Configuration
-------------

The NDPluginAttribute plugin is created with the NDAttrConfigure
command, either from C/C++ or from the EPICS IOC shell. The prototype of
its constructor is

.. code:: cpp

    NDPluginAttrPlot(const char * portName, int n_attributes, int cache_size, int n_data_blocks, 
                     const char * NDArrayPort, int NDArrayAddr, int queueSize, int blockingCallbacks, int priority, int stackSize)


Parameters ``NDArrayPort``, ``NDArrayAddr``, ``portName``,
``queueSize``, ``blockingCallbacks``, ``priority`` and ``stackSize`` are
arguments common to all plugins.

Configuration of the driver instance is tightly coupled to instantiation
of EPICS dbs and macro substitution. Unfortunately this tight coupling
must be managed manually

-  ``n_attributes`` defines the maximum number of attributes that are
   saved by the driver. ``n_attributes`` instances of
   ``NDAttrPlotAttr.template`` must be loaded with ``ATTR_IND`` macro
   ranging from 0 to ``n_attributes - 1``.
-  ``n_data_blocks`` defines the number of data exposing waveforms.
   ``n_data_blocks`` instances of ``NDAttrPlotData.template`` must be
   loaded with ``DATA_ADDR`` macro ranging from 0 to
   ``n_data_blocks - 1``.
-  ``cache_size`` defines the max length of circular buffers that are
   used to save values of the attributes. The macro ``N_CACHE`` in
   ``NDAttrPlotData.template`` must be set to the same value.

For further details please refer to the documentation in the source file
``NDPluginAttrPlot.cpp``.

Screenshots
-----------

.. figure:: NDPluginAttrPlot.png
    :align: center

    Main plugin screen of the NDPluginAttrPlot plugin

.. figure:: NDPluginAttrPlot_Plot.png
    :align: center

    Plot screen showing two parameters plotted against their
    Unique ID

.. figure:: NDPluginAttrPlot_Select.png
    :align: center
    
    Attributes to plot are selected on the select screen

Developer Notes
---------------

NDPluginAttrPlot inherits from :doc:`NDPluginDriver`. The NDPluginAttribute
class documentation describes this class in detail.

NDPluginAttrPlot defines the following parameters. It also implements
all of the standard plugin parameters from NDPluginDriver.

.. cssclass:: table-bordered table-striped table-hover
.. flat-table::
  :header-rows: 2
  :widths: 5 5 5 70 5 5

  * -
    - **Parameter Definitions in `NDPluginAttrPlot.h` and EPICS Record Definitions in `NDAttrPlot.template`**
  * - drvInfo string
    - asyn interface
    - Access
    - Description
    - EPICS record name
    - EPICS record type
  * - AP_Reset
    - asynInt32
    - w
    - Write to this record forces reinitialization of the plugin on the next callback.
    - $(P)$(R)Reset
    - ao
  * - AP_NPts
    - asynInt32
    - r
    - Number of saved cached points.
    - $(P)$(R)NPts
    - ai
  * - AP_Attribute
    - asynOctetRead
    - r
    - Attribute name.
    - $(P)$(R)Attr$(ATTR_IND)
    - stringin
  * - AP_Data
    - asynFloat64ArrayIn
    - r
    - Exposed data for $(DATA_ADDR).
    - $(P)$(R)$(AXIS):Data$(DATA_IND)
    - waveform
  * - AP_DataSelect
    - asynInt32
    - r/w
    - Index of selected attribute for $(DATA_ADDR).
    - $(P)$(R)$(AXIS):DataSelect$(DATA_IND), $(P)$(R)$(AXIS):DataSelect$(DATA_IND)_RBV
    - ao, ai
  * - AP_DataLabel
    - asynOctetRead
    - r
    - Attribute name for $(DATA_ADDR).
    - $(P)$(R)$(AXIS):DataLabel$(DATA_IND)
    - stringin

If the array data contains more than 16K bytes then in order for EPICS
clients to the environment variable ``EPICS_CA_MAX_ARRAY_BYTES`` on both the
EPICS IOC computer and EPICS client computer must be set to a value at
least as large as the array size in bytes.

