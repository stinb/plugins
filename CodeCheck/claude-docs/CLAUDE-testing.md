# Testing CodeCheck Checks

Writing checks is in [CLAUDE-authoring.md](CLAUDE-authoring.md).

## Test files

- Test files **must** be in the same folder as `check.upy`, or a descendant of it.
- Name them for the language: `test.c`, `test.cpp`, `test.java`, `test.py`.
  Multiple files are fine when a rule needs several translation units.
- Annotate the expected result at the **end of the line** it applies to, prefixed
  with the target language's comment marker — `//` for C/C++/Java/C#, `#` for
  Python, `--` for Ada/VHDL.

| Annotation | Meaning |
|---|---|
| `UndCC_Violation` | The check should flag this line. |
| `UndCC_Valid` | The check should NOT flag this line. |
| `UndCC_FalseNeg` | Known false negative (should flag, doesn't yet). |
| `UndCC_FalsePos` | Known false positive (flags, shouldn't). |

Nothing changes when a line has more than one violation — one annotation covers
the line.

```c
void setValue(void);   // UndCC_Valid
void Set_Value(void);  // UndCC_Violation
```

```python
def set_value(): pass   # UndCC_Valid
def SetValue(): pass    # UndCC_Violation
```

### Conditional annotations

Any annotation takes a parenthesized argument list narrowing *when* it applies.
It is **never** a count of violations.

| Argument | Meaning |
|---|---|
| `lin`, `mac`, `win` | Expected only on those platforms. |
| any other word | Expected only under the option set `uvalid_options_<word>.json`. |

The option-set word is the `<word>` from a `uvalid_options_<word>.json` file
beside `check.upy`. It can be a number (`uvalid_options_0.json` → `(0)`) or a
name (`uvalid_options_strict.json` → `(strict)`); names are the more common form.
The two kinds combine: `UndCC_Violation(win(0))` is Windows under option-set `0`.
Add one options file per combination you want covered:

```
CPP_L004/
├── check.upy
├── test.c                     # UndCC_Violation(0)  UndCC_Violation(1)
├── uvalid_options_0.json
└── uvalid_options_1.json
```

Each file is a flat map of option name to value:

```json
{
    "oneViolation": false,
    "functions": "system"
}
```

```c
code[5] = '\109';      // UndCC_Violation(0)     option-set 0 only
x = y >> 29;           // UndCC_Violation(1,2)   option-sets 1 and 2
size_t n = f();        // UndCC_Violation(allow_size_t)
```

### Comment rules

- Don't add redundant explanations like "should be flagged" — the annotation
  already says that. Brief technical notes on *why* are valuable ("cast to wider
  essential type").
- When adding cases to an existing test file, **don't reformat or modify the
  existing code or comments** — only add.

Cover at least one violation and one compliant case, plus edge cases likely to
confuse the check (nested parens, macros, logical operators, boundary values).

## Running tests

`uvalid` builds a throwaway project per check, runs it, and compares the result
against the annotations. It writes a CSV of results to stdout; logs, warnings,
and errors go to stderr.

```bash
uvalid -V <checkID>                 # one check, verbose
uvalid -V "MISRA04*" CPP_T069       # ID wildcards and several checks at once
uvalid -V                           # every check — slow
uvalid <checkID> -o results.csv     # CSV to a file instead of stdout
```

`-V` (`--verbose`) outputs all CSV columns and all log messages — use it while
developing. Run from anywhere; no need to `cd` to the check folder.

| Output | Meaning |
|---|---|
| `Tests Passed: N` | Everything matched. |
| `Tests Failed: N` | One or more assertions failed. |
| `Violations Missing` | A `UndCC_Violation` line wasn't flagged. |
| `Violations Unexpected` | A `UndCC_Valid` line was flagged — a false positive. |
| `Analysis Errors: N` | A test file didn't parse, so **no tests ran on it**. |

`print()` inside a check appears as Debug output — useful while iterating.

**Analysis errors must be fixed.** Every run should show `Analysis Errors: 0`.
Two very different causes:

- **The test file.** Usually an `#include` of a standard-library header that
  isn't available in the lightweight test environment. Make the file
  self-contained: drop the include and use plain equivalents
  (`std::int32_t` → `int`, `std::size_t` → `unsigned long`,
  `std::move(x)` → `static_cast<T&&>(x)`).
- **The local toolchain.** If errors appear on test files that look fine —
  especially across many unrelated checks at once — suspect a missing C++
  standard-library development package rather than the tests.

If a check needs specific analysis settings, put a `uvalid_commands.txt` beside
`check.upy` holding `und` commands, one per line:

```
settings -c++usestrict off
```

## Real-project false-positive testing

Unit tests miss false positives that real code finds. After writing or fixing a
check, run it against a large project.

**1.** Write a config JSON anywhere — `und codecheck` accepts a path, so it does
not need to be installed in the project:

```json
{
  "automatic": false, "can_baseline": true,
  "checks": ["MISRA23_7.0.5"], "excluded_checks": [], "excludes": [],
  "name": "MISRA23_7.0.5", "description": "", "parents": [], "tags": []
}
```

Dropping it in `<db>.und/codecheck/configs/` instead lets you pass the bare name.

**2.** Run it:

```bash
und codecheck my-config.json /tmp/results -db /path/to/project.und
```

This writes `/tmp/results/results.sarif` (the full record) plus the default report
`CodeCheckResultsByTable.csv` — columns File, Violation, Line, Column, Entity,
Kind, CheckID. `-reports all` generates every report, in csv, pdf and html.

**3.** **Verify flagged lines against the actual source** — don't trust the
output. Typical false-positive shapes, depending on what the check does: the
entity sitting on a different line than reported (scanner overrun), the wrong
kind of entity flagged, an explicit cast not recognized, a literal misclassified
(hex `0xF` matching a float suffix), operands that are actually the same type.

**4.** Group failures by root cause rather than patching one-offs — usually
depth-tracking bugs (the scanner doesn't stop at the right boundary),
unrecognized type patterns, literal parsing, or unhandled constructs (C-style
casts, member chains, templates).

**5.** Fix, re-run `uvalid -V <checkID>`, then re-scan against the previous run:

```bash
und codecheck -previous /tmp/results/results.sarif my-config.json /tmp/results2 \
  -db /path/to/project.und
```

That adds `New_CodeCheckResultsByTable.csv` and
`Fixed_CodeCheckResultsByTable.csv` to the output — far more useful than
comparing row counts.

### A reported false positive

A reported false positive may not be one — read the flagged code against the
rule's own wording before accepting the report. Marking a true positive
`UndCC_Valid` bakes a wrong expectation into the test suite and turns the "fix"
into a false negative. Once you've confirmed it's genuinely wrong, reproduce it as
a `UndCC_Valid` case before changing any logic; that case stays behind as the
regression guard.

