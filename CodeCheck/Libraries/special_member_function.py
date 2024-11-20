#
#          Copyright (c) 2023, Scientific Toolworks, Inc.
#
# This file contains proprietary information of Scientific Toolworks, Inc.
# and is protected by federal copyright law. It may not be copied or
# distributed in any form or medium without prior written authorization
# from Scientific Toolworks, Inc.
#
# Author: Jason Haslam
#

import re

# FIXME: volatile?
RE_FMT = r'(const\s+)?{}\s*&{{1,2}}'

def is_special_member_function(ent):
  parent = ent.parent()
  if not parent:
    return False

  if ent.name() == '~' + parent.name():
    return True # destructor

  if ent.name() == parent.name():
    parameters = ent.ents('declare,define', 'parameter')
    if not parameters:
      return True # default constructor

    # FIXME: default arguments?
    if len(parameters) == 1:
      type = parameters[0].type()
      if re.match(RE_FMT.format(parent.name()), type):
        return True # copy/move constructor

    return False

  if ent.name() == 'operator=':
    parameters = ent.ents('declare,define', 'parameter')
    if len(parameters) == 1:
      type = parameters[0].type()
      if type == parent.name():
        return True # copy assignment operator

      if re.match(RE_FMT.format(parent.name()), type):
        return True # copy/move assignment operator

  return False
