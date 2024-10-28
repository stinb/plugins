-- Note: This program was written by ChatGPT
-- This program takes an input number from the user and outputs whether it is even or odd.

entity even_odd is
  port (
	num : in integer range 0 to 9999; -- UndCC_Violation, input number from user
	s_even : out boolean         -- UndCC_Violation, output indicating whether the number is even or not
  );
end entity even_odd;

architecture behavioral of even_odd is
begin
  process(num)
  begin
    if num mod 2 = 0 then -- check if the input number is even
      is_even <= true;
    else
      is_even <= false;
    end if;
  end process;
end architecture behavioral;
