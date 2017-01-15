with Ada.Text_IO;

procedure main is
task A;				-- Deklaration der Tasks
task B;

task body A is
begin
   Ada.Text_IO.Put_Line("blu");
end A;	        -- Implementierung der Tasks

task body B is
begin
   Ada.Text_IO.Put_Line("bla");
end B;

begin					-- Tasks werden gestartet
     null;
end main;				-- Prozedur terminiert wenn alle Tasks fertig sind
