======
ADUVC_
======

An EPICS driver for USB Video Class (UVC) devices
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


:Author:               Jakub Wlodek, Brookhaven National Laboratory
:Corresponding author: Kazimierz Gofron, Brookhaven National Laboratory

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. contents:: Contents


..  _libuvc:       https://github.com/ktossell/libuvc
..  _ADUVC:        https://github.com/epicsNSLS2-areaDetector/ADUVC
..  _areaDetector: https://github.com/areaDetector
         
Overview
--------

UVC, or USB Video Class is an open Video connection standard used for
USB based cameras and webcams. Almost all consumer webcams support UVC,
along with a large number of other consumer USB cameras of different
form factors, ranging from pencil cameras to micro cameras. Some
industrial cameras support the format as well. ADUVC_ is an area detector
driver that adds support for UVC based cameras. It also includes some
useful test programs to check if your UVC device is being recognized
correctly.

--------------

Installation
------------

Detailed installation instructions for the ADUVC_ driver can be seen in
the
`README <https://github.com/epicsNSLS2-areaDetector/ADUVC/blob/master/README.md>`__
file. To install ADUVC_, you must first install libuvc_ and its
dependancies. The simplest way to do this is to run the provided bash
script in the adUVCSupport directory of the repository. This installs
libusub, libjpeg, and cmake, clones the libuvc_ repository on
github, and then builds it using
cmake. The dynamic library files resulting from the build are placed in
the /usr/local directory (on Linux), and include files must be placed in
adUVCSupport/include. From here you may compile the support programs or
the driver itself by entering the appropriate directory (the top ADUVC_
dir. for the driver), and compiling by switching to root and typing
'make'. The driver is now installed.

--------------

Issues with root ownership of UVC devices
-----------------------------------------

The USB camera device is typically owned by root, which prevents EPICS IOC from running as softioc user, and automatic startup using procServer. To grant access to USB camera device by other users, such as softioc, we wrote udev rules::
  
  kgofron@xf17bm-ioc2:/etc/udev/rules.d$ more usb-cams.rules
  # cam1 f007
  SUBSYSTEM=="usb", ATTRS{idVendor}=="f007", OWNER="softioc", GROUP="softioc", MODE="0666", SYMLINK="cam1"
  # cam2 0c45
  SUBSYSTEM=="usb", ATTRS{idVendor}=="0c45", OWNER="softioc", GROUP="softioc", MODE="0666", SYMLINK="cam2"
  # cam3 2560
  SUBSYSTEM=="usb", ATTRS{idVendor}=="2560", OWNER="softioc", GROUP="softioc", MODE="0666", SYMLINK="cam3"
  # cam4 eb1a (not attached)
  SUBSYSTEM=="usb", ATTRS{idVendor}=="eb1a", OWNER="softioc", GROUP="softioc", MODE="0666", SYMLINK="cam4"
  # cam5 eb1a
  SUBSYSTEM=="usb", ATTRS{idVendor}=="eb1a", OWNER="softioc", GROUP="softioc", MODE="0660", SYMLINK="cam5"

To find vendorID and supported modes use lsusb (see lsbusb output file entitled lsusb.rst).

--------------


Driver Contents
---------------

-  install-libuvc.sh -> Installation script for libuvc_ and its
   dependancies.
-  cameraDetector -> C++ program that detects all connected UVC devices.
   Useful for finding serial/product number for IOC
-  imageCaptureTest -> C++ program that uses OpenCV to capture 200
   frames from the camera to test if libuvc_ has been installed and the
   camera is connected.
-  ADUVC_ driver -> The source code for the area detector driver itself
   in the uvcApp/src directory.
-  iocUVC -> Example ioc for use with the ADUVC_ driver. Shows how to
   connect to a camera in the EPICS IOC shell

--------------

Usage
-----

Once the driver is installed, you may start the ioc by entering the IOC
folder and running 'sudo ./startEPICS.sh'. Sudo access is important
because access to UVC devices is limited to users with root permissions.
You may then integrate ADUVC into your area detector screen setup. Below
is an image of the ADUVC base control screen. It extends the standard
ADBase screen with UVC controls for various camera functionality, such
as brightness, exposure, backlight compenstation, saturation, etc. The
driver supports a variety of UVC formats, though the two that are used
the most often are MJPEG and Uncompressed. Because acquisition modes are
locked down in most UVC cameras, as of R1-2 there has been added a
section to the screen that allows for selecting from up to the 7 most
useful acquisition modes supported by the camera. If the camera has
fewer than 7 modes, the remaining ones will show as Unused.


--------------

Release Notes
-------------

.. raw:: html

   <div class="text-section-local">

.. rubric:: Release Notes
   :name: release-notes-1

.. rubric:: R1-6 (1-December-2020)
   :name: r1-6-1-december-2020

- Features Added

    - libuvc now builds via the EPICS build system

.. rubric:: R1-5 (29-October-2020)
   :name: r1-5-29-october-2020

- Features Added

    - Pan/Tilt/Zoom control (PTZ)
    - Improved static build control
    - Improved helper tools
    - Improved script for building libuvc

- Fixes/Improvement

   - Minor code changes to remove tabs and replace with spaces

.. rubric:: R1-4 (18-August-2020)
   :name: r1-4-18-august-2020

- Features Added

    - Added ReST based documentation for new areaDetector doc format
    - Static build support (for libuvc)

- Fixes/Improvement

    - Fix segmentation fault caused by disconnect
    - Libuvc install script fixed to support include files from build
    - Minor fixes to build system to allow for static building
    - Folder + file name changes to better follow AD naming standard
    - Many code readability improvements
    - Allow for helper utilities to compile with non-system installed libuvc

- Known Issues
    
    - libuvc relies on libusb-1.0, not libusb.
    - When camera is not connected, driver segmentation faults; should be handled more gracefully.

.. rubric:: R1-3 (06-September-2019)
   :name: r1-3-06-september-2019

-  Key detector features implemented:

   -  Added IOC feature to auto-adjust camera settings based on
      operating mode.

-  Key fixes and improvements

   -  Minor screen updates
   -  Repoint libuvc upstream repo. New repo has issues with CMake file
   -  Add documentation for fixing root ownership issues of UVC devices

-  Known Issues

   -  Auto adjust feature can break with certain odd configurations of
      the camera
   -  Uses array size to determine 8/16 bit and Mono/RGB, but certain
      compressed formats break the adjuster.

.. rubric:: R1-2 (11-June-2019)
   :name: r1-2-11-june-2019

-  Key detector features implemented:

   -  Camera modes now read into structs at startup
   -  Valid camera mode structs selectable from dropdown in CSS -
      improves usability
   -  Autosave functionality tested and working.
   -  More extensive status messages/detector feedback

-  Key fixes and improvements

   -  Removed unused PVs (ADUVC\_VendorID, ADUVC\_ProductID)
   -  Fixed memory leak caused by early return from frame conversion
      function on error
   -  Added Makefiles to the support modules
   -  Documentation updates
   -  Added information on using camera with Virtual Box
   -  Removed newlines from status messages for better readability.
   -  CSS screen updated

.. rubric:: R1-1 (28-January-2019)
   :name: r1-1-28-january-2019

-  Key detector features implemented:

   -  Support for Uncompressed and Grayscale images
   -  Support for 16-bit images
   -  Image timestamps

-  Key fixes and improvements

   -  Memory copying fixed to remove race condition that could cause
      plugins to crash
   -  Improved conversion between frame format and PV
   -  Updated documentation with params[in]/[out]
   -  Added LICENSE
   -  Code formatting cleaned up (removed all tab characters)

.. rubric:: R1-0 (7-December-2018)
   :name: r1-0-7-december-2018

-  Key detector features implemented:

   -  UVC Camera features (Sharpness, Backlight, Brightness, Gain, etc.)
   -  More format support (MJPEG, RGB, YUYV)
   -  Frame size and framerate selection (Previously only 640x480 @ 30
      fps)
   -  Custom CSS Screen Added

-  Key fixes and improvements

   -  Fixed error where first frame in stream was corrupt
   -  Fixed IOC autosave feature
   -  Resolved frame drop due to motion (camera dependant)

.. rubric:: R0-1 (Beta) (5-November-2018)
   :name: r0-1-beta-5-november-2018

-  Key detector features implemented:

   -  Image Acquisition supported and tested.
   -  Acquisition mode selection supported and tested
   -  Diagnostic information acquisition
   -  Plugin interoperability tested
   -  Detector IOC written and tested
   -  Driver report function implemented

-  Key Support Features Added

   -  Documentation for installation and usage
   -  Camera Detector program for detecting UVC cameras and diagnostics
   -  Image Acquisition program for testing camera image acquisition
   -  libuvc installation script included

-  Limitations

   -  Only tested on linux-x86\_64 systems
   -  Framerate goes down during motion (likely due to mjpeg
      compression)
   -  IOC autosave feature not working correctly
   -  Many UVC camera functions not yet implemented
   -  Limited format support (only mjpeg)
   -  Frame size must be specified in the IOC
   -  No custom screens (uses ADBase screen)

.. raw:: html

   </div>

--------------

MEDM screens
------------
The following is the OPI screen for ADUVC.opi when controlling a USB camera by Shenzhen Reyun Industrial Co., Ltd. This screen is basic and can be used for any USB camera.

.. figure:: ADUVCBase.png


The ADUVC Base screen. Based on the ADBase screen with added controls specific to ADUVC_.

------------------

Known Issues and pull requests
------------------------------

- To submit an issue or a pull request for ADUVC, please do so at the source fork on `Github <https://github.com/epicsNSLS2-areaDetector/ADUVC>`__.
- Many low end vendors do not assign Serial Numbers (S/N), and such cameras must be started using Product Number instead. 
- Some vendors assign same S/N for the same model, and such multiples of such cameras do not work well when connected to same computer USB hub.
- USB cameras have to be accessed by root, and access by other users is enabled by modifying /etc/udev/rules. 

Important links
---------------

- libuvc_, is a cross-platform library for USB video devices.
- ADUVC_ driver on Github.
- areaDetector_ on Github.


