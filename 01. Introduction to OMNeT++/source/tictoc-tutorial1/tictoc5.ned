  simple Txc5
  {
      parameters:
          bool sendMsgOnInit = default(false);
          int limit = default(2);
          @display("i=block/routing");
      gates:
          input in;
          output out;
  }
  
  simple Tic5 extends Txc5
{
    parameters:
        @display("i=,cyan");
        sendMsgOnInit = true;   // Tic modules should send a message on init
}
simple Toc5 extends Txc5
{
    parameters:
        @display("i=,gold");
        sendMsgOnInit = false;  // Toc modules should NOT send a message on init
}

 network Tictoc5
{
    submodules:
        tic: Tic5;  // the limit parameter is still unbound here. We will get it from the ini file
        toc: Toc5;
    connections:
          tic.out --> {  delay = 100ms; } --> toc.in;
          tic.in <-- {  delay = 100ms; } <-- toc.out;
  }