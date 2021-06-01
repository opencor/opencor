import os
import subprocess
import sys

# Retrieve OpenCOR's home directory

home_dir = '\\'.join(sys.executable.split('\\')[:-3])

# Add the location of our binaries to the beginning of the system PATH

path = os.environ['PATH'].split(';')

path.insert(0, '%s\\bin' % home_dir)
path.insert(0, '%s\\Python\\bin' % home_dir)
path.insert(0, '%s\\Python\\Scripts' % home_dir)

os.environ['PATH'] = ';'.join(path)

# Start IPython and wait for it to terminate

try:
    ipython = subprocess.Popen(['ipython'] + sys.argv[1:],
                               stderr=sys.stderr,
                               stdout=sys.stdout)

    ipython.wait()
except KeyboardInterrupt:
    pass
