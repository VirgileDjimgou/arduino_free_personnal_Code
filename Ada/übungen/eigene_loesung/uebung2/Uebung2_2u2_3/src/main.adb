with Ressource; use Ressource;
with Ada.Text_IO;

procedure Main is
   task TaskA;
   task TaskB;
   task TaskC;

   task body TaskA is
   begin
      loop
         Ada.Text_IO.Put_Line("TaskA: I want the Ressource!");
         AllocateRessource (medium);
         Ada.Text_IO.Put_Line("TaskA: I got the Ressource!");
         error: Integer := DeallocateRessource;
         Ada.Text_IO.Put_Line("TaskA: I released the Ressource!");
      end loop;
   end TaskA;

   task body TaskB is
   begin
      loop
         Ada.Text_IO.Put_Line("TaskB: I want the Ressource!");
         AllocateRessource (high);
         Ada.Text_IO.Put_Line("TaskB: I got the Ressource!");
         error: Integer := DeallocateRessource;
         Ada.Text_IO.Put_Line("TaskB: I released the Ressource!");
      end loop;
   end TaskB;

   task body TaskC is
   begin
      loop
         Ada.Text_IO.Put_Line("TaskC: I want the Ressource!");
         AllocateRessource (low);
         Ada.Text_IO.Put_Line("TaskC: I got the Ressource!");
         error: Integer := DeallocateRessource;
         Ada.Text_IO.Put_Line("TaskC: I released the Ressource!");
      end loop;
   end TaskB;

begin
   --  Insert code here.
   null;
end Main;
