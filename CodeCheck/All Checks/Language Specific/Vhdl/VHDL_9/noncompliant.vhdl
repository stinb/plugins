library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity ViolatingExample is
    Port ( clk : in STD_LOGIC;
           rst : in STD_LOGIC;
           data_in : in STD_LOGIC_VECTOR(7 downto 0));
end ViolatingExample;

architecture Behavioral of ViolatingExample is
    signal data_out : STD_LOGIC_VECTOR(7 downto 0);
begin
    process(clk, rst)
    begin
        if rst = '1' then
            data_out <= (others => '0'); -- First assignment
        elsif rising_edge(clk) then
            data_out <= data_in; -- Second assignment within the same process

            if data_in = "11110000" then
                data_out <= "00001111"; -- Third assignment within the same process -- UndCC_Violation
            end if;
        end if;
    end process;
end Behavioral;

