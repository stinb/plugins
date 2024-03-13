# Utility file for getting git information, used by git plugins.
import os
import understand
import subprocess

def runProcessInDbDir(args, db):
  # The git repository is assumed to be in the same folder as the Understand
  # project. If this is not True for your project, update the working directory
  # accordingly.
  wd = os.path.dirname(db.name())
  startinfo = None
  if os.name == 'nt':
    startinfo = subprocess.STARTUPINFO()
    startinfo.dwFlags |= subprocess.STARTF_USESHOWWINDOW
  try:
    out = subprocess.run(args, check=True, capture_output=True, cwd=wd, text=True, startupinfo=startinfo, encoding='latin-1')
  except Exception as e:
    return None, None # no repository ?
  return out.stdout, wd

def fileToListFromLog(format, db):
  gitlog, wd = runProcessInDbDir(["git", "log", "--format=#"+format, "--name-only"], db)
  if not gitlog:
    return dict()

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
  for line in gitlog.splitlines():
    line = line.strip()
    if line.startswith("#"):
      cur = line[1:]
    elif line:
      line.replace('\\','/')
      file = pathToFile.get(line)
      if file:
        fileToList.setdefault(file,[]).append(cur)
  return fileToList

def listFromLog(db, target, format):
  path = ""
  if isinstance(target, understand.Db):
    path = "."
  elif isinstance(target, understand.Arch):
    # This assumes that the root of the directory structure architecture is
    # at the same location as the Understand project. If the Understand project
    # is not at the root of directory structure, this line must be updated
    if target.name() == "Directory Structure":
      path = "."
    else:
      path = '/'.join(target.longname().split('/')[1:])
  else: # entity, must be file
    path = target.longname()

  gitlog, wd = runProcessInDbDir(["git", "log", "--format="+format, "--", path], db)
  if not gitlog:
    return None

  return gitlog.splitlines()

def fileToListFromCache(und_cache, db, key):
  fileToList = dict()
  for file in db.ents("file ~unknown ~unresolved"):
    keyList = und_cache.value(file=file,key=key)
    if keyList:
      fileToList[file] = keyList
  return fileToList

def fileToListFromPlugin(plugin, db, cacheKey, logKey):
  if hasCache(plugin):
    return fileToListFromCache(plugin.cache("Git"), db, cacheKey)
  return fileToListFromLog(logKey, db)

def gitAuthors(plugin, db):
  return fileToListFromPlugin(plugin, db, "authors", "%an")

def gitHashes(plugin, db):
  return fileToListFromPlugin(plugin, db, "commits", "%H")

def gitIsoDates(plugin, db):
  return fileToListFromPlugin(plugin, db, "dates", "%aI")

def hasGit(plugin,db=None):
  # If the built-in Git cache has content, then the repository for the project
  # was found
  if hasCache(plugin):
    return True

  # This assumes that the git repository, if it exists, is in the same location
  # as the Understand project. If that is not true for your repository, you
  # would need to update this code.
  if db is None and hasattr(plugin,"db"):
    db = plugin.db()
  if db is not None:
    path = os.path.join(os.path.dirname(db.name()), ".git")
    return os.path.exists(path)
  return False

def hasCache(plugin):
  return hasattr(plugin, "cache") and plugin.cache("Git") and not plugin.cache("Git").is_empty()

def archHasGit(plugin, arch):
  return hasCache(plugin) or ((arch.longname().startswith("Directory Structure/") or arch.name() == "Directory Structure") and hasGit(plugin))

def targetToFileList(target):
  files = []
  if target:
    if isinstance(target, understand.Db):
      files = target.lookup_arch("Directory Structure").ents(True)
    elif isinstance(target, understand.Arch):
      for file in target.ents(True):
        if file.kind().check("file ~unknown ~unresolved"):
          files.append(file)
    elif target.kind().check("file ~unknown ~unresolved"):
      files.append(target)
  return files

def targetAuthorCounts(plugin, target):
  """
  Return a dictionary from author name to counts and the total number of commits
  """
  authors = dict()
  sum = 0
  if hasCache(plugin):
    und_cache = plugin.cache("Git")
    commits = set()
    for file in targetToFileList(target):
      fileCommits = und_cache.value(key="commits", file=file, value=[])
      fileAuthors = und_cache.value(key="authors", file=file, value=[])
      for i in range(len(fileCommits)):
        if fileCommits[i] not in commits:
          commits.add(fileCommits[i])
          authors[fileAuthors[i]] = authors.get(fileAuthors[i],0) + 1
    sum = len(commits)
  else:
    authorList = listFromLog(plugin.db(), target, "%an")
    for author in authorList:
      authors[author] = authors.get(author,0) + 1
    sum = len(authorList)
  return authors, sum

def targetGitValues(plugin, target, cacheKey, logKey):
  """
  If a cache is available and there are multiple files, return a set of values.
  Otherwise, return the list of values.
  """
  if hasCache(plugin):
    und_cache = plugin.cache("Git")
    files = targetToFileList(target)
    if len(files) == 1:
      return und_cache.value(key=cacheKey, file = files[0], value=[])
    values = set()
    for file in files:
      for val in und_cache.value(key=cacheKey, file = files[0], value=[]):
        values.add(val)
    return values
  else:
    return listFromLog(plugin.db(), target, logKey)

def kindstringHasGit(plugin, entkindstr):
  if not hasGit(plugin):
    return False

  # Kind check if requested
  if entkindstr:
    my_kinds = set(understand.Kind.list_entity("file ~unknown ~unresolved"))
    test_kinds = set(understand.Kind.list_entity(entkindstr))
    return len(my_kinds.intersection(test_kinds)) > 0

  # Git repository was found, so git metrics are available
  return True

