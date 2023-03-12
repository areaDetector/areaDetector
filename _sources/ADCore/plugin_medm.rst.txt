Base class medm screens
=======================

There are 2 medm screens for the NDPluginDriver. The first is
``NDPluginBase.adl``. This exposes a subset of the EPICS PVs for the
NDPluginDriver base class. It is normally included in the medm screen
for every plugin. For example, the following is the medm screen for the
NDPluginStdArrays plugin. Because this plugin does not have any
additional records beyond those in the NDPluginDriver base class, this
medm screen consists only the NDPluginBase.adl file.

.. image:: NDPluginBase.png
    :align: center

``NDPluginBase.adl`` displays only the PVs that are most commonly used, and
does not expose the PVs that are intended more for expert configuration.
NDPluginBaseFull.adl displays all of the PVs in NDPluginDriver base
class, include those controlling the queue size, number of threads,
output array sorting, etc. This display can be opened from the More
related display menu in NDPluginBase.adl.

.. image:: NDPluginBaseFull.png
    :align: center
