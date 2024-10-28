-- See: https://peterfab.com/ref/vhdl/vhdl_renerta/mobile/source/vhd00023.htm

B_OUT <= transport B_IN after 1 ns;  -- Example 1: Transport delay -- UndCC_Violation

L_OUT <= inertial L_IN after 1 ns;   -- Example 2: Inertial delay -- UndCC_Violation

Q_OUT <= reject 500 ps inertial Q_IN after 1 ns;  -- Example 3: Inertial delay with rejection limit -- UndCC_Violation
