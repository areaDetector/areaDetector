#!/bin/bash


export EPICS_CA_MAX_ARRAY_BYTES=300000000

cd /local/DPbin/epics/epics_2015-03-31/synApps_5_8/support/areaDetector-2-1/ADMedipix/iocs/merlin/iocBoot/iocmerlin/

../../bin/${EPICS_HOST_ARCH}/merlin st.cmd
