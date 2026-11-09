# Name groups of entities after a few of their members, showing as much of
# each member's long name as it takes to tell the names apart.

import os
import re
from collections import Counter

# File names that say nothing about the file, so its folder is always shown
GENERIC_FILE_NAMES = {
  "__init__", "index", "main", "mod", "lib", "package-info", "module-info",
}

# Separators between the parts of a long name, such as ns::Class::method or
# package.Class.method
QUALIFIER_SEPARATOR = re.compile(r"(::|\.)")

def distinct_names(entityLists, maxJoined=3):
  '''
  Return a name for each list of entities, in the same order, with no two
  names the same unless their lists are empty.

  A list is named after its entities, sorted and joined with " + ". Only the
  first maxJoined are shown, followed by "..." when there are more. Each entity
  starts with its short name, and when names clash, the entities in them show
  more of their long names until the names differ:
  - Code entities show more of their qualified name: method, then
    Class::method, then ns::Class::method.
  - Files show their name without the extension, then add their folders in
    parentheses from the nearest one up: Repository, then Repository (git),
    then Repository (src git). Folders are separated by spaces, since a "/"
    would nest architectures. When the file it clashes with is in the same
    folder, the extension is shown instead. Generic file names such as main or
    __init__ always show their folder.
  Names that still clash get a number, in order of their entities' unique
  names.
  '''
  labels = [[EntityLabel(ent) for ent in ents] for ents in entityLists]
  while True:
    names = [joined(group, maxJoined) for group in labels]
    counts = Counter(name for name in names if name)
    grew = False
    clashes = dict()
    for name, group in zip(names, labels):
      if counts[name] > 1:
        clashes.setdefault(name, []).append(group)
    # Decide every label's peers before any of them grow
    growing = []
    for groups in clashes.values():
      for group in groups:
        for label in group:
          peers = [other for otherGroup in groups if otherGroup is not group
                   for other in otherGroup if other.text() == label.text() and other.ent != label.ent]
          if peers:
            growing.append((label, peers))
    for label, peers in growing:
      grew = label.grow(peers) or grew
    if not grew:
      break

  # Number the names that still clash in order of their entities' unique
  # names, so the numbers don't depend on the order of entityLists
  counts = Counter(name for name in names if name)
  result = list(names)
  clashing = dict()
  for i, name in enumerate(names):
    if counts[name] > 1:
      clashing.setdefault(name, []).append(i)
  for name, indices in clashing.items():
    order = sorted(indices, key=lambda i: sorted(ent.uniquename() for ent in entityLists[i]))
    for number, i in enumerate(order, 1):
      result[i] = "{} ({})".format(name, number)
  return result

def joined(group, maxJoined):
  parts = sorted(label.text() for label in group)
  if len(parts) > maxJoined:
    parts = parts[:maxJoined] + ["..."]
  return " + ".join(parts)

class EntityLabel:
  '''
  The part of an entity's long name that is shown, which can grow
  '''
  def __init__(self, ent):
    self.ent = ent
    self.isFile = ent.kind().check("file")
    if self.isFile:
      name = ent.name()
      self.base = name.split(".", 1)[0] or name
      self.fullName = name
      relname = ent.relname() if hasattr(ent, "relname") else ent.longname()
      self.parts = [part for part in reversed(os.path.dirname(relname).replace("\\", "/").split("/")) if part]
      self.level = 1 if self.base in GENERIC_FILE_NAMES and self.parts else 0
      self.showExtension = False
    else:
      longname = ent.longname()
      pieces = QUALIFIER_SEPARATOR.split(longname)
      # pieces alternates name, separator, name, ...; keep suffixes that end in
      # whole parts
      self.suffixes = ["".join(pieces[i:]) for i in range(len(pieces) - 1, -1, -2)]
      self.level = 0

  def grow(self, peers):
    '''
    Show more of the name to tell it apart from peers, the labels it clashes
    with. Return False if there is nothing more to show.
    '''
    if not self.isFile:
      if self.level < len(self.suffixes) - 1:
        self.level += 1
        return True
      return False

    sameFolder = any(peer.isFile and peer.parts == self.parts and peer.fullName != self.fullName for peer in peers)
    if not self.showExtension and (sameFolder or self.level >= len(self.parts)) and self.fullName != self.base:
      self.showExtension = True
      return True
    if self.level < len(self.parts):
      self.level += 1
      return True
    return False

  def text(self):
    if not self.isFile:
      return self.suffixes[self.level]
    name = self.fullName if self.showExtension else self.base
    if not self.level:
      return name
    return "{} ({})".format(name, " ".join(reversed(self.parts[:self.level])))
