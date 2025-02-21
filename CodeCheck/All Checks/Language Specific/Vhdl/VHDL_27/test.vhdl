library IEEE;
use IEEE.std_logic_1164.all;

entity delay_example is
    port (
        B_IN  : in  std_logic;
        L_IN  : in  std_logic;
        Q_IN  : in  std_logic;
        B_OUT : out std_logic;
        L_OUT : out std_logic;
        Q_OUT : out std_logic
    );
end entity delay_example;

architecture behavioral of delay_example is
begin
    -- See: https://peterfab.com/ref/vhdl/vhdl_renerta/mobile/source/vhd00023.htm
    B_OUT <= transport B_IN after 1 ns;  -- Example 1: Transport delay -- UndCC_Violation
    L_OUT <= L_IN after 1 ns;            -- Example 2: Inertial delay -- UndCC_Violation
    Q_OUT <= Q_IN after 1 ns;            -- Example 3: Inertial delay (simplified) -- UndCC_Violation
end architecture behavioral;

