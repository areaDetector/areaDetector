TOP = ../..
ifdef EPICS_HOST_ARCH
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

# Copy all protocol files into data directory
DATA += $(patsubst ../%, %, $(wildcard ../*.proto))

include $(TOP)/configure/RULES
endif

