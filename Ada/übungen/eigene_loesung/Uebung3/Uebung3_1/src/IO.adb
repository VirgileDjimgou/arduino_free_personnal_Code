with Ada.Text_IO;

package body IO is

   task body Console is

   begin
      loop
         accept Write(TR: Temp_Reading) do
            Ada.Text_IO.Put_Line("Temp");
         end Write;

         accept Write(PR: Pressure_Reading) do
            Ada.Text_IO.Put_Line("Pres");
         end Write;

      end loop;
   end Console;
end IO;
