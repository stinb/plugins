# Virtual debugger / variable tracker prototype script

import argparse
import understand

helpMsg = """
Welcome to the fake debugger. Available commands are:
  help        print this message
  next        go to the next control flow node. Optionally give a # argument
              to follow the path # without prompting (ex: "next 0" to follow
              the yes path)
  step        if there is a call reference, step into the function. Otherwise
              the behavior matches next. If there are multiple call references
              the call reference must be chosen with a # ("step 0" to follow
              the first call)
  continue    run until the next choice
  locals      list the local variables, including parameters
  globals     list the global variables that are referenced from the current
              function. Optionally specify "file" to list all globals in the
              current file or globals "all" for all globals in the project.
              (ex "globals file" or "globals all")
  hits        Print the references hit during the current executation for
              the given entity name (ex "hits foobar"). If no entity name
              is given, list all the entities with hits.
  exit/quit   stop debugging
"""

# Control Flow Node - > Lexer lexemes
def lexemes(node, lexer):
  ''' Return the list of lexemes in the range of the control flow graph node '''
  lexlist = []
  line = node.line_begin()
  if line:
    eline = node.line_end()
    ecol = node.column_end()
    lexeme = lexer.lexeme(line, node.column_begin())
    while lexeme and before(lexeme.line_begin(), lexeme.column_begin(), eline, ecol):
      lexlist.append(lexeme)
      lexeme = lexeme.next()
  return lexlist

def before(line, col, beforeLine, beforeCol):
  return line < beforeLine or (line == beforeLine and col <= beforeCol)

# Source Code Functions
def lexText(lexemes):
  ''' Return the source code covered by the lexemes. '''
  text = ""
  for lexeme in lexemes:
    text += lexeme.text()
  return text

def nodeText(node, lexer):
  ''' Return the source code in the control flow node's range '''
  lexlist = lexemes(node,lexer)
  if lexlist:
    return lexText(lexlist)
  return node.kind()

# Control Flow Graph filtering
def isFiltered(node):
  ''' Nodes that should be skipped when traversing the graph '''
  filtered = ["do-while", "end-case", "loop", "end-switch", "end-block",
  "end-try","end-select", "else-where", "end-where", "end-do","repeat-until",
  "end-with-do","do","passive-implicit","else","end-if","end-loop"]
  return node.kind() in filtered

# Track References that have been hit in the current run
hits = dict()

def refs(node, func, refkindstr = ""):
  file = defFile(func)
  sline = node.line_begin()
  if not file or not sline:
    return []
  scol = node.column_begin()
  eline = node.line_end()
  ecol = node.column_end()
  reflist = []
  for ref in file.filerefs():
    if (ref.scope() == func and
        (not refkindstr or ref.kind().check(refkindstr)) and
        before(sline, scol, ref.line(), ref.column()) and
        before(ref.line(), ref.column(), eline, ecol)):
      reflist.append(ref)
  return reflist

def addHits(node, lexer, func, note = ""):
  ''' Add all the references from the node's range '''
  global hits
  for ref in refs(node,func):
    context = lexText(lexer.lexemes(ref.line(), ref.line()))
    hits.setdefault(ref.ent(), []).append([ref,context,note])

def printHits(args = []):
  ''' Hits command '''
  if len(args) == 1:
    nm = args[0]
    for ent,reflist in hits.items():
      if nm in ent.longname():
        for parts in reflist:
          ref = parts[0]
          context = parts[1]
          note = parts[2]
          print ("  ",ref.kind().name(), ref.line(), ":", context.strip())
          if note:
            print ("    ", note)
  else:
    printVariables(hits.keys(), False)

# Lookup lists of variables
def locals(func):
  ''' Local variables/objects and parameters '''
  entset = set()
  for ref in func.refs():
    if ref.isforward() and ref.ent().kind().check("local object, local variable, parameter"):
      entset.add(ref.ent())
  return entset

def globals(func, args = []):
  ''' Globals '''
  entset = set()
  if "all" in args:
    entset = db.ents("global object")
  elif "file" in args:
    for ref in defFile(func).filerefs("define,declare","global object", True):
      entset.add(ref.ent())
  else:
    for ref in func.refs():
      if ref.isforward() and ref.ent().kind().check("global object"):
        entset.add(ref.ent())
  return entset

def printVariables(ents, hitsIndic = True):
  ''' Print a list of entities '''
  ents = sorted(ents, key = lambda ent: ent.name())
  for ent in ents:
    leader = "  "
    if hitsIndic and ent in hits:
      leader = "* "
    print (leader, ent.type(), ent.longname(), "(", ent.kind().longname(), ")")

# Navigation
def printText(node, lexer):
  ''' Print the current region of source code '''
  line = node.line_begin()
  if line is None:
    print("No location information for node", node.kind())
    return

  start = line - 2;
  if start < 1:
    start = 1

  end = line + 2;
  if end > lexer.lines():
    end = lexer.lines()

  for l in range(start, end+1):
    leader = "  "
    if l == line:
      leader = "->"
    text = ""
    for lexeme in lexer.lexemes(l,l):
      text += lexeme.text()
    print ("{}{:>5}{}".format(leader,l,text), end="")

def next(node, lexer, func, args = []):
  ''' Navigate to the next control flow node '''
  choices = node.children()
  if not choices:
    addHits(node,lexer, func)
    return None

  if len(choices) == 1:
    addHits(node,lexer, func)
    if isFiltered(choices[0]):
      return next(choices[0], lexer, func)
    return choices[0]

  if args:
    try:
      idx = int(args[0])
      if idx >= 0 and idx < len(choices):
        note = node.child_label(choices[idx])
        if not note:
          note = str(idx) + ": " + nodeText(choices[idx])
        note = "Choose path " + note
        addHits(node,lexer, func, note)
        if isFiltered(choices[idx]):
          return next(choices[idx],lexer,func)
        return choices[idx]
    except:
      pass

  print ("Multiple paths exist. Specify the path # in the command (ex: next 0)")
  print ("Possible paths from: ", nodeText(node, lexer))
  for i in range(len(choices)):
    label = node.child_label(choices[i])
    if not label:
      label = nodeText(choices[i])
    print ("\t",i, label)
  return node

def step(node, lexer, func, args = []):
  choices = []
  for ref in refs(node, func, "call"):
    if not ref.ent() in choices and defFile(ref.ent()):
      choices.append(ref.ent())

  if not choices:
    return next(node,lexer, func, args)

  if len(choices) == 1:
    enterFunc((choices[0]))
    return node

  if args:
    try:
      idx = int(args[0])
      if idx >= 0 and idx < len(choices):
        enterFunc(choices[idx])
        return node
    except:
      pass

  print ("Multiple calls exist. Specify the call # in the command (ex: step 0)")
  print ("Possible calls in: ", nodeText(node,lexer))
  for i in range(len(choices)):
    print("\t",i, choices[i].name())
  return node

# Main Loop
def enterFunc(func):
  ''' Execute the given function '''
  cfg = func.control_flow_graph()
  if not cfg:
    print ("Error: no control flow information for function")
    return

  lexer = defFile(func).lexer()
  if not lexer:
    print ("Error: no lexer for function's file")
    return

  curNode = next(cfg.start(), lexer, func)
  printText(curNode, lexer)
  while True:
    parts = input("Enter a command: ").split()
    if parts:
      cmd = parts[0]
    else:
      cmd = ''
    if cmd == "next" or cmd == "n":
      curNode = next(curNode, lexer, func, parts[1:])
    elif cmd == "step" or cmd == "s":
      curNode = step(curNode, lexer, func, parts[1:])
    elif cmd == "help":
      print(helpMsg)
    elif cmd == "continue":
      choices = curNode.children()
      while len(choices) == 1:
        curNode = next(curNode, lexer, func)
        choices = curNode.children()
    elif cmd == "globals":
      printVariables(globals(func, parts[1:]))
      continue
    elif cmd == "locals":
      printVariables(locals(func))
      continue
    elif cmd == "hits":
      printHits(parts[1:])
      continue
    elif cmd == "exit" or cmd == "quit":
      exit()
    elif not cmd:
      pass
    else:
      print ("Unrecognized command. Use \"help\" to list available commands.")

    if curNode:
      printText(curNode, lexer)
    else:
      break

# Argument parsing
def defFile(ent):
  if not ent:
    return None

  if ent.kind().check("file"):
    return ent

  ref = ent.ref("definein, body declarin")
  if ref:
    return ref.file()
  return None

def findFunction(db, name):
  ent = db.lookup(name, "function,method")
  while not ent:
    name = input("Function not found. Enter a function: ")
    ent = db.lookup(next, "function,method")
  if len(ent) > 1:
    print("Multiple functions found:")
    for i in range(len(ent)):
      print (i, ent[i].longname(), defFile(ent[i]))
    idx = int(input("Enter the number of the desired function "))
    ent = ent[idx]
  else:
    ent = ent[0]
  return ent

if __name__ == "__main__":
  parser = argparse.ArgumentParser(description='Fake Debugger Prototype')
  parser.add_argument('function')
  parser.add_argument('database')

  args = parser.parse_args()

  db = understand.open(args.database)
  func = findFunction(db, args.function)
  print (helpMsg)
  enterFunc(func)

