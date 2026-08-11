# Published Standards Work

Check IDs and categories are in [CLAUDE.md](CLAUDE.md).

## Aliasing an existing rule

Standards overlap heavily — a "new" rule often matches a check that already
exists. Search both Python and Clang checks first. If one covers the same rule,
add an **alias**, not a new check:

- **Python:** add the new ID to the existing check's `ids()` and to the
  `name()`/`tags()`/`detailed_description()` dicts.
- **Clang:** add an entry to the standard's JSON in `clang/` (`misra.json`,
  `autosar.json`, `sei-cert.json`, …), pointing at the same `key` (or `keys`) as
  the existing checker.
- Compare the two standards' wording. Identical → reuse the `desc`. Different at
  all → give the new ID its own description using that standard's exact wording.
- Carry over the language tags. If the existing check has both `Language: C` and
  `Language: C++`, the alias needs both.

### Noop placeholders shadow real checks

`noop/*.json` — one file per standard family (`misra.json`, `ada.json`,
`autosar.json`, `cwe.json`, `sei-cert.json`, …) — registers placeholder entries
for rules that have no automatable check.

A noop entry and a real check with the same ID cannot coexist: **the empty noop
wins.** The check reports 0 violations, `check()` never runs, nothing prints, and
`uvalid` reports the check "Aborted".

So when turning a noop rule into a real check, delete its noop entries **before
testing**, and check every noop file — one rule is often noop'd under several
standards at once (MISRA12 / MISRA23 / MISRA25). Re-run `uvalid` *after* deleting;
the noop keeps shadowing the real check until it's actually gone.

