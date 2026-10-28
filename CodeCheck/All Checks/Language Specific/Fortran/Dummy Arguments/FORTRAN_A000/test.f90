module strings
  implicit none
contains
  subroutine append_trim(s, t)
    character(len=*), intent(inout) :: s
    character(len=*), intent(in) :: t
    s = trim(s) // t ! UndCC_Violation(trunc)
  end subroutine

  subroutine append_continued(s)
    character(len=*), intent(inout) :: s
    s = 'prefix_' // & ! UndCC_Violation(trunc)
        s
  end subroutine

  subroutine append_substring(s, n)
    character(len=*), intent(inout) :: s
    integer, intent(in) :: n
    s(n:) = s(1:2) // 'x' ! UndCC_Valid
  end subroutine

  subroutine to_other(s, r)
    character(len=*), intent(in) :: s
    character(len=*), intent(out) :: r
    r = s // '.dat' ! UndCC_Valid
  end subroutine

  subroutine plain_set(s)
    character(len=*), intent(inout) :: s
    s = 'abc' ! UndCC_Valid
  end subroutine

  subroutine self_no_concat(s)
    character(len=*), intent(inout) :: s
    s = trim(adjustl(s)) ! UndCC_Valid
  end subroutine

  subroutine local_concat(s)
    character(len=*), intent(in) :: s
    character(len=40) :: buf
    buf = 'x'
    buf = buf // s ! UndCC_Valid
  end subroutine

  subroutine not_character(i)
    integer, intent(inout) :: i
    i = i + 1 ! UndCC_Valid
  end subroutine

  subroutine passed_out(s)
    character(len=*), intent(inout) :: s
    call to_other('a' // 'b', s) ! UndCC_Valid
  end subroutine

  subroutine append_bare(s)
    character(len=*), intent(inout) :: s
    s = s // '.dat' // '.bak' ! UndCC_Violation
  end subroutine

  subroutine append_bare_continued(s)
    character(len=*), intent(inout) :: s
    s = s & ! UndCC_Violation
        // '.dat'
  end subroutine

  subroutine trim_after(s, t)
    character(len=*), intent(inout) :: s
    character(len=*), intent(in) :: t
    s = t // trim(s) ! UndCC_Violation(trunc)
  end subroutine

  subroutine substring_operand(s)
    character(len=*), intent(inout) :: s
    s = s(1:3) // 'x' ! UndCC_Valid
  end subroutine

  subroutine nested_operand(s)
    character(len=*), intent(inout) :: s
    s = adjustl(s // 'x') ! UndCC_Valid
  end subroutine

  subroutine semicolon(s)
    character(len=*), intent(inout) :: s
    character(len=40) :: buf
    buf = 'b'
    s = 'a'; buf = buf // s ! UndCC_Valid
  end subroutine

  subroutine length_only(s)
    character(len=*), intent(inout) :: s
    s = repeat('x', len(s) - 1) // 'y' ! UndCC_Valid
  end subroutine

  subroutine whole_array(names)
    character(len=8), intent(inout) :: names(3)
    names = names // 'x' ! UndCC_Violation
  end subroutine

  subroutine array_element(names, i)
    character(len=8), intent(inout) :: names(3)
    integer, intent(in) :: i
    names(i) = names(i) // 'x' ! UndCC_Violation
  end subroutine

  subroutine substring_same(s, n)
    character(len=*), intent(inout) :: s
    integer, intent(in) :: n
    s(1:n) = s(1:n) // 'x' ! UndCC_Violation
    s(1:n) = trim(s(1:n)) // 'x' ! UndCC_Violation(trunc)
  end subroutine

  subroutine normalise(s)
    character(len=*), intent(inout) :: s
    s = trim(s) ! UndCC_Valid
    s = s ! UndCC_Valid
  end subroutine

  subroutine inquire_name(s)
    character(len=*), intent(out) :: s
    logical :: e
    inquire(unit=10, name=s, exist=e) ! UndCC_Valid
  end subroutine

  subroutine implicit_character(sname)
    implicit character*8 (s)
    sname = sname // 'x' ! UndCC_FalseNeg - parser types sname as REAL
  end subroutine
end module
