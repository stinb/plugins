      SUBROUTINE PITFALL()
      IMPLICIT NONE
      REAL, POINTER :: X
      REAL, TARGET :: Y
      Y = 0.
      IF (ASSOCIATED(X)) WRITE(*,*) X ! UndCC_Violation
      X => Y
      IF (ASSOCIATED(X)) WRITE(*,*) X ! UndCC_Valid
      END SUBROUTINE PITFALL
