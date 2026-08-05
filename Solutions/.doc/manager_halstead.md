Maurice Halstead introduced a suite of source-code complexity measures in his 1977 book "Elements of Software Science" [1]. Halstead's approach treats a program as a sequence of tokens, splitting them into operators (e.g. keywords, punctuation, operator symbols) and operands (e.g. identifiers, literals), then derives length, volume, difficulty, and effort measures from counts of how many are distinct versus how many appear in total.

## Halstead Metrics

| Name | Symbol | Understand Metric ID |
|------|--------|-----------------------|
| Distinct Operators | n1 | [HalsteadDistinctOperators](und://plugin/metric/HalsteadDistinctOperators) |
| Distinct Operands | n2 | [HalsteadDistinctOperands](und://plugin/metric/HalsteadDistinctOperands) |
| Total Operators | N1 | [HalsteadTotalOperators](und://plugin/metric/HalsteadTotalOperators) |
| Total Operands | N2 | [HalsteadTotalOperands](und://plugin/metric/HalsteadTotalOperands) |
| Vocabulary | n | [HalsteadVocabulary](und://plugin/metric/HalsteadVocabulary) |
| Length | N | [HalsteadLength](und://plugin/metric/HalsteadLength) |
| Volume | V | [HalsteadVol](und://plugin/metric/HalsteadVol) |
| Difficulty | D | [HalsteadDifficulty](und://plugin/metric/HalsteadDifficulty) |
| Effort | E | [HalsteadEffort](und://plugin/metric/HalsteadEffort) |

Notes:

1. Vocabulary (n = n1 + n2), Length (N = N1 + N2), Volume (V = N &times; log&#8322;n), Difficulty (D = (n1 &divide; 2) &times; (N2 &divide; n2)), and Effort (E = D &times; V) are all derived from the four base counts above.
2. [Halstead Maintainability Index](und://plugin/metric/HalsteadMaintainIdx) also builds on Halstead Volume, but isn't part of Halstead's own suite &mdash; it comes from Coleman, Ash, Lowther, and Oman [2], combining Volume with [Cyclomatic Complexity](und://plugin/metric/Cyclomatic) and [Code Lines](und://plugin/metric/CountLineCode), which come from other suites.

## References

1. Halstead, Maurice H. "Elements of Software Science." Elsevier North-Holland, 1977.
2. Coleman, Don, Dan Ash, Bruce Lowther, and Paul Oman. "Using Metrics to Evaluate Software System Maintainability." IEEE Computer, vol. 27, no. 8, 1994, pp. 44-49.
