#Makefile at top of application tree
TOP = .
include $(TOP)/configure/CONFIG

ifeq ($(OS),Windows_NT)
DIRS := $(DIRS) $(ADBINARIES)
endif

DIRS := $(DIRS) $(ADCORE)
ifeq ($(OS),Windows_NT)
$(ADCORE)_DEPEND_DIRS += $(ADBINARIES)
endif

ifdef FFMPEGSERVER
DIRS := $(DIRS) $(FFMPEGSERVER)
$(FFMPEGSERVER)_DEPEND_DIRS += $(ADCORE)
endif

ifdef ADPLUGINEDGE
DIRS := $(DIRS) $(ADPLUGINEDGE)
$(ADPLUGINEDGE)_DEPEND_DIRS += $(ADCORE)
endif

ifdef ADADSC
DIRS := $(DIRS) $(ADADSC)
$(ADADSC)_DEPEND_DIRS += $(ADCORE)
endif

ifdef ADANDOR
DIRS := $(DIRS) $(ADANDOR)
$(ADANDOR)_DEPEND_DIRS += $(ADCORE)
endif

ifdef ADANOR3
DIRS := $(DIRS) $(ADANOR3)
$(ADANOR3)_DEPEND_DIRS += $(ADCORE)
endif

ifdef ADBRUKER
DIRS := $(DIRS) $(ADBRUKER)
$(ADBRUKER)_DEPEND_DIRS += $(ADCORE)
endif

ifdef ADDEXELA
DIRS := $(DIRS) $(ADDEXELA)
$(ADDEXELA)_DEPEND_DIRS += $(ADCORE)
endif

ifdef ADEXAMPLE
DIRS := $(DIRS) $(ADEXAMPLE)
$(ADEXAMPLE)_DEPEND_DIRS += $(ADCORE)
endif

ifdef ADFASTCCD
DIRS := $(DIRS) $(ADFASTCCD)
$(ADFASTCCD)_DEPEND_DIRS += $(ADCORE)
endif

ifdef ADFIREWIREWIN
DIRS := $(DIRS) $(ADFIREWIREWIN)
$(ADFIREWIREWIN)_DEPEND_DIRS += $(ADCORE)
endif

ifdef ADLIGHTFIELD
DIRS := $(DIRS) $(ADLIGHTFIELD)
$(ADLIGHTFIELD)_DEPEND_DIRS += $(ADCORE)
endif

ifdef ADMERLIN
DIRS := $(DIRS) $(ADMERLIN)
$(ADMERLIN)_DEPEND_DIRS += $(ADCORE)
endif

ifdef ADMYTHEN
DIRS := $(DIRS) $(ADMYTHEN)
$(ADMYTHEN)_DEPEND_DIRS += $(ADCORE)
endif

ifdef ADPICAM
DIRS := $(DIRS) $(ADPICAM)
$(ADPICAM)_DEPEND_DIRS += $(ADCORE)
endif

ifdef ADPSL
DIRS := $(DIRS) $(ADPSL)
$(ADPSL)_DEPEND_DIRS += $(ADCORE)
endif

ifdef ADPERKINELMER
DIRS := $(DIRS) $(ADPERKINELMER)
$(ADPERKINELMER)_DEPEND_DIRS += $(ADCORE)
endif

ifdef ADPILATUS
DIRS := $(DIRS) $(ADPILATUS)
$(ADPILATUS)_DEPEND_DIRS += $(ADCORE)
endif

ifdef ADPIXIRAD
DIRS := $(DIRS) $(ADPIXIRAD)
$(ADPIXIRAD)_DEPEND_DIRS += $(ADCORE)
endif

ifdef ADPOINTGREY
DIRS := $(DIRS) $(ADPOINTGREY)
$(ADPOINTGREY)_DEPEND_DIRS += $(ADCORE)
endif

ifdef ADPROSILICA
DIRS := $(DIRS) $(ADPROSILICA)
$(ADPROSILICA)_DEPEND_DIRS += $(ADCORE)
endif

ifdef ADPVCAM
DIRS := $(DIRS) $(ADPVCAM)
$(ADPVCAM)_DEPEND_DIRS += $(ADCORE)
endif

ifdef ADQIMAGING
DIRS := $(DIRS) $(ADQIMAGING)
$(ADQIMAGING)_DEPEND_DIRS += $(ADCORE)
endif

ifdef ADROPER
DIRS := $(DIRS) $(ADROPER)
$(ADROPER)_DEPEND_DIRS += $(ADCORE)
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

ifdef ADNED
DIRS := $(DIRS) $(ADNED)
$(ADNED)_DEPEND_DIRS += $(ADCORE)
endif

ifdef ARAVISGIGE
DIRS := $(DIRS) $(ARAVISGIGE)
$(ARAVISGIGE)_DEPEND_DIRS += $(ADCORE)
endif

ifdef FFMPEGVIEWER
DIRS := $(DIRS) $(FFMPEGVIEWER)
$(FFMPEGVIEWER)_DEPEND_DIRS += $(ADCORE)
endif

ifdef FIREWIREDCAM
DIRS := $(DIRS) $(FIREWIREDCAM)
$(FIREWIREDCAM)_DEPEND_DIRS += $(ADCORE)
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

