# Virtual debugger / variable tracker prototype script

import argparse
import understand
from collections import deque

helpMsg = """
Welcome to the fake debugger. Available commands are:
  help        print this message
  next        go to the next control flow node. Optionally give a # argument
              to follow the path # without prompting (ex: "next 0" to follow
              the yes path)
  prev        go to the previous control flow node. Optionally give a # argument
              to follow the path #
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
  history     Print the current executation path
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
  "end-with-do","do","passive-implicit","else","end-if","end-loop","start"]
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

def addHits(node, lexer, func, note = "", isForward = True):
  ''' Add all the references from the node's range '''
  global hits
  for ref in refs(node,func):
    context = lexText(lexer.lexemes(ref.line(), ref.line()))
    if isForward:
      hits.setdefault(ref.ent(), deque()).append([ref,context,note])
    else:
      hits.setdefault(ref.ent(), deque()).appendleft([ref,context,note])

def removeHits(node, func, isForward = True):
  global hits
  for ref in refs(node, func):
    size = len(hits.get(ref.ent(),deque()))
    if size == 1:
      del hits[ref.ent()]
    elif size > 1:
      if isForward:
        hits[ref.ent()].pop()
      else:
        hits[ref.ent()].popleft()

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

# manage path
history = deque()
curNode = 0

def printHistory():
  lastFunc = None
  for i in range(len(history)):
    curFunc = history[i]["function"]
    if curFunc != lastFunc:
      print (curFunc.longname())
      lastFunc = curFunc
    node = history[i]["node"]
    if isFiltered(node):
      continue

    leader = "  "
    if i == curNode:
      leader = "->"
    l = node.line_begin()
    text = nodeText(node, history[i]["lexer"])
    print ("{}{:>5} {}".format(leader,l,text))

def changeNode(toNode, toLexer, toFunc, note, isForward):
  global history
  global curNode
  global hits
  histItem = { "node" : toNode, "lexer": toLexer, "function": toFunc }

  curItem = None
  if not history:
    history.append(histItem)
    curNode = 0
  elif isForward:
    if curNode == (len(history) - 1):
      curItem = history[curNode]
      history.append(histItem)
      curNode += 1
    elif history[1] == histItem:
      removeHits(toNode, toFunc, False)
      history.popLeft()
    else:
      curItem = history[0]
      hits.clear()
      history.clear()
      history = deque([curItem, histItem])
      curNode = 1
  else:
    if curNode == 0:
      curItem = history[curNode]
      history.appendleft(histItem)
    elif history[-2] == histItem:
      removeHits(toNode, toFunc, True)
      history.pop()
      curNode -= 1
    else:
      curItem = history[-1]
      hits.clear()
      history.clear()
      history = deque([histItem, curItem])
      curNode = 0

  if curItem:
    addHits(curItem["node"], curItem["lexer"], curItem["function"], note, isForward)

def next(node, lexer, func, args = []):
  ''' Navigate to the next control flow node '''
  choices = node.children()
  labels = []
  for choice in choices:
    labels.append(node.child_label(choice))
  nextNode,note = chooseNode(node,lexer,args,choices, labels)
  if nextNode != node:
    changeNode(nextNode, lexer, func, note, True)
    if nextNode and isFiltered(nextNode):
      return next(nextNode, lexer, func, [])
  return nextNode

def prev(node, lexer, func, reverseCFG, args = []):
  if node.kind() == "start":
    choices = func.refs("callby")
    return chooseFunc(node, choices, True, "Possible calls from:", args)
  choices = reverseCFG.get(node,[])
  labels = []
  for choice in choices:
    labels.append(choice.child_label(node))
  nextNode,note = chooseNode(node,lexer,args,choices,labels)
  if nextNode != node:
    changeNode(nextNode, lexer, func, note, False)
    if nextNode and isFiltered(nextNode):
      return prev(nextNode, lexer, func, reverseCFG, [])
  return nextNode

def chooseNode(node, lexer, args, choices, labels):
  if not choices:
    return None, ""

  if len(choices) == 1:
    return choices[0], ""

  if args:
    try:
      idx = int(args[0])
      if idx >= 0 and idx < len(choices):
        note = labels[idx]
        if not note:
          note = str(idx) + ": " + nodeText(choices[idx],lexer)
        note = "Choose path " + note
        return choices[idx],note
    except Exception as e:
      print ("Exception" + str(e))
      pass

  print ("Multiple paths exist. Specify the path # in the command (ex: next 0)")
  print ("Possible paths from: ", nodeText(node, lexer))
  for i in range(len(choices)):
    label = labels[i]
    if not label:
      label = nodeText(choices[i], lexer)
    print ("\t",i, label)
  return node, ""

def step(node, lexer, func, args = []):
  choices = []
  uniqueEnts = set()
  for ref in refs(node, func, "call"):
    if not ref.ent() in uniqueEnts and defFile(ref.ent()):
      uniqueEnts.add(ref.ent())
      choices.append(ref)
  if not choices:
    return next(node,lexer, func, args)

  contextMessage = "Possible calls in: " + nodeText(node,lexer)
  return chooseFunc(node, choices, False, contextMessage, args)

def chooseFunc(node, choices, useLine, contextMessage, args = []):
  if not choices:
    return None # nowhere to go

  line = -1
  if len(choices) == 1:
    if useLine:
      line = choices[0].line()
    enterFunc(choices[0].ent(), line)
    return node

  if args:
    try:
      idx = int(args[0])
      if idx >= 0 and idx < len(choices):
        if useLine:
          line = choices[idx].line()
        enterFunc(choices[idx].ent(), line)
        return node
    except:
      pass

  print ("Multiple calls exist. Specify the call # in the command (ex: step 0)")
  print (contextMessage)
  for i in range(len(choices)):
    lineText = ""
    if useLine:
      lineText = " (" + str(choices[i].line()) + ")"
    print("\t",i, choices[i].ent().name(), lineText)
  return node



# Main Loop
def enterFunc(func, line = -1):
  ''' Execute the given function '''
  cfg = func.control_flow_graph()
  if not cfg:
    print ("Error: no control flow information for function")
    return

  try:
    lexer = defFile(func).lexer()
  except understand.UnderstandError:
    print ("Error: no lexer for function's file")
    return

  reverseCFG = dict()
  for node in cfg.nodes():
    for child in node.children():
      reverseCFG.setdefault(child,[]).append(node)

  curNode = None
  if line != -1:
    for node in cfg.nodes():
      if node.line_begin() and node.line_begin() >= line:
        curNode = node
        # Assume that going to a line is always going back out of a call
        changeNode(curNode, lexer, func, "", False)
        break
  if not curNode:
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
    elif cmd == "prev" or cmd == "p":
      curNode = prev(curNode, lexer, func, reverseCFG, parts[1:])
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
    elif cmd == "hist" or cmd == "history":
      printHistory()
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

