TOP = ../../..
include $(TOP)/configure/CONFIG

DATA += $(patsubst ../%, %, $(wildcard ../*.edl))
DATA += $(patsubst ../%, %, $(wildcard ../*.gif))
DATA += $(patsubst ../%, %, $(wildcard ../*.png))
#DATA += <name>.edl for "edl" files created with velocity scripts

include $(TOP)/configure/RULES

# How to specify a path for velocity dependencies
VELOCITYPATH = /home/diamond/R3.14.7/prod/support/pss/4-11/pssApp/opi/velocity

 %.edl: ../%.v
	(cd ..; velocity -p$(VELOCITYPATH) -l O.Common/velocity.log -a $(notdir $<)) > $@
