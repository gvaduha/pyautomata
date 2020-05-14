#!/usr/bin/env python3
# encoding: utf-8

from distutils.core import setup, Extension

pycallback_module = Extension('pycallback', sources = ['pycallback.c'])

setup(name='pycallback',
      version='0.1.0',
      description='Python callback module written in C',
      ext_modules=[pycallback_module])
