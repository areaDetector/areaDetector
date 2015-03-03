#!/bin/bash

export PATH=$PATH:/local/DPbin/epics/caqtdm/bin/

export QT_PLUGIN_PATH=/local/DPbin/epics/caqtdm/lib/:/local/DPbin/epics/caqtdm/plugins

export CAQTDM_DISPLAY_PATH=/local/DPbin/epics/epics_2015-03-31/synApps_5_8/support/areaDetector-2-1/ADCore-R2-1/ADApp/op/ui/:/local/DPbin/epics/epics_2015-03-31/synApps_5_8/support/areaDetector-2-1/ADMedipix/medipixApp/opi/ui/:$CAQTDM_DISPLAY_PATH:./

caQtDM -x -macro "P="$1":, R=cam1:" ADMerlinQuad.ui &

