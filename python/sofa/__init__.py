"""
This module is a wrapper of ISSofaPython, especially allowing to run scenes
directly using the python executable instead of runSofa.
"""
import os
import sys
import errno

import __main__

# Import/determine some handy global variables to make them available from everywhere
# (especially to be able to determine the path to built binaries like runSofa).
# The root directory of the project
PROJECT_ROOT_DIR = None
# Whether we are in a packaged bundle context, or in a classic development
# environement with a build tree
IS_PACKAGED = None
# Directory of the binaries (where the runSofa executable resides).
BIN_DIR = None
try:
    from sitecustomize import PROJECT_ROOT_DIR, IS_PACKAGED, BIN_JSON_FILE_PATH
    if BIN_JSON_FILE_PATH:
        import json
        with open(BIN_JSON_FILE_PATH) as f:
            BIN_DIR = os.path.abspath(os.path.join(PROJECT_ROOT_DIR, json.load(f)['binary_dir']))
except Exception as err:
    print('Error trying to update sofa.PROJECT_ROOT_DIR / IS_PACKAGED / BIN_DIR vars: %r',
          err)

IS_WIN = sys.platform.startswith('win')

# Set the proper directory for the plugins before loading ISSofaPython
# (even if it is not needed when using runSofa)
if 'SOFA_PLUGIN_PATH' not in os.environ:
    os.environ['SOFA_PLUGIN_PATH'] = (os.path.join(os.path.abspath(sys.prefix)) if IS_WIN
                                      else os.path.join(os.path.abspath(sys.prefix), 'lib'))

from ISSofaPython import (
    AnimateBeginEvent, AnimateEndEvent, Base, BaseClass, BaseClassInfo,
    BaseContext, BaseData, BaseLink, BaseNode, BaseObject, BaseObjectDescription,
    CollisionBeginEvent, CollisionEndEvent, Context, DDGNode, Event, EventClass,
    GNode, GetEventRootClass, IntegrateBeginEvent, IntegrateEndEvent,
    KeypressedEvent, Node, ObjectFactory, PluginManager, PythonController,
    PythonEvent, addDataPath, cleanupGraph, createNode, createRootNode, getNode,
    getObject, getRoot, initializeGraph, loadPlugin, step)

def find_sofaenv_file():
    sofaenv_found = False
    sofaenv_path = None

    parent_level = 1
    reldir = '.'
    python_dir = os.path.dirname(os.path.abspath(os.__file__))
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
                try:
                    loadPlugin(value)
                except (RuntimeError, ValueError) as exc:
                    print('Could not load the %r plugin: %s' % (value, exc))
    fsofaenv.close()

# Load plugins declared in sofa.env with SOFA_PRELOAD
sofaenv_path = find_sofaenv_file()
read_sofaenv_file(sofaenv_path)

if hasattr(__main__, '__file__'): # if the module was imported from a script run with Python (i.e. without involving runSofa)
    # Add the scene directory to Sofa data path
    file_dir = os.path.dirname(os.path.realpath(__main__.__file__))
    addDataPath(file_dir)
