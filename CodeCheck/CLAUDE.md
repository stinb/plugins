# CodeCheck Plugins

CodeCheck is Understand's static-analysis engine. A *check* inspects analyzed
code and reports **violations**. Checks are written in Python (`check.upy`, most
common), Clang AST (C++, for type-aware C/C++ rules), or Perl (`check.upl`,
legacy — don't write new ones).

**API docs — consult these first:**
- Python API: https://docs.scitools.com/manuals/python/index.html
- CodeCheck API: https://docs.scitools.com/manuals/python/codecheck.html

## Read before you start

This file holds only what applies to every task. **Read the file for the work you
are about to do — don't work from this file alone.**

| Doing this | Read first |
|---|---|
| Writing a new Python check | [CLAUDE-authoring.md](claude-docs/CLAUDE-authoring.md) |
| Fixing or extending an existing check | [CLAUDE-authoring.md](claude-docs/CLAUDE-authoring.md) |
| Choosing Python vs Clang, or writing a Clang check | [CLAUDE-clang.md](claude-docs/CLAUDE-clang.md) |
| Writing test files, or running `uvalid` | [CLAUDE-testing.md](claude-docs/CLAUDE-testing.md) |
| Investigating a false positive | [CLAUDE-testing.md](claude-docs/CLAUDE-testing.md) |
| Adding an alias, or converting a noop to a real check | [CLAUDE-standards.md](claude-docs/CLAUDE-standards.md) |
| Writing C/C++ in a Clang checker | [coding_standards.md](claude-docs/coding_standards.md) |

For local-only notes — environment setup, machine-specific paths, test-project
locations — put them in a gitignored `CLAUDE.local.md` in this directory. Claude
Code loads it automatically alongside this file, so it needs no reference here.

## Check IDs and categories

C/C++ IDs are `CPP_<prefix><number>`. Python and Clang checks **share one ID
space** — search the whole `plugins/CodeCheck/` tree for the next free number,
not just one subfolder:

```bash
grep -roh 'CPP_CF[0-9]\+' plugins/CodeCheck/ | sort -uV | tail -5
```

Substitute your prefix. Use `sort -uV`; a `sort -t <letter>` form silently
misreports the maximum for any prefix not containing that letter.

The prefix follows the category: a single letter for the category it uniquely
abbreviates, two letters to disambiguate categories sharing that letter. Pick
the category first, then the prefix.

| Category | Prefix | Older checks also use |
|---|---|---|
| Assembly | `CPP_A` | |
| Assignment Operators | `CPP_AO` | `CPP_A` |
| Bit-fields | `CPP_B` | |
| Comments | `CPP_CM` | `CPP_C` |
| Constructors | `CPP_C` | |
| Control Flow | `CPP_CF` | `CPP_C` |
| Declarations and Definitions | `CPP_DD` | `CPP_D` |
| Destructors | `CPP_D` | |
| Exception Handling | `CPP_EH` | `CPP_E` |
| Expressions | `CPP_E` | |
| Functions | `CPP_F` | |
| Headers | `CPP_H` | |
| Identifiers | `CPP_I` | |
| Libraries | `CPP_L` | |
| Memory Allocation | `CPP_M` | |
| Naming Conventions | `CPP_N` | |
| Pointers | `CPP_P` | |
| Preprocessor | `CPP_PR` | `CPP_P` |
| Structures and Unions | `CPP_S` | |
| Styling | `CPP_ST` | |
| Types | `CPP_T` | |
| Unnecessary Constructs | `CPP_U` | |
| Variables | `CPP_V` | |
| Virtual Functions | `CPP_VF` | `CPP_V` |
| Warnings | — | no `CPP_` prefix in use |

The third column is history, not a choice: those checks predate the two-letter
prefix. **Use the middle column for new checks**, and don't infer the convention
from a neighboring file.

**Do not create new categories** — these 25 are the directory names under
`All Checks/Language Specific/C and C++/`. Verify spelling against the directory
listing rather than copying from another check; misspellings exist in the tree
(`Unnecessary Contructs`, `Definitions and Declarations`) and will silently
create a stray node.

Non-C/C++ languages use their own prefixes and ID space (`PYTH_*`, `JAVA_*`,
`ADA95_*`, …) and their own category trees under
`All Checks/Language Specific/<Language>/`.

## Conventions

- **Never commit unless asked.** Present the proposed commit message and wait.
- Commit message for a new check, an alias, or a noop entry — always the same
  form, using the newly added ID:
  `Checks: Added new <checkid>: <check name>`
  Don't describe how it was implemented ("noop", "compiler-enforced"). The
  convention names the check that was added, not how it was realized.
- Commit message for a check fix names the ID and the wrong behavior users saw:
  `Checks: Fixed: <checkid> <what it was wrongly doing> #NNNN`
  Lead with the symptom, not the internal cause.
- **Do standard-coverage work one check at a time.** Implement one, verify it with
  `uvalid`, present it with its commit message, then stop and wait for the commit
  before starting the next — even when several are mechanical and verified clean.
  Batching produces one tangled working tree that can't be split into separate
  logical commits.
- Keep code comments short — state the rule or reason, not the full rationale.
- Don't reference GitHub issue/PR numbers in code comments; that belongs in the
  commit message.
- When removing code, remove whatever becomes unused with it (declarations,
  enums, helpers). Don't comment code out or leave dead code — it creates
  standards violations, and git history is the recovery path. Leave a short note
  saying what was removed and why.

