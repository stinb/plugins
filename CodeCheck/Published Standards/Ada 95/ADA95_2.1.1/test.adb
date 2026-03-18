with Ada.Text_IO; use Ada.Text_IO;
with Ada.Numerics.Elementary_Functions;
with Ada.Numerics;

procedure Test is

   use Ada.Numerics.Elementary_Functions;

   -- Added required declarations (supporting code only)

   Default_String : constant String :=
"This is the long string returned by" &
" default. It is broken into multiple"& -- UndCC_Violation, no space after string literal and before &
" Ada source lines for convenience.";

   type Signed_Whole_16 is range -2 ** 15 .. 2**15 - 1; -- UndCC_Violation, spaces before and after **
   type Address_Area is array (Natural range <>) of Signed_Whole_16;

   Pc, A, Index : Natural := 0;
   Base_Address : Natural := 0;
   Element_Length : Natural := 1;

   Radius : Float := 1.0;
   Angle  : Float := 0.5;
   Theta  : Float := 1.0;

   X, Y, Z, C, B : Signed_Whole_16 := 1;
   Length, Width, Height : Signed_Whole_16 := 2;
   Volume : Signed_Whole_16;

   Error_Term : Float;

   Sensor : String := "OK";

   procedure Get(Value : out String) is
   begin
      Value := "Sensor Value";
   end Get;

   Register : Address_Area (16#7FF0# .. 16#7FFF#);
   Memory : Address_Area ( 0 .. 16#7FEC#);  -- UndCC_Violation, space after (

begin

   -- Original snippet (unchanged)

   Register(Pc) := Register(A);
   X := Signed_Whole_16(Radius * Sin(Angle));
   Register(Index) := Memory(Base_Address + Index * Element_Length);
   Get(Value => Sensor);
   Error_Term := 1.0 - (Cos (Theta)**2 + Sin(Theta)**2); -- UndCC_Violation, space after calling function Cos
   Z := X**3;
   Y := C * X + B;
   Volume := Length * Width * Height;

   Put_Line(Default_String) ;   -- UndCC_Violation, space before ;

end Original_Snippet_Demo;