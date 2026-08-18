import functools
import re

from understand import CFGraph, CFNode, Ent, Lexeme, Ref



###########
# General #
###########



# Control Flow / Expressions

# Find corresponding closing curly brace or parenthesis of given opening curly
# brace or parenthesis
def findClose(lexeme: Lexeme | None) -> Lexeme | None:
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
def findOpen(lexeme: Lexeme | None) -> Lexeme | None:
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
def lexemeBetweenNodes(node1: CFNode, lexeme: Lexeme, node2: CFNode) -> bool:
    # Make sure both nodes have position information
    if node1.line_begin() == None or node2.line_begin() == None:
        return False

    # Make sure the order is node1, lexeme
    if lexeme.line_begin() < node1.line_begin(): # type: ignore
        return False
    if lexeme.line_begin() == node1.line_begin() and lexeme.column_begin() <= node1.column_begin(): # type: ignore
        return False

    # Make sure the order is lexeme, node2
    if lexeme.line_begin() > node2.line_begin(): # type: ignore
        return False
    if lexeme.line_begin() == node2.line_begin() and lexeme.column_begin() >= node2.column_begin(): # type: ignore
        return False

    return True


# Given a CFGraph, get a dictionary:
    # Key: a CFNode
    # Value: the next sibling CFNode
def nextSiblingDictionary(cfg: CFGraph | None) -> dict[CFNode, CFNode]:
    result: dict[CFNode, CFNode] = {}

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

    # Check if refB is in the same node as refA — only count it if refB is
    # actually after refA by line/column position within the node
    if refInNode(refB, nodeA) and refAfterRef(refA, refB):
        return True

    # Search child nodes reachable from nodeA
    seen: set[CFNode] = {nodeA}
    stack: list[CFNode] = list(nodeA.children())
    for child in stack:
        seen.add(child)
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


# Whether refB comes strictly after refA by line/column
def refAfterRef(refA: Ref, refB: Ref) -> bool:
    if refB.line() > refA.line():
        return True
    if refB.line() == refA.line() and refB.column() > refA.column():
        return True
    return False


# Given a node/lexeme, ref, and another node/lexeme
# See if the order of the parameters is really the order
def refBetweenNodes(node1: CFNode, ref: Ref, node2: CFNode) -> bool:
    # Make sure both nodes have position information
    if node1.line_begin() == None or node2.line_begin() == None:
        return False

    # Make sure the order is node1, ref
    if ref.line() < node1.line_begin(): # type: ignore
        return False
    if ref.line() == node1.line_begin() and ref.column() <= node1.column_begin(): # type: ignore
        return False

    # Make sure the order is ref, node2
    if ref.line() > node2.line_begin(): # type: ignore
        return False
    if ref.line() == node2.line_begin() and ref.column() >= node2.column_begin(): # type: ignore
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
    if refLine < nodeLineBegin or refLine > nodeLineEnd: # type: ignore
        return False
    if refLine == nodeLineBegin and refColumn < node.column_begin(): # type: ignore
        return False
    if refLine == nodeLineEnd and refColumn > node.column_end(): # type: ignore
        return False
    return True


# References

# Given 3 refs
# See if the order of the refs is really the order
def refBetweenRefs(ref1: Ref, ref2: Ref, ref3: Ref) -> bool:
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
def refComparatorSameFileFn(a: Ref, b: Ref) -> int:
    return a.line() - b.line() or a.column() - b.column()
refComparatorSameFile = functools.cmp_to_key(refComparatorSameFileFn)



# Lexemes

# Given 2 lexemes, see if the first is before the second
def lexemeBeforeLexeme(lex1: Lexeme, lex2: Lexeme) -> bool:
    return lex1.line_begin() < lex2.line_begin() or \
        (lex1.line_begin() == lex2.line_begin() and lex1.column_begin() < lex2.column_begin())

# Given a lexeme & position, see if it is before the position
def lexemeBefore(lex: Lexeme, line: int, column: int) -> bool:
    return lex.line_begin() < line or \
        (lex.line_begin() == line and lex.column_begin() < column)

# Given a lexeme & position, see if it is at the position
def lexemeEquals(lex: Lexeme, line: int, column: int) -> bool:
    return lex.line_begin() == line and lex.column_begin() == column

# Given a lexeme & position, see if it is after the position
def lexemeAfter(lex: Lexeme, line: int, column: int) -> bool:
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
def removeStringLiterals(string: str) -> str:
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
def typeOfParameter(function: Ent, i: int) -> str | None:
    parameters = function.ents('Define', 'Parameter')
    if len(parameters) > i:
        return parameters[i].type()
    return None



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
    return None

# Given a declaration reference, return the width of a bit-field object or None
def cGetBitFieldWidth(ref: Ref) -> int | None:
    # Only objects may be bit fields
    if not ref.ent().kind().check('Object'):
        return None

    # Get lexeme
    lexer = ref.file().lexer(False)
    if not lexer:
        return None
    lex = lexer.lexeme(ref.line(), ref.column())
    if not lex:
        return None

    # Match :
    lex = lex.next(True, True)
    if not lex or lex.text() != ':':
        return None

    # Match anything
    lex = lex.next(True, True)
    if not lex:
        return None

    return cParseIntLiteral(lex.text())



# Assignments

# Operators that perform an assignment as defined in the Glossary
C_ASSIGNMENT_OPERATORS = {'=', '+=', '-=', '*=', '/=', '%=', '&=', '|=', '^=',
                          '<<=', '>>='}

# Yield each assignment in a file as (operator, target lexemes, value lexemes,
# target entity). An initializer list assigns each of its elements rather than
# one value, and an enumerator is given its value rather than assigned one, so
# neither is yielded.
def cAssignments(file: Ent):
    lexer = file.lexer()
    if not lexer:
        return

    for lexeme in lexer.lexemes():
        if lexeme.token() != 'Operator' or lexeme.text() not in C_ASSIGNMENT_OPERATORS:
            continue

        # Code the preprocessor excludes is not part of the translation unit
        if lexeme.inactive():
            continue

        target = cAssignmentTarget(lexeme)
        value = cAssignedExpression(lexeme)
        if not target or not value or value[0].text() == '{':
            continue

        ent = cAssignedEntity(target)
        if ent and ent.kind().check('Enumerator'):
            continue

        yield (lexeme, target, value, ent)

# The object being assigned to. Two adjacent primaries end the target, as does
# anything naming a type, which keeps a declaration's type and an enclosing
# condition out of it.
def cAssignmentTarget(lexeme: Lexeme) -> list[Lexeme]:
    lexemes = []
    depth = 0
    group = 0
    declaration = False
    lexeme = lexeme.previous(True, True)
    while lexeme:
        text = lexeme.text()
        token = lexeme.token()
        primary = token in ('Identifier', 'Literal') or text in (')', ']')
        if lexemes and primary and (lexemes[-1].token() in ('Identifier', 'Literal')):
            break
        # A keyword or type name only appears before the object being declared,
        # never within an lvalue
        if lexemes and depth == 0 and (token == 'Keyword' or (lexeme.ent()
                and lexeme.ent().kind().check('Typedef, Type'))):
            declaration = True
            break
        if text in (')', ']'):
            if depth == 0:
                group = len(lexemes)
            depth += 1
        elif text in ('(', '['):
            if depth == 0:
                break
            depth -= 1
            # A group belongs to the target only as a call or a subscript, so
            # anything else before it encloses the statement rather than the
            # object being assigned to
            previous = lexeme.previous(True, True)
            if depth == 0 and not (previous and (previous.token() == 'Identifier'
                    or previous.text() in (')', ']'))):
                lexemes = lexemes[:group]
                break
        elif depth == 0:
            # A colon ends a label or a case, neither of which is an lvalue
            if text in (';', ',', ':', '{', '}') or token not in ('Identifier', 'Literal', 'Operator', 'Keyword'):
                break
        lexemes.append(lexeme)
        lexeme = lexeme.previous(True, True)

    # An unbalanced group means the walk stopped inside an enclosing expression
    # rather than on a whole lvalue
    if depth:
        return []

    lexemes.reverse()

    # In a declaration a leading * declares a pointer, it does not dereference
    while declaration and lexemes and lexemes[0].text() in ('*', '&'):
        lexemes.pop(0)

    return lexemes

# The whole expression being assigned, which unlike an operand includes any
# conditional expression
def cAssignedExpression(lexeme: Lexeme) -> list[Lexeme]:
    lexemes = []
    depth = 0
    lexeme = lexeme.next(True, True)
    while lexeme:
        text = lexeme.text()
        if text in ('(', '[', '{'):
            depth += 1
        elif text in (')', ']', '}'):
            if depth == 0:
                break
            depth -= 1
        elif depth == 0 and text in (';', ','):
            break
        lexemes.append(lexeme)
        lexeme = lexeme.next(True, True)
    return lexemes

# The lexemes of an expression that no group encloses, so that call arguments
# and subscripts are left out
def cOuterLexemes(lexemes: list[Lexeme]) -> list[Lexeme]:
    outer = []
    depth = 0
    for lexeme in lexemes:
        text = lexeme.text()
        if text in ('(', '['):
            depth += 1
        elif text in (')', ']'):
            depth -= 1
        elif depth == 0:
            outer.append(lexeme)
    return outer

# The entity being assigned to, ignoring anything inside a subscript
def cAssignedEntity(lexemes: list[Lexeme]) -> Ent | None:
    for lexeme in reversed(cOuterLexemes(lexemes)):
        if lexeme.ent():
            return lexeme.ent()
    return None



# Essential types

# MISRA essential type categories
C_ESSENTIAL_BOOLEAN = 'boolean'
C_ESSENTIAL_CHARACTER = 'character'
C_ESSENTIAL_ENUM = 'enum'
C_ESSENTIAL_SIGNED = 'signed'
C_ESSENTIAL_UNSIGNED = 'unsigned'
C_ESSENTIAL_FLOATING = 'floating'

# Not an essential type. A pointer is outside the model, but an expression has to
# carry that rather than lose it, so that a rule does not read the other operand
# as the type of the whole
C_ESSENTIAL_POINTER = 'pointer'

# Type qualifiers and storage class specifiers that do not affect essential type
C_TYPE_NOISE = re.compile(r'\b(const|volatile|restrict|_Atomic|static|register|extern|inline|_Noreturn)\b')

# Fixed width typedefs, whose width is exact rather than a minimum. The least
# and fast typedefs are excluded because they are only minimums, so they are
# resolved through the typedef they name instead
C_FIXED_WIDTH_TYPEDEF = re.compile(r'^_{0,2}(u?)int(8|16|32|64)_t$')

# Standard library types whose width the source does not show, used only when
# the typedef cannot be followed
C_LIBRARY_TYPES = {
    'size_t': (C_ESSENTIAL_UNSIGNED, 32),
    'uintmax_t': (C_ESSENTIAL_UNSIGNED, 64),
    'uintptr_t': (C_ESSENTIAL_UNSIGNED, 32),
    'ptrdiff_t': (C_ESSENTIAL_SIGNED, 32),
    'intmax_t': (C_ESSENTIAL_SIGNED, 64),
    'intptr_t': (C_ESSENTIAL_SIGNED, 32),
    'char16_t': (C_ESSENTIAL_UNSIGNED, 16),
    'char32_t': (C_ESSENTIAL_UNSIGNED, 32),
}

# Tokens of a constant. Character and string constants are lexed as String
C_LITERAL_TOKENS = ('Literal', 'String')

# Binary operators whose result is a composite expression. Complement and the
# conditional operator are also composite, but neither takes a pair of operands
C_COMPOSITE_OPERATORS = {'+', '-', '*', '/', '%', '&', '|', '^', '<<', '>>'}

# Operators whose result is essentially Boolean
C_BOOLEAN_OPERATORS = {'==', '!=', '<', '>', '<=', '>=', '&&', '||', '!'}

# Operators whose result takes the essential type of the left hand operand
C_LEFT_TYPED_OPERATORS = {'<<', '>>'}

# Given the text of a C type, return its essential type as
# (category, width) where width is in bits, or None if it cannot be classified.
# Widths are the minimums guaranteed by the C standard, so a target may be
# wider; fixed width typedefs are exact. Pass the entity declared with the type
# so unresolved typedefs can be followed.
def cEssentialType(typeText: str | None, ent: Ent | None = None, depth: int = 0) -> tuple[str, int | None] | None:
    if not typeText or depth > 8:
        return None

    text = C_TYPE_NOISE.sub('', str(typeText))
    text = re.sub(r'\[[^\]]*\]', '', text)
    text = ' '.join(text.split())

    # Pointers and functions are not arithmetic types
    if '*' in text:
        return (C_ESSENTIAL_POINTER, None)
    if '(' in text:
        return None

    if text.startswith('enum'):
        return (C_ESSENTIAL_ENUM, None)
    if text.startswith('struct') or text.startswith('union'):
        return None

    essential = cEssentialTypeOfBase(text)
    if essential:
        return essential

    # Not a base type, so follow the typedef it names
    if ent:
        target = cTypedefTarget(ent, text)
        if target:
            return cEssentialType(target[0], target[1], depth + 1)

    return C_LIBRARY_TYPES.get(text)

# The next link in a typedef chain as (type text, entity), or None if there is
# none. A cast or a declaration may name the type itself, in which case its own
# definition is the next link.
def cTypedefTarget(ent: Ent, text: str) -> tuple[str, Ent] | None:
    if ent.kind().check('Typedef, Type') and ent.name() == text:
        return (str(ent.type()), ent)

    typedRef = ent.ref('Typed')
    if typedRef and typedRef.ent() != ent:
        return (str(typedRef.ent().type()), typedRef.ent())

    return None

# The enumeration an entity belongs to, or None. An object reaches it through
# the typedefs its type names, an enumerator through the enumeration defining
# it. Two entities of one enumeration share this entity, which is what
# distinguishes enumerated types from each other.
def cEnumType(ent: Ent | None, depth: int = 0) -> Ent | None:
    if not ent or depth > 8:
        return None

    if ent.kind().check('Enumerator'):
        return ent.parent()
    if ent.kind().check('Enum'):
        return ent

    typedRef = ent.ref('Typed')
    if typedRef and typedRef.ent() != ent:
        return cEnumType(typedRef.ent(), depth + 1)

    return None

# Classify a type that names no typedef, or None if it is not a base type
def cEssentialTypeOfBase(text: str) -> tuple[str, int | None] | None:
    fixed = C_FIXED_WIDTH_TYPEDEF.match(text)
    if fixed:
        signedness = C_ESSENTIAL_UNSIGNED if fixed[1] else C_ESSENTIAL_SIGNED
        return (signedness, int(fixed[2]))

    words = set(text.split())
    if not words:
        return None
    if words - {'signed', 'unsigned', 'char', 'short', 'int', 'long', 'float',
                'double', 'bool', '_Bool', '_Complex'}:
        return None

    if 'double' in words or 'float' in words:
        if '_Complex' in words:
            return None
        return (C_ESSENTIAL_FLOATING, 64 if 'double' in words else 32)
    if 'bool' in words or '_Bool' in words:
        return (C_ESSENTIAL_BOOLEAN, 1)

    # Plain char is essentially character; signed and unsigned char are not
    if 'char' in words:
        if 'signed' in words:
            return (C_ESSENTIAL_SIGNED, 8)
        if 'unsigned' in words:
            return (C_ESSENTIAL_UNSIGNED, 8)
        return (C_ESSENTIAL_CHARACTER, 8)

    if 'short' in words:
        width = 16
    elif text.count('long') > 1:
        width = 64
    elif 'long' in words:
        width = 32
    elif 'int' in words or 'signed' in words or 'unsigned' in words:
        width = 32
    else:
        return None

    signedness = C_ESSENTIAL_UNSIGNED if 'unsigned' in words else C_ESSENTIAL_SIGNED
    return (signedness, width)

# Return the essential type of an integer or character literal, or None. The
# result is flagged as constant, since the model gives a constant the lowest rank
# able to represent its value rather than the rank of its standard type
def cEssentialTypeOfLiteral(text: str) -> tuple[str, int | None, bool] | None:
    if not text:
        return None

    # Character constants are essentially character, string literals are not
    # arithmetic operands
    if re.match(r"^(?:L|u8?|U)?'", text):
        return (C_ESSENTIAL_CHARACTER, 8, True)
    if re.match(r'^(?:L|u8?|U)?"', text):
        return None

    lower = text.lower()
    # A hexadecimal exponent is p, since e is one of its digits
    hexadecimal = lower.startswith('0x')
    exponent = r'p[+\-]?[0-9]' if hexadecimal else r'[0-9]e[+\-]?[0-9]'
    if '.' in text or re.search(exponent, lower):
        return (C_ESSENTIAL_FLOATING, 32 if lower.endswith('f') else 64, True)

    if not re.match(r"^(?:0[xXbB])?[0-9a-fA-F']+[uUlL]*$", text):
        return None

    suffix = re.search(r'[uUlL]*$', text)[0].lower()
    signedness = C_ESSENTIAL_UNSIGNED if 'u' in suffix else C_ESSENTIAL_SIGNED

    # The essential type of an integer constant is the narrowest type of that
    # signedness able to represent its value
    value = cParseIntLiteral(text)

    # An unsuffixed constant that is not decimal takes an unsigned type when its
    # value does not fit the signed type of the same rank
    if (signedness == C_ESSENTIAL_SIGNED and value is not None
            and value >= (1 << 31) and len(text) > 1 and text[0] == '0'):
        signedness = C_ESSENTIAL_UNSIGNED

    # An unreadable value falls back to the rank the suffix gives it, which for
    # no suffix is int
    if value is None:
        return (signedness, 64 if 'll' in suffix else 32, True)

    for width in (8, 16, 32, 64):
        limit = (1 << width) if signedness == C_ESSENTIAL_UNSIGNED else (1 << (width - 1))
        if value < limit:
            return (signedness, width, True)
    return (signedness, 64, True)

# An expression's lexemes without whitespace or comments, and with any
# parentheses enclosing the whole of it removed
def cNormalizeExpression(lexemes: list[Lexeme]) -> list[Lexeme]:
    lexemes = [lex for lex in lexemes if lex.token() not in ('Whitespace', 'Newline', 'Comment')]
    while len(lexemes) > 2 and lexemes[0].text() == '(' and cIsMatchedPair(lexemes):
        lexemes = lexemes[1:-1]
    return lexemes

# Fold an integer constant expression to its value, or None if it is not one
def cConstantValue(lexemes: list[Lexeme], depth: int = 0) -> int | None:
    lexemes = cNormalizeExpression(lexemes)
    if not lexemes or depth > 16:
        return None

    if len(lexemes) == 1:
        if lexemes[0].token() in C_LITERAL_TOKENS:
            return cParseIntLiteral(lexemes[0].text())
        # An object-like macro holding a constant behaves as that constant
        ent = lexemes[0].ent()
        if ent and ent.kind().check('Macro') and ent.value():
            return cParseIntLiteral(str(ent.value()))
        return None

    operator = cTopLevelOperator(lexemes)

    # No binary operator, so the expression may be a unary sign or complement
    if operator is None:
        if lexemes[0].text() not in ('-', '+', '~'):
            return None
        value = cConstantValue(lexemes[1:], depth + 1)
        if value is None:
            return None
        return {'-': -value, '+': value, '~': ~value}[lexemes[0].text()]

    text = lexemes[operator].text()
    left = cConstantValue(lexemes[:operator], depth + 1)
    right = cConstantValue(lexemes[operator + 1:], depth + 1)
    if left is None or right is None:
        return None

    try:
        if text == '+':
            return left + right
        if text == '-':
            return left - right
        if text == '*':
            return left * right
        if text == '/':
            return int(left / right) if right else None
        if text == '%':
            return int(left % right) if right else None
        if text == '<<':
            return left << right if 0 <= right < 64 else None
        if text == '>>':
            return left >> right if 0 <= right < 64 else None
        if text == '&':
            return left & right
        if text == '|':
            return left | right
        if text == '^':
            return left ^ right
    except (ValueError, OverflowError):
        return None
    return None

# Return the essential type of the entity a lexeme resolves to, or None
def cEssentialTypeOfEnt(ent: Ent | None) -> tuple[str, int | None] | None:
    if not ent:
        return None

    if ent.kind().check('Enumerator'):
        # An enumeration named by a typedef is an enumerated type like any other
        parent = ent.parent()
        if parent and (not parent.kind().check('Unnamed') or parent.ref('Typedby')):
            return (C_ESSENTIAL_ENUM, None)
        # A constant of an unnamed enumeration is essentially signed, and like
        # an integer constant takes the width of the value it holds. A value
        # that is not a literal falls back to the rank of int.
        value = cEssentialTypeOfLiteral(str(ent.value())) if ent.value() else None
        return (C_ESSENTIAL_SIGNED, value[1] if value else 32, True)

    # An object-like macro holding a constant behaves as that constant
    if ent.kind().check('Macro'):
        return cEssentialTypeOfLiteral(str(ent.value())) if ent.value() else None

    return cEssentialType(ent.type(), ent)

# The essential type an entity has once dereferenced, or None if it does not
# point to an arithmetic type. The pointer itself is outside the model, but the
# object it designates is not.
def cEssentialTypeOfPointee(ent: Ent | None) -> tuple[str, int | None] | None:
    if not ent or not ent.type():
        return None

    text = str(ent.type())
    if '*' not in text:
        return None

    return cEssentialType(text[:text.rindex('*')], ent)

# Given the lexemes of a C expression, return its essential type as
# (category, width), or None if it cannot be determined. A leading cast gives
# the expression the essential type of the cast, per the essential type model.
def cEssentialTypeOfExpression(lexemes: list[Lexeme], depth: int = 0) -> tuple[str, int | None] | None:
    lexemes = cNormalizeExpression(lexemes)
    if not lexemes or depth > 16:
        return None

    cast = cLeadingCast(lexemes)
    if cast:
        return cast

    # A conditional expression takes the essential type of its second and third
    # operands; the condition does not contribute
    branches = cTernaryBranches(lexemes)
    if branches:
        return cConditionalEssentialType(
            cEssentialTypeOfExpression(branches[0], depth + 1),
            cEssentialTypeOfExpression(branches[1], depth + 1))

    operator = cTopLevelOperator(lexemes)
    if operator is None:
        prefix = lexemes[0].text()
        # A dereference designates what the pointer points to; the address of an
        # object is a pointer, which is outside the model
        if prefix == '*':
            term = cTermLexeme(lexemes[1:])
            return cEssentialTypeOfPointee(term.ent()) if term else None
        if prefix == '&':
            return None
        if prefix == '!':
            return (C_ESSENTIAL_BOOLEAN, 1)
        if prefix == '~':
            return cShiftOrComplementEssentialType(
                cEssentialTypeOfExpression(lexemes[1:], depth + 1))
        if prefix in ('-', '+', '++', '--'):
            return cEssentialTypeOfExpression(lexemes[1:], depth + 1)
        return cEssentialTypeOfTerm(lexemes)

    text = lexemes[operator].text()
    if text in C_BOOLEAN_OPERATORS:
        return (C_ESSENTIAL_BOOLEAN, 1)
    if text in C_LEFT_TYPED_OPERATORS:
        return cShiftOrComplementEssentialType(
            cEssentialTypeOfExpression(lexemes[:operator], depth + 1))

    left = cEssentialTypeOfExpression(lexemes[:operator], depth + 1)
    right = cEssentialTypeOfExpression(lexemes[operator + 1:], depth + 1)
    return cBalanceEssentialTypes(left, right, text)

# Whether an expression is a composite expression, meaning the non-constant
# result of a composite operator. A constant expression is not composite, and
# neither is an expression a cast covers in full.
def cIsCompositeExpression(lexemes: list[Lexeme]) -> bool:
    lexemes = cNormalizeExpression(lexemes)
    if not lexemes:
        return False

    # A conditional is composite when either of the operands it selects is
    branches = cTernaryBranches(lexemes)
    if branches:
        return (cIsCompositeExpression(branches[0])
                or cIsCompositeExpression(branches[1]))

    operator = cTopLevelOperator(lexemes)
    if operator is None:
        # Complement is a composite operator, while a unary plus or minus is
        # composite only where the operand it applies to already is
        if lexemes[0].text() == '~':
            return not cIsConstantExpression(lexemes)
        if lexemes[0].text() in ('+', '-'):
            return cIsCompositeExpression(lexemes[1:])
        return False

    if lexemes[operator].text() not in C_COMPOSITE_OPERATORS:
        return False

    return not cIsConstantExpression(lexemes)

# Whether every operand of an expression is a constant, so translation fixes its
# value. Macros and enumeration constants count, whether or not the value can be
# read, which folding it would need.
def cIsConstantExpression(lexemes: list[Lexeme]) -> bool:
    operands = False
    for lexeme in cNormalizeExpression(lexemes):
        token = lexeme.token()
        if token in C_LITERAL_TOKENS:
            operands = True
            continue
        if token != 'Identifier':
            continue

        ent = lexeme.ent()
        if not ent or not ent.kind().check('Macro, Enumerator, Typedef, Type'):
            return False
        operands = True

    return operands

# True if an essential type came from a constant expression. Integer constants
# exist only for int and wider, so the lowest rank the essential type model gives
# them is not their rank for the usual arithmetic conversions.
def cIsConstantType(essential) -> bool:
    return len(essential) > 2 and bool(essential[2])

# The essential type of a conditional expression. Operands of the same category
# keep it, unlike the operands of an arithmetic operator, and anything else takes
# the standard type.
def cConditionalEssentialType(second, third):
    if not second:
        return third
    if not third:
        return second
    if C_ESSENTIAL_POINTER in (second[0], third[0]):
        return (C_ESSENTIAL_POINTER, None)

    if second[0] == third[0]:
        return cWiderEssentialType(second, third)

    return cUsualArithmeticConversions(second, third)

# Two operands of one category keep it, taking the wider of their widths
def cWiderEssentialType(left, right):
    constant = cIsConstantType(left) and cIsConstantType(right)
    if left[1] is None or right[1] is None:
        return (left[0], None, constant)
    return (left[0], max(left[1], right[1]), constant)

# The essential type of the result of a shift or a complement, which follow the
# same rule. An essentially unsigned operand, the left hand one for a shift,
# keeps its essential type, and anything else takes its standard type.
def cShiftOrComplementEssentialType(operand):
    if not operand or operand[0] in (C_ESSENTIAL_UNSIGNED, C_ESSENTIAL_POINTER):
        return operand
    return cUsualArithmeticConversions(operand, operand)

# The essential type of a composite expression. Signed, unsigned and floating
# operands of the same category keep it, so integer promotion is ignored for
# them; any other pairing takes the standard type of the expression.
def cBalanceEssentialTypes(left, right, operator: str | None = None):
    if not left:
        return right
    if not right:
        return left

    # Pointer arithmetic yields a pointer, whatever the other operand is
    if C_ESSENTIAL_POINTER in (left[0], right[0]):
        return (C_ESSENTIAL_POINTER, None)

    # A shift is not balanced, its result follows the left hand operand
    if operator in C_LEFT_TYPED_OPERATORS:
        return cShiftOrComplementEssentialType(left)

    if left[0] == right[0] and left[0] in (C_ESSENTIAL_SIGNED,
            C_ESSENTIAL_UNSIGNED, C_ESSENTIAL_FLOATING):
        return cWiderEssentialType(left, right)

    # Character arithmetic keeps its essential type when the other operand is
    # numeric and of no higher rank than int, per the Rule 10.4 exceptions
    numeric = (C_ESSENTIAL_SIGNED, C_ESSENTIAL_UNSIGNED)
    if operator == '+':
        if left[0] == C_ESSENTIAL_CHARACTER and right[0] in numeric and right[1] and right[1] <= 32:
            return left
        if right[0] == C_ESSENTIAL_CHARACTER and left[0] in numeric and left[1] and left[1] <= 32:
            return right
    elif operator == '-':
        if left[0] == C_ESSENTIAL_CHARACTER and right[0] in numeric and right[1] and right[1] <= 32:
            return left

    return cUsualArithmeticConversions(left, right)

# Operands that are not balanced by the essential type model take the standard
# type of the expression, which is where integer promotion does apply
def cUsualArithmeticConversions(left, right):
    if C_ESSENTIAL_FLOATING in (left[0], right[0]):
        widths = [t[1] for t in (left, right) if t[0] == C_ESSENTIAL_FLOATING and t[1]]
        return (C_ESSENTIAL_FLOATING, max(widths) if widths else None)

    promoted = []
    for essential in (left, right):
        category, width = essential[0], essential[1]
        # Boolean, character and enumerated types promote to signed int, as do
        # types of lower rank than int. A constant is never of lower rank, so
        # only the width the model gave it is raised
        if category in (C_ESSENTIAL_BOOLEAN, C_ESSENTIAL_CHARACTER, C_ESSENTIAL_ENUM):
            promoted.append((C_ESSENTIAL_SIGNED, 32))
        elif width and width < 32 and not cIsConstantType(essential):
            promoted.append((C_ESSENTIAL_SIGNED, 32))
        else:
            promoted.append((category, max(width, 32) if width else 32))

    if promoted[0][1] != promoted[1][1]:
        return max(promoted, key=lambda t: t[1])
    if C_ESSENTIAL_UNSIGNED in (promoted[0][0], promoted[1][0]):
        return (C_ESSENTIAL_UNSIGNED, promoted[0][1])
    return promoted[0]

# Split a conditional expression into its second and third operands, or None
def cTernaryBranches(lexemes: list[Lexeme]) -> tuple[list[Lexeme], list[Lexeme]] | None:
    question = None
    depth = 0
    pending = 0
    for i, lex in enumerate(lexemes):
        text = lex.text()
        if text in ('(', '[', '{'):
            depth += 1
        elif text in (')', ']', '}'):
            depth -= 1
        elif depth != 0:
            continue
        elif text == '?':
            if question is None:
                question = i
            else:
                pending += 1
        elif text == ':' and question is not None:
            # A nested conditional claims this colon
            if pending:
                pending -= 1
                continue
            return (lexemes[question + 1:i], lexemes[i + 1:])
    return None

# True if the first lexeme's parenthesis closes on the last lexeme
def cIsMatchedPair(lexemes: list[Lexeme]) -> bool:
    return cMatchingClose(lexemes) == len(lexemes) - 1

# Index of the parenthesis closing the one the expression opens on, or None
def cMatchingClose(lexemes: list[Lexeme]) -> int | None:
    depth = 0
    for i, lex in enumerate(lexemes):
        if lex.text() == '(':
            depth += 1
        elif lex.text() == ')':
            depth -= 1
            if depth == 0:
                return i
    return None

# If the expression begins with a cast, return the cast's essential type
def cLeadingCast(lexemes: list[Lexeme]) -> tuple[str, int | None] | None:
    if len(lexemes) < 4 or lexemes[0].text() != '(':
        return None

    # A cast must be followed by the expression it applies to
    close = cMatchingClose(lexemes)
    if close is None or close == len(lexemes) - 1:
        return None

    inner = lexemes[1:close]
    if not inner or any(lex.token() in C_LITERAL_TOKENS for lex in inner):
        return None

    typeEnt = None
    for lex in inner:
        if lex.ent() and lex.ent().kind().check('Typedef, Type'):
            typeEnt = lex.ent()
        elif lex.token() not in ('Keyword', 'Identifier', 'Operator', 'Punctuation'):
            return None

    text = ' '.join(lex.text() for lex in inner)
    return cEssentialType(text, typeEnt)

# Index of the operator that binds least tightly, or None if there is none
def cTopLevelOperator(lexemes: list[Lexeme]) -> int | None:
    best = None
    bestPrecedence = 0
    depth = 0
    for i, lex in enumerate(lexemes):
        text = lex.text()
        if text in ('(', '[', '{'):
            depth += 1
        elif text in (')', ']', '}'):
            depth -= 1
        elif depth == 0 and lex.token() == 'Operator':
            # Member access, increment and decrement, and the operators of a
            # declaration take no pair of operands
            if text in ('.', '->', '::', '++', '--'):
                continue
            precedence = C_OPERATOR_PRECEDENCES.get(text)
            # A prefix operator has no left operand to balance against
            if precedence is None or (i == 0 and text in
                    ('-', '+', '~', '!', '*', '&')):
                continue
            if precedence >= bestPrecedence:
                best = i
                bestPrecedence = precedence
    return best

# The essential type of an expression holding no top level operator
def cEssentialTypeOfTerm(lexemes: list[Lexeme]) -> tuple[str, int | None] | None:
    if lexemes[0].text() == 'sizeof':
        return (C_ESSENTIAL_UNSIGNED, 32)

    lex = cTermLexeme(lexemes)
    if not lex:
        return None
    if not lex.ent():
        return cEssentialTypeOfLiteral(lex.text())

    # Subscripting a pointer designates what it points to, where an array's own
    # type already gives the element type. The subscript is only this term's when
    # it opens on it, not on an object it is a member of
    index = next((i for i, l in enumerate(lexemes) if l is lex), None)
    if index is not None and index + 1 < len(lexemes) and lexemes[index + 1].text() == '[':
        pointee = cEssentialTypeOfPointee(lex.ent())
        if pointee:
            return pointee

    return cEssentialTypeOfEnt(lex.ent())

# The lexeme a term resolves to, or None. Call arguments and subscripts are not
# part of the term, so a call ends on the function and a member access on the
# member it names.
def cTermLexeme(lexemes: list[Lexeme]) -> Lexeme | None:
    for lex in reversed(cOuterLexemes(lexemes)):
        if lex.ent() or lex.token() in C_LITERAL_TOKENS:
            return lex

    return None



# Files

# Given an ent and a set, build the translation unit of includes of a file
def buildTranslationUnit(fileEnt: Ent, translationUnitSet: set[Ent]):
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

# See if it is a constructor, even if it's in a namespace
def isConstructor(ent: Ent) -> bool:
    name = re.escape(ent.name())
    if re.search(f'\\b{name}::{name}$', ent.longname()):
        return True
    return False


# Whether the entity is a C++ copy constructor as defined in
# https://en.cppreference.com/w/cpp/language/copy_constructor.html
def is_cpp_copy_constructor(ent: Ent) -> bool:
    parent = ent.parent()
    parameters = ent.ents('Declare, Define', 'Parameter')

    # Require constructor, parent, at least 1 parameters
    if not parent or not parameters or not ent.kind().check('C Member Function') \
    or ent.name() != parent.name():
        return False

    # First parameter must be a reference to the parent type
    first = parameters[0]
    t = first.freetext('UnderlyingType') or ''
    if not re.search(rf'^(const )?(volatile )?{parent.longname()}(<.*>)? &', t):
        return False

    # If only 1 parameter, require no default value
    if len(parameters) == 1:
        return first.value() == None

    return True


# Libraries


# Whether the entity is a pointer to a FILE
def isFilePointer(ent: Ent) -> bool:
    t = ent.freetext('UnderlyingType') or ''
    t = re.sub(r'\b(const|restrict|volatile)\s*', '', t)
    return t in ('_iobuf *', '_IO_FILE *', '__sFILE *')


# Given a long string like "std::anything::cout" and a string like "cout"
# Returns true if the ent longname is something like "std::cout" or "std::anything::cout"
def stringIsStd(wholeString: str, lastPart: str) -> bool:
    nameParts = wholeString.split('::')
    return nameParts[0] == 'std' and nameParts[-1] == lastPart



########
# Java #
########



# Classes & Interfaces

# Given an ent and id, see there's an ancestor with the given id
def javaAncestorOfIdExists(ent: Ent, id: int) -> bool:
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
def javaAncestorOfLongnameExists(ent: Ent, regex: str | re.Pattern, translationUnitCache: dict[str, bool]) -> bool:
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
def javaImmutableClass(ent: Ent) -> bool:
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
def javaSimplifyNumberLiteral(text: str) -> str:
    # Remove separators & long suffix
    text = re.sub('_|l|L', '', text)
    # If it's not hex, remove float & double suffix
    if not re.match(JAVA_NUMBER_SYSTEMS['hex'], text, re.IGNORECASE):
        text = re.sub('f|F', '', text)
        text = re.sub('d|D', '', text)
    return text

# Get type of a Java number literal
def javaLiteralNumberType(text: str) -> str | None:
    # Remove separators
    text = re.sub('_', '', text)
    # Get type
    for numType in JAVA_NUMBER_TYPES.keys():
        pattern = JAVA_NUMBER_TYPES[numType]
        if re.match(pattern, text, re.IGNORECASE):
            return numType
    return None

# Convert a Java number literal from a string into a number
def javaLiteralToNumber(text: str) -> int | float | None:
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
    return None

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
