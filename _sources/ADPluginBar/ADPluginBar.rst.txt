ADPluginBar Documentation
=========================

Author: Jakub Wlodek
~~~~~~~~~~~~~~~~~~~~~

Corresponding Author: Kazimierz Gofron
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Installing the plugin
---------------------

The ADPluginBar plugin depends on several external libraries. These
include:

-  Opencv: Can be built from source from
   `github <https://github.com/opencv/opencv>`__. Otherwise install from
   package manager.
-  Zbar: Can be built from source from
   `github <https://github.com/ZBar/ZBar>`__. Otherwise install from
   package manager.
-  EPICS, Area Detector, and their submodules, all of whuch can be found
   on github.

Further installation instructions are located in the README.md file in
the top level of this repository.

--------------

Using ADPluginBar
~~~~~~~~~~~~~~~~~

*The sample image used in all screens in this document captured with a
prosilica camera*

|barcode|

| ADPluginBar (current version 2-0) has several requirements for proper
  functionality. As pf R2-0, 8 bit and 16 bit images are supported, in
  both Mono and RGB mode.
| Not that the image will be converted to grayscale if input is in RGB.
  Output arrays from the plugin will be in RGB because barcode bounding
  boxes are displayed in blue.
| Some conversion information between NDArrays and OpenCV Mat objects
  used as a conversion element is included in a file in the barSrc
  directory. Additionally, though inverted
| bar codes are supported, they require a PV flag to be set, and thus
  cannot be read in conjunction with standard bar or QR codes. They are
  also only supported in 8-bit mode. This is another feature
| that is being considered for a future release.
| Once ADPluginBar is installed, Some edits must be made to the common
  plugins screen linked to your detector. It should look as follows:

|commonplugins|

| The Address that should be passed to each of the elements is Bar1.
  Thus, the entire PV would be, for example, "$(P)Bar1:PortName\_RBV"
  for the portname field.
| The "more" button should be linked to the provided "NDPluginBar.opi"
  screen, or an equivalent adl or edl screen. (Currently only CSS
  screens are provided, though
| other types may be looked into in the future.) The NDPluginBar.opi
  screen looks as follows:

|NDPluginBar.opi|

| ADPluginBar supports reading up to 5 bar codes simultaneously, though
  currently only the first discovered barcode has its coordinates stored
  as a PV.
| In order to access the remaining 4 barcodes, simply press the button
  titled: "View 1-5". Once pressed, the following screen will display:

|barcodes1-5|

| As you can see, in the sample image used above, two barcodes are
  present, the QR code which was generated with a test message, and a
  two dimensional barcode taken form
| a box from around the test area. Both codes were detected ane placed
  into PVs BarcodeMessage1\_RBV, BarcodeType1\_RBV and the '2'
  equivalents.

|liveupdate|

| As of release R2-0, ADPluginBar supports a live view of the detected
  barcodes in an image. To use this feature, set use any EPICS viewer,
  and set the Array Port to BAR1,
| or whatever port your plugin has been set to. In the image above, I
  used the NDPluginStdArrays plugin for viewing in CSS. When a barcode
  is detected, a blue bounding box is drawn
| around the code.
| For more information on the newest release of ADPluginBar please check
  the RELEASE.md file in the top level directory. Additionally, please
  contact the
| author with any questions regarding the usage of the plugin or feature
  requests.

--------------

Release Notes
-------------

R2-1 (8-May-2019)

--------------

-  Features Added:

   -  Threading has been added (similar to NDPluginDmtx) for much faster
      performance, and no lost frames

-  Bug Fixes

   -  Threading for improved performance removes issues with dropped
      frames when image size was too large
   -  Y-coordinate for corner PVs now indicates from the bottom up
      rather than from the top down

-  Future Plans

   -  Possible integration with
      `ADPluginDmtx <https://github.com/epicsNSLS2-areaDetector/ADPluginDmtx>`__
   -  Image autosaving when barcode detected
   -  Database integration

R2-0 (4-January-2018)
~~~~~~~~~~~~~~~~~~~~~

-  Features Added:

   -  Corners can now be displayed for any of the 5 barcodes via a PV
      toggle
   -  Number Codes now lists the number of codes in the image not the
      total number of codes
   -  Passing the ADPluginBar Array Port to an EPICS Viewer allows for
      live display of detected barcodes and QR codes
   -  Support for both 8 bit and 16 bit images
   -  Added support for input of RGB images as opposed to only Mono
      images
   -  Barcode Message PVs changed to waveforms to increase max character
      count to 256

-  Bug Fixes/Code Refactoring:

   -  Fixed issue where plugin would freeze in certain situations where
      codes were repeated.
   -  Removed redundant functions, split up large functions into more
      compact pieces
   -  Improved documentation for all functions

-  Future Plans:

   -  Add support for automatic image saving when barcodes detected
   -  Support for databases like MySQL and MongoDB

R1-1 (27-June-2018)
~~~~~~~~~~~~~~~~~~~

-  Several key features added and bugs fixed:

   -  Support added for reading up to 5 codes in one image (Corner data
      stored in PV for first code)
   -  Support added for reading inverted bar codes i.e. white code on
      black background
   -  Fixed issue where number of barcodes would continue to increment
      when seeing the same bar code
   -  Fixed issuse where seeing the same barcode would cause an infinite
      loop of asyn messages in the IOC shell

-  Future plans:

   -  Add ability to save image file with detected barcodes marked.
   -  Add ability to pipe information into an NDArray and display it in
      CSS

R1-0 (22-June-2018)
~~~~~~~~~~~~~~~~~~~

-  Original release. Some issues that can be resolved with future
   releases:

   -  Only mono images supported
   -  stringin record used for barcode message, so limit of 40
      characters currently

--------------

Code Documentation
------------------

Below is an explanation of functions included in ADPluginBar and the
general plugin structure.

ADPluginBar File structure
~~~~~~~~~~~~~~~~~~~~~~~~~~

|ADPluginBar file structure|

All of the plugin Source code is housed in the barApp/barSrc directory.
The barApp/Db/NDBar.template file contains PV definitions.
barApp/screens contains CSS .opi files.

Function definitions

+-------------------------------+---------------------------+--------------+------------------------------------------------------------------------------------------------------------+
| Function Name                 | InputArgs                 | OutputArgs   | Description                                                                                                |
+===============================+===========================+==============+============================================================================================================+
| initPVArrays                  | None                      | None         | Function that places PV indexes into arrays for easier iteration                                           |
+-------------------------------+---------------------------+--------------+------------------------------------------------------------------------------------------------------------+
| printCVError                  | None                      | None         | Function for printing out OpenCV exception information                                                     |
+-------------------------------+---------------------------+--------------+------------------------------------------------------------------------------------------------------------+
| clearPreviousCodes            | None                      | None         | Function that clears out the currently detected barcodes.                                                  |
+-------------------------------+---------------------------+--------------+------------------------------------------------------------------------------------------------------------+
| ndArray2Mat                   | pArray, arrayInfo         | img          | Function that converts input NDArray pArray into output Mat img                                            |
+-------------------------------+---------------------------+--------------+------------------------------------------------------------------------------------------------------------+
| mat2NDArray                   | img                       | pScratch     | Function that converts input Mat img into output NDArray pScratch                                          |
+-------------------------------+---------------------------+--------------+------------------------------------------------------------------------------------------------------------+
| fix\_inverted                 | img                       | None         | Function that inverts black and white in image to allow for detecting inverted codes                       |
+-------------------------------+---------------------------+--------------+------------------------------------------------------------------------------------------------------------+
| push\_corners                 | symbol, update\_corners   | discovered   | Function that updates poition of discovered code from symbol and updates PVs if update\_corners flag set   |
+-------------------------------+---------------------------+--------------+------------------------------------------------------------------------------------------------------------+
| update\_corners               | discovered                | None         | Function that updateds corner PVs to those of discovered                                                   |
+-------------------------------+---------------------------+--------------+------------------------------------------------------------------------------------------------------------+
| scan\_image                   | img                       | None         | Function that changes Mat to zbar image and scans it with zbar                                             |
+-------------------------------+---------------------------+--------------+------------------------------------------------------------------------------------------------------------+
| clear\_unused\_barcode\_pvs   | counter                   | None         | Function that resets PVs that had detected barcode earlier, but no longer do                               |
+-------------------------------+---------------------------+--------------+------------------------------------------------------------------------------------------------------------+
| decode\_bar\_codes            | img                       | None         | Function that scans image, then loops over any detected codes and updates PVs                              |
+-------------------------------+---------------------------+--------------+------------------------------------------------------------------------------------------------------------+
| show\_bar\_codes              | img                       | None         | Function that takes list of discovered codes and draws them on the image based on pushed corners           |
+-------------------------------+---------------------------+--------------+------------------------------------------------------------------------------------------------------------+
| writeInt32                    | pasynUser, value          | None         | Function that is called when a PV is written to                                                            |
+-------------------------------+---------------------------+--------------+------------------------------------------------------------------------------------------------------------+
| processCallbacks              | pArray                    | None         | Function that is called for each NDArray created by the attached driver                                    |
+-------------------------------+---------------------------+--------------+------------------------------------------------------------------------------------------------------------+

More detailed documentation can be found in the plugin Src.

copyright: Brookhaven National Laboratory 2017-2019
---------------------------------------------------

.. |barcode| image:: barcode.png
   :width: 500px
.. |commonplugins| image:: commonplugins.png
   :width: 700px
.. |NDPluginBar.opi| image:: Barcodemain.png
   :width: 700px
.. |barcodes1-5| image:: bar1-5.png
   :width: 500px
.. |liveupdate| image:: liveupdate.png
   :width: 1000px
.. |ADPluginBar file structure| image:: fileStructure.png
   :width: 400px
