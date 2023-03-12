## RELEASE_LIBS.<arch> and RELEASE_PRODS.<arch> are generated files,
## don't edit them directly!

@AREA_DETECTOR@

# ADCORE provides base classes and plugins
ADCORE = $(AREA_DETECTOR)/ADCore

# ASYN provides base classes for ADCore
@ASYN@

# ADSupport can be called by ADCore plugins
ADSUPPORT = $(AREA_DETECTOR)/ADSupport

@ZLIB@

