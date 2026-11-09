# Group companion files: files that are parts of the same unit, such as a
# header and its source file.

import os

# For each language that declares entities in one file and defines them in
# another: (definition reference kind, declaration reference kind, entity kinds)
DECLARATION_REFS = {
  "C++": ("define", "declare",
          "function ~static ~unknown ~unresolved ~local, object global ~static ~unknown ~unresolved"),
  "Ada": ("ada declare body", "ada declare spec", "~unknown ~unresolved ~local ~parameter"),
}

# Share of declarations two groups of files in different folders must have in
# common to be companions
MIN_OVERLAP = 0.5

def companion_groups(files, isAborted=None):
  '''
  Split files into groups of companion files, and return a list of lists of
  files, in no particular order.

  Files with the same name up to the first dot, such as foo.h and foo.cpp, are
  companions when the following hold. For web files, the name goes up to the
  last dot, so app.component.ts and app.component.html are companions but
  app.module.ts is not.
  - They are in the same folder.
  - They are in different folders, and the files named foo in one folder
    define at least half of what the files named foo in the other folder
    declare, or at least half of what they define is declared there. Only
    C and C++ (functions and global objects that aren't static) and Ada (spec
    and body) count, and entities a file both declares and defines itself
    don't count unless the other folder's files share them. When the files in
    one folder are companions of the files in several others, such as a source
    file that defines what a public header and an internal header declare, they
    are all one group.
  - One is Qt's generated moc_foo.cpp or foo.moc and the other is a file named
    foo that it includes or that includes it. Qt writes these to the build
    folder, so they are usually in a different folder from their source.

  Returns an empty list if isAborted() returns True.
  '''
  if isAborted is None:
    isAborted = lambda: False
  groups = dict()
  generated = []
  for file in files:
    if isGenerated(file):
      generated.append(file)
    else:
      groups.setdefault(groupKey(file), []).append(file)

  pairAcrossFolders(groups, isAborted)
  if isAborted():
    return []

  byStem = dict()
  for key in groups:
    byStem.setdefault(key[1], []).append(key)

  includes = IncludeCache()
  for file in generated:
    if isAborted():
      return []
    key = groupKey(file)
    if key not in groups:
      linked = [candidate for candidate in byStem.get(key[1], [])
                if any(f in includes(file) or file in includes(f) for f in groups[candidate])]
      if linked:
        key = max(linked, key=lambda candidate: (commonPathLength(candidate[0], key[0]), candidate))
    groups.setdefault(key, []).append(file)

  return list(groups.values())

def representative(files):
  '''
  The file that represents a group of companion files: a hand-written file
  over a generated one, then a header, then the file with the fewest lines of
  code, then the first by long name
  '''
  return min(files, key=representativeKey)

def stem(file):
  '''
  The name shared by companion files: the file name up to the first dot,
  without Qt's moc_ prefix. For web files it is up to the last dot instead,
  since names like app.component.ts and app.module.ts are separate modules.
  '''
  if file.language() == "Web":
    return file.name().rsplit(".", 1)[0]
  name = file.name().split(".", 1)[0]
  return name[4:] if name.startswith("moc_") and len(name) > 4 else name

def isGenerated(file):
  name = file.name()
  return name.startswith("moc_") or name.endswith(".moc")

def pairAcrossFolders(groups, isAborted):
  '''
  Merge groups with the same name in different folders whose declarations and
  definitions overlap, along with every group linked to them that way
  '''
  byStem = dict()
  for key in groups:
    byStem.setdefault(key[1], []).append(key)

  parent = {key: key for key in groups}
  def find(key):
    while parent[key] != key:
      parent[key] = parent[parent[key]]
      key = parent[key]
    return key

  refs = RefCache()
  for keys in byStem.values():
    if isAborted():
      return
    for i in range(len(keys)):
      for j in range(i + 1, len(keys)):
        a, b = groups[keys[i]], groups[keys[j]]
        for language in {f.language() for f in a} & {f.language() for f in b} & DECLARATION_REFS.keys():
          if max(overlap(a, b, language, refs), overlap(b, a, language, refs)) >= MIN_OVERLAP:
            parent[find(keys[i])] = find(keys[j])
            break

  for key in list(groups):
    root = find(key)
    if root != key:
      groups[root].extend(groups.pop(key))

def overlap(definers, declarers, language, refs):
  '''
  The larger of the shares of what definers define and what declarers declare
  that the two have in common, leaving out what each side keeps to itself
  '''
  define, declare, _ = DECLARATION_REFS[language]
  definers = [f for f in definers if f.language() == language]
  declarers = [f for f in declarers if f.language() == language]
  defined = set().union(*(refs(f, define, language) for f in definers))
  declared = set().union(*(refs(f, declare, language) for f in declarers))
  shared = defined & declared
  if not shared:
    return 0
  defined = (defined - set().union(*(refs(f, declare, language) for f in definers))) | shared
  declared = (declared - set().union(*(refs(f, define, language) for f in declarers))) | shared
  return max(len(shared) / len(defined), len(shared) / len(declared))

class RefCache:
  def __init__(self):
    self.cache = dict()

  def __call__(self, file, refKind, language):
    if (file, refKind) not in self.cache:
      entKinds = DECLARATION_REFS[language][2]
      self.cache[(file, refKind)] = {ref.ent() for ref in file.filerefs(refKind, entKinds, True)}
    return self.cache[(file, refKind)]

class IncludeCache:
  def __init__(self):
    self.cache = dict()

  def __call__(self, file):
    if file not in self.cache:
      self.cache[file] = {ref.ent() for ref in file.filerefs("include", "file", True)}
    return self.cache[file]

def isHeader(file):
  return file.kind().check("header file")

def commonPathLength(a, b):
  return len(os.path.commonpath([a, b])) if a and b else 0

def groupKey(file):
  return (os.path.dirname(file.longname()), stem(file))

def representativeKey(file):
  return (isGenerated(file), not isHeader(file), file.metric("CountLineCode") or 0, file.longname())
