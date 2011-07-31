#!/usr/bin/env python

import os, sys, platform

version = '.'.join(map(str, sys.version_info[:2]))
ext = 'dll' if platform.system().lower().startswith('win') else 'so'
lib_path = '/'.join([sys.prefix, 'lib', 'libpython%s.%s' % (version, ext)])

if os.path.isfile(lib_path):
    if os.path.islink(lib_path):
        print(os.path.join(os.path.dirname(lib_path), os.readlink(lib_path)))
    else:
        print(lib_path)
else:
    print('/usr/lib/libpython2.6.so') # fallback to this
