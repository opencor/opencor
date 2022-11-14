"""
Author: @gbernardino
Change the shebangs of the python to refer to the local. Note that the -s option, that removes $HOME from pythonpath is not included to the shebangs.
"""

import argparse, sys, platform, os, logging, re

#logging.basicConfig(level=os.environ.get("LOGLEVEL", "INFO"))

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Update the path of scripts to a perl snipped linking to python in the same folder.')
    parser.add_argument('path', metavar='PATH', help='Path to new Python installation.')

    args = parser.parse_args()

    # If it is not osx, do nothing
    if platform.system() != 'Darwin':
        sys.exit(0)
    logging.debug(f'Changing shebangs @{args.path}')

    for file in os.listdir(args.path):
        script_filename = os.path.join(args.path, file)
        try:
            with open(script_filename, 'r') as f:
                lines = list(f)
        except (IsADirectoryError, UnicodeDecodeError, PermissionError, OSError):
            continue
        logging.info(f'Change the shebangs of file {file}')
        # Check if it is calling python
        if re.match('#!.*python', lines[0]):
            # Perl expression from @bdrx https://stackoverflow.com/questions/20095351/shebang-use-interpreter-relative-to-the-script-path/33225909#33225909
            ##!/usr/bin/perl -e$_=$ARGV[0];s/[^\/]+$/python/;exec($_,@ARGV) . 
            lines[0] = f'#!/usr/bin/perl -e$_=$ARGV[0];s/[^\/]+$/python/;exec($_,@ARGV)\n'
            logging.info(f'Change the shebangs of file {lines[0]}')

            with open(script_filename, 'w') as f:
                f.writelines(lines)
