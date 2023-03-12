NDPluginTransform
=================
:authors: Christian Roehrig, Mark Rivers, John Hammonds
:affiliation: Argonne National Laboratory

.. contents:: Contents

Overview
--------

This plugin provides 8 choices for image transforms that involve
rotations by multiples of 90 degrees and mirror reflections about the
central vertical line of the image. The plugin supports only 2-D
monochrome and color images (RGB1, RGB2, and RGB3).

NDPluginTransform inherits from NDPluginDriver. The `NDPluginTransform
class
documentation <../areaDetectorDoxygenHTML/class_n_d_plugin_transform.html>`__
describes this class in detail.

``NDPluginTransform.h`` defines the following parameters. It also implements
all of the standard plugin parameters from
:doc:`NDPluginDriver`. The EPICS database
``NDransform.template`` provide access to these parameters, listed in the
following table.

.. |br| raw:: html

    <br>

.. cssclass:: table-bordered table-striped table-hover
.. flat-table:: 
  :header-rows: 2
  :widths: 5 5 5 70 5 5 5

  * -
    - 
    - **Parameter Definitions in NDPluginTransform.h and EPICS Record Definitions in NDTransform.template**
  * - Parameter index variable
    - asyn interface
    - Access
    - Description
    - drvInfo string
    - EPICS record name
    - EPICS record type
  * - NDPluginTransformType
    - asynInt32
    - r/w
    - Type of transform. Choices are:|br|
      **None**: No transform, the output image is the same as the input image. |br|
      **Rot90**: Rotate clockwise 90 degrees. |br|
      **Rot180**: Rotate clockwise 180 degrees. |br|
      **Rot270**: Rotate clockwise 270 degrees; equivalent to counter-clockwise rotation by 90 degrees. |br|
      **Mirror**: Mirror reflection about the central column in the image. |br|
      **Rot90Mirror**: Rot90 followed by Mirror. Equivalent to image transpose, swapping
      rows and columns. |br|
      **Rot180Mirror**: Rot180 followed by Mirror. Equivalent to a mirror reflection about
      the central row in the image. |br|
      **Rot270Mirror**: Rot270 followed by Mirror. Equivalent to image transpose followed
      by mirror reflection about the central column in the image. |br|
    - TRANSFORM_TYPE
    - $(P)$(R)Type
    - mbbo


Configuration
-------------

The NDPluginTransform plugin is created with the ``NDTransformConfigure``
command, either from C/C++ or from the EPICS IOC shell.

::

   NDTransformConfigure(const char *portName, int queueSize, int blockingCallbacks,
                  const char *NDArrayPort, int NDArrayAddr,
                  int maxBuffers, size_t maxMemory,
                  int priority, int stackSize)
     

For details on the meaning of the parameters to this function refer to
the detailed documentation on the NDTransformConfigure function in the
`NDPluginROI.cpp
documentation <../areaDetectorDoxygenHTML/_n_d_plugin_transform_8cpp.html>`__
and in the documentation for the constructor for the `NDPluginTransform
class <../areaDetectorDoxygenHTML/class_n_d_plugin_transform.html>`__.

Screen shots
------------

The following MEDM screen provides access to the parameters in
``NDPluginDriver.h`` and ``NDPluginTransform.h`` through records in
``NDPluginBase.template`` and ``NDTransform.template``. The orientation of the
letter F on the screen shows what each transform type does.

.. figure:: NDTransform.png
    :align: center

Performance
-----------

The following is a measurement of the performance of the
NDPluginTransform plugin in release R2-1. The measurements were done
with the simDetector on an 8-core Linux machine. All plugins except the
NDPluginTransform plugin were disabled. The simDetector was generating
about 680 frames/s in mono mode and about 190 frames/s in RGB1 mode. The
plugin was thus always dropping frames except when the transformation
was None in mono and RGB1 mode, and when the transformation was
Rot180Mirror in mono mode.

.. cssclass:: table-bordered table-striped table-hover
.. flat-table::
  :header-rows: 2

  * -
    - **Performance (frames/s)**
  * - Dimensions
    - Transformation
    - 8-bit Mono
    - 8-bit RGB1
  * - 1024 x 1024
    - None
    - 680
    - 190
  * - 1024 x 1024
    - Rot90
    - 115
    - 40
  * - 1024 x 1024
    - Rot180
    - 145
    - 52
  * - 1024 x 1024
    - Rot270
    - 105
    - 41
  * - 1024 x 1024
    - Mirror
    - 152
    - 56
  * - 1024 x 1024
    - Rot90Mirror
    - 116
    - 40
  * - 1024 x 1024
    - Rot180Mirror
    - 680
    - 75
  * - 1024 x 1024
    - Rot270Mirror
    - 111
    - 41

Note that this performance with ADCore R2-1 and later is dramatically
improved from R2-0 and earlier. For example, in R2-0 with 1024 x 1024
8-bit mono images the frame rate for all transformations (including
None) was only 8 frames/s. With 8-bit RGB1 the frame rate for all
transformations was only 3 frames/s. Thus, R2-1 improves the performance
by a factor of 13-85 compared to previous versions.

