TOP=../..

include $(TOP)/configure/CONFIG

# -------------------------------
# Build an Diamond Support Module
# -------------------------------

#LIBRARY += medipix_low
LIBRARY += merlinDetector

PROD += merlin_sim
#PROD += medipix_test

#build cpp with debug
USR_CFLAGS += -DDEBUG
HOST_OPT=NO

# The following are compiled and added to the support library
merlin_low_SRCS += merlin_low.c

# ------------------------
# Build Test Executables
# ------------------------

# Add locally compiled object code
merlin_sim_SRCS += merlin_sim.c
merlin_sim_LDFLAGS += -lpthread

merlin_test_SRCS += merlin_test.c
merlin_test_LIBS += merlin_low

# ------------------------
# Build the Area Detector Derived Library
# ------------------------


#USR_INCLUDES += -I$(AREA_DETECTOR)/ADApp/cbfSrc

DBD += merlin.dbd

merlinDetector_SRCS += merlinDetector.cpp
merlinDetector_SRCS += mpxConnection.cpp

#medipixDetector_LIBS += cbfad

include $(ADCORE)/ADApp/commonLibraryMakefile

include $(TOP)/configure/RULES
