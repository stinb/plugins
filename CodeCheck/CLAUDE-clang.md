# Clang AST Checks

Shared conventions are in [CLAUDE.md](CLAUDE.md); tests work the same as for
Python checks, see [CLAUDE-testing.md](CLAUDE-testing.md).

## Clang AST vs Python

**When asked to write a new C/C++ check, present the tradeoff for that specific
rule and let the user pick — don't choose unilaterally.**

| Need | Use |
|---|---|
| Naming, style, formatting, comments | **Python only** — Clang can't help |
| Entity relationships (call graphs, inheritance, unused) | **Python only** — Clang sees one translation unit |
| Metrics, control flow graph | **Python only** |
| Multi-language checks | **Python only** |
| Configurable options beyond booleans, fixit hints | **Python only** |
| Implicit casts / conversions / promotions | **Clang only** — Python is blind to these |
| Type size in bits, integer promotion rules | **Clang only** |
| Constant expression evaluation | **Clang only** |
| Templates, overload resolution, bit-field detection | **Clang only** |
| Real expression structure / operator precedence | **Clang strongly preferred** — Python must walk lexemes and guess |
| Type signedness, typedef resolution | Either (`isUnsignedIntegerType()` / `getCanonicalType()` vs `freetext('UnderlyingType')`) |

**The core tradeoff:** Python iterates in seconds (edit `.upy`, re-run `uvalid`)
but its expression analysis is fragile. Clang has precise semantics but needs a
recompile per change.

Rules that genuinely require Clang tend to share a shape: the violation depends
on something the source text only implies — an inserted conversion, a computed
width, an evaluated constant, a resolved overload.

## Clang checks

| What | Where |
|---|---|
| Checker implementation | `src/understand/codecheck/clang/YourChecker.cpp` |
| Registration | `src/understand/codecheck/clang/ClangChecks.cpp` |
| Build | `src/understand/codecheck/clang/CMakeLists.txt` |
| JSON (general) | `plugins/CodeCheck/clang/checks.json` |
| JSON (per standard) | `plugins/CodeCheck/clang/<standard>.json` |
| Tests | same as Python — the standard's check folder |

Start from an existing checker rather than a blank file:
`IntImplicitConversionSignednessChecker.cpp` (simple implicit-cast matching),
`BoolOperators.cpp` (expression type checks), `InappropriateNumericAssignment.cpp`
(complex, multiple matchers).

**Registration** — add to `ClangChecks.cpp`:

```cpp
void registerYourChecker(clang::ento::CheckerManager &Mgr);
bool shouldRegisterYourChecker(const clang::ento::CheckerManager &mgr);

registry.addChecker(&registerYourChecker, &shouldRegisterYourChecker,
                    "sti.YourChecker",
                    "", "",     // description, documentation — unused in Understand
                    false);     // IsHidden
```

Then add the file to the `add_library(sticlangchecks ...)` source list in
`CMakeLists.txt`, keeping the list alphabetical.

**JSON** — every Clang check needs an entry in `clang/checks.json`, plus the JSON
for its standard family if it implements a published rule (`misra.json`,
`autosar.json`, `sei-cert.json`, `power-of-ten.json`, … — 11 files in `clang/`).
Every entry for one checker points at the same `key`:

```json
"CPP_F###": {
  "tags": ["Language: C++", "Functions"],
  "key": "sti.YourChecker",
  "test": "MISRA23_X.X.X",
  "name": "All Checks/Language Specific/C and C++/Functions/Your check description",
  "desc": { "html": ["<p><b>Rationale</b></p>", "<p>Text</p>"] },
  "options": [
    { "id": "sti.YourChecker:AllowChained", "text": "Allow chained ops", "value": true }
  ]
}
```

Options are **boolean only**, read with
`Mgr.getAnalyzerOptions().getCheckerBooleanOption(Chk, "AllowChained")`. Omit the
field when the check has none — only three checks in the tree use it.

Use `keys` (an array) in place of `key` when one ID maps to several checkers.
These are often upstream clang-tidy or clang diagnostic names rather than `sti.`
checkers:

```json
"MISRA04_13.3": { "keys": ["float-equal", "sti.FloatingEqualityTest"], … }
```

**Style** — C/C++ house style is in `coding_standards.md`. The rules that trip
up new checkers:

1. **Braces:** function definitions get the opening brace on its own line —
   *always*, including short ones. Classes, namespaces, and control statements
   keep it on the same line. Never write an inline function body inside a class
   declaration; declare it there and define it out-of-class. (Only exception: a
   constructor with just an initializer list and empty body, `Foo() : m(0) {}`.)
2. **`[[maybe_unused]]`** on any public method a framework calls implicitly —
   CRTP callbacks like `VisitCallExpr`, `dataTraverseStmtPre`,
   `checkASTCodeBody`, `TraverseDecl`. Without it the style checker reports
   "unused public function". When unsure, add it to all overrides in
   `RecursiveASTVisitor` / `Checker` subclasses.
3. **No explicit constructors on simple structs** — use aggregate
   initialization (`vec.push_back({Kind, {}, nullptr})`). Explicit constructors
   show up as "unused public function".
4. **No duplicated blocks** — CI flags duplicated runs of ~13+ lines. Extract a
   helper the first time, don't copy-paste.
5. Keep the checker class in an anonymous namespace; use default member
   initializers (`ASTContext *Ctx = nullptr;`) over constructor init lists.

**Reducing false positives** — consider skipping constant expressions
(`expr->isEvaluatable(Ctx)`), macros (`expr->getBeginLoc().isMacroID()`),
bit-fields, and same-size type conversions.

**Build:** `ninja understand` (not individual targets like `ninja undcwork`).

