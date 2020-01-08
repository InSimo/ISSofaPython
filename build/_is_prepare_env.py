# Allow our local sitecustomize.py file to be prioritary compared to a
# potential one that would be located in the system installation or user site dir
# (when being in a virtual env)
import sys
import os

SELF_DIR = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, SELF_DIR)
sys.path.insert(0, os.path.abspath(SELF_DIR + '/..'))
