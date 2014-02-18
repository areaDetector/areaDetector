#Makefile at top of application tree
TOP = .
include $(TOP)/configure/CONFIG

DIRS := $(DIRS) ADBinaries

DIRS := $(DIRS) ADCore
ADCore_DEPEND_DIRS += ADBinaries

DIRS := $(DIRS) ADFireWireWin
ADFireWireWin_DEPEND_DIRS += ADCore

DIRS := $(DIRS) ADLightField
ADLightField_DEPEND_DIRS += ADCore

DIRS := $(DIRS) ADPerkinElmer
ADPerkinElmer_DEPEND_DIRS += ADCore

DIRS := $(DIRS) ADPilatus
ADPilatus_DEPEND_DIRS += ADCore

DIRS := $(DIRS) ADPixirad
ADPixirad_DEPEND_DIRS += ADCore

DIRS := $(DIRS) ADPointGrey
ADPointGrey_DEPEND_DIRS += ADCore

DIRS := $(DIRS) ADProsilica
ADProsilica_DEPEND_DIRS += ADCore

DIRS := $(DIRS) ADPvCam
ADPvCam_DEPEND_DIRS += ADCore

DIRS := $(DIRS) ADRoper
ADRoper_DEPEND_DIRS += ADCore

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

