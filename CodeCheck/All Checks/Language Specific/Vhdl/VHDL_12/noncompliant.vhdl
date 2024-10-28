-- VHDL sample w/ commented code.

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity SampleEntity is
  Port (
    A : in STD_LOGIC;
    B : in STD_LOGIC;
    C : out STD_LOGIC
  );
end SampleEntity;

architecture Behavioral of SampleEntity is
begin
  -- C <= A AND B; -- UndCC_Violation

  -- Normal comment that shouldn't be flagged...
  -- C <= A OR B; -- UndCC_Violation

  -- if A = '1' then -- UndCC_Violation
  --   C <= B; -- UndCC_Violation
  -- else  -- UndCC_Violation
  --   C <= '0'; -- UndCC_Violation
  --end if; -- UndCC_Violation

  -- process(A)
  -- begin
  --   if rising_edge(A) then -- UndCC_Violation
  --     C <= not C; -- UndCC_Violation
  --   end if; -- UndCC_Violation
  -- end process; -- UndCC_Violation
end Behavioral;
