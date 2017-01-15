package Ressource is
   type TaskPrio is (high, medium, low);
   procedure AllocateRessource(Prio: TaskPrio);
   function DeallocateRessource;
end Ressource;
