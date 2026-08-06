package body Test_5_7_3 is

   procedure Draw (Self : Shape) is
   begin
      null;
   end Draw;

   procedure Resize (Self : Shape; Factor : Integer) is
   begin
      null;
   end Resize;

   procedure Draw (Self : Circle) is
   begin
      null;
   end Draw;

   procedure Resize (Self : Circle; Factor : Integer) is
   begin
      null;
   end Resize;

   procedure Grow (Self : Circle; Amount : Integer) is
   begin
      null;
   end Grow;

   procedure Highlight (Self : Circle) is
   begin
      null;
   end Highlight;

   package body Multi_Level is
      procedure Draw (Self : Mid) is
      begin
         null;
      end Draw;

      function Area (Self : Mid) return Float is
      begin
         return 0.0;
      end Area;

      procedure Draw (Self : Leaf) is
      begin
         null;
      end Draw;

      function Area (Self : Leaf) return Float is
      begin
         return 1.0;
      end Area;

      procedure Resize (Self : Base; Factor : Integer) is
      begin
         null;
      end Resize;

      procedure Draw (Self : Sized) is
      begin
         null;
      end Draw;

      function Area (Self : Sized) return Float is
      begin
         return 2.0;
      end Area;

      procedure Resize (Self : Sized; Amount : Integer) is
      begin
         null;
      end Resize;
   end Multi_Level;

   package body Case_Insensitive is
      procedure Draw (Self : Base) is
      begin
         null;
      end Draw;

      procedure Draw (Self : Circle) is
      begin
         null;
      end Draw;
   end Case_Insensitive;

   package body Body_Only is
      procedure Refresh (Self : Widget) is
      begin
         null;
      end Refresh;

      -- Body-only tagged type: Gadget and its override of Refresh are
      -- both declared entirely inside this body, with no separate spec
      -- entry for the override. Exercises the fallback path that checks
      -- "Declare Body" refs when no spec declaration exists.
      type Gadget is new Widget with null record;

      procedure Refresh (Self : Gadget) is -- UndCC_Violation
      begin
         null;
      end Refresh;
   end Body_Only;

end Test_5_7_3;
