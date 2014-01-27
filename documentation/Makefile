HTMLFLAGS = --book --duplex --no-numbered --format pdf14 --title --size letter

%.pdf: %.html
	-htmldoc $(HTMLFLAGS) --outfile $@ $<

all: areaDetectorDoc.pdf doxygen iocBootDocs install

doxygen:
	doxygen

iocBootDocs:
	./makeIOCBootDocs

install:
	cp -rfp html/* /home/epics/web_software/epics

clean:
	rm -rf html


realclean: clean
