import sys, os
import subprocess

# Get OpenCOR's root directory
root = '\\'.join(sys.executable.split('\\')[:-3])

# Add the location of our binaries to the head of the system PATH
path = os.environ['PATH'].split(';')
path.insert(0, '%s\\bin' % root)
path.insert(0, '%s\\Python\\Scripts' % root)
path.insert(0, '%s\\Python\\bin' % root)
os.environ['PATH'] = ';'.join(path)

try:
    # Start Jupyter and wait for it to be terminated
    jupyter = subprocess.Popen(['jupyter'] + sys.argv[1:],
                               stderr=sys.stderr,
                               stdout=sys.stdout)
    jupyter.wait()
except KeyboardInterrupt:
    pass
