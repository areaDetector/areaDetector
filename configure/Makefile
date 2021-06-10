TOP=..

include $(TOP)/configure/CONFIG
# =============================================

TARGETS = $(CONFIG_TARGETS)
CONFIGS += $(subst ../,,$(wildcard $(CONFIG_INSTALLS)))

ifeq ($(T_A),$(EPICS_HOST_ARCH))
  # Generate and install these files for submodule use
  CFG += RELEASE_LIBS.$(EPICS_HOST_ARCH)
  CFG += RELEASE_PRODS.$(EPICS_HOST_ARCH)
endif

# RELEASE files that could affect the contents of the above files
RELEASE_FILES := RELEASE RELEASE.local RELEASE.$(EPICS_HOST_ARCH).local
RELEASE_FILES += ../../RELEASE.local ../../RELEASE.$(EPICS_HOST_ARCH).local
#RELEASE_FILES += SITE_MODULES.local SITE_MODULES.$(EPICS_HOST_ARCH).local

# Support module names used in our RELEASE files
MODULES = ASYN AREA_DETECTOR ADSUPPORT ADCORE ADPLUGINEDGE EPICS_BASE \
    AUTOSAVE BUSY CALC SNCSEQ SSCAN DEVIOCSTATS ALIVE PVA

# Components for RELEASE_LIBS.<host> and RELEASE_PRODS.<host> files:
RELEASE_LIBS_PARTS  := RELEASE_TOP.frag
RELEASE_PRODS_PARTS := RELEASE_TOP.frag
RELEASE_PRODS_PARTS += RELEASE_IOCSUP.frag
ifdef PVA
RELEASE_LIBS_PARTS  += RELEASE_PVA.frag
RELEASE_PRODS_PARTS += RELEASE_PVA.frag
endif
RELEASE_LIBS_PARTS  += RELEASE_BASE.frag
RELEASE_PRODS_PARTS += RELEASE_BASE.frag

# =============================================
include $(TOP)/configure/RULES
include $(TOP)/configure/RULES_CONFIGURE
