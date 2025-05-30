The Hersteller Initiative Software (HIS) metrics are a list of metrics and thresholds developed by the automotive industry to ensure code quality. The documentation for them available in English is at [https://emenda.com/his/ &#8599;](https://emenda.com/his/).

![image](../../../IReport/.doc/his_metrics.png)

*The [HIS Metrics](und://plugin/ireport/HIS%20Metrics) interactive report*

In Understand, the primary way of accessing these metrics has been through running the CodeCheck [configuration](und://plugin/config/Hersteller%20Initiative%20Software%20(HIS)%20Metrics). There is now also an [interactive report](und://plugin/ireport/HIS%20Metrics). The interactive report and metric plugins are described in a [blog article &#8599;](https://blog.scitools.com/hersteller-initiative-software-his-metrics/).

## HIS Metrics

| Name                                  | Acronym     |  Understand Metric ID                                                             |
|---------------------------------------|-------------|-----------------------------------------------------------------------------------|
| Comment Density                       | COMF        | [RatioCommentsWithBeforeToCode](und://plugin/metric/RatioCommentsWithBeforeToCode)|
| Number of paths                       | PATH        | CountPath [1,2]                                                                   |
| Number of Goto Statements             | GOTO	      | [CountGoTo](und://plugin/metric/CountGoTo)                                        |
| Cyclomatic Complexity                 | v(G)        | Cyclomatic [1,2]                                                                  |
| Number of Calling Functions           | CALLING     | [CountCallbyUnique](und://plugin/metric/CountCallbyUnique)                        |
| Number of called functions            | CALLS       | [CountCallsUnique](und://plugin/metric/CountCallsUnique)                          |
| Number of Function Parameters         | PARAM       | [CountParams](und://plugin/metric/CountParams)                                    |
| Number of Instructions per function   | STMT        | CountStmt [1]                                                                     |
| Number of call Levels                 | LEVEL       | [MaxCallDepth](und://plugin/metric/MaxCallDepth)                                  |
| Number of return points               | RETURN      | [CountEarlyExit](und://plugin/metric/CountEarlyExit) [3]                          |
| The stability index                   | S           | [StabilityIndex](und://plugin/metric/StabilityIndex) [4]                          |
| Language scope                        | VOCF        | [LanguageScope](und://plugin/metric/LanguageScope)                                |
| Number of MISRA HIS Subset violations | NOMV		    | [HISNOMV](und://plugin/metric/HISNOMV) [5]                                        |
| Number of MISRA violations per rule	  | NOMVPR      | [5,6]                                                                             |
| Number of recursions                  | ap_cg_cycle | [IsRecursive](und://plugin/metric/IsRecursive) [7]                                |
| Statements changed                    | SCHG        | CountLineChanged [1,4,8]                                                          |
| Statements deleted                    | SDEL        | CountLineRemoved [1,4,8]                                                          |
| New statements                        | SNew        | CountLineNew [1,4,8]                                                              |


Notes:
1. This metric is built-in to Understand
2. Visible cyclomatic metrics are controlled in project settings
3. The metric is only reporting early exits, not all exits, matching the corresponding CodeCheck check. So the expected value is one lower.
4. All change metrics are line based instead of statement based. A comparison database must be set to calculate comparison metrics.
5. This metric depends on the [MISRA HIS configuration](und://plugin/config/MISRA%20HIS%20Subset) running in the background.
6. This metric is not supported. Similar output is available by running the [MISRA HIS configuration](und://plugin/config/MISRA%20HIS%20Subset) and generating a "Results By Check Report". The report will show the number of violations for each rule.
7. This metric returns 0 for non-recursive functions, 1 for directly recursive functions, and 2 for indirectly recursive functions.
8. These metrics are not listed on [https://emenda.com/his/ &#8599;](https://emenda.com/his] but have been listed on [https://www.imagix.com/user_guide/his-+-misra-c.html &#8599;](https://www.imagix.com/user_guide/his-+-misra-c.html)

