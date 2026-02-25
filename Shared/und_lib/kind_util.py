import understand

# These kind strings are designed to be non-overlapping. So things
# like "Web Javascript Function Class" that could match FUNCTION_KIND_STR
# or CLASS_KIND_STRING are excluded from both. Use the FUNCTION_LIKE_KIND_STR /
# CLASS_LIKE_KIND_STR to allow overlaps.

FUNCTION_KIND_STR_WITH_UNRESOLVED=("ada entry, ada function, ada procedure, ada protected, ada task,"
  "c function,"
  "csharp method,"
  "fortran block data, fortran function, fortran interface, fortran program, fortran subroutine,"
  "java method,"
  "jovial subroutine,"
  "pascal compunit, pascal function, pascal procedure,"
  "python function,"
  "rust function,"
  "vhdl procedure, vhdl function, vhdl process, vhdl architecture,"
  "web ~class function, web method")
FUNCTION_KIND_STR=("ada entry ~unresolved, ada function ~unresolved, ada procedure ~unresolved, ada protected ~unresolved, ada task ~unresolved,"
  "c function ~unresolved,"
  "csharp method ~unresolved,"
  "fortran block data ~unresolved, fortran function ~unresolved, fortran interface ~unresolved, fortran program ~unresolved, fortran subroutine ~unresolved,"
  "java method ~unresolved,"
  "jovial subroutine ~unresolved,"
  "pascal compunit ~unresolved, pascal function ~unresolved, pascal procedure ~unresolved,"
  "python function ~unresolved,"
  "rust function ~unresolved,"
  "vhdl procedure ~unresolved, vhdl function ~unresolved, vhdl process ~unresolved, vhdl architecture ~unresolved,"
  "web ~class function ~unresolved, web method ~unresolved")
FUNCTION_LIKE_KIND_STR = FUNCTION_KIND_STR + ", web javascript function class ~unresolved"
FUNCTION_LIKE_KIND_STR_WITH_UNRESOLVED = FUNCTION_KIND_STR_WITH_UNRESOLVED + ", web javascript function class"

CLASS_LANGUAGES=["Basic", "C++", "C#", "Java", "Pascal", "Python", "Web"]
CLASS_KIND_STR_WITH_UNRESOLVED=("basic module, basic type,"
  "c class, c struct, c union,"
  "csharp type,"
  "java file, java type, java package,"
  "pascal class, pascal interface,"
  "python class,"
  "web javascript class ~function, web php class, web php interface, web php trait")
CLASS_KIND_STR=("basic module ~unresolved, basic type ~unresolved,"
  "c class ~unresolved, c struct ~unresolved, c union ~unresolved,"
  "csharp type ~unresolved,"
  "java type ~unresolved, java package ~unresolved,"
  "pascal class ~unresolved, pascal interface ~unresolved,"
  "python class ~unresolved,"
  "web javascript class ~function ~unresolved, web php class ~unresolved, web php interface ~unresolved, web php trait ~unresolved")
CLASS_LIKE_KIND_STR = CLASS_KIND_STR + ", web javascript function class ~unresolved, java file ~unresolved"
CLASS_LIKE_KIND_STR_WITH_UNRESOLVED = CLASS_KIND_STR_WITH_UNRESOLVED + ", web javascript function class, java file"

FILE_KIND_STR_WITH_UNRESOLVED="file"
FILE_KIND_STR="file ~unresolved ~unknown"

# It's still possible to not be able to create a lexer for resolved
# files or functions if the file is out of date.
LEXER_ENTS_KIND_STR=FUNCTION_KIND_STR+","+FILE_KIND_STR

PYTHON_PACKAGE_KIND_STR="python package ~unknown"

def can_contain_functions(target):
  return isinstance(target, understand.Db) or \
         isinstance(target, understand.Arch) or \
         (isinstance(target, understand.Ent) and \
           (target.kind().check(FILE_KIND_STR) or \
            target.kind().check(CLASS_KIND_STR) or \
            target.kind().check(PYTHON_PACKAGE_KIND_STR)))

def list_functions(target, arch_recursive=True, include_unresolved=False):
  kstr = FUNCTION_KIND_STR_WITH_UNRESOLVED if include_unresolved else FUNCTION_KIND_STR
  return list_by_kind(target, kstr, arch_recursive=arch_recursive, expand_classes=True)

def can_contain_classes(target, db=None, test_ents=True):
  if db is None and isinstance(target, understand.Db):
    db = target
  if db:
    # If db is known, check languages
    dblangs = db.language()
    if not any(lang in dblangs for lang in CLASS_LANGUAGES):
      return False

  return isinstance(target, understand.Db) or \
         isinstance(target, understand.Arch) or \
         (isinstance(target, understand.Ent) and \
           (target.kind().check(FILE_KIND_STR) or \
            target.kind().check(PYTHON_PACKAGE_KIND_STR)) and \
           (not test_ents or len(list_classes(target)) > 0))

def list_classes(target, arch_recursive=True, include_unresolved=False):
  kstr = CLASS_KIND_STR_WITH_UNRESOLVED if include_unresolved else CLASS_KIND_STR
  return list_by_kind(target, kstr, arch_recursive=arch_recursive)

def can_contain_files(target):
  return isinstance(target, understand.Db) or \
         isinstance(target, understand.Arch) or \
         (isinstance(target, understand.Ent) and \
            target.kind().check(PYTHON_PACKAGE_KIND_STR))

def list_files(target, arch_recursive=True, include_unresolved=False):
  kstr = FILE_KIND_STR_WITH_UNRESOLVED if include_unresolved else FILE_KIND_STR
  return list_by_kind(target, FILE_KIND_STR, arch_recursive=arch_recursive, expand_files=False)

def list_by_kind(target, ent_kind_str, arch_recursive=True, expand_files=True, expand_classes=False, expand_python_packages=True):
  """
  Return a list or set of entities matching the entity kind string inside
  the target (database, architecture, or container entity such as a file)
  """
  if isinstance(target, understand.Db):
    return target.ents(ent_kind_str)

  start = []
  if isinstance(target, understand.Arch):
    start = target.ents(arch_recursive)
  elif isinstance(target, understand.Ent):
    start.append(target)
  else:
    raise Exception("Target must be understand.Db, understand.Arch, or understand.Ent")

  ents = set()
  files = set()
  python_packages = set()
  classes = set()

  for ent in start:
    if ent.kind().check(ent_kind_str):
      ents.add(ent)
    elif ent.kind().check(PYTHON_PACKAGE_KIND_STR):
      python_packages.add(ent)
    elif ent.kind().check(FILE_KIND_STR):
      files.add(ent)
    elif ent.kind().check(CLASS_KIND_STR):
      classes.add(ent)

  if expand_python_packages:
    for ent in python_packages:
      for ref in ent.refs("python contain","file ~unknown ~unresolved",True):
        files.add(ref.ent())
        if ref.ent().kind().check(ent_kind_str):
          ents.add(ref.ent())
  if expand_files:
    for ent in files:
      for ref in ent.filerefs("define, separate, ada declare body, vhdl declare", ent_kind_str, True):
        ents.add(ref.ent())
  if expand_classes:
    for ent in classes:
      for ref in ent.refs("define, declare", ent_kind_str, True):
        ents.add(ref.ent())

  return ents






