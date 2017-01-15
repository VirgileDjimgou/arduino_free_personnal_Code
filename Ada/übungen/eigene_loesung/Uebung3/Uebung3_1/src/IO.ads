with Data_Types; use Data_Types;
package IO is
   task type Console is
      entry Write(TR: Temp_Reading);
      entry Write(PR: Pressure_Reading);
   end Console;
end IO;
