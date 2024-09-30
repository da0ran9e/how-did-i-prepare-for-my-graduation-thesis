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
   7. Adding omnetpp.ini
8. Running the Simulation:
9. Enhancing the 2-node TicToc:
10. Turning it Into a Real Network:
11. Adding Statistics Collection:
12. Visualizing the Results: 
13. Parameter Studies:
14. Closing words:
