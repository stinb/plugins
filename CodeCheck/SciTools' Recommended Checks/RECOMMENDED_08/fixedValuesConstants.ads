package Playing_Cards is
   pragma Elaborate_Body(Playing_Cards);
 
   type Card is private;
   procedure Print(The_Card : Card);
   type Deck is private;
   procedure Print(the_Deck : Deck);
   procedure Deal(From : in out Deck; The_Card : out Card);
   procedure Shuffle(The_Deck : in out Deck);
   function New_Deck return Deck;
   Deck_Empty : exception;
private
   type Pips is (Two, Three, Four, Five, Six, Seven,
      Eight, Nine, Ten, Jack, Queen, King, Ace);
   type Suits is (Diamonds, Spades, Hearts, Clubs);
   type Card is record
      Pip : Pips;
      Suit : Suits;
   end record;
   type Deck_Reference is array(1..52) of Integer;
   type Deck is record
      Next_Card : Integer;
      Deck_Offsets : Deck_Reference;
   end record;
end Playing_Cards;
