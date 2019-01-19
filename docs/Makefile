SPHINX_MULTIBUILD=sphinx-multibuild

DOCS_PATHS += -i ../ADCore/docs/
DOCS_PATHS += -i ../ADFastCCD/docs
DOCS_PATHS += -i ../ADViewers/docs

BUILD = _build
DOCS = _docs
SOURCE = source

.PHONY: all
all: sphinx


.PHONY: all
sphinx:
	$(SPHINX_MULTIBUILD) $(DOCS_PATHS) -i $(SOURCE) -s $(DOCS) -o $(BUILD)/html -b html

.PHONY: doxygen
doxygen:
	doxygen
	cp -r ./html/areaDetectorDoxygenHTML source/_extra

.PHONY: clean
clean:
	rm -rf _docs
	rm -rf _build
