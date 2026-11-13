module nodes
  abstract interface
    subroutine iface()
    end subroutine
  end interface

  real, target, save :: fallback = 0.

  type :: node
    real :: x ! UndCC_Valid
    type(node), pointer :: next ! UndCC_Violation
    type(node), pointer :: prev => null() ! UndCC_Valid
    real, pointer :: a => null(), b ! UndCC_Violation
    real, dimension(:), pointer :: arr ! UndCC_Violation
    real, pointer :: arr2(:) => null() ! UndCC_Valid
    real, pointer :: arr3(:, :) ! UndCC_Violation
    class(node), pointer :: poly ! UndCC_Violation
    real, pointer :: to_target => fallback ! UndCC_Valid
    real, allocatable :: alloc(:) ! UndCC_Valid
    character, pointer :: s1*10 ! UndCC_Violation
    character, pointer :: s2*10 => null() ! UndCC_Valid
    character, pointer :: s3(:)*(10) => null() ! UndCC_Valid
    character, pointer :: s4(:)*(*) ! UndCC_Violation
    procedure(iface), pointer, nopass :: fp ! UndCC_Violation
    procedure(iface), pointer, nopass :: fp2 => null() ! UndCC_Valid
    real :: init = 1. ! UndCC_Valid
  contains
    procedure :: meth
  end type

  type, extends(node) :: child
    integer, pointer :: ip ! UndCC_Violation
  end type

contains
  subroutine meth(self)
    class(node) :: self
    type(node), pointer :: local ! UndCC_Valid
    procedure(iface), pointer :: local_fp ! UndCC_Valid
    nullify(local, local_fp)
  end subroutine
end module
