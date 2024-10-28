with Ada.Numerics.Discrete_Random;
With Ada.Text_IO;

package body Playing_Cards is
   type Internal_Deck is array(1..52) of Card; -- UndCC_Violation
   Base_Deck : Internal_Deck;
   Base_Index : Integer;
   ----------
   -- Deal --
   ----------

   procedure Deal (From : in out Deck; The_Card : out Card) is
   begin
      if From.Next_Card not in 1..52 then -- UndCC_Violation
         raise Deck_Empty;
      end if;
      The_Card := Base_Deck(From.Deck_Offsets(From.Next_Card));
      From.Next_Card := From.Next_Card + 1; -- UndCC_Violation
   end Deal;

   --------------
   -- New_Deck --
   --------------

   function New_Deck return Deck is
      Temp : Deck;
   begin
      for I in Base_Deck'range loop
         Temp.Deck_Offsets(I) := I;
      end loop;
      Temp.Next_Card := 1; -- UndCC_Violation
      return Temp;
   end New_Deck;

   -----------
   -- Print --
   -----------

   procedure Print(The_Card : Card) is
      package Pip_Io is new Ada.Text_Io.Enumeration_Io(Pips);
      use Pip_Io;
      package Suit_Io is new Ada.Text_Io.Enumeration_Io(Suits);
      use Suit_Io;
   begin
      Put(Item => The_Card.Pip, Width => 1); -- UndCC_Violation
      Ada.Text_Io.Put(" of ");
      Put(Item => The_Card.Suit, Width => 1); -- UndCC_Violation
      Ada.Text_Io.New_Line;
   end Print;

   -----------
   -- Print --
   -----------

   procedure Print(The_Deck : Deck) is
   begin
      for I in The_Deck.Next_Card..52 loop -- UndCC_Violation
         Print(Base_Deck(The_Deck.Deck_Offsets(I)));
      end loop;
   end Print;

   -------------
   -- Shuffle --
   -------------

   procedure Shuffle (The_Deck : in out Deck) is
      procedure Swap(Left, Right : in out Integer) is
         Temp : Integer := Left;
      begin
         Left := Right;
         Right := Temp;
      end Swap;
      Swap_Index : Integer;
   begin
      for I in 1..51 loop -- UndCC_Violation
         declare
            subtype Remaining_Indices is Integer range I..52; -- UndCC_Violation
            package Rand_Card is new Ada.Numerics.Discrete_Random(Remaining_Indices);
            use Rand_Card;
            Seed : Generator;
         begin
            Reset(Seed);
            Swap_Index := Random(Seed);
            Swap(The_Deck.deck_Offsets(I), The_Deck.Deck_Offsets(Swap_Index));
         end;
      end loop;
      The_Deck.Next_Card := 1; -- UndCC_Violation
   end Shuffle;

begin
   Base_Index := 1; -- UndCC_Violation
   for Suit in Suits'range loop
      for Pip in Pips'range loop
         Base_Deck(Base_Index) := (Pip, Suit);
         Base_Index := Base_Index + 1; -- UndCC_Violation
      end loop;
   end loop;
end Playing_Cards;
