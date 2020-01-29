SPHINX_MULTIBUILD=sphinx-multibuild

DOCS_PATHS += -i ../ADCore/docs/
DOCS_PATHS += -i ../ADViewers/docs
DOCS_PATHS += -i ../ADAndor/docs
DOCS_PATHS += -i ../ADAravis/docs
DOCS_PATHS += -i ../ADCompVision/docs
DOCS_PATHS += -i ../ADFastCCD/docs
DOCS_PATHS += -i ../ADGenICam/docs
DOCS_PATHS += -i ../ADPerkinElmer/docs
DOCS_PATHS += -i ../ADPluginBar/docs
DOCS_PATHS += -i ../ADPointGrey/docs
DOCS_PATHS += -i ../ADProsilica/docs
DOCS_PATHS += -i ../ADRIXSCam/docs
DOCS_PATHS += -i ../ADSpinnaker/docs
DOCS_PATHS += -i ../ADVimba/docs
DOCS_PATHS += -i ../specsAnalyser/docs

BUILD = _build
DOCS = _docs
SOURCE = source
#OPTS = -v -v -v
OPTS =

.PHONY: all
all: sphinx


.PHONY: all
sphinx:
	$(SPHINX_MULTIBUILD) $(DOCS_PATHS) -i $(SOURCE) -s $(DOCS) -o $(BUILD)/html $(OPTS) -b html

.PHONY: doxygen
doxygen:
	doxygen
	rsync -a doxy_output/html/ source/_extra/areaDetectorDoxygenHTML


.PHONY: clean
clean:
	rm -rf _docs
	rm -rf _build
