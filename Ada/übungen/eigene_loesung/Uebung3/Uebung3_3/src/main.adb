with Ada.Text_IO;
with client; use client;
with server; use server;

procedure Main is

  type Servpointer is access Server_Type;
  serv: Servpointer := new Server_Type;


   ClientA: Client_Type (serv);
   ClientB: Client_Type (serv);

begin
   --  Insert code here.
   null;
end Main;
