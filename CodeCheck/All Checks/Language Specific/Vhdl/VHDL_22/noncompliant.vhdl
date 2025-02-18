library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity CASE_STATEMENT is
  port (A, B, C, X         : in  integer range 0 to 15;
        Z                  : out integer range 0 to 15);
end CASE_STATEMENT;
 
architecture EXAMPLE of CASE_STATEMENT is
begin
  process (A, B, C, X)
  begin
                      -- no default signal assignment for Z +
    case X is
      when 0 =>
         Z <= A;
      when 7 | 9 =>
         Z <= B;
      when 1 to 5 =>
         Z <= C;
                      -- Z not assigned for all possible values of X +
      when others => -- UndCC_Violation
                      -- "when others" present, but empty: ➔ a latch gets inferred, violation
    end case;
  end process;
end EXAMPLE;

entity conditional_signal_assignment_when_example is
    Port ( clk : in STD_LOGIC;
           reset : in STD_LOGIC;
           data_in : in STD_LOGIC_VECTOR(7 downto 0);
           data_out : out STD_LOGIC_VECTOR(7 downto 0));
end conditional_signal_assignment_when_example;

architecture Behavioral of conditional_signal_assignment_when_example is
begin
    process(clk, reset)
    begin
        if reset = '1' then
            -- Reset state, set data_out to all zeros
            data_out <= (others => '0');
        elsif rising_edge(clk) then
            -- Normal operation, perform conditional signal assignment using 'when'
            data_out <= data_in;  -- Default value should be the last condition

            if data_in = "0000_0000" then
                data_out <= "0000_0001";
            elsif data_in = "1111_1111" then
                data_out <= "1111_1110";
            end if;
        end if;
    end process;
end Behavioral;

