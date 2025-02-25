# This script is designed to run with Understand - CodeCheck
# Rewritten by Robby Bennett
# 2023-03-21


import re, functools


########################
# Detailed Description #
########################


def detailed_description(variables):
    canChangePrecedence = variables['canChangePrecedence']
    defaultAll = variables['defaultAll']
    kind = variables['kind']
    kindPlural = _plural(variables['kind'])
    kindStrings = variables['kindStrings']
    kindStringsString = ' '.join(kindStrings)
    languageText = variables['languageText']

    kind1, kind2 = '', ''
    if canChangePrecedence and len(kindStrings) > 2:
        kind1, kind2 = kindStrings[:2]

    desc = []
    desc.append(f'Select the required naming convention for <b>{languageText} {kindPlural}.</b>')
    if len(kindStrings) > 1:
        desc.append(f'<br>{kindPlural} can have different naming conventions based off of their kind ({kindStringsString}).')
    if defaultAll:
        desc.append(f'<br>The first group of options will define the naming conventions for All {kindPlural}, the other options will let you specify a subset of {kindPlural} to test.')
    desc.append(f'<hr>')
    desc.append(f'Naming Convention Options:<ul style="margin-top:0;padding-top:0;">')
    if kind2:
        desc.append(f'<li><b>Precedence:</b> Set the precedence to determine the order that the rules will be applied to an entity that has multiple kinds. e.g. If {kind1} Precedence is set to 2 and {kind2} Precedence is 3, a {kind1} {kind2} {kind} would be tested with the {kind1} naming rules instead of the {kind2} rules.')
    desc.append(f'<li><b>Minimum Length:</b> Specify the shortest length for a valid name. Prefixes and Suffixes are included in the count. The Default value is 1 character.\n')
    desc.append(f'<li><b>Maximum Length:</b> Specify the longest length for a valid name. Prefixes and Suffixes are included in the count. The Default value is 0, which means there is no max length.\n')
    desc.append(f'<li><b>Prefix and Suffix:</b> If the entity needs to have a prefix or suffix, specify them here. If there are multiple valid options, separate them with a coma. If the entity should not have a particular prefix or suffix, put an exclamation (!) before the string.\n')
    desc.append(f'<li><b>Character Set:</b> Limit valid characters to Letters only (a-zA-Z), letters and numbers (a-zA-Z0-9) or All Characters (default). Prefixes and suffixes are ignored for this test.\n')
    desc.append(f'<li><b>Capitalization:</b> The default option is to ignore capitalization. CamelCase allows upper and lower case letters but must start with a lower case letter for lowerCamelCase or a capital letter for UpperCamelCase. Alternatively you can require all capital letters or no capital letters.  By default there can be two Capital Letters next to each other, but that can be disallowed. Required prefixes and suffixes are not tested for capitalization.\n')
    desc.append(f'<li><b>Regular Expression:</b> For complete flexibility you can test the name against a perl regular expression. To find results that do not match the expression put an exclamation (!) before it.\n')
    desc.append(f'</ul>')

    return ''.join(desc)


def _plural(string):
    string, replaced = re.subn('y$', 'ies', string)
    if replaced:
        return string

    string, replaced = re.subn('s$', 'ses', string)
    if replaced:
        return string

    return string + 's'


###########
# Options #
###########


def define_options(check, variables):
    # Make sure that each value in kindStrings is in kindNameLookup
    kind = variables['kind']
    for kindString in variables['kindStrings']:
        if kindString not in variables['kindNameLookup']:
            variables['kindNameLookup'][kindString] = f'{kindString} {kind}'

    if variables['defaultAll']:
        variables['kindNameLookup'][kind] = kind
        precedenceCount = len(variables['kindStrings']) + 1
        _optionsHelper(check, variables['kind'], precedenceCount, True, variables)

    precedenceCount = 1
    for itemKind in variables['kindStrings']:
        _optionsHelper(check, itemKind, precedenceCount, False, variables)
        precedenceCount += 1


def _optionsHelper(check, itemKind, precedenceCount, testAll, variables):
    precedenceTotal = len(variables['kindStrings'])
    if testAll and variables['defaultAll']:
        name = 'All ' + variables['kind']
    else:
        name = variables['kindNameLookup'][itemKind]
    if variables['defaultAll']:
        precedenceTotal += 1
    runValue = precedenceTotal <= 1
    if testAll:
        runValue = True
    space = ' ' * 10

    check.options().checkbox(f'{itemKind}Run', f'Test {name} names', runValue)
    if precedenceTotal > 1 and variables['canChangePrecedence']:
        check.options().choice(f'{itemKind}Precedence', f'{space}Precedence', list(map(str, range(1, precedenceTotal+1))), str(precedenceCount))
    check.options().integer(f'{itemKind}minLength', f'{space}Minimum Length:', 1)
    check.options().integer(f'{itemKind}maxLength', f'{space}Maximum Length:')
    check.options().text(f'{itemKind}prefix', f'{space}Required Prefix:')
    check.options().text(f'{itemKind}suffix', f'{space}Required Suffix:')
    check.options().choice(f'{itemKind}charSet', f'{space}Character Set', ['All Characters', 'Letters and Numbers Only', 'Letters Only'], 'All Characters')
    check.options().choice(f'{itemKind}capOpt', f'{space}Capitalization:', ['ignore', 'UpperCamelCase', 'lowerCamelCase', 'ALLCAPS', 'nocaps'], 'ignore')
    check.options().choice(f'{itemKind}consCaps', f'{space}Consecutive Capitals', ['Allowed', 'Not Allowed'], 'Allowed')
    check.options().text(f'{itemKind}regExp', f'{space}Regular Expression:')


#########
# Check #
#########


def getPrimaryKind(options, kindStr, variables):
    varKinds = []

    # Discard invalid kinds (those not in the testing list)
    kindPart = None
    for kindPart in kindStr.split():
        if options.lookup(f'{kindPart}Run'):
            varKinds.append(kindPart)

    # Sort the valid kind list based off the precedence option and get the first kind
    if varKinds:
        varKinds.sort(key=_kindComparator)
        return varKinds[0]

    if variables['defaultAll']:
        return variables['kind']


def checkNamingRules(options, kindStr, variables, name):
    errors        = []
    length        = len(name)
    minLength     = options.lookup(f'{kindStr}minLength')
    maxLength     = options.lookup(f'{kindStr}maxLength')
    prefixList    = _commaSplit(options.lookup(f'{kindStr}prefix'))
    regExp        = options.lookup(f'{kindStr}regExp')
    suffixList    = _commaSplit(options.lookup(f'{kindStr}suffix'))
    capOpt        = options.lookup(f'{kindStr}capOpt')
    charSet       = options.lookup(f'{kindStr}charSet')
    allowConsCaps = options.lookup(f'{kindStr}consCaps') == 'Allowed'
    runTest       = options.lookup(f'{kindStr}Run')

    if not runTest:
        return

    if maxLength > 0 and length > maxLength:
        errors.append(_TOO_LONG)
    elif length < minLength:
        errors.append(_TOO_SHORT)

    if regExp:
        regExpUsed = False
        regExp, badRegExp = _alterRegexp(regExp)
        if re.search(regExp, name):
            regExpUsed = True
        if regExpUsed and badRegExp:
            errors.append(_BAD_REGEXP)
        elif not regExpUsed and not badRegExp:
            errors.append(_MISSING_REGEXP)

    # Process prefix and suffix, don't use for the rest of the tests
    if prefixList or suffixList:
        # Prefix
        prefixUsed = False
        badPrefix = False
        for prefix in prefixList:
            prefix, _badPrefix = _alterRegexp(prefix)
            if _badPrefix:
                badPrefix = True
            if re.search(f'^{prefix}', name):
                prefixUsed = True
                break
        if prefixUsed and badPrefix:
            errors.append(_BAD_PREFIX)
        elif prefixList and not prefixUsed and not badPrefix:
            errors.append(_MISSING_PREFIX)
        # Suffix
        suffixUsed = False
        badSuffix = False
        for suffix in suffixList:
            suffix, _badSuffix = _alterRegexp(suffix)
            if _badSuffix:
                badSuffix = True
            if re.search(f'{suffix}$', name):
                suffixUsed = True
                break
        if suffixUsed and badSuffix:
            errors.append(_BAD_SUFFIX)
        elif suffixList and not suffixUsed and not badSuffix:
            errors.append(_MISSING_SUFFIX)

    # Check case
    if re.search('UpperCamelCase', capOpt) and not re.search(r'^[A-Z]', name):
        errors.append(_MISSING_INIT_CAP)
    if re.search('lowerCamelCase', capOpt) and not re.search(r'^[a-z]', name):
        errors.append(_MISSING_INIT_LC)
    if re.search('ALLCAPS', capOpt) and re.search(r'[a-z]', name):
        errors.append(_HAS_LOWERCASE)
    if re.search('nocaps', capOpt) and re.search(r'[A-Z]', name):
        errors.append(_HAS_UPPERCASE)
    if not allowConsCaps and re.search(r'[A-Z][A-Z]', name):
        errors.append(_CONSECUTIVE_CAPS)

    # Check character set
    if re.search('Letters and Numbers Only', charSet) and not re.search(r'^[A-Za-z0-9]+$', name):
        errors.append(_NON_ALPHA_NUM)
    if re.search('Letters Only', charSet) and not re.search(r'^[A-Za-z]+$', name):
        errors.append(_NON_ALPHA)

    if errors:
        return _NAME_ERROR + ''.join(errors)


def _kindComparator(a, b):
    a = check.options().lookup(f'{a}Precedence')
    b = check.options().lookup(f'{b}Precedence')
    return a - b
_kindComparator = functools.cmp_to_key(_kindComparator)


def _commaSplit(string):
    return list(filter(None, string.split(',')))


def _intOrDefault(string, default):
    try:
        return int(string)
    except:
        return default


def _alterRegexp(regExp):
    return re.subn(r'^!', '', regExp.strip())


_NAME_ERROR        = '%1 name \'%2\' is invalid.'
_TOO_SHORT        = ' It is too short.'
_TOO_LONG         = ' It is too long.'
_MISSING_PREFIX   = ' It is missing the required prefix.'
_BAD_PREFIX       = ' It uses a disallowed prefix.'
_MISSING_SUFFIX   = ' It is missing the required suffix.'
_BAD_SUFFIX       = ' It uses a disallowed suffix.'
_MISSING_INIT_CAP = ' It is missing the initial capital.'
_MISSING_INIT_LC  = ' It has a leading capital.'
_HAS_LOWERCASE    = ' It has lowercase letters.'
_HAS_UPPERCASE    = ' It has uppercase letters.'
_CONSECUTIVE_CAPS = ' It has consecutive capitals.'
_NON_ALPHA_NUM    = ' It has characters other than letters and numbers.'
_NON_ALPHA        = ' It has characters other than letters.'
_BAD_REGEXP       = ' It matches the disallowed regular expression.'
_MISSING_REGEXP   = ' It does not match the required regular expression.'
