IDL Viewer
==========
:author: Mark Rivers
:affiliation: University of Chicago

.. contents:: Contents

There is an IDL procedure called
`epics_ad_display <https://cars.uchicago.edu/software/idl/imaging_routines.html#epics_ad_display>`__
that can be used to display 2-dimensional array data that the
:doc:`../ADCore/NDPluginStdArrays` sends to EPICS. This IDL
client is available as source code (which requires an IDL license), and
also as a pre-built IDL .sav file that can be run for free under the IDL
Virtual Machine. This IDL program can run on any machine that IDL runs
on, and that has the ezcaIDL shareable library built for it. This
includes Windows, Linux, Solaris, and Mac. ``epics_ad_display`` is
included in the `CARS IDL imaging
software. <https://cars.uchicago.edu/software/IDL/imaging.html>`__ It is
also available in the Viewers/IDL directory in the areaDetector
application.

The Viewers/IDL directory contains both the IDL source code and a
standalone IDL file, epics_ad_display.sav, for the epics_ad_display GUI
to display images from areaDetector detectors. This file can be run for
free on any Linux or Windows system under the IDL Virtual Machine, which
can be downloaded free of charge from `ITT
VIS <http://www.ittvis.com/idl>`__. That directory also contains the
shareable libraries used to call EPICS Channel Access from IDL
(ezcaIDL.dll for Windows and libezcaIDL.so for Linux). Before using the
IDL source code or .sav file it is necessary to define the environment
variable EZCA_IDL_SHARE to point to the complete path to ezcaIDL.dll or
libezcaIDL.so. For example on Linux:

::

   setenv EZCA_IDL_SHARE /home/epics/support/areaDetector/1-5/Viewers/IDL/libezcaIDL.so
     

On Windows use

::

   My Computer/Properties/Advanced/Environment Variables/ 
     

to add a new environment variable EZCA_IDL_SHARE to point to the
location of ezcaIDL.dll on your system. To run the standalone IDL
epics_ad_display.sav file without an IDL license execute the following
on Linux:

::

   idl -32 -vm=epics_ad_display.sav 
     

On Windows simply double-click on the icon for the epics_ad_display.sav
file.

When the GUI comes up type the base PV name for the NDStdArrays plugin
for your detector in the "Base PV" widget. For example with the
simulation detector supplied with the areaDetector application this is
"13SIM1:image1:" (without the quotes). Once the detector begins
acquiring images they should be displayed in the IDL window.

To run the GUI from the IDL command line on a system with an IDL license
type the epics_ad_display command followed by the base PV name of the
NDStdArrays plugin. For example:

::

     IDL> epics_ad_display, '13SIM1:image1:'
     

The control window for ``epics_ad_display`` is shown below. It has a
field to input the base name of the EPICS PVs with the image data. It
also has fields to enable/display the IDL display update, to change the
display mode, to autoscale the intensity, and to invert the image in the
Y direction. If autoscale is set to No then manual scaling can be
entered in the Min and Max fields. The number of frames per second
actually being displayed by IDL is shown. There is a status window that
shows whether the EPICS PVs are connected and the time the last was
array received, updated once per second.

.. figure:: IDL_epics_ad_display.png
    :align: center

    Main window for IDL epics_ad_display

``epics_ad_display`` can use the simple IDL routine ``tv`` to display
the images. This is the fastest mode, and results in a non-scalable
unadorned window.

.. figure:: IDL_epics_ad_display_tv.jpg
    :align: center

    IDL epics_ad_display using the IDL ``tv`` routine.

``epics_ad_display`` can also use the routine
`image_display.pro <https://cars.uchicago.edu/software/IDL/imaging_routines.html#IMAGE_DISPLAY>`__
to display the images. This routine displays row and column profiles as
the cursor is moved. It allows changing the color lookup tables, and
zooming in (right mouse click) and out (left mouse click). Note that
image_display is not currently capable of displaying color data i.e.,
RGB1, RGB2, or RGB3 NDArrays). It can however, display Mono data in
false color. The following is an example of ``image_display`` displaying
an image from the simulation detector.

.. figure:: simDetector_image_display.png
    :align: center

    epics_ad_display using the image_display routine

The Viewers/IDL directory also contains an IDL function 
`read_nd_netcdf <https://cars.uchicago.edu/software/idl/detector_routines.html#read_nd_netcdf>`__
to read the areaDetector netCDF files. This is described in the :ref:`NDFileNetCDF`
documentation.
