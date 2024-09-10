#
#          Copyright (c) 2024, Scientific Toolworks, Inc.
#
# This file contains proprietary information of Scientific Toolworks, Inc.
# and is protected by federal copyright law. It may not be copied or
# distributed in any form or medium without prior written authorization
# from Scientific Toolworks, Inc.
#
# Author: Natasha Stander
#

import re
import understand
import concurrency

LOCK_FUNCTIONS = {
  "pthread_mutex_lock"
}

UNLOCK_FUNCTIONS = {
  "pthread_mutex_unlock"
}

class Pthread(concurrency.Concurrency):
  def __init__(self, db):
    self.db = db


  def lock_functions(self):
    return LOCK_FUNCTIONS

  def unlock_functions(self):
    return UNLOCK_FUNCTIONS


  def thread_entry_points(self):
    create_functions = self.db.lookup("pthread_create", "function")
    if not create_functions:
      return []

    start_routine = self._lookup_start_routine(create_functions[0])
    if not start_routine:
      return []

    return self._assigned_functions(start_routine)

  def _lookup_start_routine(self, create_function):
    type = re.compile(r"void \*.*\(\*.*\)\(void \*.*\)")
    for param in self.db.ents("parameter"):
      if param.parent() == create_function and type.match(param.type()):
        return param
    return None

  def _assigned_functions(self, start_routine):
    # FIXME: Traverse assignment tree recursively.
    return start_routine.ents("assign", "function")
