We need to build OAuth ourselves so that it can be used as a 'proper' plugin within OpenCOR. The problem with the latter is that it's not possible to build OAuth as a shared library using MSVC (because of the need to expose classes/methods as __declspec(dllexport) or __declspec(dllimport) depending on whether OAuth is being built or referenced, respectively).

To address this shortcoming, we must 'manually' expose the various classes/structures that make up OAuth (grep for ---OPENCOR--- to find out which ones those are).
