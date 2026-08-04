Thomas McCabe introduced Cyclomatic Complexity, along with Essential Complexity, in his 1976 paper "A Complexity Measure" [1]. Cyclomatic Complexity counts the decision points in a function plus one; Essential Complexity does the same after iteratively replacing well-structured control structures (like `if`/`else` and loops) with a single statement, isolating the "unstructured" part of the complexity. See [Interpreting Cyclomatic Complexity &#8599;](https://docs.scitools.com/help/metrics/cyclomatic-complexity-meaning.html#interpreting-the-value) for risk-threshold guidance (such as the SEI's commonly cited bands) on reading the values these metrics produce.

## McCabe Metrics

| Name | Acronym | Understand Metric ID |
|------|---------|-----------------------|
| Cyclomatic Complexity | CC | [Cyclomatic](und://plugin/metric/Cyclomatic) |
| Strict Cyclomatic Complexity | CC2 | [CyclomaticStrict](und://plugin/metric/CyclomaticStrict) |
| Modified Cyclomatic Complexity | CC3 | [CyclomaticModified](und://plugin/metric/CyclomaticModified) |
| Essential Complexity | ev(G) | [Essential](und://plugin/metric/Essential) |

Notes:

1. [Strict Modified Cyclomatic Complexity](und://plugin/metric/CyclomaticStrictModified) and [Strict Modified Essential Complexity](und://plugin/metric/EssentialStrictModified) combine both the Strict and Modified adjustments. Understand computes them for convenience, but they aren't named/numbered variants in McCabe's own work, so they're left out of the table above and not tagged as part of this suite.
2. Knot Count ([Knots](und://plugin/metric/Knots), and its [Max](und://plugin/metric/MaxEssentialKnots)/[Min](und://plugin/metric/MinEssentialKnots) essential-complexity variants) measures a related kind of control-flow complexity, but it comes from Woodward, Hennell, and Hedley [2], not McCabe, so it also isn't part of this suite.

## Aggregated Values

Each metric above is also available summed, maxed, and averaged across a class, file, architecture, or the whole project:

| Metric | Sum | Max | Average |
|--------|-----|-----|---------|
| Cyclomatic Complexity | [SumCyclomatic](und://plugin/metric/SumCyclomatic) | [MaxCyclomatic](und://plugin/metric/MaxCyclomatic) | [AvgCyclomatic](und://plugin/metric/AvgCyclomatic) |
| Strict Cyclomatic Complexity | [SumCyclomaticStrict](und://plugin/metric/SumCyclomaticStrict) | [MaxCyclomaticStrict](und://plugin/metric/MaxCyclomaticStrict) | [AvgCyclomaticStrict](und://plugin/metric/AvgCyclomaticStrict) |
| Modified Cyclomatic Complexity | [SumCyclomaticModified](und://plugin/metric/SumCyclomaticModified) | [MaxCyclomaticModified](und://plugin/metric/MaxCyclomaticModified) | [AvgCyclomaticModified](und://plugin/metric/AvgCyclomaticModified) |
| Essential Complexity | [SumEssential](und://plugin/metric/SumEssential) | [MaxEssential](und://plugin/metric/MaxEssential) | [AvgEssential](und://plugin/metric/AvgEssential) |

## References

1. McCabe, Thomas J. "A Complexity Measure." IEEE Transactions on Software Engineering, vol. SE-2, no. 4, 1976, pp. 308-320.
2. Woodward, M.R., M.A. Hennell, and D. Hedley. "A Measure of Control Flow Complexity in Program Text." IEEE Transactions on Software Engineering, vol. SE-5, no. 1, 1979, pp. 45-50.
