entity T05_WhileLoopTb is
end entity;

architecture sim of T05_WhileLoopTb is
begin

    process is
        variable i : integer := 0;
    begin

        while i < 10 loop  -- while loop used -- UndCC_Violation
            report "i=" & integer'image(i);
            i := i + 2;
        end loop;
        wait;

    end process;

end architecture;
