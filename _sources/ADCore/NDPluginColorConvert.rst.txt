NDPluginColorConvert
====================

:author: Mark Rivers, University of Chicago

.. contents:: Contents

Overview
--------

NDPluginColorConvert is a tool for converting the color mode of NDArray
data. It receives an input NDArray with one color mode and outputs
another NDArray with a (potentially) different color mode. All other
attributes of the array are preserved.

NDPluginColorConvert inherits from NDPluginDriver. The
`NDPluginColorConvert class
documentation <../areaDetectorDoxygenHTML/class_n_d_plugin_color_convert.html>`__
describes this class in detail.

NDPluginColorConvert defines the following parameters. It also
implements all of the standard plugin parameters from
:doc:`NDPluginDriver`. The EPICS database
NDColorConvert.template provides access to these parameters, listed in
the following table.

.. cssclass:: table-bordered table-striped table-hover
.. flat-table::
  :header-rows: 2
  :widths: 5 5 5 70 5 5 5

  * -
    - **Parameter Definitions in NDPluginColorConvert.h and EPICS Record Definitions in
      NDColorConvert.template**
  * - Parameter index variable
    - asyn interface
    - Access
    - Description
    - drvInfo string
    - EPICS record name
    - EPICS record type
  * - NDPluginColorConvertColorModeOut
    - asynInt32
    - r/w
    - The output color mode (NDColorMode_t).
    - COLOR_MODE_OUT
    - $(P)$(R)ColorModeOut, $(P)$(R)ColorModeOut_RBV
    - mbbo, mbbi
  * - NDPluginColorConvertFalseColor
    - asynInt32
    - r/w
    - The false color map index. There are currently 2 false color maps defined, Rainbow
      and Iron. Additional color maps can easily be added in the future.
    - FALSE_COLOR
    - $(P)$(R)FalseColor, $(P)$(R)FalseColor_RBV
    - mbbo, mbbi
      
When converting from 8-bit mono to RGB1, RGB2 or RGB3 a false-color map
will be applied if FalseColor is not zero.

The Bayer color conversion supports the 4 Bayer formats (NDBayerRGGB,
NDBayerGBRG, NDBayerGRBG, NDBayerBGGR) defined in ``NDArray.h``. If the
input color mode and output color mode are not one of these supported
conversion combinations then the output array is simply a copy of the
input array and no conversion is performed.

Configuration
-------------

The NDPluginColorConvert plugin is created with the following command,
either from C/C++ or from the EPICS IOC shell.

::

    int NDColorConvertConfigure(const char *portName, int queueSize, int blockingCallbacks, 
                                const char *NDArrayPort, int NDArrayAddr, 
                                int maxBuffers, size_t maxMemory,
                                int priority, int stackSize)
     

For details on the meaning of the parameters to this function refer to
the detailed documentation on the NDColorConvertConfigure function in
the `NDPluginColorConvert.cpp
documentation <../areaDetectorDoxygenHTML/_n_d_plugin_color_convert_8cpp.html>`__
and in the documentation for the constructor for the
`NDPluginColorConvert
class <../areaDetectorDoxygenHTML/class_n_d_plugin_color_convert.html>`__.

Screen shots
------------

The following is the MEDM screen that provides access to the parameters
in ``NDPluginDriver.h`` and ``NDPluginColorConvert.h`` through records in
``NDPluginBase.template`` and ``NDColorConvert.template``.

.. image:: NDColorConvert.png
    :align: center

Performance
-----------

The following performance for Bayer conversion was measured on a RHEL6 workstation with a 
3.6 GHz 4C/8T Intel W-2123 CPU and 16 GB of RAM. 
The first two rows show the plugin not having to do anything, and the next four show 
real conversions with two different image sizes and different output modes.

.. cssclass:: table-bordered table-striped table-hover
.. flat-table::
  :header-rows: 1

  * - Input
    - Output
    - SizeX
    - Size Y
    - Average (ms)
    - S.D. (ms)
    - Samples
  * - RGB	
    - RGB
    - 1292
    - 964
    - 1.436
    - 0.344
    - 103
  * - RGB
    - RGB
    - 646
    - 482
    - 0.488	
    - 0.142
    - 158
  * - Bayer
    - RGB
    - 1292
    - 964
    - 19.534
    - 3.247
    - 154
  * - Bayer
    - RGB
    - 646
    - 482
    - 9.306
    - 2.912
    - 148
  * - Bayer	
    - Mono
    - 1292
    - 964
    - 20.889
    - 2.737
    - 151
  * - Bayer
    - Mono
    - 646
    - 482
    - 8.985
    - 2.927
    - 135

Restrictions
------------

- The Bayer color conversion uses a simple linear interpolation and as seen in the table above can only
  process about 60 MPixels per second. 

  * For Point Grey/FLIR cameras the ADPointGrey and ADSpinnaker drivers do Bayer color conversion
    in the vendor library, which is significantly faster.
  * For Prosilica/AVT cameras the ADProsilica and ADVimba drivers also do Bayer color conversion
    in the vendor library, which is significantly faster.

- YUV color conversion is not supported. This may be added in a future
  release.


