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

CREATE_FUNCTIONS = {
  "pthread_create"
}

class Pthread(concurrency.Concurrency):
  def __init__(self, db):
    super().__init__(db)


  def name(self):
    return "pthreads"

  def lock_functions(self):
    return LOCK_FUNCTIONS

  def unlock_functions(self):
    return UNLOCK_FUNCTIONS


  def thread_entry_points(self):
    type = re.compile(r"void \*.*\(\*.*\)\(void \*.*\)")
    return self.entry_points(CREATE_FUNCTIONS, type)
