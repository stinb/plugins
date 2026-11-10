module allocs
  integer, allocatable :: modarr(:)
contains
  subroutine branch_only(c)
    logical, intent(in) :: c
    integer, allocatable :: iarr(:)
    allocate(iarr(16)) ! UndCC_Violation
    if (c) then
      deallocate(iarr)
    end if
  end subroutine

  subroutine always(c)
    logical, intent(in) :: c
    integer, allocatable :: a(:)
    allocate(a(16)) ! UndCC_Valid
    if (c) print *, size(a)
    deallocate(a)
  end subroutine

  subroutine guarded(c)
    logical, intent(in) :: c
    integer, allocatable :: a(:), b(:)
    if (c) allocate(a(2)) ! UndCC_Valid
    if (allocated(a)) deallocate(a)
    allocate(b(2)) ! UndCC_Valid
    if (.not. allocated(b)) then
      print *, 'none'
    else
      deallocate(b)
    end if
  end subroutine

  subroutine early_return(n)
    integer, intent(in) :: n
    real, allocatable :: w(:)
    allocate(w(n)) ! UndCC_Violation
    if (n > 10) return
    deallocate(w)
  end subroutine

  subroutine stops(n)
    integer, intent(in) :: n
    real, allocatable :: w(:)
    allocate(w(n)) ! UndCC_Valid
    if (n > 10) error stop 'too big'
    deallocate(w)
  end subroutine

  subroutine moved(out1, out2)
    integer, allocatable, intent(out) :: out1(:), out2(:)
    integer, allocatable :: a(:), b(:)
    allocate(a(2), b(2)) ! UndCC_Valid
    call move_alloc(from=a, to=out1)
    call move_alloc(b, out2)
  end subroutine

  subroutine moved_to(src)
    integer, allocatable, intent(inout) :: src(:)
    integer, allocatable :: a(:)
    allocate(a(2)) ! UndCC_Valid
    deallocate(a)
    allocate(a(3)) ! UndCC_Violation
    call move_alloc(src, a)
  end subroutine

  subroutine several(k)
    integer, intent(out) :: k
    integer, allocatable :: a(:), b(:)
    allocate(a(2), b(2), stat=k) ! UndCC_Violation
    deallocate(a, stat=k)
  end subroutine

  subroutine typed()
    character(len=:), allocatable :: s
    allocate(character(len=10) :: s) ! UndCC_Violation
    s = 'x'
  end subroutine

  subroutine cases(i)
    integer, intent(in) :: i
    integer, allocatable :: a(:)
    allocate(a(2)) ! UndCC_Violation
    select case (i)
    case (1)
      deallocate(a)
    case default
      print *, i
    end select
  end subroutine

  subroutine loop(n)
    integer, intent(in) :: n
    integer, allocatable :: a(:)
    integer :: k
    do k = 1, n
      allocate(a(k)) ! UndCC_Valid
      a = k
      deallocate(a)
    end do
  end subroutine

  subroutine in_block()
    block
      real, allocatable :: w(:)
      allocate(w(2)) ! UndCC_Violation
    end block
  end subroutine

  subroutine not_checked(d)
    integer, allocatable, intent(inout) :: d(:)
    integer, allocatable, save :: sv(:)
    integer, allocatable :: byassign(:)
    allocate(d(2)) ! UndCC_Valid
    if (.not. allocated(sv)) allocate(sv(2)) ! UndCC_Valid
    allocate(modarr(2)) ! UndCC_Valid
    byassign = [1, 2]
  end subroutine

  function res() result(r)
    integer, allocatable :: r(:)
    allocate(r(3)) ! UndCC_Valid
  end function

  function named()
    integer, allocatable :: named(:)
    allocate(named(3)) ! UndCC_Valid
  end function
end module

program main
  integer, allocatable :: pa(:)
  allocate(pa(2)) ! UndCC_Valid
end program

subroutine saves_all()
  integer, allocatable :: a(:)
  save
  if (.not. allocated(a)) allocate(a(2)) ! UndCC_Valid
end subroutine

subroutine saves_one()
  integer, allocatable :: a(:), b(:)
  save :: a
  allocate(a(2)) ! UndCC_Valid
  allocate(b(2)) ! UndCC_Violation
end subroutine
