#!/usr/bin/env python3

import understand
import argparse
import datetime
import re
import sys

defaultCheckArguments = {
	# Check 1: COMF
	'minimumCommentPercentage': 20,
	# Check 2: PATH
	'maximumPathsAllowed': 80,
	# Check 4: vG
	'maximumComplexity': 20,
	'variant': 'modified',
	# Check 5: CALLING
	'maximumCallingFunctions': 5,
	# Check 6: CALLS
	'maximumCalls': 7,
	# Check 7: PARAM
	'maximumNumberOfParameters': 5,
	# Check 8: STMT
	'maximumStatementsAllowed': 50,
	# Check 9: LEVEL
	'maximumCallLevel': 4,
	'skipRecursiveFunctions': False,
	# Check 10: RETURN
	'ignoreConstructorsAndDestructors': False,
	'ignoreInactiveCode': False,
	# Check 11: VOCF
	'maximumVocfLevel': 4,
}

# Main Arguments:p
parser = argparse.ArgumentParser()
parser.add_argument('db', help='Specify Understand database. Example: upython hisMetricsCsv.py project/project.und')
parser.add_argument('-csv', help='Specify CSV Name. Default is the timestamp. Example: upython hisMetricsCsv.py project/project.und -csv hisMetrics.csv')

# Check Arguments:
parser.add_argument('-mcp', help='Minimum Comment Percentage. Default is 20. Example: upython hisMetricsCsv.py project/project.und -mcp 20')
parser.add_argument('-mpa', help='Maximum Paths Allowed. Default is 80. Example: upython hisMetricsCsv.py project/project.und -mpa 70')
parser.add_argument('-mco', help='Maximum Complexity. Default is 20. Example: upython hisMetricsCsv.py project/project.und -mco 15')
parser.add_argument('-v', help='Variant of normal modified, or strict. Default is modified. Example: upython hisMetricsCsv.py project/project.und -v normal')
parser.add_argument('-mcf', help='Maximum Calling Functions. Default is 5. Example: upython hisMetricsCsv.py project/project.und -mcf 6')
parser.add_argument('-mca', help='Maximum Calls. Default is 7. Example: upython hisMetricsCsv.py project/project.und -mca 4')
parser.add_argument('-mnp', help='Maximum Number of Parameters. Default is 5. Example: upython hisMetricsCsv.py project/project.und -mnp 6')
parser.add_argument('-msa', help='Maximum Statements Allowed. Default is 8. Example: upython hisMetricsCsv.py project/project.und -msa 8')
parser.add_argument('-mcl', help='Maximum Call Levels. Default is 4. Example: upython hisMetricsCsv.py project/project.und -mcl 3')
parser.add_argument('-srf', help='Skip Recursive Functions. Default is 0. Example: upython hisMetricsCsv.py project/project.und -srf 1')
parser.add_argument('-icd', help='Ignore Constructors & Destructors. Default is 0. Example: upython hisMetricsCsv.py project/project.und -icd 1')
parser.add_argument('-iic', help='Ignore Inactive Code. Default is 0. Example: upython hisMetricsCsv.py project/project.und -iic 1')
parser.add_argument('-mvl', help='Maximum VOCF Level. Default is 4. Example: upython hisMetricsCsv.py project/project.und -mvl 3')
args = parser.parse_args()

checkCodes = ['COMF', 'PATH', 'GOTO', 'vG', 'CALLING', 'CALLS', 'PARAM', 'STMT', 'LEVEL', 'RETURN', 'VOCF', 'AP_CG_CYCLE']

# Function Types
refDefineKinds = 'define, ada declare body, vhdl declare'
entDefineKinds = 'ada entry, ada function, ada procedure, ada protected, ada task, c function, csharp method, fortran block data, fortran function, fortran interface, fortran program, fortran subroutine, java method, jovial subroutine, pascal compunit, pascal function, pascal procedure, vhdl procedure, vhdl function, vhdl process, vhdl architecture, web function, web method'

refCallKinds = 'ada call ~dispatch, c call ~virtual, c use ptr, cobol call, csharp call ~virtual, csharp use ptr, fortran call, java call jovial call, jovial asm use, pascal call ~virtual, vhdl call, web call'
entCallKinds = 'ada entry, ada function, ada package, ada procedure, ada protected, ada task, c function, cobol program, csharp method, fortran block data, fortran function, fortran interface, fortran program, fortran subroutine, java method, jovial file, jovial subroutine, pascal compunit, pascal function, pascal procedure, vhdl procedure, vhdl function, vhdl process, vhdl architecture, web function, web method'

class ProgressBar:
	def __init__(self, max):
		self.i = 0
		self.max = max

	def progress(self):
		print('   ', round(self.i / self.max * 100), '%', end='\r')
		sys.stdout.flush()
		self.i += 1

def timeStamp():
	return datetime.datetime.now().strftime('%Y-%m-%d-%H-%M-%S')

def checkForViolatingMin(value, min):
	if value == None:
		return str(value)
	if value < min:
		return str(value) + '*'
	return str(value)

def checkForViolatingMax(value, max):
	if value == None:
		return str(value)
	if value > max:
		return str(value) + '*'
	return str(value)

def sanitizeForCSV(oldString):
	newString = ''
	containsCommas = False
	for c in oldString:
		if c != '"':
			newString += c
		if c == ',':
			containsCommas = True
	if containsCommas:
		return '"' + newString + '"'
	return newString

def reportFunction(fileReport, functionId, checkKey, checkResult):
	if functionId in fileReport:
		fileReport[functionId][checkKey] = checkResult

def reportFile(csv, functionReport):
	# Excel doesn't allow for a comma and a space for seperating strings in cells. Just do a comma to separate cells.

	# Meta-data of function
	csv.write(sanitizeForCSV(functionReport['parent']) + ',')
	csv.write(sanitizeForCSV(functionReport['file']) + ',')
	csv.write(sanitizeForCSV(functionReport['line']) + ',')
	csv.write(sanitizeForCSV(functionReport['function']) + ',')

	# Results of each check on function
	for i in range(len(checkCodes)):
		code = checkCodes[i]
		if code in functionReport:
			csv.write(sanitizeForCSV(functionReport[code]))
		if i < len(checkCodes) - 1:
			csv.write(',')
	csv.write('\n')

def addFunctionToReport(fileReport, ref):
	ent = ref.ent()

	# Skip the standard library in VHDL
	if (ent.language() == 'VHDL'):
		if (ent.library() == 'std' or ent.library() == 'ieee'):
			return

	# If the function is defined in the project, it has as parent
	if (ent.parent()):
		functionId = ent.id()
		fileReport[functionId] = {}
		fileReport[functionId]['parent'] = str(ent.parent().longname())
		fileReport[functionId]['file'] = str(ref.file())
		fileReport[functionId]['line'] = str(ref.line())
		fileReport[functionId]['function'] = str(ent)

def addFileToReport(fileReport, defineRefs):
	# Check all functions in defineRefs
	for ref in defineRefs:
		addFunctionToReport(fileReport, ref)

def check1(fileReport, defineRefs, args):
	# Arguments
	minimumCommentPercentage = int(args.mcp)

	# Check all functions in defineRefs
	for ref in defineRefs:
		# Do the check
		ent = ref.ent()
		precedingComments = len(ent.comments('before', True))
		comments = ent.metric(['countlinecomment'])['countlinecomment']
		if not comments:
			comments = 0
		totalComments = comments + precedingComments
		code = ent.metric(['countlinecode'])['countlinecode']
		if not code:
			code = 0
		newrctc = ent.metric(['ratiocommenttocode'])['ratiocommenttocode']
		if not newrctc:
			newrctc = 0
		if code > 0 and precedingComments:
			newrctc = totalComments / code
		ccRatio = round(float(newrctc) * 100)

		# Add a star for violations
		ccRatio = checkForViolatingMin(ccRatio, minimumCommentPercentage)

		# Report the result
		functionId = ent.id()
		reportFunction(fileReport, functionId, 'COMF', ccRatio)

def check2(fileReport, defineRefs, args):
	# Arguments
	maxPathsAllowed = int(args.mpa)

	# Check all functions in defineRefs
	for ref in defineRefs:
		# Do the check
		ent = ref.ent()
		paths = ent.metric(['CountPath'])['CountPath']

		# Add a star for violations
		paths = checkForViolatingMax(paths, maxPathsAllowed)

		# Report the result
		functionId = ent.id()
		reportFunction(fileReport, functionId, 'PATH', paths)

def check3(fileReport, defineRefs, args):
	# Check all functions in defineRefs
	for ref in defineRefs:
		# Do the check
		ent = ref.ent()
		startLine = ref.line()
		endLine = ent.refs('End')[0].line()
		lexemes = ent.lexer().lexemes(startLine, endLine)
		gotoStatements = 0
		for lexeme in lexemes:
			if lexeme.text() == 'goto' and lexeme.token() == 'Keyword':
				gotoStatements += 1

		# Add a star for violations
		gotoStatements = checkForViolatingMax(gotoStatements, 0)

		# Report the result
		functionId = ent.id()
		reportFunction(fileReport, functionId, 'GOTO', gotoStatements)

def check4(fileReport, defineRefs, args):
	# Arguments
	maxComplexity = int(args.mco)
	variant = args.v

	metric = 'Cyclomatic'
	if variant == 'modified':
		metric = 'CyclomaticModified'
	elif variant == 'strict':
		metric = 'CyclomaticStrict'

	# Check all functions in defineRefs
	for ref in defineRefs:
		# Do the check
		ent = ref.ent()
		complexity = ent.metric([metric])[metric]

		# Add a star for violations
		complexity = checkForViolatingMax(complexity, maxComplexity)

		# Report the result
		functionId = ent.id()
		reportFunction(fileReport, functionId, 'vG', complexity)

def check5(fileReport, defineRefs, args):
	# Arguments
	maxCallingFunctions = int(args.mcf)

	# Check all functions in defineRefs
	for ref in defineRefs:
		# Do the check
		ent = ref.ent()
		callingFunctions = len(ent.ents('callby'))

		# Add a star for violations
		callingFunctions = checkForViolatingMax(callingFunctions, maxCallingFunctions)

		# Report the result
		functionId = ent.id()
		reportFunction(fileReport, functionId, 'CALLING', callingFunctions)

def check6(fileReport, defineRefs, args):
	# Arguments
	maxCalls = int(args.mca)

	# Check all functions in defineRefs
	for ref in defineRefs:
		# Do the check
		ent = ref.ent()
		calls = len(ent.ents('call'))

		# Add a star for violations
		calls = checkForViolatingMax(calls, maxCalls)

		# Report the result
		functionId = ent.id()
		reportFunction(fileReport, functionId, 'CALLS', calls)

def check7(fileReport, defineRefs, args):
	# Arguments
	maxNumberOfParameters = int(args.mnp)

	# Check all functions in defineRefs
	for ref in defineRefs:
		# Do the check
		ent = ref.ent()
		parameters = ent.parameters()
		paramCount = None
		if parameters != None:
			paramCount = len(parameters.split(','))

		# Add a star for violations
		paramCount = checkForViolatingMax(paramCount, maxNumberOfParameters)

		# Report the result
		functionId = ent.id()
		reportFunction(fileReport, functionId, 'PARAM', paramCount)

def check8(fileReport, defineRefs, args):
	# Arguments
	maxStatements = int(args.msa)

	# Check all functions in defineRefs
	for ref in defineRefs:
		# Do the check
		ent = ref.ent()
		statements = ent.metric(['CountStmt'])['CountStmt']

		# Add a star for violations
		statements = checkForViolatingMax(statements, maxStatements)

		# Report the result
		functionId = ent.id()
		reportFunction(fileReport, functionId, 'STMT', statements)

def getDepth(functionEnt, knownFunctions):
	if not functionEnt:
		return
	if functionEnt.id() in knownFunctions and knownFunctions[functionEnt.id()] == -1:
		return 'Recursive'
	if functionEnt.id() in knownFunctions:
		return knownFunctions[functionEnt.id()]

	knownFunctions[functionEnt.id()] = -1 # Marker for recursion

	calls = functionEnt.refs('call ~inactive, use ptr ~inactive', entDefineKinds, True)

	for call in calls:
		depth = getDepth(call.ent(), knownFunctions)
		if depth == 'Recursive':
			return 'Recursive'
		if depth > knownFunctions[functionEnt.id()]:
			knownFunctions[functionEnt.id()] = depth

	knownFunctions[functionEnt.id()] += 1
	return knownFunctions[functionEnt.id()]

def check9(fileReport, defineRefs, args):
	# Arguments
	maxCallLevels = int(args.mcl)
	skipRecursiveFunctions = int(args.srf)

	knownFunctions = {}

	# Check all functions in defineRefs
	for ref in defineRefs:
		# Do the check
		ent = ref.ent()
		depth = getDepth(ent, knownFunctions)
		if (depth == 'Recursive' and not skipRecursiveFunctions):
			depth += '*'
		elif depth != 'Recursive':
			# Add a star for violations
			depth += 1
			depth = checkForViolatingMax(depth, maxCallLevels)

		# Report the result
		functionId = ent.id()
		reportFunction(fileReport, functionId, 'LEVEL', depth)

# This function takes a lexeme and an end function reference and determines if we are inside a lambda function
# If we are inside a lambda function, then we walk through it and return a new lexeme
def passLambda(parent, lexeme, allDefines):
	for define in allDefines:
		end = define.ent().ref('C End')
		if not end:
			continue

		inLambda = False
		if lexeme.line_begin() > define.line() and lexeme.line_begin() < end.line():
			inLambda = True
		elif lexeme.line_begin() == define.line() and lexeme.line_begin() != end.line() and lexeme.column_begin() >= define.column():
			inLambda = True
		elif lexeme.line_begin() == end.line() and lexeme.line_begin() != define.line() and lexeme.column_begin() <= end.column():
			inLambda = True
		elif lexeme.line_begin() == end.line() and lexeme.line_begin() == define.line() and lexeme.column_begin() >= define.column() and lexeme.column_begin() <= end.column():
			inLambda = True

		# If we are inside a lambda, move through that lambda
		if inLambda:

			while lexeme.line_begin() > define.line():
				lexeme = lexeme.previous()

			while lexeme.column_begin() > define.column() or lexeme.line_begin() < define.line():
				lexeme = lexeme.previous()

	return lexeme

def check10(fileReport, file, args):
	# Arguments
	ignoreConstructorsDestructors = int(args.icd)
	ignoreInactiveCode = int(args.iic)

	# Currently lambda functions are filtered because we lack implicit type detection in the Strict parser
	funEnds = file.filerefs('end', 'function ~lambda')
	if not funEnds:
		return

	lexer = file.lexer()
	if not lexer:
		return

	for funEnd in funEnds:
		ent = funEnd.ent()

		# Determine if we are testing this function
		if ignoreConstructorsDestructors and ent.parent() and ent.parent.name() == ent.name():
			continue
		if re.search('^~', ent.name()):
			continue
		start = ent.ref('definein')
		if not start:
			continue

		# Skip funcitons defined with MACROS
		if start.line() == funEnd.line():
			continue

		# Initialize tracking variables and options
		atEnd = False
		statementCount = 0
		exitCount = 0
		lexeme = lexer.lexeme(funEnd.line(), funEnd.column())
		lambdas = ent.refs('C Define', 'C Lambda Function')

		# This test is for inline and lambda function compatibility
		while ( lexeme ) and ( ( lexeme.line_begin() > start.line() ) or ( lexeme.line_begin() == start.line() and lexeme.column_begin() >= start.column() ) ):

			# Determine if this lexeme should be tested give the user options
			testLexeme = not (ignoreInactiveCode and lexeme.inactive())

			# lambdas break this check so we detect if we are in one and move the lexeme past it if we are
			if testLexeme and lambdas:
				lexeme = passLambda(funEnd, lexeme, lambdas)

			# Count function exits and statements
			if lexeme.token() == 'Keyword' and re.search('Exit|return|goto', lexeme.text()) and testLexeme:
				exitCount += 1
			if lexeme.token() == 'Punctuation' and re.search('[;}]', lexeme.text()) and testLexeme:
				statementCount += 1
			if statementCount == 2 and exitCount:
				atEnd = True

			lexeme = lexeme.previous()

		# Add a star for violations
		result = ''
		if exitCount > 1:
			result = 'Multiple exit points from function*'
		elif not exitCount and ent.type() != None and not re.search('\bvoid\b', ent.type()):
			result = 'No exit point in function*'
		elif exitCount == 1 and not atEnd:
			result = 'Exit point not at end of function*'

		# Report the result
		functionId = ent.id()
		reportFunction(fileReport, functionId, 'RETURN', result)

def getDeclRef(ent):
	if not ent:
		return

	decl = ent.refs('definein', '', True)
	if not decl:
		decl = ent.refs('declarein', '', True)
	return decl

def getVOCF(fun):
	startRef = getDeclRef(fun)
	endRef = fun.refs('end', '', True)
	if not (startRef and endRef):
		return
	startRef = startRef[0]
	endRef = endRef[0]
	file = startRef.file()
	fileId = file.id()
	lexer = file.lexer()
	n1 = {}
	n2 = {}
	N1 = 0
	N2 = 0

	for lexeme in lexer.lexemes(startRef.line(), endRef.line()):
		if lexeme.token() == 'Operator' or lexeme.token() == 'Keyword' or lexeme.token() == 'Punctuation':
			if not re.search('[)}\]]', lexeme.text()):
				n1[lexeme.text()] = 1
				N1 += 1
		elif lexeme.token() == 'Identifier' or lexeme.token() == 'Literal' or lexeme.token() == 'String':
			n2[lexeme.text()] = 1
			N2 += 1

	n1 = len(n1)
	n2 = len(n2)
	if n1 + n2 > 0:
		result = round((N1 + N2) / (n1 + n2), 2)
	else:
		result = 0
	return result

def check11(fileReport, defineRefs, args):
	# Arguments
	maxVocfLevel = int(args.mvl)

	# Check all functions in defineRefs
	for ref in defineRefs:
		# Do the check
		ent = ref.ent()
		vocfLevel = getVOCF(ent)

		# Add a star for violations
		vocfLevel = checkForViolatingMax(vocfLevel, maxVocfLevel)

		# Report the result
		functionId = ent.id()
		reportFunction(fileReport, functionId, 'VOCF', vocfLevel)

def discoverRecursion(fileReport, ent, firstTime, seen, originalEnt):
	if not ent.kind().check(entCallKinds):
		return

	for callRef in ent.refs(refCallKinds, entCallKinds, True):
		callEnt = callRef.ent()
		if callEnt.uniquename() not in seen:
			seen[callEnt.uniquename()] = 0
		seen[callEnt.uniquename()] += 1
		if seen[callEnt.uniquename()] > 1:
			continue

		# Not sure if this commented line works, but it's slow
		if callEnt.uniquename() == originalEnt.uniquename():
			# It's possible the following line was intentional, but I can't think why you would want a random ref when callRef should be the call site of the recursion
			# I'm leaving this here for future Kyle in case an issue arises from using callRef directly
			# ref = callRef.ent().ref();

			ref = callRef
			functionId = ref.ent().id()
			if functionId in fileReport:
				if firstTime:
					reportFunction(fileReport, functionId, 'AP_CG_CYCLE', 'Violation: function ' + originalEnt.longname() + ' is directly recursive, which is unsafe.*')
				else:
					reportFunction(fileReport, functionId, 'AP_CG_CYCLE', 'Violation: function ' + originalEnt.longname() + ' is indirectly recursive through ' + ent.longname() + ', which is unsafe.*')
			return

		discoverRecursion(fileReport, callEnt, False, seen, originalEnt)

def check12(fileReport, file, args):
	# Check all functions in defineRefs
	for ref in file.filerefs('define, body declare', entCallKinds, True):
		# Do the check
		ent = ref.ent()
		discoverRecursion(fileReport, ent, True, {}, ent)

if __name__ == '__main__':
	# Get arguments, or the defaults
	if not args.csv: # CSV Name
		args.csv = args.db[:-4] + '-' + timeStamp() + '.csv'
	if not args.mcp:
		args.mcp = defaultCheckArguments['minimumCommentPercentage']
	if not args.mpa:
		args.mpa = defaultCheckArguments['maximumPathsAllowed']
	if not args.mco:
		args.mco = defaultCheckArguments['maximumComplexity']
	if not args.v:
		args.v = defaultCheckArguments['variant']
	if not args.mcf:
		args.mcf = defaultCheckArguments['maximumCallingFunctions']
	if not args.mca:
		args.mca = defaultCheckArguments['maximumCalls']
	if not args.mnp:
		args.mnp = defaultCheckArguments['maximumNumberOfParameters']
	if not args.msa:
		args.msa = defaultCheckArguments['maximumStatementsAllowed']
	if not args.mcl:
		args.mcl = defaultCheckArguments['maximumCallLevel']
	if not args.srf:
		args.srf = defaultCheckArguments['skipRecursiveFunctions']
	if not args.icd:
		args.icd = defaultCheckArguments['ignoreConstructorsAndDestructors']
	if not args.iic:
		args.iic = defaultCheckArguments['ignoreInactiveCode']
	if not args.mvl:
		args.mvl = defaultCheckArguments['maximumVocfLevel']

	# Open database
	db = understand.open(args.db)

	# Open csv file for appending and add table header
	csv = open(args.csv, 'w')
	csv.write('Parent, File, Line, Function, Comment Density (COMF), Number of Paths (PATH), Number of Goto Statements (GOTO), Cyclomatic Complexity (v(G)), Calling Functions (CALLING), Called Funcitons (CALLS), Function Parameters (PARAM), Number of Statements (STMT), Number of Call Levels (LEVEL), Number of Return Points (RETURN), Language Scope (VOCF), Recursion (AP_CG_CYCLE)\n')

	# Report on each function only once
	functionIds = {}

	# Check each file
	fileFilter = 'File ~unknown ~unresolved ~jar'
	fileCount = len(db.ents(fileFilter))
	print('Analyzing', fileCount, 'files using Hersteller Initiative Software (HIS) Metrics')
	progressBar = ProgressBar(fileCount)
	for file in db.ents(fileFilter):
		# A fileReport dictionary of functionReport dictionaries
		fileReport = {}

		# Get all the function define refs
		defineRefs = file.filerefs(refDefineKinds, entDefineKinds, True)
		addFileToReport(fileReport, defineRefs)

		# Figure out the language
		language = file.language()
		cOrCpp = language == 'C++'

		# Do all 12 reports
		check1(fileReport, defineRefs, args)
		check2(fileReport, defineRefs, args)
		if cOrCpp:
			check3(fileReport, defineRefs, args) # c++
		check4(fileReport, defineRefs, args)
		check5(fileReport, defineRefs, args)
		check6(fileReport, defineRefs, args)
		check7(fileReport, defineRefs, args)
		check8(fileReport, defineRefs, args)
		check9(fileReport, defineRefs, args)
		if cOrCpp:
			check10(fileReport, file, args) # c++
		check11(fileReport, defineRefs, args)
		check12(fileReport, file, args)

		# Output the results of the 12 checks for the file
		for functionId in fileReport:
			if functionId not in functionIds:
				reportFile(csv, fileReport[functionId])
				functionIds[functionId] = True

		progressBar.progress()

	print()

	# Close csv file
	csv.close()
