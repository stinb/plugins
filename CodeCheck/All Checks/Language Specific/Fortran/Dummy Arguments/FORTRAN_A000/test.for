      SUBROUTINE HELPER1(FSTRING)
      IMPLICIT NONE
      CHARACTER(LEN=*),INTENT(INOUT) :: FSTRING

      FSTRING = FSTRING//'.dat' ! UndCC_Violation
      END SUBROUTINE HELPER1

      SUBROUTINE HELPER2(N,FSTRING)
      IMPLICIT NONE
      INTEGER,INTENT(IN) :: N
      CHARACTER(LEN=N),INTENT(INOUT) :: FSTRING

      FSTRING = FSTRING//'.dat' ! UndCC_Violation
      END SUBROUTINE HELPER2

      SUBROUTINE HELPER3(FSTRING)
      IMPLICIT NONE
      CHARACTER(LEN=10),INTENT(INOUT) :: FSTRING

      FSTRING = FSTRING//'.dat' ! UndCC_Violation
      END SUBROUTINE HELPER3

      SUBROUTINE OLDSTYLE(A, B, C)
      CHARACTER*10 A
      CHARACTER*(*) B
      CHARACTER C*5

      A = A // 'x' ! UndCC_Violation
      B = 'pre' // ! UndCC_Violation(trunc)
     &    B
      C = 'abc' ! UndCC_Valid
      END
