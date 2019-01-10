SPHINX_MULTIBUILD=sphinx-multibuild

#DOCS_PATHS += -i ../ADCore/docs
#DOCS_PATHS += -i ../ADFastCCD/docs

.PHONY: all
all:
	$(SPHINX_MULTIBUILD) $(DOCS_PATHS) -i source -s _docs  -o ./_build/html -b html
