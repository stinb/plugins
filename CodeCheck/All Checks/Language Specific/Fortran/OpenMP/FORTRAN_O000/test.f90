subroutine omp_loops(a, n)
  integer, intent(in) :: n
  integer, intent(inout) :: a(n, n, n)
  integer :: i, j, k, m

  ! Not in a parallel construct
  do j = 1, n
    do i = 1, n
      a(i, j, 1) = 0
    end do
  end do

!$omp parallel do shared(a)
  do j = 1, n
    do i = 1, n ! UndCC_Violation
      a(i, j, 1) = 0
    end do
    do k = 1, n ! UndCC_Violation
      a(1, j, k) = 0
    end do
  end do
!$omp end parallel do

  do j = 1, n
    do i = 1, n
      a(i, j, 1) = 0
    end do
  end do

!$omp parallel do private(i) shared(a)
  do j = 1, n
    do i = 1, n
      a(i, j, 1) = 0
    end do
  end do

!$omp parallel do &
!$omp& shared(a) &
!$omp lastprivate(i)
  do j = 1, n
    do i = 1, n
      a(i, j, 1) = 0
    end do
  end do

!$omp parallel do collapse(2)
  do k = 1, n
    do j = 1, n
      do i = 1, n ! UndCC_Violation
        a(i, j, k) = 0
      end do
    end do
  end do

!$omp parallel do ordered(2)
  do k = 1, n
    do j = 1, n
      do i = 1, n ! UndCC_Violation
        a(i, j, k) = 0
      end do
    end do
  end do

!$omp parallel do shared(a, i)
  do j = 1, n
    do i = 1, n ! UndCC_Violation
      a(i, j, 1) = 0
    end do
  end do

!$omp parallel do default(private) shared(a)
  do j = 1, n
    do i = 1, n
      a(i, j, 1) = 0
    end do
  end do

!$omp parallel do firstprivate(m) linear(k:1)
  do j = 1, n
    do m = 1, n
      a(m, j, 1) = 0
    end do
    do k = 1, n
      a(1, j, k) = 0
    end do
  end do

!$omp parallel do
  do j = 1, n
    inner: do i = 1, n ! UndCC_Violation
      a(i, j, 1) = 0
    end do inner
    do while (m < n)
      m = m + 1
    end do
    do concurrent (k = 1:n)
      a(1, j, k) = 0
    end do
    block
      integer :: b
      do b = 1, n
        a(b, j, 1) = 0
      end do
    end block
    do implicit_var = 1, n ! UndCC_Violation
    end do
  end do
end subroutine

subroutine omp_regions(a, n)
  integer, intent(in) :: n
  integer, intent(inout) :: a(n, n)
  integer :: i, j, m

!$omp parallel shared(a)
  do m = 1, n ! UndCC_Violation
    a(m, 1) = 0
  end do
!$omp do
  do j = 1, n
    do i = 1, n ! UndCC_Violation
      a(i, j) = 0
    end do
  end do
!$omp end do
!$omp end parallel

  do m = 1, n
    a(m, 1) = 0
  end do

!$omp parallel private(i, m)
  do m = 1, n
    a(m, 1) = 0
  end do
!$omp do
  do j = 1, n
    do i = 1, n
      a(i, j) = 0
    end do
  end do
!$omp end parallel

  do m = 1, n
    a(m, 1) = 0
  end do

  ! Orphaned loop construct
!$omp do
  do j = 1, n
    do i = 1, n
      a(i, j) = 0
    end do
  end do
end subroutine

subroutine omp_spellings(a, n)
  integer, intent(in) :: n
  integer, intent(inout) :: a(n, n)
  integer :: i, j, s

!$omp parallel
!$omp endparallel
  do j = 1, n
    do i = 1, n
      a(i, j) = 0
    end do
  end do

!$omp parallel shared(a, s)
!$omp single private(i)
  s = 1
!$omp end single nowait
!$omp do
  do j = 1, n
    do i = 1, n ! UndCC_Violation
      a(i, j) = 0
    end do
  end do
!$omp enddo
!$omp end parallel

!$omp parallel do
  do j = 1, n
    do i = 1, n; a(i, j) = 0; end do ! UndCC_Violation
  end do
  do j = 1, n
    do i = 1, n
      a(i, j) = 0
    end do
  end do
end subroutine

subroutine unclosed(a, n)
  integer, intent(in) :: n
  integer, intent(inout) :: a(n)
  integer :: i
!$omp parallel
  do i = 1, n ! UndCC_Violation
    a(i) = 0
  end do
end subroutine

subroutine after_unclosed(a, n)
  integer, intent(in) :: n
  integer, intent(inout) :: a(n)
  do k = 1, n
    a(k) = 0
  end do
end subroutine
