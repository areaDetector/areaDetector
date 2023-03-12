NDFileMagick
============

:author: Mark Rivers, University of Chicago

.. contents:: Contents

Overview
--------

NDFileMagick inherits from NDPluginFile. This plugin saves data in any
of the formats supported by the
`GraphicsMagick <http://www.graphicsmagick.org/>`__ package.
GraphicsMagick supports dozens of file formats, including TIFF, JPEG,
PNG, PDF, and many others. GraphicsMagick automatically selects the
output file format based on the extension of the file (.jpg=JPEG,
.tif=TIFF, etc. The GraphicsMagick plugin should be able to write files
in any format in the `list of GraphicsMagick supported file
formats <http://www.graphicsmagick.org/formats.html>`__ that has a "W"
or "RW" in the Mode column.

The GraphicsMagick library can either come from ADSupport or from a
package installation. The maximum bit-depth of images in GraphicsMagick
is configured at compile time using a parameter called QuantumDepth. The
GraphicsMagick library in ADSupport is built with QuantumDepth=32, so it
can handle 8, 16, and 32-bit images. Package installations may only
support 8 or 16-bit images. This plugin supports color modes Mono and
RGB1. NDFileMagick is limited to a single array per file, but capture
and stream mode are supported by writing multiple files.

The GraphicsMagick plugin supports the Int32 parameter
``NDFileMagickCompressType`` to control the compression mode of the file.
``NDFileMagick.template`` defines 2 records to support this:
``$(P)$(R)CompressType`` (longout) and ``$(P)$(R)CompressType_RBV`` (longin).
The following are the supported compression types:

-  "None"
-  "BZip"
-  "FAX"
-  "Group 4"
-  "JPEG"
-  "LZW"
-  "RLE""
-  "Zip"

No formats support all of these compression types. Many support only
one, or have an implicit compression mode and so ignore the CompressType
parameter.

The GraphicsMagick plugin supports two additional Int32 parameters.
NDFileMagickQuality is used to control the image quality in some
formats, for example JPEG. It can have values from 0 to 100%.
NDFileMagickBitDepth controls the bit depth in some file formats. The
choices are 1, 8, 16, and 32.

The following table lists the formats that I have tested to work on
Linux, and comments on compression and quality they appear to support.

+-----------------------------------+-----------------------------------+
| File format                       | Notes                             |
+===================================+===================================+
| JPEG                              | Compression is implicit,          |
|                                   | CompressType has no effect.       |
|                                   | Quality is supported.             |
+-----------------------------------+-----------------------------------+
| BMP                               | Compression and quality are not   |
|                                   | supported.                        |
+-----------------------------------+-----------------------------------+
| EPS                               | Compression and quality are not   |
|                                   | supported.                        |
+-----------------------------------+-----------------------------------+
| FITS                              | Color, compression and quality    |
|                                   | are not supported.                |
+-----------------------------------+-----------------------------------+
| GIF                               | Selecting any CompresType except  |
|                                   | None results in a compressed      |
|                                   | image. Quality is not supported.  |
+-----------------------------------+-----------------------------------+
| HTML                              | Produces .shtml, .html, and .gif  |
|                                   | files.                            |
+-----------------------------------+-----------------------------------+
| JBIG, JB2                         | BZIP, RLE, and ZIP compression    |
|                                   | are supported.                    |
+-----------------------------------+-----------------------------------+
| PDF                               | BZIP, FAX, GROUP4, JPEG, LZW,     |
|                                   | RLE, and ZIP compression are      |
|                                   | supported.                        |
+-----------------------------------+-----------------------------------+
| PNG                               | Compression is implicit,          |
|                                   | CompressType has no effect.       |
+-----------------------------------+-----------------------------------+
| TIFF                              | JPEG, LZW and ZIP compression     |
|                                   | supported. ImageJ cannot open     |
|                                   | JPEG compressed images. ZIP       |
|                                   | compressed images open in ImageJ  |
|                                   | but data are incorrect.           |
+-----------------------------------+-----------------------------------+

The `NDFileMagick class
documentation <../areaDetectorDoxygenHTML/class_n_d_file_magick.html>`__
describes this class in detail.

Configuration
-------------

The NDFileMagick plugin is created with the ``NDFileMagickConfigure``
command, either from C/C++ or from the EPICS IOC shell.

.. code-block:: c

   NDFileMagickConfigure(const char *portName, int queueSize, int blockingCallbacks, 
                         const char *NDArrayPort, int NDArrayAddr, size_t maxMemory, 
                         int priority, int stackSize)
     

For details on the meaning of the parameters to this function refer to
the detailed documentation on the NDFileMagickConfigure function in the
`NDFileMagick.cpp
documentation <../areaDetectorDoxygenHTML/_n_d_file_magick_8cpp.html>`__
and in the documentation for the constructor for the `NDFileMagick
class <../areaDetectorDoxygenHTML/class_n_d_file_magick.html>`__.

.. figure:: NDFileMagick.png
    :align: center

