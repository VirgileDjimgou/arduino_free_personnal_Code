with Ada.Text_IO;

procedure Main is

   EinfahrtAnfrage: Boolean := False;
   AusfahrtAnfrage: Boolean := False;

   task EinfahrtSchranke;
   task AusfahrtSchranke;
   task Signal is
      entry PlatzVerfuegbar(verfuegbar: out Boolean);
      entry Increase;
      entry Decrease;
   end Signal;

   task body Einfahrtschranke is
      frei: Boolean := False;
   begin
      loop
         if EinfahrtAnfrage then
            Signal.PlatzVerfuegbar (frei);
            if frei then
               -- Schranke oeffnen
               -- Warten bis Auto schranke passiert
               Signal.Increase;
               -- Schranke schliessen
               EinfahrtAnfrage := False;
            end if;
         end if;
      end loop;
   end EinfahrtSchranke;

   task body AusfahrtSchranke is

   begin
      loop
         if AusfahrtAnfrage then
            -- Schranke oeffnen
            -- Warten bis Auto schranke passiert
            Signal.Decrease;
            -- Schranke schliessen
            AusfahrtAnfrage := False;
         end if;
      end loop;
   end AusfahrtSchranke;

   task body Signal is
      maxAnzahl: Integer := 2;
      anzahl: Integer := 0;
   begin
      loop
         select
            accept PlatzVerfuegbar (verfuegbar: out Boolean) do
               Ada.Text_IO.Put_Line (Integer'Image(anzahl));
               if (anzahl < maxAnzahl) then
                  verfuegbar := True;
               else
                  verfuegbar := False;
               end if;
            end PlatzVerfuegbar;
         or
            accept Increase  do
               anzahl := anzahl + 1;
               Ada.Text_IO.Put_Line ("Increase aufgerufen");
            end Increase;
         or
            accept Decrease  do
               anzahl := anzahl - 1;
               Ada.Text_IO.Put_Line ("Decrease aufgerufen");
            end Decrease;
         end select;
      end loop;
   end Signal;

begin
   --  Insert code here.
   EinfahrtAnfrage := True;
   delay 2.0;
   EinfahrtAnfrage := True;
   delay 2.0;
   EinfahrtAnfrage := True;
   delay 1.0;
   AusfahrtAnfrage := True;

   null;
end Main;
