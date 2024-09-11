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

import re
import understand
import concurrency

LOCK_FUNCTIONS = {
  "QBasicMutex::lock",
  "QMutexLocker::QMutexLocker" 
}

UNLOCK_FUNCTIONS = {
  "QBasicMutex::unlock",
  "QMutexLocker::~QMutexLocker"
}

QT_CONCURRENT_FUNCTIONS = {
  "QtConcurrent::blockingFilter",
  "QtConcurrent::blockingFiltered",
  "QtConcurrent::blockingFilteredReduced",
  "QtConcurrent::blockingMap",
  "QtConcurrent::blockingMapped",
  "QtConcurrent::blockingMappedReduced",
  "QtConcurrent::filter",
  "QtConcurrent::filtered",
  "QtConcurrent::filteredReduced",
  "QtConcurrent::map",
  "QtConcurrent::mapped",
  "QtConcurrent::mappedReduced",
  "QtConcurrent::run"
}

class Qt(concurrency.Concurrency):
  def __init__(self, db):
    super().__init__(db)


  def name(self):
    return "qt"

  def lock_functions(self):
    return LOCK_FUNCTIONS

  def unlock_functions(self):
    return UNLOCK_FUNCTIONS


  def thread_entry_points(self):
    # Find run overrides.
    result = set()
    prototype = re.compile(r"(QThread|QRunnable)::run")
    for ent in self.db.lookup(prototype, "function"):
      result |= set(ent.ents("overriddenby"))

    # Find functions passed to QtConcurrent functions.
    type = re.compile(r"(Map|Reduce|Keep)?Funct(or|ion) &&")
    return list(result) + self.entry_points(QT_CONCURRENT_FUNCTIONS, type)
