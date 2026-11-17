module pointers
  use iso_c_binding
  type :: t
    real :: a
  contains
    procedure :: meth
  end type
contains
  subroutine meth(self, a)
    class(t) :: self
    real, pointer :: a
    a => null()
  end subroutine

  subroutine bound(obj)
    type(t), intent(in) :: obj
    real, pointer :: p
    call obj%meth(p)
    print *, p ! UndCC_Valid
  end subroutine

  subroutine init(q)
    real, pointer :: q
    allocate(q)
  end subroutine

  subroutine noargs()
  end subroutine

  subroutine reads(a, b)
    real, intent(in) :: a
    real, optional :: b
  end subroutine

  subroutine readsp(a)
    real, pointer, intent(in) :: a
  end subroutine

  subroutine setsp(a, b)
    real, pointer, intent(inout) :: a
    real, pointer, optional :: b
    a => null()
  end subroutine

  subroutine mixed(a, b)
    real, pointer :: a
    real, intent(in) :: b
    a => null()
  end subroutine

  subroutine uses(n, cond, cp)
    integer, intent(in) :: n
    logical, intent(in) :: cond
    type(c_ptr), intent(in) :: cp
    real, pointer :: x, z, w, v, u, both, never, late
    real, pointer :: arr(:), rm(:, :)
    type(t), pointer :: tp, tq
    real, pointer :: initialized => null()
    real, pointer, save :: saved
    real, target :: y, tarr(10)

    if (associated(x)) print *, x ! UndCC_Violation
    x => y
    print *, x ! UndCC_Valid
    nullify(z)
    allocate(w)
    call init(v)
    call c_f_pointer(cp, u)
    x = 1.
    print *, z, w + v, u ! UndCC_Valid
    tp%a = 2. ! UndCC_Violation
    rm(1:2, 1:5) => tarr
    arr => tarr(1:n)
    arr(1) = 3. ! UndCC_Valid
    print *, rm(1, 1) ! UndCC_Valid
    if (cond) then
      both => y
    else
      nullify(both)
    end if
    print *, associated(both) ! UndCC_Valid
    if (cond) then
      late => y
    end if
    print *, associated(late) ! UndCC_Violation
    late = 4. ! UndCC_Valid
    print *, associated(initialized), associated(saved) ! UndCC_Valid
  end subroutine

  subroutine loop(n)
    integer, intent(in) :: n
    real, pointer :: p
    real, target :: y
    integer :: i
    do i = 1, n
      if (i > 1) print *, p ! UndCC_Violation
      p => y
    end do
  end subroutine

  subroutine freed()
    real, pointer :: p
    deallocate(p) ! UndCC_Violation
  end subroutine

  subroutine keyword(cp)
    type(c_ptr), intent(in) :: cp
    real, pointer :: p
    call c_f_pointer(cptr=cp, fptr=p)
    print *, p ! UndCC_Valid
  end subroutine

  subroutine typed(n)
    integer, intent(in) :: n
    class(t), pointer :: c
    character(:), pointer :: s, arr(:)
    allocate(t::c)
    allocate(character(10):: s, arr(n))
    print *, c%a, s, arr ! UndCC_Valid
  end subroutine

  subroutine passed()
    real, pointer :: p, q, r, s, k, e(:), c, m, g, h
    type(t), pointer :: tp, tq
    real :: other
    other = 0.
    call noargs
    print *, p, other ! UndCC_Violation
    call reads(q) ! UndCC_Violation
    call readsp(r) ! UndCC_Violation
    call reads(1., b=k) ! UndCC_Violation
    call reads(e(1)) ! UndCC_Violation
    call reads(tp%a) ! UndCC_Violation
    call setsp(s, b=c)
    print *, s, c ! UndCC_Valid
    call noargs; print *, m, other ! UndCC_Violation
    call mixed(g, h) ! UndCC_Violation
    print *, g ! UndCC_Valid
    call external_sub(tq%a) ! UndCC_Violation
  end subroutine

  subroutine molds()
    real, pointer :: p, q
    q => null(p)
    print *, associated(q), associated(p) ! UndCC_Violation
  end subroutine

  subroutine cray()
    integer :: pte, data, pte2
    pointer (ptr, pte)
    integer :: i; pointer (ptr2, pte2)
    ptr = loc(data) ! UndCC_Valid
    ptr2 = loc(data) ! UndCC_Valid
    print *, pte, pte2 ! UndCC_Valid
  end subroutine

  subroutine all_saved()
    real, pointer :: p
    integer :: i; save
    print *, associated(p) ! UndCC_Valid
  end subroutine

  function make() result(r)
    real, pointer :: r
    print *, associated(r) ! UndCC_Violation
    allocate(r)
  end function
end module

program main
  real, pointer :: p
  print *, associated(p) ! UndCC_Violation
end program
