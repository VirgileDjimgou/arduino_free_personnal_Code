with Ada.Text_IO;

procedure Main is
   task Server is
      entry Service_A;
      entry Service_B;
      entry Service_C;
   end Server;

   task ClientA;
   task ClientB;

   task body ClientA is
   begin
      for I in Integer range 0 .. 10 loop
         Server.Service_A;
         delay 1.0;
      end loop;
   end ClientA;

   task body ClientB is
   begin
      for I in Integer range 0 .. 10 loop
         Server.Service_B;
         delay 1.0;
      end loop;
   end ClientB;

   task body Server is
   begin
      loop
         select
            accept Service_A  do
               Ada.Text_IO.Put_Line("A");
            end Service_A;
         or
            accept Service_B  do
               Ada.Text_IO.Put_Line("B");
            end Service_B;
         or
            accept Service_C  do
               Ada.Text_IO.Put_Line("C");
            end Service_C;
         or
            terminate;
         end select;
      end loop;
   end Server;
begin
   --  Insert code here.
   null;
end Main;
