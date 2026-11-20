! Another program's procedure with the same name
subroutine other_program()
  interface
    subroutine external_reads(changed)
      real, intent(inout) :: changed
    end subroutine
  end interface
end subroutine
