  simple Txc2
  {
      gates:
          input in;
          output out;
  }
  
  //
  // Two instances (tic and toc) of Txc1 connected both ways.
  // Tic and toc will pass messages to one another.
  //
  network Tictoc2
  {
      submodules:
          tic: Txc2;
          toc: Txc2;
      connections:
          tic.out --> {  delay = 100ms; } --> toc.in;
          tic.in <-- {  delay = 100ms; } <-- toc.out;
  }