The intent of this standard is to help provide a consistent coding environment for us all to work in. Our engineers are likely to make fewer mistakes in such an environment and are more likely to understand what's going on in each others' code. The hope is that these standards will annoy everyone equally and not squelch people's creativity. With that in mind, some of these standards may be thought of as strong suggestions rather than hard rules, but make sure you have a strong, defensible position if you choose to use your own style. This standard applies to all C/C++ code.

# Names
   * Classes: upper camel case (e.g. MyClass)
   * Functions: lower camel case (e.g. setValue())
   * Member Variables: lower camel case starting with 'm' (e.g. mValue)
   * Static Variables (including static members, file static, & local static): lower camel case starting with 's' (e.g. sSharedValue)
   * Constant Variables: lower camel case starting with 'k' (e.g. kMessageText)
   * Enumeration Literals: upper camel case with an additional suggestion that each enum value be prefixed with a common name that resembles the enum name (e.g. enum Alignment { AlignLeft, AlignRight };)
   * Namespaces: "STI" namespace should be used in all new code. Additionally it's good practice to use a namespace for each library (e.g. STI::VisualStudio). Avoid `using namespace` directives.

# Files
   * There must be a copyright notice at the top of all source files.
   * Add recursive include guards to the top of all header files. The recommended format is like the following:
     ```
     #ifndef STI_MAINTAIN_VISUALSTUDIO_PROJECT_H
     #define STI_MAINTAIN_VISUALSTUDIO_PROJECT_H
        ...
     #endif
     ```
   * There should only be one public class per file, with a header file of the same name as the class. You may create private classes within a code file if needed.
   * Header files should be self-contained (i.e. do not require the user to include some other header first).
   * Header files should not have any restriction requiring any certain include order when including the file with other header files.
   * Header files should include as few other headers as possible (i.e. forward declare classes when possible instead of including other headers). This doesn't apply to third-party (e.g. Qt) headers that change infrequently. They *should* be included in headers, especially when they define templates.
   * Avoid the use of *.inl files. Inline functions should be done "in place" for trivial cases and at the end of the header file for less trivial cases.

# Documentation
   * All functions should be documented with comments.
   * Public/protected functions should be documented above the declaration.
   * Private functions may be documented above the declaration or above the definition.

# Formatting
   * Use spaces rather than tabs for indentation.
   * Indentation should be two spaces.
   * Limit line length to 80 characters unless breaking them up makes them harder to read.

# Whitespace
   * Avoid trailing whitespace *even on otherwise blank lines*.
   * Add a space between control statement keywords and the opening parentheses:
```c++
if(a == b) // Wrong!
  ...
```

# Curly Braces
   * Opening curly braces should be on the same line as the if/else/for/while/do/switch statement:
```c++
if (a == b) {
  ...
}
```
   * The same applies to constructs such as class/struct/enum and namespace declarations:
```c++
class Foo : public Bar {
public:
  ...
}
```
   * Function definitions are an exception. Opening braces should appear on their own line:
```c++
void Foo::foo(
  int param1,
  int param2)
{
  ...
}
```
   * Always separate the brace from the remainder of the line with a space:
```c++
if (...){ // Wrong!
```
   * Omit curly braces when possible, except where explicit braces aid readability:
```c++
if (...)
  trivialStatement();
```
   * Avoid mixing explicit and omitted curly braces in the same if-else statement:
```c++
// Wrong!
if (...) {
  ...
} else
  trivialStatement();
```

# Pointer and Reference Types
Attach pointer (`*`) and reference (`&`) modifiers to the names that they modify:

```c++
int *i;
int* j; // Wrong!
int * k; // Wrong!
```

This guideline is motivated by the C/C++ declaration syntax. The `*` and `&` belong to the declarator, not the type specifier. That means that if you declare multiple variables in the same declaration, each variable requires its own modifier:

```c++
int *i, *j, k; // k is not a pointer!
```

# Assert
Verify pre-conditions, post-conditions, and invariants with the `Q_ASSERT` macro

# Include Order
Include header files in sorted sections in the following order with each section separated by a space:

1. Related header
2. Standard library headers
3. Third-party library headers (Qt)
4. Headers from other libraries
5. Headers from the current library
6. Conditional includes

Including the related header first ensures that it's self-contained. If it ever accidentally becomes dependent on include order, compilation of this file is guaranteed to fail.

In C++ files, prefer to include the C++ variants of C library headers (e.g. `cstddef` instead of `stddef.h`). If it's necessary to include C library headers with the .h extension, they can be split into their own section.

If there are multiple groups of third-party libraries that use angle bracket includes, they can be split into their own sections.

A typical example would look like this:

```c++
#include "MyClass.h"

#include <cstddef>
#include <vector>

#include <QDir>
#include <QMap>
#include <QString>

#include "api/lib1/Lib1ApiClass.h"
#include "api/lib2/Lib2ApiClass.h"
#include "ui/lib1/Lib1UiClass.h"

#include "MyLibClass1.h"
#include "MyLibClass2.h"

#ifdef Q_OS_WIN
#include <windows.h>
#endif
```
