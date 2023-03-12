Guidelines and rules for drivers
================================

The following are guidelines and rules for writing areaDetector drivers

-  Drivers will generally implement one or more of the writeInt32(),
   writeFloat64() or writeOctet() functions if they need to act
   immediately on a new value of a parameter. For many parameters it is
   normally sufficient to simply have them written to the parameter
   library, and not to handle them in the writeXXX() functions. The
   parameters are then retrieved from the parameter library with the
   getIntParam(), getDoubleParam(), or getStringParam() function calls
   when they are needed.
-  If the writeInt32(), writeFloat64() or writeOctet() functions are
   implemented they **must** call the base class function for parameters
   that they do not handle and whose parameter index value is less than
   the first parameter of this class, i.e. parameters that belong to a
   base class.
-  Drivers will need to call the createParam() function in their
   constructor if they have additional parameters beyond those in the
   asynPortDriver or ADDriver base classes.
-  Drivers will generally need to create a new thread in which they run
   the acquisition task. Some vendor libraries create such a thread
   themselves, and then the driver must just implement a callback
   function that runs in that thread (the Prosilica is an example of
   such a driver).
-  The acquisition thread will typically monitor the acquisition process
   and perform periodic status update callbacks. The details of how to
   implement this will vary depending on the specifics of the vendor
   API. There are many existing detector drivers that can be used as
   examples of how to write a new driver.
-  If the detector hardware does not support fixed-period acquisition or
   muliple-image acquisition sequence (ADNumImages parameter) then these
   should be emulated in the driver. The simDetector, marCCD and other
   drivers can be used as examples of how to do this.
-  If NDArrayCallbacks is non-zero then drivers should do the following:

   -  Call asynNDArrayDriver::getAttributes to attach any attributes
      defined for this driver to the current array.
   -  Call doCallbacksGenericPointer() so that registered clients can
      get the values of the new arrays. Drivers must release their mutex
      by calling this->unlock() before they call
      doCallbacksGenericPointer(), or a deadlock can occur if the plugin
      makes a call to one of the driver functions.
