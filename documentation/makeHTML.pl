#!/usr/bin/perl
#*************************************************************************
# Copyright (c) 2002 The University of Chicago, as Operator of Argonne
#     National Laboratory.
# Copyright (c) 2002 The Regents of the University of California, as
#     Operator of Los Alamos National Laboratory.
# EPICS BASE Versions 3.13.7
# and higher are distributed subject to a Software License Agreement found
# in file LICENSE that is included with this distribution. 
#*************************************************************************
#
#	makeHTML.pl
#
#       If subdirectory html exits it is deleted
#       files *.html, *.png are copied from AD*/documentation to html
#	doxygen is executed
#
#	2013.12.19 Marty Kraimer
#
##########################################################

use Getopt::Std;
use File::Path;
use File::Copy;
use File::Path;

@subdirs=("./ADCore/documentation",
          "./ADADSC/documentation",
          "./ADAndor/documentation",
          "./ADAndor3/documentation",
          "./ADBruker/documentation",
          "./ADDexela/documentation",
          "./ADXpad/documentation",
          "./ADFireWireWin/documentation",
          "./ADLightField/documentation",
          "./ADPSL/documentation",
          "./ADPerkinElmer/documentation",
          "./ADPilatus/documentation",
          "./ADPixirad/documentation",
          "./ADPointGrey/documentation",
          "./ADProsilica/documentation",
          "./ADPvCam/documentation",
          "./ADQImaging/documentation",
          "./ADRoper/documentation",
          "./ADURL/documentation",
          "./ADmar345/documentation",
          "./ADmarCCD/documentation",
          );

foreach $dir (@subdirs) {
    $files = "../" . $dir . "/*.html";
    copy($files,"./html");
    $command = "cp " . $files . " " . "./html";
    system($command);
    $files = "../" . $dir . "/*.png";
    copy($files,"./html");
    $command = "cp " . $files . " " . "./html";
    system($command);
    $files = $dir . "/*.png";
}
