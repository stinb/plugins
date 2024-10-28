library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity MultipleAssignment is
    Port ( clk : in STD_LOGIC;
           rst : in STD_LOGIC;
           data_in : in STD_LOGIC;
           data_out : out STD_LOGIC);
end MultipleAssignment;

architecture Behavioral of MultipleAssignment is
    signal my_signal : STD_LOGIC;
begin
    process(clk, rst)
    begin
        if rst = '1' then
            my_signal <= '0';  -- First assignment
        elsif rising_edge(clk) then
            my_signal <= data_in;  -- Second assignment
            my_signal <= not my_signal;  -- Third assignment -- UndCC_Violation
        end if;
    end process;

    data_out <= my_signal;
end Behavioral;
