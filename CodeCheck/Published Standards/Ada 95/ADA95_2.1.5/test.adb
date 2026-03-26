procedure Test is

   type Menus is array (Positive range <>) of String (1 .. 1);
   subtype Alpha_Numerics is Character;

   type Color_Scheme is (Red, Purple, Blue, Green, Yellow, White, Black, Brown, Gray, Pink);

   function "&" (Left, Right : Color_Scheme) return Color_Scheme;

   procedure Display_Menu (Title : in String; 
                           Options : in Menus; 
                           Choice : out Alpha_Numerics) is
   begin
      null;
   end Display_Menu;

   procedure Display_Menu_On_Primary_Window
     (Title : in String; 
      Options : in Menus; 
      Choice : out Alpha_Numerics) is
   begin
      null;
   end Display_Menu_On_Primary_Window;

   procedure Display_Menu_On_Screen (
      Title : in String; 
      Options : in Menus; 
      Choice : out Alpha_Numerics
      ) is
   begin
      null;
   end Display_Menu_On_Screen;

   procedure Display_Noncompliant_1 (
      Title : in String; Options : in Menus; -- UndCC_Violation
      Choice : out Alpha_Numerics
      ) is
   begin
      null;
   end Display_Noncompliant_1;

   procedure Display_Noncompliant_2 (
      Title : in String; 
      Options : in Menus; 
   Choice : out Alpha_Numerics   -- UndCC_Violation
      ) is
   begin
      null;
   end Display_Noncompliant_2;

begin
   null;
end Test;
