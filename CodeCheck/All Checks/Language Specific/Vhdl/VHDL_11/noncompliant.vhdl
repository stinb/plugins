library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity FourLevelNestedCaseExample is
    Port ( input1 : in STD_LOGIC;
           input2 : in STD_LOGIC;
           input3 : in STD_LOGIC;
           input4 : in STD_LOGIC;
           output : out STD_LOGIC);
end FourLevelNestedCaseExample;

architecture Behavioral of FourLevelNestedCaseExample is
begin
    process (input1, input2, input3, input4)
    begin
        case input1 is
            when '0' =>
                case input2 is
                    when '0' =>
                        case input3 is
                            when '0' =>
                                case input4 is  -- UndCC_Violation - case statement nested 4 deep
                                    when '0' =>
                                        output <= '0';
                                    when others =>
                                        output <= '1';
                                end case;
                            when others =>
                                output <= '1';
                        end case;
                    when others =>
                        output <= '1';
                end case;
            when '1' =>
                output <= '1';
            when others =>
                output <= '0';
        end case;
    end process;
end Behavioral;
