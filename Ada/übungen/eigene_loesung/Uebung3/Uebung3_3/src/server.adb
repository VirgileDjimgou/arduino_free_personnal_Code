with Ada.Text_IO;

package body server is
   task body Server_Type is
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
   end Server_Type;
end server;
