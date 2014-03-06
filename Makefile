#Makefile at top of application tree
TOP = .
include $(TOP)/configure/CONFIG

DIRS := $(DIRS) ADBinaries

DIRS := $(DIRS) ADCore
ADCore_DEPEND_DIRS += ADBinaries

DIRS := $(DIRS) ADADSC
ADADSC_DEPEND_DIRS += ADCore

DIRS := $(DIRS) ADAndor
ADAndor_DEPEND_DIRS += ADCore

DIRS := $(DIRS) ADAndor3
ADAndor3_DEPEND_DIRS += ADCore

DIRS := $(DIRS) ADBruker
ADBruker_DEPEND_DIRS += ADCore

DIRS := $(DIRS) ADFireWireWin
ADFireWireWin_DEPEND_DIRS += ADCore

DIRS := $(DIRS) ADLightField
ADLightField_DEPEND_DIRS += ADCore

DIRS := $(DIRS) ADPSL
ADPSL_DEPEND_DIRS += ADCore

DIRS := $(DIRS) ADPerkinElmer
ADPerkinElmer_DEPEND_DIRS += ADCore

DIRS := $(DIRS) ADPilatus
ADPilatus_DEPEND_DIRS += ADCore

DIRS := $(DIRS) ADPixirad
ADPixirad_DEPEND_DIRS += ADCore

# ADPointGrey cannot be built on systems without libc 2.14
#DIRS := $(DIRS) ADPointGrey
#ADPointGrey_DEPEND_DIRS += ADCore

DIRS := $(DIRS) ADProsilica
ADProsilica_DEPEND_DIRS += ADCore

DIRS := $(DIRS) ADPvCam
ADPvCam_DEPEND_DIRS += ADCore

# ADRoper cannot be built on systems without WinView installed, so it is disabled by default
#DIRS := $(DIRS) ADRoper
#ADRoper_DEPEND_DIRS += ADCore

DIRS := $(DIRS) ADURL
ADURL_DEPEND_DIRS += ADCore

DIRS := $(DIRS) ADmar345
ADmar345_DEPEND_DIRS += ADCore

DIRS := $(DIRS) ADmarCCD
ADmarCCD_DEPEND_DIRS += ADCore

include $(TOP)/configure/RULES_TOP

uninstallTargets = $(foreach dir, $(DIRS), $(dir)$(DIVIDER)uninstall)
uninstall: $(uninstallTargets)
define UNINSTALL_template
$(1)$(DIVIDER)uninstall:
	$(MAKE) -C $(1) uninstall
endef
$(foreach dir, $(DIRS), $(eval $(call UNINSTALL_template,$(dir))))
.PHONY: uninstall $(uninstallTargets)

realuninstallTargets = $(foreach dir, $(DIRS), $(dir)$(DIVIDER)realuninstall)
realuninstall: $(realuninstallTargets)
define REALUNINSTALL_template
$(1)$(DIVIDER)realuninstall:
	$(MAKE) -C $(1) realuninstall
endef
$(foreach dir, $(DIRS), $(eval $(call REALUNINSTALL_template,$(dir))))
.PHONY: realuninstall $(realuninstallTargets)

