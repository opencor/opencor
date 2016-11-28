Support for the ZIP format is done through Qt's undocumented (and unaccessible, by default) QZipReader and QZipWriter classes. For this, we rely on the following files:
 - [QtSource]/qtbase/src/gui/text/qzip.cpp;
 - [QtSource]/qtbase/src/gui/text/qzipreader_p.h; and
 - [QtSource]/qtbase/src/gui/text/qzipwriter_p.h.
