#Makefile at top of application tree
TOP = .
include $(TOP)/configure/CONFIG

DIRS := $(DIRS) ADBinaries

DIRS := $(DIRS) ADCore
ADCore_DEPEND_DIRS += ADBinaries

DIRS := $(DIRS) ADProsilica
ADProsilica_DEPEND_DIRS += ADCore

include $(TOP)/configure/RULES_TOP
