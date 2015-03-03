#Makefile at top of application tree
#TOP = .
#include $(TOP)/configure/CONFIG
#DIRS := $(DIRS) $(filter-out $(DIRS), configure)
#DIRS := $(DIRS) $(filter-out $(DIRS), $(wildcard *App))
#DIRS := $(DIRS) $(filter-out $(DIRS), $(wildcard *app))
#DIRS := $(DIRS) $(filter-out $(DIRS), $(wildcard iocBoot))
#DIRS := $(DIRS) $(filter-out $(DIRS), $(wildcard iocboot))
# Comment out the following line to creation of example iocs and documentation
#DIRS := $(DIRS) $(filter-out $(DIRS), $(wildcard etc))
# Comment out the following line to disable building of example iocs
#DIRS := $(DIRS) $(filter-out $(DIRS), $(wildcard iocs))
#include $(TOP)/configure/RULES_TOP


TOP = .
include $(TOP)/configure/CONFIG
DIRS := $(DIRS) configure
DIRS := $(DIRS) merlinApp
ifeq ($(BUILD_IOCS), YES)
DIRS := $(DIRS) $(filter-out $(DIRS), $(wildcard iocs))
iocs_DEPEND_DIRS += merlinApp
endif
include $(TOP)/configure/RULES_TOP

uninstall: uninstall_iocs
uninstall_iocs:
	$(MAKE) -C iocs uninstall
.PHONY: uninstall uninstall_iocs

realuninstall: realuninstall_iocs
realuninstall_iocs:
	$(MAKE) -C iocs realuninstall
.PHONY: realuninstall realuninstall_iocs

