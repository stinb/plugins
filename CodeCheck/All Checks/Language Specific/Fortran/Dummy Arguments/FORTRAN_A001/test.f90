module outargs
  implicit none
  type :: withinit
    integer :: x = 0
  end type
  type :: noinit
    integer :: x
  end type
  type :: allocs
    integer, allocatable :: x
  end type
  type :: empty
  end type
  type, extends(withinit) :: childinit
    integer :: y = 1
  end type
  type, extends(noinit) :: childpartial
    integer :: y = 1
  end type
  type :: nullptr
    integer, pointer :: p => null()
  end type
  abstract interface
    subroutine callback(a)
      integer, intent(out) :: a ! UndCC_Valid
    end subroutine
  end interface
  interface
    subroutine external_sub(a)
      integer, intent(out) :: a ! UndCC_Valid
    end subroutine
  end interface
contains
  subroutine setter(a)
    integer, intent(out) :: a ! UndCC_Valid
    a = 1
  end subroutine

  subroutine early_return(i, a)
    integer, intent(in) :: i
    integer, intent(out) :: a ! UndCC_Violation
    if (i == 1) return
    a = 1
  end subroutine

  subroutine set_before_return(i, a)
    integer, intent(in) :: i
    integer, intent(out) :: a ! UndCC_Valid
    a = 0
    if (i == 1) return
    a = 1
  end subroutine

  subroutine stops(i, a)
    integer, intent(in) :: i
    integer, intent(out) :: a ! UndCC_Valid
    if (i == 1) then
      a = 1
    else if (i == 2) then
      error stop 'bad'
    else
      stop
    end if
  end subroutine

  subroutine select(i, a, b)
    integer, intent(in) :: i
    integer, intent(out) :: a ! UndCC_Valid
    integer, intent(out) :: b ! UndCC_Violation
    select case (i)
    case (1)
      a = 1
      b = 1
    case default
      a = 2
    end select
  end subroutine

  subroutine one_line_if(i, a)
    integer, intent(in) :: i
    integer, intent(out) :: a ! UndCC_Violation
    if (i == 1) a = 0
  end subroutine

  subroutine other_sets(b, c, d, p, e)
    integer, intent(out) :: b ! UndCC_Valid
    integer, intent(out) :: c ! UndCC_Valid
    integer, intent(out) :: d ! UndCC_Valid
    integer, pointer, intent(out) :: p ! UndCC_Valid
    integer, intent(out) :: e ! UndCC_Valid
    read (*, *) b
    call setter(c)
    call unknown_sub(1, d)
    nullify(p)
    call setter(a=e)
  end subroutine

  subroutine not_passed(a, n)
    integer, intent(out) :: a ! UndCC_Violation
    integer, intent(in) :: n
    integer :: arr(10)
    print *, a
    arr(a) = n
    print *, abs(a)
  end subroutine

  subroutine never_set(a)
    integer, intent(out) :: a ! UndCC_Violation
  end subroutine

  subroutine exempt(al, ti)
    integer, allocatable, intent(out) :: al ! UndCC_Valid
    type(withinit), intent(out) :: ti ! UndCC_Valid
  end subroutine

  subroutine exempt_types(ta, te, tc, tp)
    type(allocs), intent(out) :: ta ! UndCC_Valid
    type(empty), intent(out) :: te ! UndCC_Valid
    type(childinit), intent(out) :: tc ! UndCC_Valid
    type(nullptr), intent(out) :: tp ! UndCC_Valid
  end subroutine

  subroutine partial_type(tn, tc)
    type(noinit), intent(out) :: tn ! UndCC_Violation
    class(childpartial), intent(out) :: tc ! UndCC_Violation
  end subroutine

  subroutine local_interface(x)
    integer, intent(out) :: x ! UndCC_Valid
    interface
      subroutine ext(y)
        integer, intent(out) :: y ! UndCC_Valid
      end subroutine
    end interface
    call ext(x)
  end subroutine

  subroutine component(tn)
    type(noinit), intent(out) :: tn ! UndCC_Valid
    tn%x = 1
  end subroutine

  subroutine loops(n, a, arr, b, c)
    integer, intent(in) :: n
    integer, intent(out) :: a ! UndCC_Violation(zero)
    integer, intent(out) :: arr(n) ! UndCC_Violation(zero)
    integer, intent(out) :: b ! UndCC_Valid
    integer, intent(out) :: c ! UndCC_Violation
    integer :: k
    do k = 1, n
      a = k
      arr(k) = 0
    end do
    do
      b = 1
      exit
    end do
    do k = 1, n
      if (k == 2) exit
      c = k
    end do
  end subroutine

  subroutine set_in_internal(f)
    integer, intent(out) :: f ! UndCC_Valid
    call inner()
  contains
    subroutine inner()
      f = 2
    end subroutine
  end subroutine

  subroutine intrinsic_sets(x, s, n)
    real, intent(out) :: x ! UndCC_Valid
    character(len=*), intent(out) :: s ! UndCC_Valid
    integer, intent(out) :: n ! UndCC_Valid
    call random_number(x)
    call get_command_argument(1, s, length=n)
  end subroutine

  subroutine associated_set(a)
    integer, intent(out) :: a ! UndCC_Valid
    associate (x => a)
      x = 1
    end associate
  end subroutine

  subroutine indexes(a, d, darr)
    integer, intent(out) :: a ! UndCC_Violation
    integer, intent(out) :: d ! UndCC_Violation
    integer, intent(inout) :: darr(10)
    integer, parameter :: tab(3) = [1, 2, 3]
    darr(a) = tab(d)
  end subroutine

  integer function func(a)
    integer, intent(out) :: a ! UndCC_Violation
    func = 1
  end function
end module
