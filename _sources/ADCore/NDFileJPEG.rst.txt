NDFileJPEG
==========

:author: Mark Rivers, University of Chicago

.. contents:: Contents

Overview
--------

NDFileJPEG inherits from NDPluginFile. This plugin saves data in the
`JPEG <http://en.wikipedia.org/wiki/JPEG>`__ file format, which is a
compressed file format for storing images. There is JPEG support for
almost all languages and programs such as IDL and Matlab.

The JPEG plugin is limited to 8-bit arrays. It supports all color modes
(Mono, RGB1, RGB2, and RGB3). It is limited to a single array per file,
but capture and stream mode are supported by writing multiple JPEG
files.

The JPEG plugin supports the Int32 parameter NDFileJPEGQuality to
control the amount of compression in the file. This parameter varies
from 0 (maximum compression, lowest quality) to 100 (least compression,
best quality). NDFileJPEG.template defines 2 records to support this:
$(P)$(R)JPEGQuality (longout) and $(P)$(R)JPEGQuality_RBV (longin).

The `NDFileJPEG class
documentation <../areaDetectorDoxygenHTML/class_n_d_file_j_p_e_g.html>`__
describes this class in detail.

Configuration
-------------

The NDFileJPEG plugin is created with the ``NDFileJPEGConfigure`` command,
either from C/C++ or from the EPICS IOC shell.

.. code-block:: c

   NDFileJPEGConfigure (const char *portName, int queueSize, int blockingCallbacks, 
                        const char *NDArrayPort, int NDArrayAddr, size_t maxMemory, 
                        int priority, int stackSize)
     

For details on the meaning of the parameters to this function refer to
the detailed documentation on the ``NDFileJPEGConfigure`` function in the
`NDFileJPEG.cpp
documentation <../areaDetectorDoxygenHTML/_n_d_file_j_p_e_g_8cpp.html>`__
and in the documentation for the constructor for the `NDFileJPEG
class <../areaDetectorDoxygenHTML/class_n_d_file_j_p_e_g.html>`__.

Screen Shots
------------

.. figure:: NDFileJPEG.png
    :align: center

