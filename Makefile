#Makefile at top of application tree
TOP = .
include $(TOP)/configure/CONFIG

DIRS := $(DIRS) $(ADBINARIES)

DIRS := $(DIRS) $(ADCORE)
ADCore_DEPEND_DIRS += $(ADBINARIES)

ifdef ADADSC
DIRS := $(DIRS) $(ADADSC)
ADADSC_DEPEND_DIRS += $(ADCORE)
endif
ifdef ADANDOR
DIRS := $(DIRS) $(ADANDOR)
ADAndor_DEPEND_DIRS += $(ADCORE)
endif

ifdef ADANOR3
DIRS := $(DIRS) $(ADANOR3)
ADAndor3_DEPEND_DIRS += ADCore
endif

ifdef ADBRUKER
DIRS := $(DIRS) $(ADBRUKER)
ADBruker_DEPEND_DIRS += ADCore
endif

ifdef ADFIREWINWIN
DIRS := $(DIRS) $(ADFIREWINWIN)
ADFireWireWin_DEPEND_DIRS += ADCore
endif

ifdef ADLIGHTFIELD
DIRS := $(DIRS) $(ADLIGHTFIELD)
ADLightField_DEPEND_DIRS += ADCore
endif

ifdef ADPSL
DIRS := $(DIRS) $(ADPSL)
ADPSL_DEPEND_DIRS += ADCore
endif

ifdef ADPERKINELMER
DIRS := $(DIRS) $(ADPERKINELMER)
ADPerkinElmer_DEPEND_DIRS += ADCore
endif

ifdef ADPILATUS
DIRS := $(DIRS) $(ADPILATUS)
ADPilatus_DEPEND_DIRS += ADCore
endif

ifdef ADPIXIRAD
DIRS := $(DIRS) $(ADPIXIRAD)
ADPixirad_DEPEND_DIRS += ADCore
endif

ifdef ADPOINTGREY
# ADPointGrey cannot be built on systems without libc 2.14
DIRS := $(DIRS) $(ADPOINTGREY)
ADPointGrey_DEPEND_DIRS += ADCore
endif

ifdef ADPROSILICA
DIRS := $(DIRS) $(ADPROSILICA)
ADProsilica_DEPEND_DIRS += ADCore
endif

ifdef ADPVCAM
DIRS := $(DIRS) $(ADPVCAM)
ADPvCam_DEPEND_DIRS += ADCore
endif

ifdef ADROPER
# ADRoper cannot be built on systems without WinView installed, so it is disabled by default
DIRS := $(DIRS) $(ADROPER)
ADRoper_DEPEND_DIRS += ADCore
endif

ifdef ADQIMAGING
DIRS := $(DIRS) $(ADQIMAGING)
ADQImaging_DEPEND_DIRS += ADCore
endif

ifdef ADURL
DIRS := $(DIRS) $(ADURL)
ADURL_DEPEND_DIRS += ADCore
endif

ifdef ADMAR345
DIRS := $(DIRS) $(ADMAR345)
ADmar345_DEPEND_DIRS += ADCore
endif

ifdef ADMARCCD
DIRS := $(DIRS) $(ADMARCCD)
ADmarCCD_DEPEND_DIRS += ADCore
endif

ifdef ADQIMAGING
DIRS := $(DIRS) $(ADQIMAGING)
ADQImaging_DEPEND_DIRS += ADCore
endif

ifdef AVISGIGE
DIRS := $(DIRS) $(AVISGIGE)
aravisGigE_DEPEND_DIRS += ADCore
endif

ifdef FFMPEGSERVER
DIRS := $(DIRS) $(FFMPEGSERVER)
ffmpagServer_DEPEND_DIRS += ADCore
endif

ifdef FIREWIREDCAM
DIRS := $(DIRS) $(FIREWIREDCAM)
firewireDCAM_DEPEND_DIRS += ADCore
endif

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
