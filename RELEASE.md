areaDetector Releases
====================

The latest untagged master branch can be obtained at
https://github.com/areaDetector/areaDetector.

Tagged source code and pre-built binary releases prior to R2-0 are included
in the areaDetector releases available via links at
http://cars.uchicago.edu/software/epics/areaDetector.html.

Tagged source code releases from R2-0 onward can be obtained at 
https://github.com/areaDetector/areaDetector/releases.

Tagged prebuilt binaries from R2-0 onward can be obtained in the detector-specific directories in:
http://cars.uchicago.edu/software/pub/.  For example the Pilatus binaries are in 
http://cars.uchicago.edu/software/pub/ADPilatus.

The versions of EPICS base, asyn, and other synApps modules used for each release can be obtained from 
the EXAMPLE_RELEASE_PATHS.local, EXAMPLE_RELEASE_LIBS.local, and EXAMPLE_RELEASE_PRODS.local
files respectively, in the appropriate release of the 
[top-level areaDetector](https://github.com/areaDetector/areaDetector) repository.


Release Notes
=============

Each submodule contains detailed release notes for each release of that submodule.  These release notes
provide a short summary of the most significant items from the submodules.

R2-0
----
* Moved the repository to [Github](https://github.com/areaDetector/areaDetector).
* Re-organized the directory structure to separate the driver libraries from the example IOC applications.
* Added new detector drivers:
    - ADLightField:  Driver for newer Princeton Instruments detectors using their LightField program
    - ADPixirad: Driver for CdTe pixel array detectors from Pixirad
    - ADPointGrey: Driver for Firewire, USB, and GigE cameras from Point Grey
* ADmarCCD driver: Added support for triggered and timed acquisition modes on the new high-speed (-HE) series
  of detectors.

TO DO BEFORE R2-0 RELEASE
=========================
* Release notes and new screen shot for marCCD
* Update README.md for all modules that have not been updated
* Add RELEASE.md for all modules that don't have it yet
* Update areaDetector.doc to point to github, notes about locations for releases of each module
* Read areaDetectorDoc.html and update anything that needs to be changed
* Convert Marty's installation guide to .md file, complete it
* See if GraphicsMagick can be made to work on Linux: 32, 64 bit, static and dynamic
* See if GraphicsMagick can be made to work on Windows: 32, 64 bit, static and dynamic

R1-9-1 and earlier
------------------
Release notes are part of the
[areaDetector Release Notes](http://cars.uchicago.edu/software/epics/areaDetectorReleaseNotes.html).

