from ISSofaPython import *

import __main__
import os
import sys
import errno

def find_sofaenv_file():
    sofaenv_found = False
    sofaenv_path = None

    parent_level = 1
    reldir = '.'
    python_dir = os.path.dirname(os.path.realpath(sys.executable))
    while parent_level < 4 and not sofaenv_found:
        reldir = os.path.join(reldir, '..')
        sofaenv_path = os.path.join(python_dir, reldir, "sofa.env")
        sofaenv_found = os.path.exists(sofaenv_path)
        parent_level += 1

    if sofaenv_path is None:
        raise FileNotFoundError(errno.ENOENT, os.strerror(errno.ENOENT), "sofa.env")
    
    return sofaenv_path

def read_sofaenv_file(sofaenv_path):
    fsofaenv = open(sofaenv_path, "r")
    for line in fsofaenv:
        if line[0] != '#':
            variable, value = line.split('=')
            value = value.rstrip()
            if variable == "SOFA_PRELOAD":
                loadPlugin(value)
                print('--- sofa loadPlugin %s' % value)
    fsofaenv.close()

# Load plugins declared in sofa.env with SOFA_PRELOAD
sofaenv_path = find_sofaenv_file()
read_sofaenv_file(sofaenv_path)

if hasattr(__main__, '__file__'): # if the module was imported from a script run with Python (i.e. without involving runSofa)
    # Add the scene directory to Sofa data path
    file_dir = os.path.dirname(os.path.realpath(__main__.__file__))
    addDataPath(file_dir)
