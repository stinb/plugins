#include <../include/lib_mod.f90> /* UndCC_Violation */

subroutine includes()
#include <inc.fi> /* UndCC_Violation */
#include "inc.fi" /* UndCC_Valid */
#include <sub/sub.fi> /* UndCC_Violation */
#  include   <inc.fi> /* UndCC_Violation */
#include <x.fi> /* UndCC_Violation: found through the include path */
#include <../../projext/ext.fi> /* UndCC_Valid: outside the project */
#include <missing.fi> /* UndCC_Valid: not found */
  include 'inc.fi' ! UndCC_Valid
end subroutine
