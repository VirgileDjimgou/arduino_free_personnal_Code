package body Semaphore_Package is
   protected body Semaphore is
      procedure SemSignal is
      begin
         slots++;
      end;

      entry SemWait when (slots > 0) is
      begin
         slots--;
      end;

   end Semaphore;

end Semaphore_Package;
