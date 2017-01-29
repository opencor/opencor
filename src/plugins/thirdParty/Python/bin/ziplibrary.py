import os

from zipfile import PyZipFile, ZIP_DEFLATED

# Exclude some sub-directories from ZIP
EXCLUDE_SUBDIRECTORIES = ['__pycache__',
                          'lib2to3',
                          'lib-dynload',
                          'site-packages',
                         ]


class ZipLibrary(PyZipFile):
#---------------------------

    """Class to create ZIP archives of Python files."""

    def writepy(self, pathname, basename="", verbose=False):
        """Add all files from "pathname" to the ZIP archive.

        If pathname is a directory, search the directory and
        all subdirectories recursively for all *.py and enter
        the modules into the archive. Added files are always compiled
        if necessary.
        """
        dir, name = os.path.split(pathname)
        if os.path.isdir(pathname):
            # This is a directory, add all *.py files and subdirectories
            if basename: basename = "%s/%s" % (basename, name)
            else:        basename = name
            dirlist = os.listdir(pathname)
            for filename in dirlist:
                path = os.path.join(pathname, filename)
                root, ext = os.path.splitext(filename)
                if os.path.isdir(path):
                    if os.path.basename(path) not in EXCLUDE_SUBDIRECTORIES:
                        self.writepy(path, basename, verbose)  # Recursive call
                elif ext == ".py":
                    if verbose: print("Compiling: ", path)
                    fname, arcname = self._get_codename(path[0:-3], basename)
                    self.write(fname, arcname)
                elif not ext in [".pyc", ".pyo"]:
                    archivename = os.path.split(path)[1]
                    if basename:
                        archivename = "%s/%s" % (basename, archivename)
                    if verbose: print("Compressing: ", path)
                    self.write(path, archivename)


if __name__ == '__main__':
#-------------------------

    import sys

    if len(sys.argv) < 3:
        exit("Usage: %s ZIPFILE LIBRARY_DIRECTORY" % sys.argv[0])

    zip = ZipLibrary(sys.argv[1], 'w', ZIP_DEFLATED)

    libdir = sys.argv[2]
    if libdir[-1] != '/': libdir += '/'
    zip.writepy(libdir)

    zip.close()
