import os, shutil, logging

VERSION = 1

def get(ports, settings, shared): # not currently used; no real need for configure on emscripten users' machines!
  if settings.USE_ZLIB == 1:
    ports.fetch_project('zlib', 'https://github.com/emscripten-ports/zlib/archive/master.zip', VERSION)
    return [ports.build_project('zlib', 'zlib-master',
                                ['sh', './configure'],
                                ['libz.a'])]
  else:
    return []

def process_args(ports, args, settings, shared):
  if settings.USE_ZLIB == 1:
    get(ports, settings, shared)
    args += ['-Xclang', '-isystem' + os.path.join(shared.Cache.get_path('ports-builds'), 'zlib')]
  return args

def show():
  return 'zlib (zlib license)'

