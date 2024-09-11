#
#          Copyright (c) 2024, Scientific Toolworks, Inc.
#
# This file contains proprietary information of Scientific Toolworks, Inc.
# and is protected by federal copyright law. It may not be copied or
# distributed in any form or medium without prior written authorization
# from Scientific Toolworks, Inc.
#
# Author: Jason Haslam
#

import understand

class CriticalSection:
  def __init__(self, lock: understand.Ref, unlock: understand.Ref):
    self.lock = lock
    self.unlock = unlock

  def contains(self, ref: understand.Ref) -> bool:
    line = ref.line()
    return line > self.lock.line() and line < self.unlock.line()

class Concurrency:
  def __init__(self, db):
    self.db = db


  def name(self) -> str:
    raise NotImplementedError

  def lock_functions(self) -> set[understand.Ent]:
    raise NotImplementedError

  def unlock_functions(self) -> set[understand.Ent]:
    raise NotImplementedError

  def thread_entry_points(self) -> list[understand.Ent]:
    raise NotImplementedError


  def critical_sections(self, file: understand.Ent) -> list[CriticalSection]:
    calls = file.filerefs("call", "function")
    calls.sort(key=understand.Ref.line)

    lock_functions = self.lock_functions()
    unlock_functions = self.unlock_functions()
    locks = [ref for ref in calls if ref.ent().longname() in lock_functions]
    unlocks = [ref for ref in calls if ref.ent().longname() in unlock_functions]
    return [CriticalSection(*args) for args in zip(locks, unlocks)]


  def entry_points(self, names: set[str], type: str) -> list[understand.Ent]:
    def functor_call_operator(assignee):
      type = None
      if assignee.kind().check("function"):
        parent = assignee.parent()
        if parent and assignee.name() == parent.name():
          type = parent # This is a constructor.
      elif assignee.kind().check("object"):
        if types := assignee.ents("typed"):
          type = types[0]

      if type:
        for member_function in type.ents("define", "member function"):
          if member_function.name() == "operator()":
            return member_function

      return None

    def visit_assignments(result, visited, root):
      if root not in visited:
        visited.add(root)
        for assignee in root.ents("assign"):
          if call_operator := functor_call_operator(assignee):
            result.add(call_operator)
          elif assignee.kind().check("function"):
            result.add(assignee)
          else:
            visit_assignments(result, visited, assignee)

    # Lookup named functions.
    functions = set()
    for function in self.db.ents("function"):
      if function.longname() in names:
        functions.add(function)

    # Find child parameters of the functions with the matching type.
    params = set()
    for param in self.db.ents("parameter"):
      if param.parent() in functions and type.match(param.type()):
        params.add(param)

    # Traverse assignment tree to find functions assigned to the parameters.
    result = set()
    visited = set()
    for param in params:
      visit_assignments(result, visited, param)
    return list(result)
