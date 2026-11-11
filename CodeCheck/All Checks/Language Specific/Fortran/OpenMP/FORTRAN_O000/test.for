      SUBROUTINE PWD010
      IMPLICIT NONE
      INTEGER IRESULT(2, 2)
      INTEGER I, J

!$OMP PARALLEL DO SHARED(IRESULT)
      DO J = 1, 2
        DO I = 1, 2 ! UndCC_Violation
          IRESULT(I, J) = 0
        END DO
      END DO
      END SUBROUTINE PWD010

      SUBROUTINE LABELS(A, N)
      INTEGER N, A(N, N), I, J, K
C$OMP PARALLEL DO
C$OMP+SHARED(A)
      DO 20 J = 1, N
      DO 20 I = 1, N ! UndCC_Violation
        A(I, J) = 0
   20 CONTINUE
      DO 30 K = 1, N
        A(K, 1) = 0
   30 CONTINUE
*$OMP PARALLEL DO SHARED(A)
*$OMP+PRIVATE(I)
      DO 40 J = 1, N
      DO 40 I = 1, N
        A(I, J) = 0
   40 CONTINUE
      END
