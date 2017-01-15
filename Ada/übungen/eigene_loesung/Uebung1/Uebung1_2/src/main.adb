with Ada.Text_IO;

procedure main is

begin
   declare
      task forktask;
      task body forktask is
      begin
         Ada.Text_IO.Put_Line("bla");
      end;
   begin
      null;
   end;

end main;
