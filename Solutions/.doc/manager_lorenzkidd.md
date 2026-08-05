Mark Lorenz and Jeff Kidd catalogued a suite of object-oriented design metrics in their 1994 book "Object-Oriented Software Metrics: A Practical Guide" [1], covering class size, structure, and coupling. Understand implements the class-size measures from that suite.

## Lorenz & Kidd Metrics

| Name | Acronym | Understand Metric ID |
|------|---------|-----------------------|
| Number of Methods | NM | [CountDeclMethodAll](und://plugin/metric/CountDeclMethodAll) |
| Public Instance Methods | PIM | [CountDeclMethodPublic](und://plugin/metric/CountDeclMethodPublic) |
| Number of Instance Methods | NIM | [CountDeclInstanceMethod](und://plugin/metric/CountDeclInstanceMethod) |
| Number of Class Methods | NCM | [CountDeclClassMethod](und://plugin/metric/CountDeclClassMethod) |
| Number of Class Variables | NCV | [CountDeclClassVariable](und://plugin/metric/CountDeclClassVariable) |
| Number of Instance Variables | NIV | [CountDeclInstanceVariable](und://plugin/metric/CountDeclInstanceVariable) |
| Number of Friends | NF | [CountDeclMethodFriend](und://plugin/metric/CountDeclMethodFriend) |
| Average Method Size | AMS | [AvgCountLineCode](und://plugin/metric/AvgCountLineCode) |

Notes:

1. [CountDeclMethodAll](und://plugin/metric/CountDeclMethodAll) is also tagged as Chidamber & Kemerer's Response for a Class (RFC), since Lorenz & Kidd's Number of Methods and Chidamber & Kemerer's simplified RFC (ignoring the methods called by a class) count the same thing.
2. [CountDeclMethodFriend](und://plugin/metric/CountDeclMethodFriend) also goes by Number of Friend Methods (NFM) in Lorenz & Kidd's own naming.
3. [CountDeclMethodPublic](und://plugin/metric/CountDeclMethodPublic) also goes by Number of Public Methods (PM, NPM) in industry usage. Lorenz & Kidd's PIM counts public instance methods only; Understand's metric also includes public static/class methods.

## References

1. Lorenz, Mark, and Jeff Kidd. "Object-Oriented Software Metrics: A Practical Guide." Prentice Hall, 1994.
