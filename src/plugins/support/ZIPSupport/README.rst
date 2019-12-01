Support for the `ZIP format <https://en.wikipedia.org/wiki/Zip_(file_format)>`__ is done through `Qt <https://qt.io/>`__'s undocumented (and inaccessible, by default) ``QZipReader`` and ``QZipWriter`` classes.
For this, we rely on the following files:

- |[QtSource]/qtbase/src/gui/text/qzip.cpp|_;
- |[QtSource]/qtbase/src/gui/text/qzipreader_p.h|_;
- |[QtSource]/qtbase/src/gui/text/qzipwriter_p.h|_;

.. |[QtSource]/qtbase/src/gui/text/qzip.cpp| replace:: ``[QtSource]/qtbase/src/gui/text/qzip.cpp``
.. _[QtSource]/qtbase/src/gui/text/qzip.cpp: https://code.qt.io/cgit/qt/qtbase.git/tree/src/gui/text/qzip.cpp

.. |[QtSource]/qtbase/src/gui/text/qzipreader_p.h| replace:: ``[QtSource]/qtbase/src/gui/text/qzipreader_p.h``
.. _[QtSource]/qtbase/src/gui/text/qzipreader_p.h: https://code.qt.io/cgit/qt/qtbase.git/tree/src/gui/text/qzipreader_p.h

.. |[QtSource]/qtbase/src/gui/text/qzipwriter_p.h| replace:: ``[QtSource]/qtbase/src/gui/text/qzipwriter_p.h``
.. _[QtSource]/qtbase/src/gui/text/qzipwriter_p.h: https://code.qt.io/cgit/qt/qtbase.git/tree/src/gui/text/qzipwriter_p.h
