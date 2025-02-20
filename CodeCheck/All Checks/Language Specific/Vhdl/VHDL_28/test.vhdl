library IEEE;
use IEEE.STD_LOGIC;

entity test_processes is
    Port ( clk : in  STD_LOGIC;
           reset : in  STD_LOGIC;
           output_labeled : out  STD_LOGIC;
           output_unlabeled : out  STD_LOGIC);
end test_processes;

architecture Behavioral of test_processes is
begin
    -- Labeled process
    labeled_process : process(clk, reset)
    begin
        if reset = '1' then
            output_labeled <= '0';
        elsif rising_edge(clk) then
            output_labeled <= not output_labeled;
        end if;
    end process labeled_process;

    -- Unlabeled process
    process(clk, reset) -- UndCC_Violation
    begin
        if reset = '1' then
            output_unlabeled <= '0';
        elsif rising_edge(clk) then
            output_unlabeled <= not output_unlabeled;
        end if;
    end process;
end Behavioral;
