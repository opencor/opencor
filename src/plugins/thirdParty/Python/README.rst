To build Python:

- The host system needs to have Python 3.10+.
  For Ubuntu 20.04 LTS, this means upgrading from the bundled Python 3.9.

- **Linux:** `bzip2 <https://sourceware.org/bzip2>`__, `libffi <https://sourceware.org/libffi/>`__, `LZMA <https://tukaani.org/xz/>`__, `readline <https://savannah.gnu.org/projects/readline>`__, and `SQLite3 <https://sqlite.org/>`__ are needed. On Ubuntu and Debian, they can be installed using:

  ::

    sudo apt install libbz2-dev libffi-dev liblzma-dev libreadline-dev libsqlite3-dev

  On RHEL-based distributions, they can be installed using:

  ::

    sudo dnf install bzip2-devel libffi-devel xz-lzma-compat readline-devel sqlite-devel

- **macOS:** `LZMA <https://tukaani.org/xz/>`__ and `SQLite3 <https://sqlite.org/>`__, along with a GNU compatible `patch <https://github.com/gitGNU/gnu_patch>`__ are needed.
  They can be installed using:

  ::

    $ brew install gpatch sqlite xz

  If needed, first install `Homebrew <https://brew.sh/>`__.

  **macOS** may also have problems building Python if CMake is less than version 3.27.
