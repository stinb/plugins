with GNAT.Sockets;
use GNAT.Sockets;

package Test_7_4_2 is

   User_Error : exception;
   Renamed_Vendor_Error : exception renames GNAT.Sockets.Socket_Error;

   package GNAT_Lookalike is
      Lookalike_Error : exception;
   end GNAT_Lookalike;

   procedure Raise_Vendor;
   procedure Raise_Vendor_Via_Use;
   procedure Raise_User;
   procedure Raise_Predefined;
   procedure Raise_Renamed_Vendor;
   procedure Raise_Bare_Reraise;
   procedure Raise_Lookalike;

end Test_7_4_2;
