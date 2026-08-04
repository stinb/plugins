Shyam Chidamber and Chris Kemerer proposed a suite of six object-oriented design metrics in their 1994 paper "A Metrics Suite for Object Oriented Design" [1], covering complexity, inheritance, coupling, and cohesion at the class level.

## Chidamber & Kemerer Metrics

| Name | Acronym | Understand Metric ID |
|------|---------|-----------------------|
| Weighted Methods per Class | WMC | [CountDeclMethod](und://plugin/metric/CountDeclMethod) |
| Weighted Methods per Class (complexity-weighted) | WMC | [SumCyclomatic](und://plugin/metric/SumCyclomatic) |
| Depth of Inheritance Tree | DIT | [MaxInheritanceTree](und://plugin/metric/MaxInheritanceTree) |
| Number of Children | NOC | [CountClassDerived](und://plugin/metric/CountClassDerived) |
| Coupling Between Objects | CBO | [CountClassCoupled](und://plugin/metric/CountClassCoupled) |
| Response for a Class | RFC | [CountDeclMethodAll](und://plugin/metric/CountDeclMethodAll) |
| Lack of Cohesion in Methods | LCOM | [PercentLackOfCohesion](und://plugin/metric/PercentLackOfCohesion) |

Notes:

1. Chidamber & Kemerer's original WMC definition weights each method by a complexity metric of the implementer's choosing, without specifying one. Understand offers two interpretations: [CountDeclMethod](und://plugin/metric/CountDeclMethod) counts methods unweighted (every method has weight 1), while [SumCyclomatic](und://plugin/metric/SumCyclomatic) weights each method by its own [Cyclomatic Complexity](und://plugin/metric/Cyclomatic) before summing, which is the more common weighting used in practice.
2. [CountDeclMethodAll](und://plugin/metric/CountDeclMethodAll) is also tagged as [Lorenz & Kidd](und://plugins/Solutions/.doc/manager_lorenzkidd.md?filter=Suite%3A%22Lorenz%20%26%20Kidd%22)'s Number of Methods (NM), since RFC's simplified form (methods in the class, ignoring the methods it calls elsewhere) and Lorenz & Kidd's NM count the same thing.
3. [PercentLackOfCohesion](und://plugin/metric/PercentLackOfCohesion) also goes by LCOM2, Chidamber & Kemerer's refined, percentage-based cohesion measure, as opposed to their original 1991 pairwise LCOM.

## References

1. Chidamber, Shyam R., and Chris F. Kemerer. "A Metrics Suite for Object Oriented Design." IEEE Transactions on Software Engineering, vol. 20, no. 6, 1994, pp. 476-493.
