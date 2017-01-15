with Data_Types; use Data_Types;
with MyIO; use MyIO;
procedure Main is

   BlockedGate: Boolean := False;
   Blocked: condition;

   task Signal is
      entry einfahrt();
      entry ausfahrt();
      entry notfall();
      --  Definition eine neu Rendez Vous
      entry ausfall_1();
      entry ausfall_2();
      entry Test_Second_Entry();
      entry  Test_ThirdENtry();

      -- Second Entry nur testen  ...
      --- die muss unbedingt spater geloscht werden

      entry Notfall_Second()

      entry notfall_aufheben(Parkplaetze: Integer);
   end Signal;

   task Einfahrt is
      entry suspend;
   end Einfahrt;

   task Ausfahrt is
      entry suspend;
   end Ausfahrt;


   task body Einfahrt is
      EA : EAnfrage := False;
      EOpen : Einfahrt := Open;
      EClose : Einfahrt := Close;
      ED : EDurchfahrt := False;
   begin
      loop
         if BlockedGate = True then wait(Blocked);
         Read(EA);
         if (EA = True) then --Wenn Anfrage Einfahrt
            Signal.einfahrt() ; --Signal informieren, ggfs. warten
            Write(EOpen); --Einfahrt öffnen
            ED = True; --Durchfahrt beginnt
            select
               delay 10;
               Signal.notfall();
            then abort
               while (ED = True) loop
                  Read(ED); -- Durchfahren lassen
               end loop;
               Write(EClose); --Einfahrt schließen
            end select;
         end if;
      end loop;
   end Einfahrt;

   task body Ausfahrt is
      AA : AAnfrage := False;
      AD : ADurchfahrt := False;
      AOpen : Ausfahrt := Open;
      AClose : Ausfahrt := Close;
   begin
      loop
         if BlockedGate = True then wait(Blocked); end if;
         Read(AA);
         if (AA = true) then --Wenn Anfrage Ausfahrt
            Signal.ausfahrt(); --Signal informieren
            Write(AOpen); --Ausfahrt öffnen
            AD = True; --Durchfahrt beginnt
            select
               delay 10;
               Signal.notfall();
            then abort
               while (AD = True) loop
                  Read(AD);  --Durchfahren lassen
               end loop;
               Write(AClose); --Ausfahrt schließen
            end select;

         end if;
      end loop;
   end Ausfahrt;

   task body Signal is
      PP: Integer := 0;

   begin
      write(Free); --Signal auf grün
      loop
         select
            when PP < 50 =>
               accept einfahrt do
                  PP := PP+1;
                  if PP = 50 then
                     write(Full); --Signal auf Rot setzen
                  end if;
               end einfahrt;
         or
            accept ausfahrt do
               if PP = 50 then write(Free); --Signal auf Grün setzen
                  PP := PP-1;
               end if;
            end ausfahrt;
         or
            accept notfall do
               write(Full); --Signal auf Rot setzen
                            --Einfahrt und Ausfahrt blockieren
               BlockedGate := True;

            end notfall
         or
            accept notfall_aufheben(Parkplaetze: Integer) do
               PP := Parkplaetze;
               BlockedGate := False;      --Tasks aufwecken
               signal(Blocked);
            end notfall_aufheben;
         or
            terminate; -- Falls Einfahrt und Ausfahrt nicht mehr aktiv sind
            -- Wird auch die Signal-Task beendet.
         end select;
      end loop
   end Signal;
begin null;
end Main; --Einfahrt, Ausfahrt und Signal werden gestartet
