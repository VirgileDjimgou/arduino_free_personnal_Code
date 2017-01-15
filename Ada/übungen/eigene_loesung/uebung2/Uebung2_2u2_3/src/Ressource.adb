with Semaphore_Package; use Semaphore_Package;

package body Ressource is
   type ConditionType is array (Integer range <>) of Semaphore;
   mutex   : Semaphore := 1;
   cond    : ConditionType (0 .. 2) := (others => 0);
   busy    : Boolean := False;


   procedure AllocateRessource(prio: TaskPrio) is
   begin
      SemWait(mutex);
      if busy then
         SemSignal(mutex);
         SemWait(cond(prio));
      end if;
      busy := True;
      SemSignal(mutex);
   end AllocateRessource;

   function DeallocateRessource return Integer is
   begin
      SemWait(mutex);
      if busy then
         busy := False;
         if SemGetvalue(cond(high)) < 0 then
            SemSignal(cond(high));
         else
            if SemGetValue(cond(medium)) < 0 then
               SemSignal(cond(medium));
            else
               if SemGetValue(cond(low)) < 0 then
                  SemSignal(cond(low));
               else
                  SemSignal(mutex);
               end if;
            end if;
         end if;
      else
         return -1;
      end if;
   end DeallocateRessource;
end Ressource;
