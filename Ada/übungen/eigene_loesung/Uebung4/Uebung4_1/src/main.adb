with Ada.Real_Time; use Ada.Real_Time;
with Ada.Text_IO; use Ada.Text_IO;

procedure Main is

   task temp;
   task pres;

   task body temp is
      Next_Release : Time;
      Release_Interval : constant Time_Span := Milliseconds(1000);
   begin
      Next_Release := Clock + Release_Interval;
      loop
         select
            delay until Next_Release - Milliseconds(995);
         then abort
            Put_Line("Reading temperature...");
         end select;

         select
            delay until Next_Release - Milliseconds(10);
         then abort
            Put_Line("Calculating...");
         end select;

         select
            delay until Next_Release;
         then abort
            Put_Line("Writing sensorvalues...");
         end select;

          select
            delay until Next_Release;
         then abort
            Put_Line("Rendezvous with console task...");
         end select;


         delay until Next_Release;
         Next_Release:= Next_Release + Release_Interval;
      end loop;
   end temp;

   task body pres is
      Next_Release : Time;
      Release_Interval : constant Time_Span := Milliseconds(2000);
   begin
      Next_Release := Clock + Release_Interval;
      loop
         select
            delay until Next_Release - Milliseconds(1995);
         then abort
            Put_Line("Reading pressure...");
         end select;

         select
            delay until Next_Release - Milliseconds(10);
         then abort
            Put_Line("Calculating pressure value...");
         end select;

         select
            delay until Next_Release;
         then abort
            Put_Line("Writing pressure sensorvalues...");
         end select;

          select
            delay until Next_Release;
         then abort
            Put_Line("Rendezvous with pressure console task...");
         end select;


         delay until Next_Release;
         Next_Release:= Next_Release + Release_Interval;
      end loop;
   end pres;

begin
   --  Insert code here.
   null;
end Main;
