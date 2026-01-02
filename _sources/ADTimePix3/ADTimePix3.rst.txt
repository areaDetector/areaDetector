======================================
ADTimePix3
======================================

:author: Kazimierz Gofron, Oak Ridge National Laboratory

.. contents:: Contents

Notes
-----

* Depends on the CPR version 1.9.1.
* Depends on the json version v3.11.2.
* Developed with ADCore R3-11 and ADSupport R1-10 or newer.
* This has only been developed/tested on ubuntu 22.04, 20.04, 18.04, RHEL 7.9, RHEL 9.1 Linux 64-bit machines.
* This has only been developed for 2 x 2 chips layout and 1 chip tpx3CAM, since that is what I have access to now.
* This has been tested with serval version 3.3.0, 3.2.0, 3.1.0 and 3.0.0. Only most recent serval version(s) are tested extensively.
* Driver is specific to Serval version, since features differ. Driver for Serval 2.x.y is in separate branch, and is not under current development
* The driver has been developed using TimePix3 Emulator, and real detectors.

Before compiling
----------------

* Compile cpr
* Clone json

How to run
----------

* Under ADTimePix3/iocs/tpx3IOC/iocBoot/iocTimePix there is already a ready to use IOC for the TimePix3

  #. run serval

  #. Change the IP address in st.cmd or st_base.cmd

  #. Run ./st.cmd

* There are CSS-Boy, screens under TimePix3/tpx3App/op/.

  #. Phoebus .bob screens have been converted/created but not yet included in the driver.


Adjust chip thresholds
----------------------

* Optimize/equalize chips in the 'count' mode (not ToT)
* Threshold fine increase is closer to noise (you get more counts)
* Threshold fine decrease is away from noise, higher threshold in keV. (you get less counts)
* These threshold settings can depend on X-ray energy.
* After changing the thresholds take a background image to check that you do not get extra noise pixels.


CSS screens
-----------

The following is the CSS screen ADTimePix3.opi when controlling an ADTimePix3 camera.

.. figure:: Screenshots/TimePix3_main.png
    :align: center

The following are the CSS screens for features of ADTimePix3 detector 

.. figure:: Screenshots/TimePix3_base.png
    :align: center

.. figure:: Screenshots/TimePix3_config.png
    :align: center

.. figure:: Screenshots/TimePix3_FileWriter.png
    :align: center

.. figure:: Screenshots/TimePix3_LoadBPC.png
    :align: center

.. figure:: Screenshots/TimePix3_status.png
    :align: center

Serval 3.3.0 allows multiple raw streams (socket, .tpx3 file writing)

.. figure:: Screenshots/TimePix3_FileWriterRaw.png
    :align: center

Neutron clusters

.. figure:: Screenshots/NeutronClusters.png
    :align: center

    