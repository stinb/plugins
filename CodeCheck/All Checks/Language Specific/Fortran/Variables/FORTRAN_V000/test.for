      SUBROUTINE COUNT
      IMPLICIT NONE
      INTEGER :: N = 0 ! UndCC_Violation
      LOGICAL :: FIRST = .TRUE. ! UndCC_Violation
      INTEGER :: M = 0 ! UndCC_Valid
      SAVE M
      IF (FIRST) THEN
        FIRST = .FALSE.
      ENDIF
      N = N + 1
      M = M + 1
      END
