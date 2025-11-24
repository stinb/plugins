import functools
import re

from understand import CFGraph, CFNode, Ent, Lexeme, Ref



###########
# General #
###########



# Control Flow / Expressions

# Find corresponding closing curly brace or parenthesis of given opening curly
# brace or parenthesis
def findClose(lexeme):
    count = 0
    if not lexeme:
        return lexeme
    open = lexeme.text()
    if lexeme.text() == '{':
        close = '}'
    elif lexeme.text() == '(':
        close = ')'
    elif lexeme.text() == '[':
        close = ']'
    else:
        return None


    while lexeme and not (lexeme.text() == close and count == 0):
        if lexeme.text() == open:
            count += 1

        lexeme = lexeme.next(True, True)

        if lexeme and lexeme.text() == close:
            count -= 1

    return lexeme


# Find corresponding closing curly brace or parenthesis of given opening curly
# brace or parenthesis
def findOpen(lexeme):
    count = 0
    if not lexeme:
        return lexeme
    close = lexeme.text()
    if lexeme.text() == '}':
        open = '{'
    elif lexeme.text() == ')':
        open = '('
    else:
        return None


    while lexeme and not (lexeme.text() == open and count == 0):
        if lexeme.text() == close:
            count += 1

        lexeme = lexeme.previous(True, True)

        if lexeme and lexeme.text() == open:
            count -= 1

    return lexeme


# Given a node/lexeme, node/lexeme, and another node/lexeme
# See if the order of the parameters is really the order
def lexemeBetweenNodes(node1, lexeme, node2):
    # Make sure both nodes have position information
    if node1.line_begin() == None or node2.line_begin() == None:
        return False

    # Make sure the order is node1, lexeme
    if lexeme.line_begin() < node1.line_begin():
        return False
    if lexeme.line_begin() == node1.line_begin() and lexeme.column_begin() <= node1.column_begin():
        return False

    # Make sure the order is lexeme, node2
    if lexeme.line_begin() > node2.line_begin():
        return False
    if lexeme.line_begin() == node2.line_begin() and lexeme.column_begin() >= node2.column_begin():
        return False

    return True


# Given a CFGraph, get a dictionary:
    # Key: a CFNode
    # Value: the next sibling CFNode
def nextSiblingDictionary(cfg):
    result = {}

    if not cfg:
        return result

    def addToDictionaryIf(key, value):
        if value:
            result[key] = value

    for node in cfg.nodes():
        match node.kind():
            case 'if':
                # Next sibling of 'if'
                children = node.children()
                if children:
                    falseNode = node.children()[-1]
                    if falseNode:
                        result[node] = falseNode
                        # Next sibling of 'else'
                        if falseNode.kind() == 'else':
                            addToDictionaryIf(falseNode, node.end_node())

            case 'switch':
                # Next sibling of 'switch'
                addToDictionaryIf(node, node.end_node())
                # Next sibling of 'case' or 'default'
                children = node.children()
                for i in range(0, len(children) - 1):
                    result[children[i]] = children[i + 1]

            case 'try':
                # Next sibling of 'try'
                children = node.children()
                if len(children) > 1:
                    addToDictionaryIf(node, children[1])
                # Next sibling of 'catch' or 'finally'
                for i in range(1, len(children) - 1):
                    result[children[i]] = children[i + 1]
                if len(children) > 1:
                    addToDictionaryIf(children[-1], node.end_node())

            case _:
                addToDictionaryIf(node, node.end_node())

    return result

# Whether ref A can be before ref B in the control flow, assuming that they are
# both in the same function as the given CFG
def refBeforeRef(cfg: CFGraph, refA: Ref, refB: Ref) -> bool:
    # Find a node with ref A
    nodeA = None
    for node in cfg.nodes():
        if refInNode(refA, node):
            nodeA = node
    if not nodeA:
        return False

    # Find a node afterward with ref B
    stack: list[CFNode] = [nodeA]
    seen: set[CFNode] = {nodeA}
    while stack:
        node = stack.pop()
        if refInNode(refB, node):
            return True
        for child in node.children():
            if child in seen:
                continue
            seen.add(child)
            stack.append(child)

    return False


# Given a node/lexeme, ref, and another node/lexeme
# See if the order of the parameters is really the order
def refBetweenNodes(node1, ref, node2):
    # Make sure both nodes have position information
    if node1.line_begin() == None or node2.line_begin() == None:
        return False

    # Make sure the order is node1, ref
    if ref.line() < node1.line_begin():
        return False
    if ref.line() == node1.line_begin() and ref.column() <= node1.column_begin():
        return False

    # Make sure the order is ref, node2
    if ref.line() > node2.line_begin():
        return False
    if ref.line() == node2.line_begin() and ref.column() >= node2.column_begin():
        return False

    return True


# Whether the reference is in the node range (not the same as the curly braces)
def refInNode(ref: Ref, node: CFNode) -> bool:
    refLine = ref.line()
    refColumn = ref.column()
    nodeLineBegin = node.line_begin()
    if nodeLineBegin == None:
        return False
    nodeLineEnd = node.line_end()
    if refLine < nodeLineBegin or refLine > nodeLineEnd:
        return False
    if refLine == nodeLineBegin and refColumn < node.column_begin():
        return False
    if refLine == nodeLineEnd and refColumn > node.column_end():
        return False
    return True


# References

# Given 3 refs
# See if the order of the refs is really the order
def refBetweenRefs(ref1, ref2, ref3):
    # Make sure the files are the same
    if ref1.file() != ref2.file() or ref1.file() != ref3.file():
        return False

    # Make sure the order is ref1, ref2
    if ref2.line() < ref1.line():
        return False
    if ref2.line() == ref1.line() and ref2.column() <= ref1.column():
        return False

    # Make sure the order is ref2, ref3
    if ref3.line() < ref2.line():
        return False
    if ref3.line() == ref2.line() and ref3.column() <= ref2.column():
        return False

    return True

# Comparator for list.sort(key=refComparatorSameFile)
def refComparatorSameFile(a, b):
    return a.line() - b.line() or a.column() - b.column()
refComparatorSameFile = functools.cmp_to_key(refComparatorSameFile)



# Lexemes

# Given 2 lexemes, see if the first is before the second
def lexemeBeforeLexeme(lex1, lex2):
    return lex1.line_begin() < lex2.line_begin() or \
        (lex1.line_begin() == lex2.line_begin() and lex1.column_begin() < lex2.column_begin())

# Given a lexeme & position, see if it is before the position
def lexemeBefore(lex, line, column):
    return lex.line_begin() < line or \
        (lex.line_begin() == line and lex.column_begin() < column)

# Given a lexeme & position, see if it is at the position
def lexemeEquals(lex, line, column):
    return lex.line_begin() == line and lex.column_begin() == column

# Given a lexeme & position, see if it is after the position
def lexemeAfter(lex, line, column):
    return lex.line_begin() > line or \
        (lex.line_begin() == line and lex.column_begin() > column)

# Given a value lexeme, get the entities assigned that value
def entsAssigned(lex):
    ents = []
    beforeStatement = {';', '{'}

    # Make sure this is an assignment to the given lexeme
    assignment = False
    lex = lex.previous(True, True)
    if lex and lex.text() == '=':
        assignment = True

    # Get all the assigned entities
    if assignment:
        while lex and lex.text() not in beforeStatement:
            if lex.text() == '=':
                lex = lex.previous(True, True)
                if not lex or lex.text() in beforeStatement:
                    break
                if lex.ent():
                    ents.append(lex.ent())
            lex = lex.previous(True, True)

    return ents

# Given a parameter lexeme, get the function/method entity
def functionCalled(lex):
    l = lex.previous(True, True)
    if l.text() not in {'(', ','}:
        return

    r = lex.next(True, True)
    if not r or r.text() not in {')', ','}:
        return

    # Go back until the end of the call/statement
    while lex and lex.text() not in {'(', ';', '{'}:
        lex = lex.previous(True, True)

    # Go back until function/method
    lex = lex.previous(True, True)
    if not lex or lex.token() != 'Identifier' or not lex.ref():
        return

    return lex.ent()

# Given a function call ref and a zero-based index for the argument (what is
# given to the parameter), gets an array of lexemes that make the argument
def nthParamOfCall(call_ref: Ref, n: int, lookup_ents: bool = True) -> list[Lexeme]:
    lex = call_ref.file().lexer(lookup_ents).lexeme(call_ref.line(), call_ref.column())

    lexemes = []

    parenCount = 0
    angleCount = 0

    i = 0

    while lex:
        lex = lex.next(True, True)
        if not lex:
            break

        if lex.text() == '(':
            parenCount += 1
            if parenCount == 1:
                continue

        if lex.text() == ')':
            parenCount -= 1
            if parenCount == 0:
                break

        if lex.text() == '<':
            angleCount += 1

        if lex.text() == '>':
            angleCount -= 1

        if parenCount == 0:
            break

        if lex.text() == ',' and parenCount == 1 and angleCount == 0:
            i += 1
            continue

        if parenCount == 1 and i == n:
            lexemes.append(lex)

    return lexemes



# Regular Expressions

# Given a file/function ent and a pattern, see if the pattern is found
def searchContents(ent, pattern, flags=0, removeComments=True, removeStrings=True):
    contents = ent.contents()

    if contents == None:
        return False

    # Optionally remove block comments then line comments
    if removeComments:
        contents = re.sub(r'\/\/.*?\n', '', re.sub(r'\/\*[\s\S]*?\*\/', '', contents))

    # Optionally remove all string literals
    if removeStrings:
        contents = removeStringLiterals(contents)

    # Check if pattern exists
    return True if re.search(pattern, contents, flags) else False



# Strings

# Given a string, remove all double-quote and single-quote string literals
def removeStringLiterals(string):
    result = []

    quoteType = 0
    consecutiveBackslashes = 0
    for char in string:

        # Non-quotes: if not in a literal, add the character
        if char != "'" and char != '"':
            if not quoteType:
                result.append(char)
            if char == '\\':
                consecutiveBackslashes += 1
            continue

        # Quotes: if preceded by an even number of \ process the quote
        evenNumberOfConsecutiveBackslashes = consecutiveBackslashes & 1 == 0
        if evenNumberOfConsecutiveBackslashes:
            if char == "'":
                if quoteType == 0:
                    quoteType = 1
                elif quoteType == 1:
                    quoteType = 0
            else:
                if quoteType == 0:
                    quoteType = 2
                elif quoteType == 2:
                    quoteType = 0
        consecutiveBackslashes = 0

    return ''.join(result)



# Types

# Get the type of a value after an explicit conversion
def typeOfCast(lex):
    lex = lex.previous(True, True)
    if lex.text() != ')':
        return

    lex = lex.previous(True, True)
    if lex.token() != 'Keyword':
        return

    return lex.text()

# Get the type of value after an assignment
def typeOfAssignment(lex):
    lex = lex.previous(True, True)
    if lex.text() != '=':
        return

    lex = lex.previous(True, True)
    if lex.token() != 'Identifier' or not lex.ent():
        return

    return lex.ent().type()

# Get the type of a given function/method parameter index number
def typeOfParameter(function, i):
    parameters = function.ents('Define', 'Parameter')
    if len(parameters) > i:
        return parameters[i].type()



#####
# C #
#####



# Expressions

C_OPERATOR_PRECEDENCES = {'::': 1, '.': 2, '->': 2, '++': 2, '--': 2, '!': 3,
'~': 3, 'sizeof': 3, '_Alignof': 3, 'co_await': 3, 'new': 3, 'new[]': 3,
'delete': 3, 'delete[]': 3, '.*': 4, '->*': 4, '*': 5, '/': 5, '%': 5, '+': 6,
'-': 6, '<<': 7, '>>': 7, '<=>': 8, '<': 9, '<=': 9, '>': 9, '>=': 9, '==': 10,
'!=': 10, '&': 11, '^': 12, '|': 13, '&&': 14, '||': 15, '?': 16, ':': 16,
'throw': 16, 'co_yield': 16, '=': 16, '+=': 16, '-=': 16, '*=': 16, '/=': 16,
'%=': 16, '<<=': 16, '>>=': 16, '&=': 16, '^=': 16, '|=': 16, ',': 17}

# Given a lexeme and a boolean for left/right, get the operand as lexemes
def cGetOperand(lex, left):
    operand = []

    # Stop if not a supported operator
    if lex.token() != 'Operator' or lex.text() not in C_OPERATOR_PRECEDENCES:
        return operand

    # Info about operator
    originalPrecedence = C_OPERATOR_PRECEDENCES[lex.text()]
    lessOrGreater = lex.text() in {'<', '>'}

    # Count braces, brackets, and parentheses
    openBrace = 0
    openBrack = 0
    openParen = 0
    leftSymbol = -1 if left else 1

    # Parse operand
    while lex:
        # Move left/right
        if left:
            lex = lex.previous(True, True)
        else:
            lex = lex.next(True, True)
        if not lex:
            break

        # Decide what to do with different lexemes
        text = lex.text()
        if text == '{':
            openBrace += leftSymbol
        elif text == '}':
            openBrace -= leftSymbol
        elif text == '[':
            openBrack += leftSymbol
        elif text == ']':
            openBrack -= leftSymbol
        elif text == '(':
            openParen += leftSymbol
        elif text == ')':
            openParen -= leftSymbol
        # Stop if at the edge of the operand by an operator
        elif text in C_OPERATOR_PRECEDENCES:
            if openBrace < 1 and openBrack < 1 and openParen < 1:
                thisPrecedence = C_OPERATOR_PRECEDENCES[text]
                if thisPrecedence >= originalPrecedence:
                    break
        # Stop if at the edge of the operand by some other token kind
        elif lex.token() not in {'Identifier', 'Literal'}:
            break

        # Stop if at the edge of the operand by braces, brackets, or parentheses
        if openBrace < 0 or openBrack < 0 or openParen < 0:
            break

        operand.append(lex)

    # Return nothing if there is actually no operand
    operandExists = False
    for lex in operand:
        token = lex.token()
        if token == 'Identifier':
            ent = lex.ent()
            if ent and ent.kind().check('Object, Parameter'):
                operandExists = True
                break
        elif token == 'Literal':
            operandExists = True
            break
    if not operandExists:
        return []

    # Fix the order
    if left:
        operand.reverse()

    return operand



# Numbers

C_INTEGER_BASES = (
    (r'-?0b([0|1]+)[u|l]?$',    2),
    (r'-?0([0-7]+)[u|l]?$',     8),
    (r'-?(\d+)[u|l]?$',         10),
    (r'-?0x([\d|a-f]+)[u|l]?$', 16),
)

# Try to parse an integer literal
def cParseIntLiteral(string: str) -> int | None:
    for pattern, base in C_INTEGER_BASES:
        match = re.match(pattern, string, re.IGNORECASE)
        if match:
            try:
                return int(match[1], base)
            except:
                return None

# Given a declaration reference, return the width of a bit-field object or None
def cGetBitFieldWidth(ref):
    # Only objects may be bit fields
    if not ref.ent().kind().check('Object'):
        return

    # Get lexeme
    lexer = ref.file().lexer(False)
    if not lexer:
        return
    lex = lexer.lexeme(ref.line(), ref.column())
    if not lex:
        return

    # Match :
    lex = lex.next(True, True)
    if not lex or lex.text() != ':':
        return

    # Match anything
    lex = lex.next(True, True)
    if not lex:
        return

    return cParseIntLiteral(lex.text())



# Files

# Given an ent and a set, build the translation unit of includes of a file
def buildTranslationUnit(fileEnt, translationUnitSet):
    translationUnitSet.add(fileEnt)
    for includeRef in fileEnt.filerefs('Include'):
        includeEnt = includeRef.ent()
        if includeEnt in translationUnitSet:
            continue
        buildTranslationUnit(includeEnt, translationUnitSet)


#######
# C++ #
#######



# Classes

# Given an entity, see if it is a constructor, even if it's in a namespace
def isConstructor(ent):
    name = re.escape(ent.name())
    if re.search(f'\\b{name}::{name}$', ent.longname()):
        return True
    return False


# Libraries


# Whether the entity is a pointer to a FILE
def isFilePointer(ent: Ent) -> bool:
    t = ent.freetext('UnderlyingType')
    t = re.sub(r'\b(const|restrict|volatile)\s*', '', t)
    return t in ('_iobuf *', '_IO_FILE *', '__sFILE *')


# Given a long string like "std::anything::cout" and a string like "cout"
# Returns true if the ent longname is something like "std::cout" or "std::anything::cout"
def stringIsStd(wholeString, lastPart):
    nameParts = wholeString.split('::')
    return nameParts[0] == 'std' and nameParts[-1] == lastPart



########
# Java #
########



# Classes & Interfaces

# Given an ent and id, see there's an ancestor with the given id
def javaAncestorOfIdExists(ent, id):
    # Base case: object already cached
    if not ent:
        return False

    # Base case: correct ent
    if ent.id() == id:
        return True

    # Recurse
    for ancestor in ent.ents('Implement, Extend'):
        if javaAncestorOfIdExists(ancestor, id):
            return True

    # No correct ent found
    return False

# Given an ent, regular expression, and a dictionary, see there's an ancestor
# with the given longname
def javaAncestorOfLongnameExists(ent, regex, translationUnitCache):
    # Key for the entity in the dictionary
    key = ent.uniquename()

    # Base case: object already cached
    if ent in translationUnitCache:
        return translationUnitCache[key]

    # Base case: no ent
    if not ent:
        return False

    # Base case: correct ent found
    if re.search(regex, ent.longname()):
        translationUnitCache[key] = True
        return True

    # Recurse
    for ancestor in ent.ents('Implement, Extend'):
        if javaAncestorOfLongnameExists(ancestor, regex, translationUnitCache):
            translationUnitCache[key] = True
            return True

    # No correct ent found
    translationUnitCache[key] = False
    return False



# Immutability

# Given a class entity, see if 'this' escapes the constructor
def javaThisEscapesConstructor(ent, check=None, errors=None):
    classEnt = ent
    if errors:
        ERR1, ERR2, ERR3, ERR4, ERR5 = errors

    # Constructors
    for constructorRef in classEnt.refs('Define', 'Constructor'):
        constructorEnt = constructorRef.ent()

        # Find violations with anonymous classes in constructors
        for anonRef in constructorEnt.refs('Define', 'Class Type Anonymous'):
            anonEnt = anonRef.ent()

            # Skip if not defined in a nonprivate constructor
            if not anonEnt.ents('Definein', '~Private Constructor'):
                continue

            if not check:
                return True
            check.violation(constructorEnt, anonRef.file(), anonRef.line(), anonRef.column(), ERR1)


        # Lexer variables
        end = constructorEnt.ref('End')
        if not end:
            continue
        lex = classEnt.lexer(True).lexeme(constructorRef.line(), constructorRef.column())
        lastCodeLex1 = classEnt.lexer(True).lexeme(end.line(), end.column())
        if not lastCodeLex1:
            continue
        lastCodeLex = lastCodeLex1.previous(True, True)
        lastCodeLine, lastCodeColumn = lastCodeLex.line_begin(), lastCodeLex.column_begin()

        # Constructor lexemes
        while lex and lexemeBefore(lex, lastCodeLine, lastCodeColumn):
            text = lex.text()

            if text != 'this':
                lex = lex.next(True, True)
                continue

            semicolonNext = True
            nextLex = lex.next(True, True)
            if not nextLex or nextLex.text() != ';':
                semicolonNext = False

            # 'this' used in assignment
            if semicolonNext:
                variables = entsAssigned(lex)
                for var in variables:
                    # Violation if the statement isn't last
                    nextLex = lex.next(True, True)
                    if not lexemeEquals(nextLex, lastCodeLine, lastCodeColumn):
                        if not check:
                            return True
                        check.violation(constructorEnt, constructorRef.file(), lex.line_begin(), lex.column_begin(), ERR2)

                    # Violation if the variable assigned is nonvolatile, nonfinal, & public
                    if var.kind().check('Public ~Final') and 'volatile ' not in var.type():
                        if not check:
                            return True
                        check.violation(constructorEnt, constructorRef.file(), lex.line_begin(), lex.column_begin(), ERR3)

            # 'this' used as parameter
            methodOrClass = functionCalled(lex)
            if methodOrClass:
                # Method
                if methodOrClass.kind().check('~Unknown Method'):
                    # Method is of a superclass or superinterface
                    method = methodOrClass
                    methodParentId = method.parent().id()
                    if javaAncestorOfIdExists(classEnt, methodParentId):
                        if not check:
                            return True
                        check.violation(constructorEnt, constructorRef.file(), lex.line_begin(), lex.column_begin(), ERR4)

                # Class
                elif methodOrClass.kind().check('Class'):
                    # Thread
                    classEnt = methodOrClass
                    if classEnt.longname() != 'java.lang.Thread':
                        lex = lex.next(True, True)
                        continue

                    # Go back until 'new'
                    prevLex = lex.previous(True, True)
                    while prevLex and prevLex.text() != 'new':
                        prevLex = prevLex.previous(True, True)

                    # For each usage of each Thread object assigned with 'this'
                    threads = entsAssigned(prevLex)
                    for thread in threads:
                        for useby in thread.refs('Useby Deref Partial', 'Constructor'):
                            useLex = useby.file().lexer(True).lexeme(useby.line(), useby.column())
                            # Match .
                            nextLex = useLex.next(True, True)
                            if nextLex.text() != '.':
                                lex = lex.next(True, True)
                                continue
                            # Match start
                            nextLex = nextLex.next(True, True)
                            if nextLex.text() != 'start':
                                lex = lex.next(True, True)
                                continue
                            if not check:
                                return True
                            check.violation(constructorEnt, constructorRef.file(), lex.line_begin(), lex.column_begin(), ERR5)

            lex = lex.next(True, True)

    return False

# Given an entity class, see if it is immutable (defined by SEI CERT)
# https://wiki.sei.cmu.edu/confluence/display/java/Rule+BB.+Glossary
def javaImmutableClass(ent):
    # Exception for String
    if ent.longname() == 'java.lang.String':
        return True

    # Its state cannot be modified after construction (too vague)

    # All its fields are final
    if ent.ents('Define', '~Final Variable'):
        return False

    # It is properly constructed (the this reference does not escape during construction)
    if javaThisEscapesConstructor(ent):
        return False

    return True




# Numbers

JAVA_NUMBER_SYSTEMS = {
    'bin':    r'-?0b[0|1]+l?$',
    'dec':    r'-?\d+l?$',
    'hex':    r'-?0x[\d|a-f]+l?$',
}
JAVA_NUMBER_TYPES = {
    'int':    r'-?(0b[0|1]+|\d+|0x[\d|a-f]+)$',
    'long':   r'-?(0b[0|1]+|\d+|0x[\d|a-f]+)l$',
    'float':  r'-?(\d+\.\d*|\d*\.\d+|\d+)f$',
    'double': r'-?(\d+\.\d*d?|\d*\.\d+d?|\d+d)$',
}

# Remove separators & suffix from a Java number literal
def javaSimplifyNumberLiteral(text):
    # Remove separators & long suffix
    text = re.sub('_|l|L', '', text)
    # If it's not hex, remove float & double suffix
    if not re.match(JAVA_NUMBER_SYSTEMS['hex'], text, re.IGNORECASE):
        text = re.sub('f|F', '', text)
        text = re.sub('d|D', '', text)
    return text

# Get type of a Java number literal
def javaLiteralNumberType(text):
    # Remove separators
    text = re.sub('_', '', text)
    # Get type
    for numType in JAVA_NUMBER_TYPES.keys():
        pattern = JAVA_NUMBER_TYPES[numType]
        if re.match(pattern, text, re.IGNORECASE):
            return numType

# Convert a Java number literal from a string into a number
def javaLiteralToNumber(text):
    text = javaSimplifyNumberLiteral(text)
    # Binary
    if re.match(JAVA_NUMBER_SYSTEMS['bin'], text, re.IGNORECASE):
        return int(text, 2)
    # Decimal
    if re.match(JAVA_NUMBER_SYSTEMS['dec'], text):
        return int(text)
    # Hexadecimal
    if re.match(JAVA_NUMBER_SYSTEMS['hex'], text, re.IGNORECASE):
        return int(text, 16)
    # Float with f suffix removed or double
    if re.match(JAVA_NUMBER_TYPES['double'], text):
        return float(text)

# Get the value of an identifier or literal, if it has only 1 value
def javaValueOfLexeme(lex):
    # Identifier is only set/modified once
    if lex.token() == 'Identifier' and lex.ent() and \
    len(lex.ent().refs('Setby, Modifyby')) == 1 and lex.ent().value() != None:
        return javaLiteralToNumber(lex.ent().value())
    # Literal
    elif lex.token() == 'Literal':
        return javaLiteralToNumber(lex.text())



# Types

# Get the type of a lexeme if it's a literal, identifier, or method call
def javaTypeOfLexeme(lex):
    # Identifier
    if lex.ent():
        return lex.ent().type()
    # Literal
    if lex.token() == 'Literal':
        return javaLiteralNumberType(lex.text())

# Get the type of a lexeme after conversion by parameterization
def javaTypeOfParameterization(lex):
    l = lex.previous(True, True)
    if l.text() not in {'(', ','}:
        return

    r = lex.next(True, True)
    if not r or r.text() not in {')', ','}:
        return

    # Go back until the end of the call/statement
    commas = 0
    while lex and lex.text() not in {'(', ';', '{'}:
        if lex.text() == ',':
            commas += 1
        lex = lex.previous(True, True)

    # Go back until method
    lex = lex.previous(True, True)
    if not lex or lex.token() != 'Identifier' or not lex.ref():
        return

    # See if the identifier is being created or called
    ent = lex.ent()
    if not ent.kind().check('Method, Class Type'):
        return

    # Get parameter type of method
    if ent.kind().check('Method'):
        return typeOfParameter(ent, commas)
    # Get parameter type of class constructor
    else:
        constructors = ent.ents('Define', 'Constructor')
        if not constructors:
            return
        constructor = constructors[0]
        return typeOfParameter(constructor, commas)
