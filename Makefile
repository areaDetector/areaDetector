#Makefile at top of application tree
TOP = .
include $(TOP)/configure/CONFIG

# Always build ADSupport and ADCore
DIRS := $(DIRS) $(ADSUPPORT)

DIRS := $(DIRS) $(ADCORE)
$(ADCORE)_DEPEND_DIRS += $(ADSUPPORT)

# Build optional plugins next
ifdef FFMPEGSERVER
DIRS := $(DIRS) $(FFMPEGSERVER)
$(FFMPEGSERVER)_DEPEND_DIRS += $(ADCORE)
endif

ifdef ADPLUGINBAR
DIRS := $(DIRS) $(ADPLUGINBAR)
$(ADPLUGINBAR)_DEPEND_DIRS += $(ADCORE)
endif

ifdef ADPLUGINEDGE
DIRS := $(DIRS) $(ADPLUGINEDGE)
$(ADPLUGINEDGE)_DEPEND_DIRS += $(ADCORE)
endif

ifdef ADCOMPVISION
DIRS := $(DIRS) $(ADCOMPVISION)
$(ADCOMPVISION)_DEPEND_DIRS += $(ADCORE)
endif

# Build base class drivers next
ifdef ADGENICAM
DIRS := $(DIRS) $(ADGENICAM)
$(ADGENICAM)_DEPEND_DIRS += $(ADCORE)
endif

# Build simulation drivers next
ifdef ADSIMDETECTOR
DIRS := $(DIRS) $(ADSIMDETECTOR)
$(ADSIMDETECTOR)_DEPEND_DIRS += $(ADCORE)
endif

ifdef ADCSIMDETECTOR
DIRS := $(DIRS) $(ADCSIMDETECTOR)
$(ADCSIMDETECTOR)_DEPEND_DIRS += $(ADCORE)
endif

# Build software drivers next (no associated hardware)
ifdef NDDRIVERSTDARRAYS
DIRS := $(DIRS) $(NDDRIVERSTDARRAYS)
$(NDDRIVERSTDARRAYS)_DEPEND_DIRS += $(ADCORE)
endif

ifeq ($(WITH_PVA), YES)
  ifdef PVADRIVER
  DIRS := $(DIRS) $(PVADRIVER)
  $(PVADRIVER)_DEPEND_DIRS += $(ADCORE)
  endif
endif

# Finally build hardware drivers
ifdef ADANDOR
DIRS := $(DIRS) $(ADANDOR)
$(ADANDOR)_DEPEND_DIRS += $(ADCORE)
endif

ifdef ADANDOR3
DIRS := $(DIRS) $(ADANDOR3)
$(ADANDOR3)_DEPEND_DIRS += $(ADCORE)
endif

ifdef ADARAVIS
DIRS := $(DIRS) $(ADARAVIS)
$(ADARAVIS)_DEPEND_DIRS += $(ADCORE)
$(ADARAVIS)_DEPEND_DIRS += $(ADGENICAM)
endif

ifdef ADDEXELA
DIRS := $(DIRS) $(ADDEXELA)
$(ADDEXELA)_DEPEND_DIRS += $(ADCORE)
endif

ifdef ADEIGER
DIRS := $(DIRS) $(ADEIGER)
$(ADEIGER)_DEPEND_DIRS += $(ADCORE)
endif

ifdef ADFASTCCD
DIRS := $(DIRS) $(ADFASTCCD)
$(ADFASTCCD)_DEPEND_DIRS += $(ADCORE)
endif

ifdef ADLAMBDA
DIRS := $(DIRS) $(ADLAMBDA)
$(ADLAMBDA)_DEPEND_DIRS += $(ADCORE)
endif

ifdef ADLIGHTFIELD
DIRS := $(DIRS) $(ADLIGHTFIELD)
$(ADLIGHTFIELD)_DEPEND_DIRS += $(ADCORE)
endif

ifdef ADMERLIN
DIRS := $(DIRS) $(ADMERLIN)
$(ADMERLIN)_DEPEND_DIRS += $(ADCORE)
endif

ifdef ADMMPAD
DIRS := $(DIRS) $(ADMMPAD)
$(ADMMPAD)_DEPEND_DIRS += $(ADCORE)
endif

ifdef ADMYTHEN
DIRS := $(DIRS) $(ADMYTHEN)
$(ADMYTHEN)_DEPEND_DIRS += $(ADCORE)
endif

ifdef ADPCOWIN
DIRS := $(DIRS) $(ADPCOWIN)
$(ADPCOWIN)_DEPEND_DIRS += $(ADCORE)
endif

ifdef ADPICAM
DIRS := $(DIRS) $(ADPICAM)
$(ADPICAM)_DEPEND_DIRS += $(ADCORE)
endif

ifdef ADPERKINELMER
DIRS := $(DIRS) $(ADPERKINELMER)
$(ADPERKINELMER)_DEPEND_DIRS += $(ADCORE)
endif

ifdef ADPILATUS
DIRS := $(DIRS) $(ADPILATUS)
$(ADPILATUS)_DEPEND_DIRS += $(ADCORE)
endif

ifdef ADPVCAM
DIRS := $(DIRS) $(ADPVCAM)
$(ADPVCAM)_DEPEND_DIRS += $(ADCORE)
endif

ifdef ADSPINNAKER
DIRS := $(DIRS) $(ADSPINNAKER)
$(ADSPINNAKER)_DEPEND_DIRS += $(ADCORE)
$(ADSPINNAKER)_DEPEND_DIRS += $(ADGENICAM)
endif

ifdef ADTIMEPIX3
DIRS := $(DIRS) $(ADTIMEPIX3)
$(ADTIMEPIX3)_DEPEND_DIRS += $(ADCORE)
endif

ifdef ADURL
DIRS := $(DIRS) $(ADURL)
$(ADURL)_DEPEND_DIRS += $(ADCORE)
endif

ifdef ADMAR345
DIRS := $(DIRS) $(ADMAR345)
$(ADMAR345)_DEPEND_DIRS += $(ADCORE)
endif

ifdef ADMARCCD
DIRS := $(DIRS) $(ADMARCCD)
$(ADMARCCD)_DEPEND_DIRS += $(ADCORE)
endif

ifdef ADVIMBA
DIRS := $(DIRS) $(ADVIMBA)
$(ADVIMBA)_DEPEND_DIRS += $(ADCORE)
$(ADVIMBA)_DEPEND_DIRS += $(ADGENICAM)
endif

ifdef FFMPEGVIEWER
DIRS := $(DIRS) $(FFMPEGVIEWER)
$(FFMPEGVIEWER)_DEPEND_DIRS += $(ADCORE)
endif

ifdef SPECSANALYSER
DIRS := $(DIRS) $(SPECSANALYSER)
$(SPECSANALYSER)_DEPEND_DIRS += $(ADCORE)
endif

ifdef ADRIXSCAM
DIRS := $(DIRS) $(ADRIXSCAM)
$(ADRIXSCAM)_DEPEND_DIRS += $(ADCORE)
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

