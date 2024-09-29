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
   6. Adding omnetpp.ini
7. Running the Simulation:
8. Enhancing the 2-node TicToc:
9. Turning it Into a Real Network:
10. Adding Statistics Collection:
11. Visualizing the Results: 
12. Parameter Studies:
13. Closing words:
