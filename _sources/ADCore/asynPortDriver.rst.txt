asynPortDriver
==============

Detector drivers and plugins are asyn port drivers, meaning that they
implement one or more of the standard asyn interfaces. They register
themselves as interrupt sources, so that they do callbacks to registered
asyn clients when values change. They inherit from the `asynPortDriver
base C++
class <https://epics-modules.github.io/asyn/asynPortDriver.html>`__
that is provided in the asyn module. That base class handles all of the
details of registering the port driver, registering the supported
interfaces, and registering the required interrupt sources. It also
provides a parameter library for int, double, and string parameters
indexed by the integer index values defined in the driver. The parameter
library provides methods to write and read the parameter values, and to
perform callbacks to registered clients when a parameter value has
changed. The `asynPortDriver class
documentation <https://epics-modules.github.io/asyn/doxygenHTML/classasyn_port_driver.html>`__
describes this class in detail.
