module counters
  integer :: module_count = 0 ! UndCC_Valid
contains
  integer function sum_array(array)
    integer, intent(in) :: array(:)
    integer :: total = 0 ! UndCC_Violation
    integer, save :: calls = 0 ! UndCC_Valid
    integer :: kept = 0 ! UndCC_Valid
    save :: kept
    integer :: fixed = 5 ! UndCC_Valid
    integer, parameter :: n = 3 ! UndCC_Valid
    integer :: reset ! UndCC_Valid
    integer :: from_data ! UndCC_Valid
    integer, dimension(3) :: weights = [1, 2, 3] ! UndCC_Violation
    integer :: passed = 1 ! UndCC_Violation
    integer :: read_only = 1 ! UndCC_Valid
    integer :: out_only = 1 ! UndCC_Violation
    integer, pointer :: p => null() ! UndCC_Valid
    integer, target :: t = 0 ! UndCC_Valid
    integer :: i
    data from_data /4/

    reset = 0
    do i = 1, size(array)
      total = total + array(i)
    end do
    calls = calls + 1
    kept = kept + 1
    from_data = from_data + 1
    weights(1) = 0
    call changes(passed)
    call reads(read_only)
    call sets(out_only)
    p => t
    block
      integer :: b = 7 ! UndCC_Violation
      b = b + 1
    end block
    module_count = module_count + 1
    sum_array = total + fixed + n + reset + weights(1) + p
  end function

  subroutine all_saved()
    integer :: count = 0 ! UndCC_Valid
    save
    count = count + 1
  end subroutine

  subroutine host()
    integer :: summed(2) = [1, 2] ! UndCC_Valid
    real :: random = 0. ! UndCC_Violation
    integer :: to_internal(2) = [3, 4] ! UndCC_Valid
    integer :: to_value = 1 ! UndCC_Valid
    integer :: to_setter = 1 ! UndCC_Violation
    integer :: by_keyword = 1 ! UndCC_Violation
    integer :: by_keyword_in = 1 ! UndCC_Valid
    integer :: element(2) = [5, 6] ! UndCC_Violation
    integer :: element_value(2) = [5, 6] ! UndCC_Valid
    integer :: in_expression = 1 ! UndCC_Valid
    integer :: to_unknown = 1 ! UndCC_Violation

    print *, sum(summed)
    call random_number(random)
    print *, internal_sum(to_internal)
    call internal_value(to_value)
    call internal_sets(to_setter)
    call internal_two(b=by_keyword, a=by_keyword_in)
    call internal_sets(element(1))
    call internal_value(element_value(1))
    call internal_sets(in_expression + 1)
    call unknown_proc(to_unknown)
  contains
    integer function internal_sum(a)
      integer, intent(in) :: a(:)
      internal_sum = sum(a)
    end function

    subroutine internal_value(a)
      integer, value :: a
      a = a + 1
    end subroutine

    subroutine internal_sets(a)
      integer :: a
      a = 0
    end subroutine

    subroutine internal_two(a, b)
      integer, intent(in) :: a
      integer :: b
      b = a
    end subroutine
  end subroutine

  subroutine indexes(a)
    integer, intent(inout) :: a(3)
    forall (integer :: i = 1:3) a(i) = i ! UndCC_Valid
    do concurrent (integer :: j = 1:3) ! UndCC_Valid
      a(j) = 0
      i = 4
    end do
  end subroutine

  subroutine changes(x)
    integer :: x
    x = x + 1
  end subroutine

  subroutine reads(x)
    integer, intent(in) :: x
    print *, x
  end subroutine

  subroutine sets(x)
    integer, intent(out) :: x
    x = 2
  end subroutine
end module

program main
  integer :: runs = 0 ! UndCC_Valid
  runs = runs + 1
end program
