module consts
  integer, parameter :: dp = selected_real_kind(15)
  real(dp), parameter :: big = 1.d30 ! UndCC_Violation
contains
  subroutine literals(x)
    real(dp), intent(out) :: x
    character(10) :: s
    integer :: i

    x = sqrt(1.d3) ! UndCC_Violation
    x = 1D0 ! UndCC_Violation
    x = 2.5d-1 ! UndCC_Violation
    x = .5d0 ! UndCC_Violation
    x = 3d+2 ! UndCC_Violation
    x = 1.d0_dp ! UndCC_Violation
    if (x.eq.1.d0) x = 2. ! UndCC_Violation
    x = 1.e3_dp ! UndCC_Valid
    x = 1.0_dp ! UndCC_Valid
    x = 1.e3 ! UndCC_Valid
    x = 2.5 ! UndCC_Valid
    i = 10 ! UndCC_Valid
    i = int(z'1d0') ! UndCC_Valid
    s = '1.d3' ! UndCC_Valid
    print '(d12.4)', x ! UndCC_Valid
    write (*, 100) x ! UndCC_Valid
100 format (2d12.4, 1pd12.4, d10.2)
    x = 1.; x = 2d0 ! UndCC_Violation
200 format (a); x = 3d0 ! UndCC_Violation
  end subroutine
end module
