## RELEASE_LIBS.<arch> and RELEASE_PRODS.<arch> are generated files,
## don't edit them directly!

# ASYN provides base classes
@ASYN@

@AREA_DETECTOR@

# ADSupport is needed by ADCore plugins
ADSUPPORT = $(AREA_DETECTOR)/ADSupport

# ADCORE provides base classes and plugins
ADCORE = $(AREA_DETECTOR)/ADCore

