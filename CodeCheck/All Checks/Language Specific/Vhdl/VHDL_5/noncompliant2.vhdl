-- process with label and without label
entity and_or is
port(
  a       : in std_logic;
  b       : in std_logic;
  d       : in std_logic;
  e       : in std_logic;
  g       : out std_logic);
end and_or;

architecture and_or_a of and_or is
-- declarative part: empty

begin
process_and_or : process(a,b,d,e)
-- declarative part: empty
begin
  g <= (a and b) or (d and e);
end process process_and_or;
end and_or_a;

architecture ExampleArch2 of ExampleEntity is
begin
  -- Unlabeled process
  process -- UndCC_Violation
  begin
    -- Your logic here
    if condition = '1' then
      -- Do something
    else
      -- Do something else
    end if;
    
    -- Additional logic
    
  end process;
  
  -- Other architecture statements
  
end ExampleArch2;
