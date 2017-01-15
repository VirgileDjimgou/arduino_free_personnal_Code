

package Semaphore_Package is
   type Semaphore is limited private;
   procedure SemWait(S: in out Semaphore);
   procedure SemSignal(S: in out Semaphore);
   procedure SemGetvalue(S: in out Semaphore);
   procedure SemInit(N: in Integer);

private
   protected type Semaphore is
      procedure SemInit(N: in Integer);
      entry SemWait;
      procedure SemSignal;
   private
      slots : Integer;
   end Semaphore;
end Semaphore_Package;
