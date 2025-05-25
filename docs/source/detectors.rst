Supported Detectors
===================

Detectors in the Github areaDetector Project
--------------------------------------------

Beginning with release R2-0 (March 2014) the areaDetector module is in
the `areaDetector project on
Github <https://github.com/areaDetector>`__. This project is organized
as a `top level module <https://github.com/areaDetector/areaDetector>`__
and a set of submodules, e.g.
`ADCore <https://github.com/areaDetector/ADCore>`__,
`ADEiger <https://github.com/areaDetector/ADEiger>`__,
`ADGenICam <https://github.com/areaDetector/ADGenICam>`__, etc.

The following table provides links to the github repository, the
documentation, release notes, name of maintainer, and repository status.

.. list-table::
  :widths: 10 30 10 10 10 20
  :header-rows: 1

  * - Github repository
    - Description
    - Documentation
    - Release Notes
    - Primary maintainer
    - Status
  * - `areaDetector <https://github.com/areaDetector/areaDetector>`__
    - Top-level module; ADCore, ADSupport, ADProsilica, etc. go under this
    - :doc:`index`
    - `Release Notes <https://github.com/areaDetector/areaDetector/blob/master/RELEASE.md>`__
    - Mark Rivers
    - Active
  * - `ADCore <https://github.com/areaDetector/ADCore>`__
    - Base classes, plugins
    - :doc:`user_guide`
    - `Release Notes <https://github.com/areaDetector/ADCore/blob/master/RELEASE.md>`__
    - Mark Rivers
    - Active
  * - `ADSupport <https://github.com/areaDetector/ADSupport>`__
    - Source code for support libraries (TIFF, JPEG, NETCDF, HDF5, etc.)
    - N.A.
    - `Release Notes <https://github.com/areaDetector/ADSupport/blob/master/RELEASE.md>`__
    - Mark Rivers
    - Active
  * - `ADADSC <https://github.com/areaDetector/ADADSC>`__
    - Driver for ADSC detectors
    - `adscDoc <https://htmlpreview.github.io/?https://github.com/areaDetector/ADADSC/blob/master/documentation/adscDoc.html>`__
    - `Release Notes <https://github.com/areaDetector/ADADSC/blob/master/RELEASE.md>`__
    - Lewis Muir?
    - Should be archived
  * - `ADAndor <https://github.com/areaDetector/ADAndor>`__
    - Driver for Andor CCD detectors
    - :doc:`ADAndor/andorDoc`
    - `Release Notes <https://github.com/areaDetector/ADAndor/blob/master/RELEASE.md>`__
    - Unknown
    - Active
  * - `ADAndor3 <https://github.com/areaDetector/ADAndor3>`__
    - Driver for Andor sCMOS detectors
    - :doc:`ADAndor3/ADAndorDoc`
    - `Release Notes <https://github.com/areaDetector/ADAndor3/blob/master/RELEASE.md>`__
    - Unknown
    - Active
  * - `ADBitBlow <https://github.com/areaDetector/ADBitFlow>`__
    - Driver for BitFlow CoaXPress frame grabbers
    - None
    - `Release Notes <https://github.com/areaDetector/ADBitFlow/blob/master/RELEASE.md>`__
    - Mark Rivers
    - Inactive, never completed, should be achived
  * - `ADBruker <https://github.com/areaDetector/ADBruker>`__
    - Driver for Bruker detectors using the Bruker Instrument Server (BIS)
    - `BrukerDoc <https://htmlpreview.github.io/?https://github.com/areaDetector/ADBruker/blob/master/documentation/BrukerDoc.html>`__
    - `Release Notes <https://github.com/areaDetector/ADBruker/blob/master/RELEASE.md>`__
    - Mark Rivers
    - Should be archived
  * - `ADCameraLink <https://github.com/areaDetector/ADCameraLink>`__
    - Drivers for Silicon Software and Dalsa/Coreco frame grabbers
    - `ADCameraLinkDriver <https://htmlpreview.github.io/?https://github.com/areaDetector/ADCameralink/blob/master/documentation/ADCameralinkDriver.html>`__
    - `Release Notes <https://github.com/areaDetector/ADCameraLink/blob/master/RELEASE.md>`__
    - Tim Madden
    - Should be archived
  * - `ADCompVision <https://github.com/areaDetector/ADCompVision>`__
    - Plugin that provides access to OpenCV library functions
    - `ADCompVision <ADCompVision.html>`__
    - `Release Notes <https://github.com/areaDetector/ADCompVision/blob/master/RELEASE.md>`__
    - Jakub Wlodek,  Kazimierz Gofron
    - Active?
  * - `ADCSimDetector <https://github.com/areaDetector/ADCSimDetector>`__
    - Driver for ADC simulation
    - :doc:`ADCSimDetector/ADCSimDetectorDoc`
    - `Release Notes <https://github.com/areaDetector/ADCSimDetector/blob/master/RELEASE.md>`__
    - Mark Rivers
    - Active
  * - `ADDexela <https://github.com/areaDetector/ADDexela>`__
    - Driver for Perkin Elmer Dexela detectors
    - :doc:`ADDexela/ADDexela`
    - `Release Notes <https://github.com/areaDetector/ADDexela/blob/master/RELEASE.md>`__
    - Mark Rivers
    - Active?
  * - `ADEiger <https://github.com/areaDetector/ADEiger>`__
    - Driver for Dectris Eiger detectors
    - :doc:`ADEiger/eiger`
    - `Release Notes <https://github.com/areaDetector/ADEiger/blob/master/RELEASE.md>`__
    - Mark Rivers
    - Active
  * - `ADEursys <https://github.com/areaDetector/ADEuresys>`__
    - Driver CoaXPress cameras using Euresys frame grabbers
    - :doc:`ADEuresys/ADEuresys`
    - `Release Notes <https://github.com/areaDetector/ADEuresys/blob/master/RELEASE.md>`__
    - Mark Rivers
    - Active
  * - `ADFastCCD <https://github.com/areaDetector/ADFastCCD>`__
    - Driver for APS/LBL Fast CCD detector
    - :doc:`ADFastCCD/index`
    - `Release Notes <https://github.com/areaDetector/ADFastCCD/blob/master/RELEASE.md>`__
    - Stuart Wilkins
    - Active?
  * - `ADFireWireWin <https://github.com/areaDetector/ADFireWireWin>`__
    - Driver for Firewire DCAM detectors on Windows using the Carnegie Mellon Firewire driver
    - `FirewireWinDoc <FirewireWinDoc.html>`__
    - `Release Notes <https://github.com/areaDetector/ADFireWireWin/blob/master/RELEASE.md>`__
    - Mark Rivers
    - Should be archived
  * - `ADGenICam <https://github.com/areaDetector/ADGenICam>`__
    - Base class driver for GenICam drivers
    - :doc:`ADGenICam/ADGenICam`
    - `Release Notes <https://github.com/areaDetector/ADGenICam/blob/master/RELEASE.md>`__
    - Mark Rivers
    - Active
  * - `ADHamamatsuDCAM <https://github.com/areaDetector/ADHamamatsuDCAM>`__
    - Driver for Hamamatsu cameras using their DCAM library
    - :doc:`ADHamamatsuDCAM/ADHamamatsuDCAM`
    - `Release Notes <https://github.com/areaDetector/ADHamamatsuDCAM/blob/master/RELEASE.md>`__
    - Mark Rivers
    - Active
  * - `ADLambda <https://github.com/areaDetector/ADLambda>`__
    - Driver for Lambda detectors
    - :doc:`ADLambda/ADLambda`
    - `Release Notes <https://github.com/areaDetector/ADLambda/blob/master/RELEASE.md>`__
    - Keanan Lang
    - Active
  * - `ADLightField <https://github.com/areaDetector/ADLightField>`__
    - Driver for Princeton Instruments detectors using their LightField application
    - :doc:`ADLightField/ADLightField`
    - `Release Notes <https://github.com/areaDetector/ADLightField/blob/master/RELEASE.md>`__
    - Mark Rivers
    - Active
  * - `ADmar345 <https://github.com/areaDetector/ADmar345>`__
    - Driver for the mar345 image plate detector
    - :doc:`ADmar345/mar345Doc`
    - `Release Notes <https://github.com/areaDetector/ADmar345/blob/master/RELEASE.md>`__
    - Mark Rivers
    - Active?
  * - `ADmarCCD <https://github.com/areaDetector/ADmarCCD>`__
    - Driver for CCD detectors from Rayonix (formerly Mar-USA)
    - :doc:`ADmarCCD/marCCDDoc`
    - `Release Notes <https://github.com/areaDetector/ADmarCCD/blob/master/RELEASE.md>`__
    - Mark Rivers
    - Active
  * - `ADMerlin <https://github.com/areaDetector/ADMerlin>`__
    - Driver for Merlin detectors from Quantum Detectors
    - N.A.
    - `Release Notes <https://github.com/areaDetector/ADMerlin/blob/master/RELEASE.md>`__
    - Unknown
    - Should be archived?
  * - `ADMMPAD <https://github.com/areaDetector/ADMMPAD>`__
    - Driver for MMPAD detectors from CHESS and Sydor
    - None
    - `Release Notes <https://github.com/areaDetector/ADMMPAD/blob/master/RELEASE.md>`__
    - Mark Rivers
    - Active?
  * - `ADMythen <https://github.com/areaDetector/ADMythen>`__
    - Driver for Mythen detectors from Dectris
    - N.A.
    - `Release Notes <https://github.com/areaDetector/ADMythen/blob/master/RELEASE.md>`__
    - Joe Sullivan and Xiaoqiang Wang
    - Active?
  * - `ADnED <https://github.com/areaDetector/ADnED>`__
    - Driver for neutron event data
    - N.A.
    - `Release Notes <https://github.com/areaDetector/ADnED/blob/master/RELEASE.md>`__
    - Matthew Pearson
    - Active?
  * - `ADPCO <https://github.com/areaDetector/ADPCO>`__
    - Driver for PCO CameraLink cameras
    - `PCODriver <PCODriver.html>`__
    - `Release Notes <https://github.com/areaDetector/ADPCO/blob/master/RELEASE.md>`__
    - Tim Madden
    - Should be archived
  * - `ADPcoWin <https://github.com/areaDetector/ADPcoWin>`__
    - Driver for PCO cameras using their SDK
    - `Doc <https://github.com/areaDetector/ADPcoWin/blob/master/documentation/index.html>`__
    - `Release Notes <https://github.com/areaDetector/ADPcoWin/blob/master/RELEASE.md>`__
    - Benjamin Bradnick?
    - Active
  * - `ADPerkinElmer <https://github.com/areaDetector/ADPerkinElmer>`__
    - Driver for Perkin Elmer flat-panel detectors
    - :doc:`ADPerkinElmer/PerkinElmerDoc`
    - `Release Notes <https://github.com/areaDetector/ADPerkinElmer/blob/master/RELEASE.md>`__
    - Mark Rivers
    - Active
  * - `ADPhotonII <https://github.com/areaDetector/ADPhotonII>`__
    - Driver for Bruker PhotonII detectors
    - `Doc <https://github.com/areaDetector/ADPhotonII/blob/master/documentation/PhotonIIDoc.html>`__
    - `Release Notes <https://github.com/areaDetector/ADPhotonII/blob/master/RELEASE.md>`__
    - Mark Rivers
    - Should be archived
  * - `ADPICam <https://github.com/areaDetector/ADPICam>`__
    - Driver for Princeton Instruments detectors using the PICam library
    - :doc:`ADPICam/PICamDoc`
    - `Release Notes <https://github.com/areaDetector/ADPICam/blob/master/RELEASE.md>`__
    - Jakub Wlodek?
    - Active
  * - `ADPilatus <https://github.com/areaDetector/ADPilatus>`__
    - Driver for Pilatus pixel-array detectors
    - :doc:`ADPilatus/pilatusDoc`
    - `Release Notes <https://github.com/areaDetector/ADPilatus/blob/master/RELEASE.md>`__
    - Mark Rivers
    - Active
  * - `ADPixirad <https://github.com/areaDetector/ADPixirad>`__
    - Driver for Pixirad pixel-array detectors
    - :doc:`ADPixirad/ADPixirad`
    - `Release Notes <https://github.com/areaDetector/ADPixirad/blob/master/RELEASE.md>`__
    - Mark Rivers
    - Should be archived
  * - `ADPointGrey <https://github.com/areaDetector/ADPointGrey>`__
    - Driver for FLIR (formerly Point Grey Research) cameras
    - :doc:`ADPointGrey/PointGreyDoc`
    - `Release Notes <https://github.com/areaDetector/ADPointGrey/blob/master/RELEASE.md>`__
    - Mark Rivers
    - Replaced by ADSpinnaker, should be archived
  * - `ADProsilica <https://github.com/areaDetector/ADProsilica>`__
    - Driver for Allied Vision Technologies (formerly Prosilica) cameras
    - :doc:`ADProsilica/ADProsilica`
    - `Release Notes <https://github.com/areaDetector/ADProsilica/blob/master/RELEASE.md>`__
    - Mark Rivers
    - Replaced by ADVimba, should be archived
  * - `ADPSL <https://github.com/areaDetector/ADPSL>`__
    - Driver for Photonic Science detectors
    - `PSLDoc <PSLDoc.html>`__
    - `Release Notes <https://github.com/areaDetector/ADPSL/blob/master/RELEASE.md>`__
    - Mark Rivers
    - Should be archived
  * - `ADPvCam <https://github.com/areaDetector/ADPvCam>`__
    - Driver for Photometics and Princeton Instruments detectors using the PvCam library
    - :doc:`ADPvCam/pvcamDoc`
    - `Release Notes <https://github.com/areaDetector/ADPvCam/blob/master/RELEASE.md>`__
    - Jakub Wlodek?
    - Active?
  * - `ADPylon <https://github.com/areaDetector/ADPylon>`__
    - Driver for Basler cameras
    - :doc:`ADPylon/ADPylon`
    - `Release Notes <https://github.com/areaDetector/ADPylon/blob/master/RELEASE.md>`__
    - Xiaoqiang Wang
    - Active
  * - `ADQImaging <https://github.com/areaDetector/ADQImaging>`__
    - Driver for QImaging detectors
    - :doc:`ADQImaging/QImagingDoc`
    - `Release Notes <https://github.com/areaDetector/ADQImaging/blob/master/RELEASE.md>`__
    - Arthur Glowacki?
    - Should be archived
  * - `ADRIXSCam <https://github.com/areaDetector/ADRIXSCam>`__
    - Driver for XCAM RIXSCam Detectors
    - :doc:`ADRIXSCam/ADRIXSCam`
    - `Release Notes <https://github.com/areaDetector/ADRIXSCam/blob/master/RELEASE.md>`__
    - Gustavo de Souza dos Reis
    - Active
  * - `ADRoper <https://github.com/areaDetector/ADRoper>`__
    - Driver for Princeton Instruments and Photometics detectors using the WinView/WinSpec programs
    - `RoperDoc <RoperDoc.html>`__
    - `Release Notes <https://github.com/areaDetector/ADRoper/blob/master/RELEASE.md>`__
    - Mark Rivers
    - Should be archived
  * - `ADSimDetector <https://github.com/areaDetector/ADSimDetector>`__
    - Driver for simulation detector
    - :doc:`ADSimDetector/simDetector`
    - `Release Notes <https://github.com/areaDetector/ADSimDetector/blob/master/RELEASE.md>`__
    - Mark Rivers
    - Active
  * - `ADSpinnaker <https://github.com/areaDetector/ADSpinnaker>`__
    - Driver for FLIR cameras using their Spinnaker SDK
    - :doc:`ADSpinnaker/ADSpinnaker`
    - `Release Notes <https://github.com/areaDetector/ADSpinnaker/blob/master/RELEASE.md>`__
    - Mark Rivers
    - Active
  * - `ADTimePix3 <https://github.com/areaDetector/ADTimePix3>`__
    - Driver for TimePix3 cameras from amscins.com
    - :doc:`ADTimePix3/ADTimePix3`
    - `Release Notes <https://github.com/areaDetector/ADTimePix3/blob/master/RELEASE.md>`__
    - Kaz Gofron
    - Active?
  * - `ADTucsen <https://github.com/areaDetector/ADTucsen>`__
    - Driver for cameras from tucsen.com
    - None
    - None
    - David Vine
    - Should be archived?
  * - `ADURL <https://github.com/areaDetector/ADURL>`__
    - Driver for reading images from any URL using the GraphicsMagick library
    - :doc:`ADURL/ADURL`
    - `Release Notes <https://github.com/areaDetector/ADURL/blob/master/RELEASE.md>`__
    - Mark Rivers
    - Active
  * - `ADUVC <https://github.com/areaDetector/ADUVC>`__
    - Driver for USB Video Class (UVC) devices
    - :doc:`ADUVC/ADUVC`
    - `Release Notes <https://github.com/areaDetector/ADUVC/blob/master/RELEASE.md>`__
    - Jakub Wlodek?
    - Active
  * - `ADVimba <https://github.com/areaDetector/ADVimba>`__
    - Driver for Allied Vision (formerly Prosilica) cameras using their Vimba SDK
    - :doc:`ADVimba/ADVimba`
    - `Release Notes <https://github.com/areaDetector/ADVimba/blob/master/RELEASE.md>`__
    - Mark Rivers
    - Active
  * - `aravisGigE <https://github.com/areaDetector/aravisGigE>`__
    - Driver using the GNOME Aravis library for Genicam GigE cameras
    - `README <https://github.com/areaDetector/aravisGigE/blob/master/README.md>`__
    - `Release Notes <http://controls.diamond.ac.uk/downloads/support/aravisGigE/>`__
    - Tom Cobb?
    - Archived
  * - `ffmpegViewer <https://github.com/areaDetector/ffmpegViewer>`__
    - A stand-alone Qt4 application to display a stream of ffmpeg compressed images
    - `README <https://github.com/areaDetector/ffmpegViewer/blob/master/README.md>`__
    - None
    - Tom Cobb?
    - Active?
  * - `ffmpegServer <https://github.com/areaDetector/ffmpegServer>`__
    - Plugin that use the ffmpeg libraries to compress a stream of images to files or via an html service
    - `ffmpegServer <http://controls.diamond.ac.uk/downloads/support/ffmpegServer/>`__
    - `Release Notes <http://controls.diamond.ac.uk/downloads/support/ffmpegServer/>`__
    - Tom Cobb?
    - Active?
  * - `firewireDCAM <https://github.com/areaDetector/firewireDCAM>`__
    - Driver for Firewire DCAM detectors on Linux
    - `README <https://github.com/areaDetector/firewireDCAM/blob/master/README.md>`__
    - `Release Notes <https://github.com/areaDetector/firewireDCAM/blob/master/RELEASE_NOTES.md>`__
    - Ulrik Pedersen?
    - Should be archived
  * - `NDDriverStdArrays <https://github.com/areaDetector/NDDriverStdArrays>`__
    - Driver that allows EPICS Channel Access clients to create NDArrays in an IOC
    - :doc:`NDDriverStdArrays/NDDriverStdArraysDoc`
    - `Release Notes <https://github.com/areaDetector/NDDriverStdArrays/blob/master/RELEASE.md>`__
    - Mark Rivers
    - Active
  * - `pvaDriver <https://github.com/areaDetector/pvaDriver>`__
    - Driver that receives EPICS V4 NTNDArrays and converts them to NDArrays in an IOC
    - :doc:`pvaDriver/pvaDriver`
    - `Release Notes <https://github.com/areaDetector/pvaDriver/blob/master/RELEASE.md>`__
    - Mark Rivers
    - Active
  * - `specsAnalyser <https://github.com/areaDetector/specsAnalyser>`__
    - Driver for SPECS Phoibos electron spectrometers, with DLD (Delay Line Detector)
    - :doc:`specsAnalyser/index`
    - `Release Notes <https://github.com/areaDetector/specsAnalyser/blob/master/RELEASE.md>`__
    - Stuart Wilkins
    - Active?
