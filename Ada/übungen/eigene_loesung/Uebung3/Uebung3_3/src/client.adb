
package body client is

task body Client_Type(sss: access Server_Type) is
   begin
      for I in Integer range 0 .. 10 loop
         sss.Service_A;
         --Service_A;
         delay 1.0;
      end loop;
   end Client_Type;

end client;
