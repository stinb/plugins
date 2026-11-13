      MODULE OLD
      TYPE REC
        REAL :: X
        TYPE(REC), POINTER :: P ! UndCC_Violation
        TYPE(REC), POINTER :: Q
     &    => NULL() ! UndCC_Valid
      END TYPE REC
      END MODULE
