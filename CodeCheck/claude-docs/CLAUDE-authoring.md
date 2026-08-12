# Writing a CodeCheck Check

Conventions and check IDs are in [CLAUDE.md](../CLAUDE.md). Tests are in
[CLAUDE-testing.md](CLAUDE-testing.md).

## Anatomy of a Python check

Each check is one folder containing `check.upy` plus its test files. CodeCheck
calls these functions:

| Function | Purpose |
|---|---|
| `ids()` | Unique check ID(s), as a tuple. |
| `name(id)` | Slash-separated path placing the check in CodeCheck's tree. |
| `tags(id)` | Tags for filtering — must include the language(s). |
| `detailed_description(id)` | HTML rule description shown in the UI. |
| `test_language(language)` | `True` for languages the check applies to. |
| `test_entity(file)` | `True` to run on this file. Usually just `return True`. |
| `check(check, file)` | The body — inspect and report. |
| `define_options(check)` | Optional. Declare user-configurable options. |
| `test_global()` | Optional. `True` for one whole-project pass; body is then called as `check(check)` with no `file`. |

```python
import re

ERR1 = 'Function "%1" is not named in camelCase'


def ids():
    return ('CPP_N###',)


def name(id):
    return 'All Checks/Language Specific/C and C++/Naming Conventions/Functions should be camelCase'


def tags(id):
    return ['Language: C', 'Language: C++', 'Naming Conventions']


def detailed_description(id):
    return '''\
<p>Function names shall be written in lower camelCase
(e.g. <code>setValue</code>, not <code>SetValue</code>).</p>
'''


def test_entity(file):
    return True


def test_language(language):
    return language == 'C++'  # covers both C and C++


def check(check, file):
    for ref in file.filerefs('Define', 'Function'):
        ent = ref.ent()
        if not re.match(r'^[a-z][a-zA-Z0-9]*$', ent.name()):
            check.violation(ent, file, ref.line(), ref.column(), ERR1, ent.name())
```

A check can expose several IDs (e.g. a published-standard ID plus a general
`CPP_*` alias). Return them all from `ids()` and key `name()`/`tags()`/
`detailed_description()` off `id` with a dict — no second file or symlink
needed, and the check is listed in both trees automatically.

```python
def ids():
    return ('MISRA23_X.X.X', 'CPP_T###')

def name(id):
    return {
        'MISRA23_X.X.X': 'Published Standards/MISRA C++ 2023/X.X.X Rule description',
        'CPP_T###':      'All Checks/Language Specific/C and C++/Types/Short description',
    }[id]
```

## Where files live

A check is one folder holding `check.upy` and its test files. **The folder is
named for the check ID**, never for the rule text:

```
<CHECK_ID>/
├── check.upy
└── test.c            # or test.cpp, test.java, test.py, …
```

Where that folder goes depends on whether the check implements a published
standard:

| The check… | Folder |
|---|---|
| implements a published standard | `Published Standards/<Standard>/<CHECK_ID>/` |
| is a general check with no originating standard | `All Checks/Language Specific/<Language>/<Category>/<CPP_ID>/` |

Other top-level buckets exist (`Bug Hunter`, `Libraries`, `Configs`, `SciTools`,
`SciTools' Recommended Checks`) but you won't normally add to them.

A check that implements a standard lives **only** in that standard's folder, even
when it also exposes a `CPP_*` ID — the extra ID in `ids()` generates the
`All Checks/` listing automatically. Don't create a second folder or a symlink.
(157 older checks do sit under `All Checks/` with a `Standard:` tag; that's
legacy, not the pattern to follow.)

**A new check goes in the folder of the standard it was written for, and stays
there.** When the same rule later turns up in another standard, add an alias ID
to the existing check (see [Aliasing an existing rule](CLAUDE-standards.md#aliasing-an-existing-rule))
— don't copy the check into the new standard's folder. A rule first implemented
for MISRA C++ 2023 keeps its files under
`Published Standards/MISRA C++ 2023/MISRA23_X.X.X/` even after a `MISRA25_*`
alias is added.

**Don't confuse the folder name with the `name()` path.** The folder is the bare
ID; the `name()` path is the tree location shown in the UI and *does* include the
rule text:

```
folder:     Published Standards/MISRA C++ 2023/MISRA23_X.X.X/
name(id):   'Published Standards/MISRA C++ 2023/X.X.X Rule description'
```

## Writing the description

`detailed_description(id)` returns the HTML shown in the UI. When implementing a
published rule, **use the standard's exact wording** — convert it to HTML, but
don't paraphrase, summarize, or expand it.

Structure it with the same section headings the standard uses, marked up as
`<p><b>Heading</b></p>`. The established ones, most common first: `Rationale`,
`Example`, `See also`, `Amplification`, `Noncompliant Code Example`,
`Developer's Notes`, `Risk Assessment`, `Exception`.

Add a **Developer's Notes** section for anything specific to *your* check rather
than the standard — behavior, options, known limitations.

Markup vocabulary: `<p>`, `<b>`, `<i>`, `<code>` for inline identifiers,
`<ul>`/`<li>` for lists. Code examples need a `language` attribute — one of
`C`, `C++`, `Java`, `Ada`, `VHDL`, `Python`:

```python
def detailed_description(id):
    return '''\
<p><b>Amplification</b></p>
<p>A <code>volatile</code> qualifier shall not appear in ...</p>

<p><b>Rationale</b></p>
<p>...</p>

<p><b>Example</b></p>
<pre><code language="C++">volatile int x;   // Noncompliant
</code></pre>

<p><b>Developer's Notes</b></p>
<p>Bit-fields are not checked.</p>
'''
```

`description()` is deprecated — don't add it to new checks. (333 older checks
still define it; leave those alone.)

## API gotchas

These cause silently wrong results, not errors.

- **Never put `[` or `]` in violation messages** — Understand silently drops them
  (Python and Clang alike); `uvalid` just reports "Violation missing". If the
  logic fires but violations vanish, suspect brackets first. Write
  `"the range -1 to 1"`, not `"[-1, 1]"`.
- **Prefer `None` as the entity for file-level violations.**
  `violation(file, file, …)` also appears in older checks, but switching to `None`
  fixes a file-level violation that won't register.
- **`%1`, `%2`, …** in the message are filled by the trailing args, in order.
- **Avoid `check.db()` / `db.lookup()`.** Resolve names via the file's refs or
  `file.lexer(lookup_ents=True)` + `lex.ent()`; db-wide lookups can match the
  wrong entity when a name is reused. Treat a rule that truly needs one as a
  known limitation.
- **`ent.type()`, not `ent.kind()`**, for an entity's C/C++ type.
- **`ent.freetext('UnderlyingType')`** resolves the full typedef chain — prefer it
  over `ent.type()` for type decisions. Spell it CamelCase.
- **Never parse `ent.parameters`** (plain text). Use
  `ent.refs('Define', 'Parameter')`.
- **`file.kind()` returns `Module File` for Python files** — `check('Python File')`
  does not match. Let `test_language()` filter and `return True` from
  `test_entity()`.
- **Bracket matching needs a tuple, not a string:** `lex.text() in ('(', '[',
  '{')`, not `in '([{'` (the latter is substring matching).
- **Word-boundary regexes** for identifier matching: `re.search(r'\bword\b', ...)`.

### Reference kinds

Test kinds with `kind().check(...)`:

```python
if ref.kind().check('Assign Ptr, Assignby Ptr'):
```

**`ref.kindname()` is lossy** — it collapses kinds: `Assign Ptr` and
`Assignby Ptr` both give `'Assign'`, `Useby Return` gives `'Return'`, `Setby Init`
gives `'Init'`. For direction, or to see `Deref`/`Ptr`/`Value`, use
`ref.kind().longname()`, which includes a language prefix (`'C Assignby Ptr'`) —
so match by substring, not `==`. The filters in `refs('Assign Ptr')` already match
the full kind; only `kindname()` is lossy.

**Language filtering:** `test_language()` returning `language == 'C++'` covers
**both C and C++**. To restrict to C++ only, add
`file.file_type() == 'C++'` in `test_entity()`. The `Language:` tags must match
what the check actually does — both `Language: C` and `Language: C++` if it
applies to both.

**Useful entry points:**

| Need | Use |
|---|---|
| Entities in a file | `file.filerefs('Define', 'Function')` |
| References on an entity | `ent.refs('Call')`, `ent.refs('Useby')` |
| Metrics | `ent.metric(['CyclomaticStrict', 'CountLine'])` |
| Control flow | `ent.control_flow_graph()` |
| Whitespace / comments / formatting | the lexer (below) |
| Raw text (trailing whitespace, line endings) | `file.contents()` |

```python
lexer = file.lexer(lookup_ents=False)   # text only — skips entity resolution
lexer = file.lexer(lookup_ents=True)    # lex.ent() resolves identifiers

lex = lexer.first()
while lex:
    # lex.text(), lex.token(), lex.line_begin(), lex.column_begin(), lex.ent()
    lex = lex.next()
```

A check that walks the token list has no notion of where an expression ends, so
its characteristic bug is reading tokens that belong to something else. When one
false-positives, verify the walk halts at `&&` / `||`, tracks parenthesis depth,
and skips macro expansions and inactive preprocessor branches.

### Worked examples

The fastest way to learn the idioms is to read a check that already does what you
need:

| For | Read |
|---|---|
| Unused-entity detection (`refs('Useby, Callby')`) | `STI_UNUSED` |
| Recursion via the call graph (`refs('Callby')`) | `AC_01` |
| Naming-convention regex on `ent.name()` | `CPP_N002` |
| Indentation from lexeme columns | `CPP_ST004` |
| Comment alignment and placement | `CPP_CM000` |
| Parameter modification (`kind().check('Modifyby, Setby')`) | `CPP_F023` |
| Metrics via `ent.metric()` | `METRIC_13` |
| Loop structure via lexeme traversal | `CPP_C015` |
| Control-flow triviality (`control_flow_graph()`) | `STI_SPECIAL_MEMBER_FUNCTIONS` |

Locate any of them with `grep -rl "'<ID>'" --include=check.upy .`

## Options

Make thresholds configurable rather than hard-coded. `check.options()` and
`check.option()` are the same object — plural is the more common spelling.

```python
def define_options(check):
    check.options().checkbox('strict', 'Also flag protected members', False)
    check.options().integer('limit', 'Maximum name length', 31)
    check.options().text('allowed', 'Allowed names (space-separated)', '')
    check.options().choice('arch', 'Target width', ['16 bit', '32 bit', '64 bit'], '32 bit')
    check.options().radio_vert('mode', 'Mode', ['Strict', 'Relaxed'], 'Strict')

def check(check, file):
    limit = check.options().lookup('limit')
```

`lookup()` is the only accessor. Cover option combinations in tests with
`uvalid_options_<word>.json` files — see
[Conditional annotations](CLAUDE-testing.md#conditional-annotations).

**Fixit hints.** Capture the violation, then attach a replacement range:

```python
violation = check.violation(ent, file, line, col, ERR1)
violation.add_fixit_hint(line, col, line, col, 'int ')            # insert
violation.add_fixit_hint(line, col, line, col + len(old), 'new')  # replace
```

A zero-width range inserts; a non-empty range replaces.

