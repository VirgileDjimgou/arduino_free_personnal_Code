with Data_Types; use Data_Types;
with IO; use IO;
--with Control_Procedures; use Control_Procedures;
procedure Main is
   task Temp_Controller;
   task Pressure_Controller;
   Display: Console;

   task body Temp_Controller is
      TR : Temp_Reading; HS : Heater_Setting;
   begin
      loop
         Display.Write(TR);
      end loop;
   end Temp_Controller;
   task body Pressure_Controller is
      PR : Pressure_Reading; PS : Pressure_Setting;
   begin
      loop
         Display.Write(PR);
      end loop;
   end Pressure_Controller;
begin
   null;
end Main;
