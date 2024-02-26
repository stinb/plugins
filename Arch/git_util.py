# Utility file for getting git information, used by git_ architecture plugins.
import os
import understand
import subprocess

def fileToListFromLog(format, db):
  wd = os.path.dirname(db.name())
  startinfo = None
  if os.name == 'nt':
    startinfo = subprocess.STARTUPINFO()
    startinfo.dwFlags |= subprocess.STARTF_USESHOWWINDOW
  try:
    gitlog = subprocess.run(["git", "log", "--format=#"+format, "--name-only"], check=True, capture_output=True, cwd=wd, text=True, startupinfo=startinfo, encoding='latin-1')
  except Exception as e:
    return # no repository ?

  pathToFile = dict()
  wd.replace('\\','/')
  if not wd.endswith('/'):
    wd += '/'
  for file in db.ents("file ~unknown ~unresolved"):
    path = file.longname()
    path.replace('\\','/')
    if path.startswith(wd):
      path = path[len(wd):]
    pathToFile[path] = file

  cur = ""
  fileToList = dict()
  for line in gitlog.stdout.splitlines():
    line = line.strip()
    if line.startswith("#"):
      cur = line[1:]
    elif line:
      line.replace('\\','/')
      file = pathToFile.get(line)
      if file:
        fileToList.setdefault(file,[]).append(cur)
  return fileToList

def fileToListFromCache(und_cache, db, key):
  fileToList = dict()
  for file in db.ents("file ~unknown ~unresolved"):
    keyList = und_cache.value(file=file,key=key)
    if keyList:
      fileToList[file] = keyList
  return fileToList

def fileToListFromPlugin(plugin, db, cacheKey, logKey):
  if hasattr(plugin, "cache") and plugin.cache("Git") and not plugin.cache("Git").is_empty():
    return fileToListFromCache(plugin.cache("Git"), db, cacheKey)
  return fileToListFromLog(logKey, db)

def gitAuthors(plugin, db):
  return fileToListFromPlugin(plugin, db, "authors", "%an")

def gitHashes(plugin, db):
  return fileToListFromPlugin(plugin, db, "commits", "%H")

def gitIsoDates(plugin, db):
  return fileToListFromPlugin(plugin, db, "dates", "%aI")

