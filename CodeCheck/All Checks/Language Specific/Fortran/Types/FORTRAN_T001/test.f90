module precisions
  implicit double precision (a-h) ! UndCC_Violation
  integer, parameter :: dp = selected_real_kind(15)

  type :: t
    double precision :: c ! UndCC_Violation
    real(dp) :: d ! UndCC_Valid
  end type

  type :: pair
    real :: precision ! UndCC_Valid
  end type
contains
  double precision function f(x) ! UndCC_Violation
    doubleprecision, intent(in) :: x ! UndCC_Violation
    double complex :: z ! UndCC_Violation
    class(*), allocatable :: p
    double precision, allocatable :: arr(:) ! UndCC_Violation
    real(kind(1.0d0)) :: k ! UndCC_Valid
    real(8) :: r8 ! UndCC_Valid
    complex(dp) :: c ! UndCC_Valid
    character(20) :: s = 'double precision' ! UndCC_Valid
    f = x
    allocate(double precision :: p) ! UndCC_Violation
    select type (p)
    type is (double precision) ! UndCC_Violation
    end select
    z = dcmplx(1., 2.)
    print *, dble(f), precision(x), s, k, r8, c ! UndCC_Valid
    ! double precision in a comment ! UndCC_Valid
  end function

  subroutine names()
    real :: double, precision
    type(pair) :: q
    double = 1.
    precision = double ! UndCC_Valid
    q%precision = double ! UndCC_Valid
    print *, double, precision, q ! UndCC_Valid
  end subroutine
end module
