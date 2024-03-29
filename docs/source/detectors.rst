Supported Detectors
===================

Commuinity Supported Detectors
------------------------------

Beginning with release R2-0 (March 2014) the areaDetector module is in
the `areaDetector project on
Github <https://github.com/areaDetector>`__. This project is organized
as a `top level module <https://github.com/areaDetector/areaDetector>`__
and a set of submodules, e.g.
`ADCore <https://github.com/areaDetector/ADCore>`__,
`ADProsilica <https://github.com/areaDetector/ADProsilica>`__,
`ADPilatus <https://github.com/areaDetector/ADPilatus>`__, etc.

The following table provides links to the github repository, the
documentation, and pre-built binaries.

.. list-table::
  :widths: 10 30 10 10 10
  :header-rows: 1

  * - Github repository
    - Description
    - Documentation
    - Release Notes
    - Pre-built binaries
  * - `areaDetector <https://github.com/areaDetector/areaDetector>`__           
    - Top-level module; ADCore, ADSupport, ADProsilica, etc. go under this                                
    - :doc:`index`                                                                     
    - `Release Notes <https://github.com/areaDetector/areaDetector/blob/master/RELEASE.md>`__       
    - N.A.                                                                             
  * - `ADCore <https://github.com/areaDetector/ADCore>`__                       
    - Base classes, plugins, simulation detector                                                          
    - :doc:`user_guide`                                                                
    - `Release Notes <https://github.com/areaDetector/ADCore/blob/master/RELEASE.md>`__             
    - `Pre-built binaries <https://cars.uchicago.edu/software/pub/ADCore>`__           
  * - `ADSupport <https://github.com/areaDetector/ADSupport>`__                 
    - Source code for support libraries (TIFF, JPEG, NETCDF, HDF5, etc.)                                  
    - N.A.                                                                             
    - `Release Notes <https://github.com/areaDetector/ADSupport/blob/master/RELEASE.md>`__          
    - N.A.                                                                             
  * - `ADADSC <https://github.com/areaDetector/ADADSC>`__                       
    - Driver for ADSC detectors                                                                           
    - `adscDoc <ADSCDoc.html>`__                                                       
    - `Release Notes <https://github.com/areaDetector/ADADSC/blob/master/RELEASE.md>`__             
    - `Pre-built binaries <https://cars.uchicago.edu/software/pub/ADADSC>`__           
  * - `ADAndor <https://github.com/areaDetector/ADAndor>`__                     
    - Driver for Andor CCD detectors                                                                      
    - :doc:`ADAndor/andorDoc`                                            
    - `Release Notes <https://github.com/areaDetector/ADAndor/blob/master/RELEASE.md>`__            
    - `Pre-built binaries <https://cars.uchicago.edu/software/pub/ADAndor>`__          
  * - `ADAndor3 <https://github.com/areaDetector/ADAndor3>`__                   
    - Driver for Andor sCMOS detectors                                                                    
    - :doc:`ADAndor3/ADAndorDoc`                                         
    - `Release Notes <https://github.com/areaDetector/ADAndor3/blob/master/RELEASE.md>`__           
    - `Pre-built binaries <https://cars.uchicago.edu/software/pub/ADAndor3>`__         
  * - `ADBruker <https://github.com/areaDetector/ADBruker>`__                   
    - Driver for Bruker detectors using the Bruker Instrument Server (BIS)                                
    - `ADBruker <ADBruker/BrukerDoc.html>`__                                          
    - `Release Notes <https://github.com/areaDetector/ADBruker/blob/master/RELEASE.md>`__           
    - `Pre-built binaries <https://cars.uchicago.edu/software/pub/ADBruker>`__         
  * - `ADCameraLink <https://github.com/areaDetector/ADCameraLink>`__           
    - Drivers for Silicon Software and Dalsa/Coreco frame grabbers                                        
    - `ADCameraLinkDriver <ADCameraLinkDriver.html>`__                                 
    - `Release Notes <https://github.com/areaDetector/ADCameraLink/blob/master/RELEASE.md>`__       
    - N.A.                                                                             
  * - `ADCSimDetector <https://github.com/areaDetector/ADCSimDetector>`__       
    - Driver for ADC simulation                                                                           
    - :doc:`ADCSimDetector/ADCSimDetectorDoc`                                   
    - `Release Notes <https://github.com/areaDetector/ADCSimDetector/blob/master/RELEASE.md>`__     
    - `Pre-built binaries <https://cars.uchicago.edu/software/pub/ADCSimDetector>`__   
  * - `ADDexela <https://github.com/areaDetector/ADDexela>`__                   
    - Driver for Perkin Elmer Dexela detectors                                                            
    - :doc:`ADDexela/ADDexela`                                                   
    - `Release Notes <https://github.com/areaDetector/ADDexela/blob/master/RELEASE.md>`__           
    - `Pre-built binaries <https://cars.uchicago.edu/software/pub/ADDexela>`__         
  * - `ADFastCCD <https://github.com/areaDetector/ADFastCCD>`__                 
    - Driver for APS/LBL Fast CCD detector                                                                
    - :doc:`ADFastCCD/index`                                                           
    - `Release Notes <https://github.com/areaDetector/ADFastCCD/blob/master/RELEASE.md>`__          
    - N.A.                                                                             
  * - `ADFireWireWin <https://github.com/areaDetector/ADFireWireWin>`__         
    - Driver for Firewire DCAM detectors on Windows using the Carnegie Mellon Firewire driver             
    - `FirewireWinDoc <FirewireWinDoc.html>`__                                         
    - `Release Notes <https://github.com/areaDetector/ADFireWireWin/blob/master/RELEASE.md>`__      
    - `Pre-built binaries <https://cars.uchicago.edu/software/pub/ADFireWireWin>`__    
  * - `ADLambda <https://github.com/areaDetector/ADLambda>`__                   
    - Driver for Lambda detectors                                                                         
    - N.A.                                                                             
    - `Release Notes <https://github.com/areaDetector/ADLambda/blob/master/RELEASE.md>`__           
    - N.A.                                                                             
  * - `ADLightField <https://github.com/areaDetector/ADLightField>`__           
    - Driver for Princeton Instruments detectors using their LightField application                       
    - :doc:`ADLightField/ADLightField`                                           
    - `Release Notes <https://github.com/areaDetector/ADLightField/blob/master/RELEASE.md>`__       
    - N.A.                                                                             
  * - `ADmar345 <https://github.com/areaDetector/ADmar345>`__                   
    - Driver for the mar345 image plate detector                                                          
    - :doc:`ADmar345/mar345Doc`                                                   
    - `Release Notes <https://github.com/areaDetector/ADmar345/blob/master/RELEASE.md>`__           
    - `Pre-built binaries <https://cars.uchicago.edu/software/pub/ADmar345>`__         
  * - `ADmarCCD <https://github.com/areaDetector/ADmarCCD>`__                   
    - Driver for CCD detectors from Rayonix (formerly Mar-USA)                                            
    - :doc:`ADmarCCD/marCCDDoc`                                                   
    - `Release Notes <https://github.com/areaDetector/ADmarCCD/blob/master/RELEASE.md>`__           
    - `Pre-built binaries <https://cars.uchicago.edu/software/pub/ADmarCCD>`__         
  * - `ADMerlin <https://github.com/areaDetector/ADMerlin>`__                   
    - Driver for Merlin detectors from Quantum Detectors                                                  
    - N.A.                                                                             
    - `Release Notes <https://github.com/areaDetector/ADMerlin/blob/master/RELEASE.md>`__           
    - N.A.                                                                             
  * - `ADMythen <https://github.com/areaDetector/ADMythen>`__                   
    - Driver for Mythen detectors from Dectris                                                            
    - N.A.                                                                             
    - `Release Notes <https://github.com/areaDetector/ADMythen/blob/master/RELEASE.md>`__           
    - N.A.                                                                             
  * - `ADnED <https://github.com/areaDetector/ADnED>`__                         
    - Driver for neutron event data                                                                       
    - N.A.                                                                             
    - `Release Notes <https://github.com/areaDetector/ADnED/blob/master/RELEASE.md>`__              
    - N.A.                                                                             
  * - `ADPCO <https://github.com/areaDetector/ADPCO>`__                         
    - Driver for PCO detectors                                                                            
    - `PCODriver <PCODriver.html>`__                                                   
    - `Release Notes <https://github.com/areaDetector/ADPCO/blob/master/RELEASE.md>`__              
    - N.A.                                                                             
  * - `ADPerkinElmer <https://github.com/areaDetector/ADPerkinElmer>`__         
    - Driver for Perkin Elmer flat-panel detectors                                                        
    - :doc:`ADPerkinElmer/PerkinElmerDoc`                                         
    - `Release Notes <https://github.com/areaDetector/ADPerkinElmer/blob/master/RELEASE.md>`__      
    - `Pre-built binaries <https://cars.uchicago.edu/software/pub/ADPerkinElmer>`__    
  * - `ADPICam <https://github.com/areaDetector/ADPICam>`__                     
    - Driver for Princeton Instruments detectors using the PICam library                                  
    - :doc:`ADPICam/PICamDoc`                                                     
    - `Release Notes <https://github.com/areaDetector/ADPICam/blob/master/RELEASE.md>`__            
    - N.A.                                                                             
  * - `ADPilatus <https://github.com/areaDetector/ADPilatus>`__                 
    - Driver for Pilatus pixel-array detectors                                                            
    - :doc:`ADPilatus/pilatusDoc`                                                 
    - `Release Notes <https://github.com/areaDetector/ADPilatus/blob/master/RELEASE.md>`__          
    - `Pre-built binaries <https://cars.uchicago.edu/software/pub/ADPilatus>`__        
  * - `ADPixirad <https://github.com/areaDetector/ADPixirad>`__                 
    - Driver for Pixirad pixel-array detectors                                                            
    - :doc:`ADPixirad/ADPixirad`                                                 
    - `Release Notes <https://github.com/areaDetector/ADPixirad/blob/master/RELEASE.md>`__          
    - `Pre-built binaries <https://cars.uchicago.edu/software/pub/ADPixirad>`__        
  * - `ADPointGrey <https://github.com/areaDetector/ADPointGrey>`__             
    - Driver for Point Grey Research cameras                                                              
    - :doc:`ADPointGrey/PointGreyDoc`                     
    - `Release Notes <https://github.com/areaDetector/ADPointGrey/blob/master/RELEASE.md>`__        
    - `Pre-built binaries <https://cars.uchicago.edu/software/pub/ADPointGrey>`__      
  * - `ADProsilica <https://github.com/areaDetector/ADProsilica>`__             
    - Driver for Allied Vision Technologies (formerly Prosilica) cameras                                  
    - :doc:`ADProsilica/ADProsilica`                                                   
    - `Release Notes <https://github.com/areaDetector/ADProsilica/blob/master/RELEASE.md>`__        
    - `Pre-built binaries <https://cars.uchicago.edu/software/pub/ADProsilica>`__      
  * - `ADPSL <https://github.com/areaDetector/ADPSL>`__                         
    - Driver for Photonic Science detectors                                                               
    - `PSLDoc <PSLDoc.html>`__                                                         
    - `Release Notes <https://github.com/areaDetector/ADPSL/blob/master/RELEASE.md>`__              
    - `Pre-built binaries <https://cars.uchicago.edu/software/pub/ADPSL>`__            
  * - `ADPvCam <https://github.com/areaDetector/ADPvCam>`__                     
    - Driver for Photometics and Princeton Instruments detectors using the PvCam library                  
    - :doc:`ADPvCam/pvcamDoc`                                                     
    - `Release Notes <https://github.com/areaDetector/ADPvCam/blob/master/RELEASE.md>`__            
    - `Pre-built binaries <https://cars.uchicago.edu/software/pub/ADPvCam>`__          
  * - `ADPylon <https://github.com/areaDetector/ADPylon>`__
    - Driver for Basler cameras
    - :doc:`ADPylon/ADPylon`
    - `Release Notes <https://github.com/areaDetector/ADPylon/blob/master/RELEASE.md>`__
    - N.A.
  * - `ADQImaging <https://github.com/areaDetector/ADQImaging>`__               
    - Driver for QImaging detectors                                                                       
    - :doc:`ADQImaging/QImagingDoc`                                             
    - `Release Notes <https://github.com/areaDetector/ADQImaging/blob/master/RELEASE.md>`__         
    - `Pre-built binaries <https://cars.uchicago.edu/software/pub/ADQImaging>`__       
  * - `ADRIXSCam <https://github.com/areaDetector/ADRIXSCam>`__                 
    - Driver for XCAM RIXSCam Detectors                                                                   
    - :doc:`ADRIXSCam/ADRIXSCam`                                                       
    - `Release Notes <https://github.com/areaDetector/ADRIXSCam/blob/master/RELEASE.md>`__          
    - N/A                                                                              
  * - `ADRoper <https://github.com/areaDetector/ADRoper>`__                     
    - Driver for Princeton Instruments and Photometics detectors using the WinView/WinSpec programs       
    - `RoperDoc <RoperDoc.html>`__                                                     
    - `Release Notes <https://github.com/areaDetector/ADRoper/blob/master/RELEASE.md>`__            
    - `Pre-built binaries <https://cars.uchicago.edu/software/pub/ADRoper>`__          
  * - `ADSimDetector <https://github.com/areaDetector/ADSimDetector>`__         
    - Driver for simulation detector                                                                      
    - :doc:`ADSimDetector/simDetector`                                         
    - `Release Notes <https://github.com/areaDetector/ADSimDetector/blob/master/RELEASE.md>`__      
    - `Pre-built binaries <https://cars.uchicago.edu/software/pub/ADSimDetector>`__    
  * - `ADURL <https://github.com/areaDetector/ADURL>`__                         
    - Driver for reading images from any URL using the GraphicsMagick library                             
    - :doc:`ADURL/ADURL`                                             
    - `Release Notes <https://github.com/areaDetector/ADURL/blob/master/RELEASE.md>`__              
    - `Pre-built binaries <https://cars.uchicago.edu/software/pub/ADURL>`__            
  * - `aravisGigE <https://github.com/areaDetector/aravisGigE>`__               
    - Driver using the GNOME Aravis library for Genicam GigE cameras                                      
    - `README <https://github.com/areaDetector/aravisGigE/blob/master/README.md>`__    
    - `Release Notes <http://controls.diamond.ac.uk/downloads/support/aravisGigE/>`__               
    - N.A.                                                                             
  * - `ffmpegViewer <https://github.com/areaDetector/ffmpegViewer>`__           
    - A stand-alone Qt4 application to display a stream of ffmpeg compressed images                       
    - `README <https://github.com/areaDetector/ffmpegViewer/blob/master/README.md>`__  
    - N.A                                                                                           
    - N.A.                                                                             
  * - `ffmpegServer <https://github.com/areaDetector/ffmpegServer>`__           
    - Plugin that use the ffmpeg libraries to compress a stream of images to files or via an html service 
    - `ffmpegServer <http://controls.diamond.ac.uk/downloads/support/ffmpegServer/>`__ 
    - `Release Notes <http://controls.diamond.ac.uk/downloads/support/ffmpegServer/>`__             
    - N.A.                                                                             
  * - `firewireDCAM <https://github.com/areaDetector/firewireDCAM>`__           
    - Driver for Firewire DCAM detectors on Linux                                                         
    - `README <https://github.com/areaDetector/firewireDCAM/blob/master/README.md>`__  
    - `Release Notes <https://github.com/areaDetector/firewireDCAM/blob/master/RELEASE_NOTES.md>`__ 
    - N.A.                                                                             
  * - `NDDriverStdArrays <https://github.com/areaDetector/NDDriverStdArrays>`__ 
    - Driver that allows EPICS Channel Access clients to create NDArrays in an IOC                        
    - :doc:`NDDriverStdArrays/NDDriverStdArraysDoc`                             
    - `Release Notes <https://github.com/areaDetector/NDDriverStdArrays/blob/master/RELEASE.md>`__  
    - `Pre-built binaries <https://cars.uchicago.edu/software/pub/NDDriverStdArrays>`__
  * - `pvaDriver <https://github.com/areaDetector/pvaDriver>`__                 
    - Driver that receives EPICS V4 NTNDArrays and converts them to NDArrays in an IOC                    
    - :doc:`pvaDriver/pvaDriver`                                            
    - `Release Notes <https://github.com/areaDetector/pvaDriver/blob/master/RELEASE.md>`__          
    - `Pre-built binaries <https://cars.uchicago.edu/software/pub/pvaDriver>`__        

areaDetector camera drivers supplied by 3rd parties
---------------------------------------------------

Some areaDetector support have been developed by others. These are not
distributed with the areaDetector releases (source or binary) and are
not directly supported by the areaDetector working group, but may be
useful for users:

From `ImXPAD <http://www.imxpad.com>`__

- XPAD photon counting detectors.
- areaDetector driver source, info and documentation on
   `github <https://github.com/ImXPAD/ADXpad>`__

