NDPluginFile
============
:author: Mark Rivers, University of Chicago

.. contents:: Contents

.. toctree::
    :maxdepth: 1
    :caption: File Plugins

    NDFileJPEG
    NDFileTIFF
    NDFileHDF5
    NDFileNexus
    NDFileMagick
    NDFileNetCDF

Overview
--------

NDPluginFile is a base class from which actual file plugins are derived.
There are currently file plugins for JPEG, TIFF, netCDF, Nexus, HDF5,
and GraphicsMagick. The GraphicsMagick plugin can write a large number
of formats, including JPEG, TIFF, PNG, PDF and many others.

NDPluginFile inherits from NDPluginDriver. The `NDPluginFile class
documentation <../areaDetectorDoxygenHTML/class_n_d_plugin_file.html>`__
describes this class in detail. This class is designed to simplify the
task of supporting a new file format. A derived class to support a new
file format will typically need to implement only the pure virtual
functions ``openFile()``, ``readFile()``, ``writeFile()``, and 
``closeFile()``. Note that none of the current file plugins actually 
support the ``readFile()`` function yet, but this is planned for future 
releases.

The NDArray callback data can be written to disk in 1 of 3 modes:

#. Single mode. In this mode each NDArray callback results in a separate
   disk file.
#. Capture mode. In this mode a memory buffer is allocated before saving
   begins. Callback arrays are placed into this buffer, and when capture
   stops the file is written to disk. This mode limits the number of
   frames that can be saved, because they all must fit in a memory
   buffer. It is the fastest mode, with the least probability of
   dropping arrays, because no disk I/O is required while capture is in
   progress.
#. Stream mode. In this mode the data are written to a single disk file
   for those file formats that support multiple arrays per file (netCDF,
   NeXus and HDF5). Each frame is appended to the file without closing
   it. It is intermediate in speed between single mode and capture mode,
   but unlike capture mode it is not limited by available memory in the
   number of arrays that can be saved. For file formats that do not
   support multiple arrays per file (e.g. JPEG, TIFF) this mode is
   really the same as Single mode, except that one can specify a total
   number of files to save before stopping.

The CreateDirectory record controls whether directories are created if
they don't exist. If it is zero (default), no directories are created.
If it is negative, then the absolute value is the maximum of directories
that will be created (i.e. -1 will create a maximum of one directory to
complete the path, -2 will create a maximum of 2 directories). If it is
positive, then at least that many directories in the path must exist
(i.e. a value of 1 will create all directories below the root directory
and 2 will not create a directory in the root directory).

If value of the LazyOpen record is "No" (0) then at least one array with
the same datatype, array size, and attributes must have been collected
by the driver (and/or plugins) from which the file saving plugin is
getting its data **before** capture or stream mode file saving is
started. This is required so that the openFile() function can know the
dimensions and datatype of the arrays. If LazyOpen is "Yes" and the mode
is Stream then the file open if deferred until the first array callback
after streaming is started. This will slow down the saving of the first
file.

NDPluginFile supports all of the file saving parameters defined in
`asynNDArrayDriver <areaDetectorDoc.html#asynNDArrayDriver>`__, e.g.
NDFilePath, NDFileName, etc. Thus, the same interface that is used for
saving files directly in a driver is used for this plugin.

The base class will delete the "original" file that the driver created
for that array if the following are all true:

#. The DeleteDriverFile record is "Yes".
#. The file plugin has successfully written a new file.
#. The array contains an attribute called "DriverFileName" that contains
   the full file name of the original file. The driver attributes XML
   file should contain a line like the following:

   ::

       <Attribute name="DriverFileName" type="PARAM" source="FULL_FILE_NAME" datatype="STRING" description="Driver file name"/>

The file saving plugins normally determine the name of the file from the
FileName and FileNumber records. However, it is possible to have these
values come instead from attributes in the array passed to the callback.
The following 3 special attributes are used:

#. FilePluginFileName: This attribute contains the file name.
#. FilePluginFileNumber - This attribute contains the file number.
#. FilePluginDestination - If this attribute contains the string "all"
   or the name of the asyn port for this plugin (e.g. FileTIFF1) then
   the plugin will write the array to a file. If this attribute has any
   other value then the plugin will ignore this array, and not write a
   file.

Having the file information come from the array allows the driver to
control which plugin saves a particular array. For example, there may be
two file writing plugins active; the first saves the flat field files
for a tomography experiment, and the second saves the normal
projections. These plugins each stream data to a separate file. The
driver knows which files are flat fields and which are normal
projections, and adds the appropriate attributes to control which plugin
saves each array. This would not be possible using a single plugin and
EPICS PVs to switch the file, because of the problem of frames being
buffered in the plugin queue.

Normally files in Stream and Capture mode are automatically closed when
the requested number of frames have been collected, or when the Capture
record is set back to 0. It is also possible to use an NDArray attribute
to close the file. If the NDArray contains an attribute named
FilePluginClose and the attribute value is non-zero then the current
file will be closed.

.. _Null:

Null file plugin
----------------

NDFileNull inherits from NDPluginFile. This is a dummy file writer,
which does not actually write anything. Its main purpose is to delete
original driver files (DeleteDriverFile record) without writing the data
to disk. For example, if the Pilatus is being used simply for alignment
or testing one might want to display the images but then immediately
delete the files that the Pilatus had created. This plugin allows one to
do this, since it always reports success in "writing" the file, so the
NDPluginFile base class will deleted the driver file if all the
conditions outlined above are met.

The `NDFileNull class
documentation <../areaDetectorDoxygenHTML/class_n_d_file_null.html>`__\ describes
this class in detail.

The NDFileNull plugin is created with the NDFileNullConfigure command,
either from C/C++ or from the EPICS IOC shell.

::

   NDFileNullConfigure (const char *portName, int queueSize, int blockingCallbacks, 
                        const char *NDArrayPort, int NDArrayAddr, size_t maxMemory, 
                        int priority, int stackSize)
     

.. _Performance:

Performance
-----------

Performance measurements were made on the Linux and Windows systems
shown in the following table.

+-------------+-------------+-------------+-------------+-------------+
| Model       | Operating   | CPU         | RAM         | Disk        |
|             | system      |             |             |             |
+=============+=============+=============+=============+=============+
| Dell        | Windows 7   | Two         | 96 GB       | Two 500 GB  |
| Precision   | 64-bit      | quad-core   |             | 15K RPM SAS |
| T7500       |             | Xeon X5647, |             | disks RAID  |
|             |             | 2.93 GHz (8 |             | 0           |
|             |             | cores       |             |             |
|             |             | total)      |             |             |
+-------------+-------------+-------------+-------------+-------------+
| Penguin     | Linux,      | Two         | 12 GB       | Three 300   |
| Relion 1751 | Fedora Core | quad-core   |             | GB 15K RPM  |
| Server      | 14 64-bit   | Xeon E5630, |             | SAS disks   |
|             |             | 2.53 GHz (8 |             | No RAID     |
|             |             | cores       |             |             |
|             |             | total)      |             |             |
+-------------+-------------+-------------+-------------+-------------+

The tests were performed under the following conditions:

-  Simulation detector with a single small peak in Peaks mode. The
   driver was running in Continuous mode with AcquireTime=0, and
   AcquirePeriod=0, to achieve maximum frame rate. Both Windows and
   Linux were able to generate about 1500 frames/sec, which was faster
   than any of the file plugins could write, so they were always
   dropping frames.
-  1024 x 1024 8-bit images, so each image was 1 MB.
-  File plugins operating in non-blocking mode.
-  Files were written to a local disk.
-  For the file plugins that cannot save multiple images per file (JPEG,
   TIFF, Magick), tests were done only in Stream mode, which allows
   specifying the total number of files to write. For the drivers that
   can save multiple images per file (netCDF, Nexus, HDF5) tests were
   done first in Stream mode which saved 5000 images to a single 5GB
   file. Tests were then done in Single mode. In this mode it is not
   possible to tell the file plugin to save exactly 5000 frames. Instead
   the simulation driver was put in Multiple mode with NumImages=5000.
   Thus the driver generated 5000 frames, but the plugin saved fewer
   than 5000 because it was dropping frames. The file rate was
   calculated as the number of frames actually written divided by the
   elapsed time.
-  Note that the amount of data written to the files (5GB) was less than
   the amound of RAM on each system. It is thus possible that files are
   being cached in RAM, and that for more frames the I/O rate would
   drop. This is almost certainly true for the HDF5 file in stream mode
   on Windows, which has a data rate over 1GB/s.

+--------------+--------+---------------------------+-------------------------+
| File plugin  | Mode   | Windows (frames/s = MB/s) | Linux (frames/s = MB/s) |
+==============+========+===========================+=========================+
| JPEG         | Stream | 79.1                      | 68.4                    |
+--------------+--------+---------------------------+-------------------------+
| TIFF         | Stream | 259.9                     | 186.4                   |
+--------------+--------+---------------------------+-------------------------+
| Magick, TIFF | Stream | 55.7                      | 39.0                    |
+--------------+--------+---------------------------+-------------------------+
| netCDF       | Stream | 204.3                     | 185.0                   |
+--------------+--------+---------------------------+-------------------------+
| netCDF       | Single | 133.7                     | 179.1                   |
+--------------+--------+---------------------------+-------------------------+
| Nexus        | Stream | 192.3                     | 193.4                   |
+--------------+--------+---------------------------+-------------------------+
| Nexus        | Single | 106.3                     | 145.6                   |
+--------------+--------+---------------------------+-------------------------+
| HDF5         | Stream | 1113.5                    | 192.0                   |
+--------------+--------+---------------------------+-------------------------+
| HDF5         | Single | 139.7                     | 108.6                   |
+--------------+--------+---------------------------+-------------------------+
