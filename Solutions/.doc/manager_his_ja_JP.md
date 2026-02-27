
Hersteller Initiative Software（HIS）メトリクスは、コード品質を保証するために自動車業界で策定されたメトリクスとしきい値の一覧です。英語によるドキュメントは [https://emenda.com/his/ &#8599;](https://emenda.com/his/) にあります。

![image](../../../IReport/.doc/his_metrics.png)

*The [HIS Metrics](und://plugin/ireport/HIS%20Metrics) インタラクティブ レポート*

Understand では、これらのメトリクスにアクセスする方法として [インタラクティブ レポート](und://plugin/ireport/HIS%20Metrics) が用意されています。インタラクティブレポートとメトリクスプラグインについては、[ブログ記事 &#8599;](https://blog.scitools.com/hersteller-initiative-software-his-metrics/) で解説されています。

## HIS メトリクス

| 名前                                  | 頭字語     |  Understand メトリクス ID                                                             |
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


注意:
1. このメトリクスはUnderstandに標準で付属しています。
2. 表示するサイクロマティック複雑度メトリクスはプロジェクトの設定にて変更できます。
3. このメトリクスは最終リターンを除いた関数からの明示的な終了/リターンの回数を表示します。そのため、値は1つ少なくなります。
4. すべての変更系メトリクスはステートメント単位ではなく行単位で算出されます。比較メトリクスを計算するには、比較用データベースを設定する必要があります。
5. このメトリクスは、バックグラウンドで実行されている [MISRA HIS configuration](und://plugin/config/MISRA%20HIS%20Subset) に依存します。
6. このメトリクスはサポートされていません。代替として、[MISRA HIS configuration](und://plugin/config/MISRA%20HIS%20Subset) を実行し、「Results By Check Report」を生成すると、各ルールの違反件数が表示されるため、同様の出力を得られます。
7. このメトリクスは、非再帰関数に対しては0を、直接再帰関数に対しては1を、間接再帰関数に対しては2を返します。
8. これらのメトリクスは [https://emenda.com/his/ &#8599;](https://emenda.com/his) には掲載されていませんが、[https://www.imagix.com/user_guide/his-+-misra-c.html &#8599;](https://www.imagix.com/user_guide/his-+-misra-c.html) には掲載されています。

