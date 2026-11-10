      SUBROUTINE HELPER(I,J)
      IMPLICIT NONE
      INTEGER,INTENT(IN) :: I
      INTEGER,INTENT(OUT) :: J ! UndCC_Violation

      IF(I==1) THEN
        J = 0
      ENDIF
      END SUBROUTINE HELPER

      SUBROUTINE BOTH(I,J)
      IMPLICIT NONE
      INTEGER,INTENT(IN) :: I
      INTEGER,INTENT(OUT) :: J ! UndCC_Valid

      IF(I==1) THEN
        J = 0
      ELSE
        J = 1
      ENDIF
      END SUBROUTINE BOTH
