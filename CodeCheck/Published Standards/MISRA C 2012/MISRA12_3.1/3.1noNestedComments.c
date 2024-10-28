/* some comment, end comment marker accidentally omitted // UndCC_Violation
<<New Page>>
Perform_Critical_Safety_Function( X );
/* this comment is non-compliant */

void func() {
  int x,y,z;
x = y 	// /* // UndCC_Violation
+ z
// */
;
}
