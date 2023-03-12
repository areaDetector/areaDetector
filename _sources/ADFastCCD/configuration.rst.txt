Configuration
-------------

IOC Shell
~~~~~~~~~

The FastCCD driver is created with the FastCCDConfig command, either
from C/C++ or from the EPICS IOC shell.

.. code-block:: c

   int FastCCDConfig(char *portName, 
                     int maxBuffers, size_t maxMemory,
                     int priority, int stackSize,
                     int packetBuffer, int imageBuffer,
                     const char *baseIP, const char *fabricIP, 
                     const char *fabricMAC)


The `packetBuffer` and ``imageBuffer`` parameters specify how many
packet frames and images to buffer in the ``libcin`` driver. These are
directly passed to ``libcin`` on initialization.

The ``baseIP`` and ``fabricIP`` are the ip addresses of the base and
fabric interfaces on the camera interface node (CIN). A null "" string
sets the default as defined in the ``libcin`` driver.

The ``fabricMAC`` parameter causes the ``libcin`` driver to set the MAC
address of the CIN on initialization. This can be used to broadcast the
image data by using the ``FF:FF:FF:FF:FF`` mac address.

For details on the meaning of the other parameters to this function
refer to the detailed documentation on the FastCCDConfig function in the
`FastCCD.cpp
documentation <http://cars9.uchicago.edu/software/epics/areaDetectorDoxygenHTML/_fast_c_c_d_8cpp.html>`__
and in the documentation for the constructor for the `FastCCD
class <http://cars9.uchicago.edu/software/epics/areaDetectorDoxygenHTML/class_fast_c_c_d.html>`__.

Setting the image size
~~~~~~~~~~~~~~~~~~~~~~

Due to the multicolum nature of the FastCCD, setting the image size is
not trivial. This is due to the nature of the image descrambling and if
overscan is used on each supercolumn. For this reason the image size is
governed by two paramaters, ``$(P)$(R)OverscanCols`` and
``$(P)$(R)ADSizeY``.

The definition used for the image orientation in this driver is that
columns are along the *X* direction and rows are along the *Y*
direction. As the number of columns per *supercolumn* is fixed in timing
to 10 columns, the only free parameter is the number of overscan cols to
include and specified by the ``$(P)$(R)OverscanCols`` PV. For this
reason setting of the ``$(P)$(R)ADSizeX`` PV is ignored by the driver.
The number of rows defines how many rows per column to process and is
set by the ``$(P)$(R)ADSizeY`` PV.

The actual image size is therefore given by:

-  *X* Direction (row wise) ``$(P)$(R)ADSizeY`` [usually set to 2000 to
   have some overscan]
-  *Y* Direction (column wise) (10 + ``$(P)$(R)OverscanCols``) \* 96
   [for no overscan 960]

Auto Configuration of the FastCCD
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The FastCCD can be configured by using the built in *firmware* and
settings in the `libcin driver <https://github.com/NSLS-II/libcin>`__.
This is controled by the PVs defined in the section *FastCCD Auto
Setting Commands*. The PVs ``$(P)$(R)TimingName[0..9]_RBV`` show strings
with names defined in the ``libcin`` driver corresponding to the modes 1
through 10. To boot into one of these modes, the user should set the PV
``$(P)$(R)TimingMode`` to number of the mode required (0 through 9). The
PV ``$(P)$(R)TimingName_RBV`` will indicate the mode selected.

To boot the FastCCD, the user should write 1 to the PV ``$(P)$(R)Boot``.
This will cause the FastCCD to reset the CIN (by power cycling it) and
upload the firmware and timing configuration. At this point the user
should be able to see the test pattern from the CIN by triggering the
detector. The user can then configure the *fCRICs* by writing 1 to the
PV ``$(P)$(R)SendFCRIC``. The bias configuration can be upload by
writing 1 to the PV ``$(P)$(R)SendBias``. The latter two steps require
the camera head to be powered. This is out of the scope of this driver.

Connection management
~~~~~~~~~~~~~~~~~~~~~

As the FastCCD *CIN* only uses UDP sockets, there is no permanent
connection, the driver relies on being able to read such parameters as
the serial nnumber of the software to be able to confirm that
commuincation with the driver is occuring.
