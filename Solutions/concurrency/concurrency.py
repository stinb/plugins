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
