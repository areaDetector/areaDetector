all: areaDetectorDoc.pdf doxygen iocBootDocs copyFromSource copyToWebServer

doxygen:
	doxygen

areaDetectorDoc.pdf:
	-htmldoc --book --duplex --no-numbered --format pdf14 --title --size letter --outfile html/areaDetectorDoc.pdf ../ADCore/documentation/areaDetectorDoc.html

iocBootDocs:
	./makeIOCBootDocs

copyFromSource:
	./makeHTML.pl

copyToWebServer:
	rm -f html/index.html
	cp -rf html/* /home/epics/web_software/epics

clean:
	rm -rf html
	rm -rf xml


realclean: clean
