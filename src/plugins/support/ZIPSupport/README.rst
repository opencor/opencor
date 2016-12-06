Support for the [ZIP format](https://en.wikipedia.org/wiki/Zip_(file_format)) is done through [Qt](https://www.qt.io/)'s undocumented (and inaccessible, by default) `QZipReader` and `QZipWriter` classes. For this, we rely on the following files:
 * [`[QtSource]/qtbase/src/gui/text/qzip.cpp`](https://code.qt.io/cgit/qt/qtbase.git/tree/src/gui/text/qzip.cpp);
 * [`[QtSource]/qtbase/src/gui/text/qzipreader_p.h`](https://code.qt.io/cgit/qt/qtbase.git/tree/src/gui/text/qzipreader_p.h); and
 * [`[QtSource]/qtbase/src/gui/text/qzipwriter_p.h`](https://code.qt.io/cgit/qt/qtbase.git/tree/src/gui/text/qzipwriter_p.h).
