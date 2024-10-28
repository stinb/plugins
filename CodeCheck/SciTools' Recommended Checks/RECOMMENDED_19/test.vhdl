library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

package Func1_Package is
  
  function Func1 (A, B: in integer; MaxValue: in integer) return integer; --UndCC_Violation hi this is T.B.D.
end Func1_Package;

package body Func1_Package is
  
  function Func1 (A, B: in integer; MaxValue: in integer) return integer is 
    variable Temp, --UndCC_Violation this is a to do item
    Max: integer; 
  begin

    return Max;
  end Func1;
end Func1_Package;

