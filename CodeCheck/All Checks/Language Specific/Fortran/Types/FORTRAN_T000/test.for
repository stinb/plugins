      SUBROUTINE OLD(X)
      DOUBLE PRECISION X, Y
      DATA Y /1D2/ ! UndCC_Violation
      X = 1.D3 ! UndCC_Violation
     &  + 2D0 ! UndCC_Violation
      WRITE(*, 100) X, Y
  100 FORMAT(2D12.4, 3HD12) ! UndCC_Valid
      X = 1.0E3 ! UndCC_Valid
      END
