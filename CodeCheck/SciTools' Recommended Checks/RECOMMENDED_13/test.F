function func(i) result(j) ! UndCC_Violation(0)
    integer, intent (in) :: i ! input
    integer              :: j ! output

    j = i**2 + i**3
end function

program main
    implicit none
    integer :: i

    i = 3
end program
