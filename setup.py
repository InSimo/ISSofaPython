from setuptools import setup, find_packages

setup(name='Sofa',
      version='0.1',
      packages=find_packages('python'), # include all packages under python dir
      package_dir={'' : 'python'}, # tell distutils packages are under python dir
      )