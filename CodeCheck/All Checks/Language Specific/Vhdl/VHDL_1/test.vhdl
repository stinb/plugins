library ieee;
use ieee.std_logic_1164.all;

entity Unused_Signal_Constant is
end Unused_Signal_Constant;

architecture Behavioral of Unused_Signal_Constant is
    signal unused_signal : std_logic; -- UndCC_Violation
    constant UNUSED_CONST : integer := 10; -- UndCC_Violation

begin
    -- No logic that utilizes the unused signal or constant

end Behavioral;
