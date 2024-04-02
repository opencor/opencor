The following dependencies are needed to build Python:

- The host system needs to have a recent Python (3.10+). For Ubuntu 20.04 this means upgrading from the bundled Python 3.9.

- **Linux:** `bzip2 <https://sourceware.org/bzip2>`__, `libffi <https://sourceware.org/libffi/>`__, `LZMA <https://tukaani.org/xz/>`__, `readline <https://savannah.gnu.org/projects/readline>`__ and `SQLite3 <https://sqlite.org/>`__, which can be installed in Ubuntu or Debian by using:

  ::

    $ sudo apt install libbz2-dev libffi-dev liblzma-dev libreadline-dev libsqlite3-dev

  and in RHEL based systems by using:

  ::

    $ sudo dnf install bzip2-devel libffi-devel xz-lzma-compat readline-devel sqlite-devel

- **macOS:** `LZMA <https://tukaani.org/xz/>`__ and `SQLite3 <https://sqlite.org/>`__, along with a GNU compatible `patch <https://github.com/gitGNU/gnu_patch>`__, which can be installed using:

  ::

    $ brew install gpatch sqlite xz

  If needed, first install `Homebrew <https://brew.sh/>`__.

  **macOS** may also have problems building Python if CMake is less than version 3.27.
