import re
import understand

TWO_ARG_FUNCTIONS = [
  "mappedReduced",
  "blockingMappedReduced",
  "filteredReduced",
  "blockingFilteredReduced"
]

class QtConcurrency:
  def __init__(self, db):
    self.db = db

  def thread_entry_points(self):
    result = set()
    function_object_refs = self._function_object_refs()

    # Find functions passed to QtConcurrent functions.
    for function in self.db.lookup("QtConcurrent", "function"):
      count = 2 if function.name() in TWO_ARG_FUNCTIONS else 1
      self._find_function_args(result, function, function_object_refs, count)

    # Find run overloads.
    for ent in self.db.lookup(re.compile(r"(QThread|QRunnable)::run"), "function"):
      for ref in ent.refs("overriddenby"):
        result.add(ref.ent())

    return list(result)

  def _function_object_refs(self):
    # Find potential function object arguments, mapping from scope to
    # tuple with line, column, operator() entity.
    result = dict()
    for ent in self.db.lookup(re.compile(r"operator\(\)"), "function"):
      parent = ent.parent()
      if parent and parent.longname() != "[unnamed]":
        for define_ref in parent.refs("define", "member function"):
          method = define_ref.ent()
          if method.name() == parent.name():
            # This is a constructor. A call could be an argument
            # to a concurrency function.
            for callby_ref in method.refs("callby"):
              line = callby_ref.line()
              column = callby_ref.column()
              result.setdefault(callby_ref.ent(), []).append((line, column, ent))

        for typedby_ref in parent.refs("typedby"):
          # For each variable of the object type, the variable may be
          # passed as an argument to the concurrency function.
          for use_ref in typedby_ref.ent().refs("useby"):
            line = use_ref.line()
            column = use_ref.column()
            result.setdefault(use_ref.ent(), []).append((line, column, ent))

    return result
  
  def _find_function_args(self, result, function, function_object_refs, count):
    # For each place this function is called
    for callby_ref in function.refs("callby"):
      # Need to find function pointers or lambda functions passed as arguments
      # so most accurate would be to use the lexer to find the range to look
      # for references. But, the lexer doesn't seem to report the define in
      # reference for lambda functions so grab all the references for the scope
      # function.
      define_refs = callby_ref.ent().refs("c define, c use ptr", "function")
      arg_refs = [(ref.line(), ref.column(), ref.ent()) for ref in define_refs]

      # Also grab any locations that indicate a function object
      # for the scope function.
      arg_refs += function_object_refs.get(callby_ref.ent(), [])

      # Find the first 'count' number of applicable references.
      found_count = count
      for arg_ref in sorted(arg_refs):
        (line, column, operator) = arg_ref
        if (line > callby_ref.line() or
            (line == callby_ref.line() and column > callby_ref.column())):
          result.add(operator)
          found_count -= 1
        if found_count <= 0:
          break
