There are two `XSL <https://w3.org/Style/XSL>`__ files for converting `Content MathML <https://w3.org/TR/MathML3/chapter4.html>`__ to `Presentation MathML <https://w3.org/TR/MathML2/chapter2.html>`__: |ctop.xsl|_ and |ctopff.xsl|_.
The difference between the two is that |ctopff.xsl|_ uses a modified namespace and mode usage.
Those differences are necessary if we want to use the `XSL <https://w3.org/Style/XSL>`__ file with `Firefox <https://mozilla.org/en-US/firefox/new/>`__ (hence the file name!) and, as it happens, they are also necessary for use with `Qt <https://qt.io/>`__ and `QtWebKit <https://wiki.qt.io/QtWebKit>`__ in particular.
This is therefore the version we use here.
However, the `Presentation MathML <https://w3.org/TR/MathML2/chapter2.html>`__ it generates is not exactly what we want, so we edited the file quite a bit.

.. |ctop.xsl| replace:: ``ctop.xsl``
.. _ctop.xsl: https://github.com/davidcarlisle/web-xslt/blob/master/ctop/ctop.xsl

.. |ctopff.xsl| replace:: ``ctopff.xsl``
.. _ctopff.xsl: https://github.com/davidcarlisle/web-xslt/blob/master/ctop/ctopff.xsl
