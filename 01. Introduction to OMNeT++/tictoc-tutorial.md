# Tictoc tutorial
## Introduction:
   - The Example simulation in the `samples/tictoc` directory.
   - Nice to have a good C++ knowledge,  familiar with C/C++ development (editing source files, compiling, debugging etc.)
   - All source code is cross-linked to the OMNeT++ API documentation.
   - The original TicToc tutorial from Ahmet Sekercioglu (Monash University).
## Getting Started:
   1. The model:
      - Start with a "network" that consists of two nodes.
      - One of the nodes will create a packet, and the two nodes will keep passing the same packet back and forth.
      - We'll call the nodes `tic` and `toc`.
   2. Setting up the project
      - Start the OMNeT++ IDE by typing `omnetpp` in your terminal.
      - Choose <mark>New -> OMNeT++ Project</mark> from the menu.
      ![image](https://github.com/user-attachments/assets/96bc4e55-504c-41aa-90c7-0d1d29a67da3)
      - Enter `tictoc` as project name 
      - Choose <mark>Empty project</mark> when asked about the initial content of the project
      - Then click <mark>Finish</mark>.
   3. Adding the NED file
      - OMNeT++ uses NED files to define components and to assemble them into larger units like networks.
      - We start implementing our model by adding a NED file.
      - Right-click the project directory and <mark>choose New -> Network Description File (NED)</mark> from the menu.
      - Enter `tictoc1.ned` as the name.
      - Switch into Source mode, and enter:
      ``` c
        simple Txc1
        {
            gates:
                input in;
                output out;
        }
        
        //
        // Two instances (tic and toc) of Txc1 connected both ways.
        // Tic and toc will pass messages to one another.
        //
        network Tictoc1
        {
            submodules:
                tic: Txc1;
                toc: Txc1;
            connections:
                tic.out --> {  delay = 100ms; } --> toc.in;
                tic.in <-- {  delay = 100ms; } <-- toc.out;
        }
      ```
       - switch back to Design mode:
       ![image](https://github.com/user-attachments/assets/8b1c4ee1-9beb-4b40-9e0d-a47eaae39dc2)
       - The first block in the file declares `Txc1` as a simple module type.
       - `Txc1` has an input gate named `in`, and an output gate named `out`.
       - The second block declares Tictoc1 as a network.
    
   5. Adding the C++ files
      - Create a file named `txc1.cc` by choosing <mark>New -> Source File</mark>
      - Enter the following content:
        ```C
         #include <string.h>
         #include <omnetpp.h>
         
         using namespace omnetpp;
         
         /**
          * Derive the Txc1 class from cSimpleModule. In the Tictoc1 network,
          * both the `tic' and `toc' modules are Txc1 objects, created by OMNeT++
          * at the beginning of the simulation.
          */
         class Txc1 : public cSimpleModule
         {
           protected:
             // The following redefined virtual function holds the algorithm.
             virtual void initialize() override;
             virtual void handleMessage(cMessage *msg) override;
         };
         
         // The module class needs to be registered with OMNeT++
         Define_Module(Txc1);
         
         void Txc1::initialize()
         {
             // Initialize is called at the beginning of the simulation.
             // To bootstrap the tic-toc-tic-toc process, one of the modules needs
             // to send the first message. Let this be `tic'.
         
             // Am I Tic or Toc?
             if (strcmp("tic", getName()) == 0) {
                 // create and send first message on gate "out". "tictocMsg" is an
                 // arbitrary string which will be the name of the message object.
                 cMessage *msg = new cMessage("tictocMsg");
                 send(msg, "out");
             }
         }
         
         void Txc1::handleMessage(cMessage *msg)
         {
             // The handleMessage() method is called whenever a message arrives
             // at the module. Here, we just send it to the other module, through
             // gate `out'. Because both `tic' and `toc' does the same, the message
             // will bounce between the two.
             send(msg, "out"); // send out the message
         }
        ```
        - The `Txc1` simple module type is represented by the C++ class `Txc1`.
        - The `Txc1` class needs to subclass from OMNeT++'s `cSimpleModule` class, and needs to be registered in OMNeT++ with the `Define_Module()` macro.

         > [!NOTE]
         > **Don't forget the `Define_Module()`**
         
         - We redefine two methods from `cSimpleModule`: `initialize()` and `handleMessage()`. They are invoked from the simulation kernel: the first one only once, and the second one whenever a message arrives at the module.
         - In `initialize()` we create a message object (`cMessage`), and send it out.
         - Since this gate is connected to the other module's input gate, the simulation kernel will deliver this message to the other module in the argument to `handleMessage()` -- after a 100ms propagation delay assigned to the link in the NED file. The other module just sends it back (another 100ms delay), so it will result in a continuous ping-pong.
         - Messages (packets, frames, jobs, etc) and events (timers, timeouts) are all represented by `cMessage` objects (or its subclasses) in OMNeT++.
         - After you send or schedule them, they will be held by the simulation kernel in the "scheduled events" or "future events" list until their time comes and they are delivered to the modules via handleMessage().

         > [!NOTE]
         > Note that there is no stopping condition built into this simulation: it would continue forever. You will be able to stop it from the GUI. (You could also specify a simulation time limit or CPU time limit in the configuration file, but we don't do that in the tutorial.)
   5. Adding omnetpp.ini
      - To be able to run the simulation, we need to create an `omnetpp.ini` file.
      - `omnetpp.ini` tells the simulation program which network you want to simulate (as NED files may contain several networks).
      - Create an `omnetpp.ini` file using the <mark>File -> New -> Initialization</mark> file (INI) menu item.
      - The Inifile Editor also has two modes, Form and Source.
      - For now, just switch to Source mode and enter the following:
      ```
      [General]
      network = Tictoc1
      ```
      - Save and compile the project.
## Running the Simulation:
   1. Launching the simulation program
      - Select `omnetpp.ini`
      - Press the Run button
      >
      >If you want to build the simulation executable on the command-line, create a Makefile using the opp_makemake command, then enter make to build the project. It will produce an executable that can be run by entering ./tictoc
   2. Running the simulation
      - Press the Run button on the toolbar to start the simulation.
      - You can stop the simulation by hitting F8 (equivalent to the STOP button on the toolbar), single-step through it (F4), run it with (F5) or without (F6) animation. F7 (express mode) completely turns off tracing features for maximum speed.
   3. Debugging
      - Click the Debug button on the IDE's main toolbar.
      - The simulation program will be launched under a debugger (usually gdb).
      - The IDE will switch into "Debug perspective"
   4. The Debug/Run dialog
      - clicking the little down arrow next to the Run (Debug) toolbar button to open a menu, and choosing Run (Debug) Configurations...
      - Click the name of a launch configuration (e.g. `tictoc`) while holding down the `Ctrl`
   5. Visualizing on a Sequence Chart
      - The OMNeT++ simulation kernel can record the message exchanges during the simulation into an event log file.
      - Check the Record eventlog checkbox in the launch configuration dialog
      - Alternatively, you can specify record-eventlog = true in omnetpp.ini, or even, use the Record button in the Qtenv graphical runtime environment after launching.
      - The results directory in the project folder contains the .elog file.
## Enhancing the 2-node TicToc:
   1. Adding icons
   2. Adding logging
   3. Adding state variables
   4. Adding parameters
   5. Using NED inheritance
   6. Modeling processing delay
   7. Random numbers and parameters
   8. Timeout, cancelling timers
   9. Retransmitting the same message
 
11. Turning it Into a Real Network:
12. Adding Statistics Collection:
13. Visualizing the Results: 
14. Parameter Studies:
15. Closing words:
