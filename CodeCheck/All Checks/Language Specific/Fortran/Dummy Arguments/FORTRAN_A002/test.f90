module intents
  type :: obj
  contains
    procedure :: meth
  end type
  interface gen
    module procedure gen_real
  end interface
contains
  subroutine gen_real(a)
    real :: a
    a = 6.
  end subroutine

  subroutine sets(a)
    real :: a
    a = 0.
  end subroutine

  subroutine passes(a)
    real :: a
    call sets(a)
  end subroutine

  subroutine reads(a, b)
    real :: a, b
    print *, a, b
  end subroutine

  subroutine sets_second(a, b)
    real :: a, b
    b = a
  end subroutine

  subroutine in_only(a)
    real, intent(in) :: a
    print *, a
  end subroutine

  subroutine out_arg(a)
    real, intent(out) :: a
    print *, 'not set'
  end subroutine

  subroutine inout_arg(a)
    real, intent(inout) :: a
    print *, a
  end subroutine

  subroutine by_value(a)
    real, value :: a
    a = 1.
  end subroutine

  subroutine in_internal(a)
    real :: a
    call inner()
  contains
    subroutine inner()
      a = 2.
    end subroutine
  end subroutine

  recursive subroutine ping(a, n)
    real :: a
    integer :: n
    if (n > 0) call pong(a, n - 1)
  end subroutine

  recursive subroutine pong(a, n)
    real :: a
    integer :: n
    call ping(a, n)
  end subroutine

  real function modifies(a)
    real :: a
    a = 3.
    modifies = a
  end function

  subroutine meth(self, a)
    class(obj) :: self
    real :: a
    a = 4.
  end subroutine

  subroutine checks(x, y, p, cb, o)
    real, intent(in) :: x
    real, intent(in) :: y
    real, pointer, intent(in) :: p
    external :: cb
    type(obj), intent(in) :: o
    real :: t

    call sets(x) ! UndCC_Violation
    call passes(x) ! UndCC_Violation
    call reads(x, y) ! UndCC_Valid
    call sets_second(x, t) ! UndCC_Valid
    call sets_second(t, y) ! UndCC_Violation
    call sets_second(b=t, a=x) ! UndCC_Valid
    call sets_second(b=y, a=t) ! UndCC_Violation
    call in_only(x) ! UndCC_Valid
    call out_arg(x) ! UndCC_Violation
    call inout_arg(x) ! UndCC_Violation
    call by_value(x) ! UndCC_Valid
    call in_internal(x) ! UndCC_Violation
    call ping(x, 3) ! UndCC_Valid
    call sets(x + 1.) ! UndCC_Valid
    t = modifies(x) ! UndCC_Violation
    call sets(p) ! UndCC_Valid
    call cb(x) ! UndCC_Valid
    call o%meth(x) ! UndCC_Valid
    call unknown_sub(x) ! UndCC_Valid
    call gen(x) ! UndCC_Valid
    print *, t
  end subroutine
end module

subroutine external_sets(a)
  real :: a
  a = 5.
end subroutine

subroutine calls_external(x)
  real, intent(in) :: x
  call external_sets(x) ! UndCC_Violation
end subroutine
