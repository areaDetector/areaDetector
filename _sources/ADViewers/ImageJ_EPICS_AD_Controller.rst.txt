ImageJ Controller (EPICS_AD_Controller.java)
============================================

:author: Mark Rivers
:affiliation: University of Chicago

.. contents:: Contents

This is an ImageJ plugin which can be used to graphically control the
following:

-  The readout region of the detector or camera.
-  The size and position of an ROI (NDPluginROI).
-  The size and position of an overlay (NDPluginOverlay).

Normally this plugin will be used together with the EPICS_AD_Viewer
plugin described above. However, that is not required. For example, the
ImageJ window used with EPICS_AD_Controller could be a window read from
a TIFF file that the detector previously wrote.

The detector and plugin chain can include any of the following elements
and settings:

-  Camera/detector (MinX, MinY, SizeX, SizeY, BinX, BinY, ReverseX,
   ReverseY)
-  Transform plugin (NDPluginTransform) (Type, i.e. the transform
   operation None, Rot90, Mirror, etc.)
-  ROI plugin (NDPluginROI) (MinX, MinY, SizeX, SizeY, BinX, BinY,
   ReverseX, ReverseY))
-  Overlay plugin (NDPluginOverlay)
-  NDPluginStdArrays plugin (used by EPICS_AD_Viewer to get images)

All of the above components are optional, they do not need to be
present. However, the components that are present must be in the above
order in the "viewing" plugin chain. In other words the Transform plugin
must come before the ROI and Overlay plugins, and the ROI plugin must
come before the Overlay plugin. For the Transform and ROI plugins there
is a flag that allows selecting whether or not this plugin is included
the plugin chain. Note that this flag does not control the plugin chain,
rather it must be set to correctly reflect the actual setting of the
plugin chain. It is possible to use EPICS_AD_Control to control the ROI
without the ROI plugin being in the plugin chain. In fact this is a
common use case.

The following is the EPICS_AD_Controller plugin control, located in the
ImageJ "Plugins/EPICS_areaDetector/EPICS AD Controller" menu.

.. figure:: ImageJ_EPICS_AD_Controller.png
    :align: center

    ImageJ EPICS_AD_Controller plugin control window

The following are the controls on the EPICS_AD_Controller screen:

-  String input controls for the EPICS PV prefixes for the Camera,
   Transform plugin, ROI plugin, and Overlay plugin.
-  "Output PVs" combo box control that selects which component will be
   defined when the Set button is pushed. Choices are "Camera", "ROI",
   and "Overlay".
-  "Set" control button. Pressing this button calculates the coordinates
   of the component to be defined and writes the values to the desired
   PVs.
-  "Reset camera region" control button. Pressing this button resets the
   MinX and MinY of the camera to 0 and sets SizeX and SizeY to the
   maximum image size (MaxSizeX, MaxSizeY). Note that it does not modify
   the camera BinX, BinY, ReverseX, or ReverseY.
-  "Reset ROI" control button. Pressing this button resets the MinX and
   MinY of the ROI to 0 and sets SizeX and SizeY to the maximum camera
   image size (MaxSizeX, MaxSizeY). Note that it does not modify the ROI
   BinX, BinY, ReverseX, or ReverseY.
-  "Transform Plugin In Chain" checkbox. Check this box if the Transform
   plugin is part of the viewing plugin chain.
-  "ROI Plugin In Chain" checkbox. Check this box if the ROI plugin is
   part of the viewing plugin chain. Note that EPICS_AD_Controller can
   define the ROI even if it is not part of the viewing plugin chain,
   and in fact this is a common use case.
-  Status text box. Errors and informational messages are displayed
   here.

When using EPICS_AD_Controller to define the camera readout region then
the following rule must be followed. If the ROI plugin is in the viewing
chain then it must first be set to pass the entire image, i.e. MinX and
MinY must be 0, and SizeX and SizeY must be at least as large as the
image from the camera. This can be conveniently done by pressing the
"Reset ROI" button before defining the ImageJ ROI to select the camera
readout region. This is not required if the ROI is not in the viewing
plugin chain.

The following is the EPICS_AD_Viewer image display window with an
ellipse overlay that was defined using an ImageJ rectangular ROI and
EPICS_AD_Controller.

.. figure:: ImageJ_EPICS_AD_Controller_Overlay.png
    :align: center

    ImageJ EPICS_AD_Viewer plugin display window with
    rectangular Image ROI defining an elliptical overlay NDPluginOverlay

The EPICS_AD_Control plugin must be compiled in the same manner
described for EPICS_AD_Viewer above.
