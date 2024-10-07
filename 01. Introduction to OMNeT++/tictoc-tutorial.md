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

         > [!NOTE]:
         > **Don't forget the `Define_Module()`**
         
         - We redefine two methods from `cSimpleModule`: `initialize()` and `handleMessage()`. They are invoked from the simulation kernel: the first one only once, and the second one whenever a message arrives at the module.
         - In `initialize()` we create a message object (`cMessage`), and send it out.
         - Since this gate is connected to the other module's input gate, the simulation kernel will deliver this message to the other module in the argument to `handleMessage()` -- after a 100ms propagation delay assigned to the link in the NED file. The other module just sends it back (another 100ms delay), so it will result in a continuous ping-pong.
         - Messages (packets, frames, jobs, etc) and events (timers, timeouts) are all represented by `cMessage` objects (or its subclasses) in OMNeT++.
         - After you send or schedule them, they will be held by the simulation kernel in the "scheduled events" or "future events" list until their time comes and they are delivered to the modules via handleMessage().

         > [!NOTE]:
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
	- Make the model look a bit prettier in the GUI. 
	- We assign the block/routing icon (the file images/block/routing.png), and paint it cyan for tic and yellow for toc. 
	- This is achieved by adding display strings to the NED file. The i= tag in the display string specifies the icon.
	```c
    simple Txc2
    {
        parameters:
            @display("i=block/routing"); // add a default icon
        gates:
            input in;
            output out;
    }
    ```
   2. Adding logging
   - We add log statements to Txc1 so that it prints what it is doing. 
   - OMNeT++ provides a sophisticated logging facility with log levels, log channels, filtering, etc.
   - In this model we'll use its simplest form EV:
   ```c
   EV << "Received message `" << msg->getName() << "', sending it out again\n";
   ```
   - You can also open separate output windows for tic and toc by <mark>right-clicking on their icons and choosing Component log</mark> from the menu.
   3. Adding state variables
   - In this step we add a counter to the module, and delete the message after ten exchanges.
   ```c
       class Txc3 : public cSimpleModule
    {
      private:
        int counter;  // Note the counter here

      protected:
    ```
    - set the variable to 10 in `initialize()` and decrement in `handleMessage()`
    - the line`WATCH(counter);` makes it possible to see the counter value in the graphical runtime environment.
   4. Adding parameters
   - we'll turn the "magic number" 10 into a parameter and add a boolean parameter to decide whether the module should send out the first message in its initialization code.
   - Module parameters have to be declared in the NED
   - The data type can be numeric, string, bool, or xml (the latter is for easy access to XML config files), among others.
    ```c
    simple Txc4
    {
        parameters:
            bool sendMsgOnInit = default(false); // whether the module should send out a message on initialization
            int limit = default(2);   // another parameter with a default value
            @display("i=block/routing");
        gates:
    ```
    - C++ code to read the parameter in initialize(), and assign it to the counter.
    ```c
    counter = par("limit");
    ```
    - the second parameter to decide whether to send initial message:
    ```c
        if (par("sendMsgOnInit").boolValue() == true) {
    ```
    - we assign one parameter in the NED file:
    ```c
    network Tictoc4
    {
        submodules:
            tic: Txc4 {
                parameters:
                    sendMsgOnInit = true;
                    @display("i=,cyan");
            }
            toc: Txc4 {
                parameters:
                    sendMsgOnInit = false;
                    @display("i=,gold");
            }
        connections:
    ```
    - and the other in `omnetpp.ini`:
    ```c
    Tictoc4.toc.limit = 5
    ```
    > [!NOTE]:
    >  omnetpp.ini supports wildcards, we could have used: ` Tictoc4.t*c.limit=5 ` or `Tictoc4.*.limit=5` or even `**.limit=5`

   5. Using NED inheritance
   - Here is the base module:
   ```c
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
   ```
   - And here is the derived module:
   ```c
   simple Tic5 extends Txc5
    {
        parameters:
            @display("i=,cyan");
            sendMsgOnInit = true;   // Tic modules should send a message on init
    }
   ``` 
   6. Modeling processing delay
   - We added two `cMessage *` variables, `event` and `tictocMsg` to the class, to remember the message we use for timing and message whose processing delay we are simulating.
   ```c
   class Txc6 : public cSimpleModule
    {
      private:
        // Set the pointers to nullptr, so that the destructor won't crash
        // even if initialize() doesn't get called because of a runtime
        // error or user cancellation during the startup process.
        cMessage *event = nullptr;  // pointer to the event object which we'll use for timing
        cMessage *tictocMsg = nullptr;  // variable to remember the message until we send it back

      public:
   ```
   - We "send" the self-messages with the `scheduleAt()` function, specifying when it should be delivered back to the module.
   ```c
   scheduleAt(simTime()+1.0, event);
   ```
   - In handleMessage() now we have to differentiate whether a new message has arrived via the input gate or the self-message came back (timer expired). Here we are using
   ```c
    if (msg == event)
   ```
    - but we could have written

    ```c
    if (msg->isSelfMessage())
    ```
   7. Random numbers and parameters
   - We change the delay from 1s to a random value which can be set from the NED file or from omnetpp.ini. 
   - Module parameters are able to return random variables
   - To make use of this feature we have to read the parameter in handleMessage() every time we use it.
   ```c
    // The "delayTime" module parameter can be set to values like
    // "exponential(5)" (tictoc7.ned, omnetpp.ini), and then here
    // we'll get a different delay every time.
    simtime_t delay = par("delayTime");

    EV << "Message arrived, starting to wait " << delay << " secs...\n";
    tictocMsg = msg;
   ```
   - We'll "lose" (delete) the packet with a small probability
   ```c
    if (uniform(0, 1) < 0.1) {
        EV << "\"Losing\" message\n";
        delete msg;
    }
    ```
    - We'll assign the parameters in omnetpp.ini:
    ```java
    Tictoc7.tic.delayTime = exponential(3s)
    Tictoc7.toc.delayTime = truncnormal(3s,1s)
    ```
    - You can experiment with different seeds 
    ``` sql
    [General]
    seed-0-mt=532569  # or any other 32-bit value
    ```
   8. Timeout, cancelling timers
   - Transform model into a stop-and-wait simulation.
   - `toc` will "lose" the message with some nonzero probability, and in that case `tic` will have to resend it.
   ```c
   void Toc8::handleMessage(cMessage *msg)
    {
        if (uniform(0, 1) < 0.1) {
            EV << "\"Losing\" message.\n";
            bubble("message lost");  // making animation more informative...
            delete msg;
        }
        else {
    ```
    - `bubble()` call to display the message.
    - `tic` will start a timer whenever it sends the message. When the timer expires, we'll assume the message was lost and send another one. If toc's reply arrives, the timer has to be cancelled. The timer will be (what else?) a self-message.
    ```c
    scheduleAt(simTime()+timeout, timeoutEvent);
    ```
    - Cancelling the timer will be done with the cancelEvent() call. Note that this does not prevent us from being able to reuse the same timeout message over and over.
    ```c
    cancelEvent(timeoutEvent);
    ```

   9. Retransmitting the same message
    - Keep the original packet and send only copies of it. We delete the original when toc's acknowledgement arrives. To make it easier to visually verify the model, we'll include a message sequence number in the message names.
    - To avoid `handleMessage()` growing too large, we'll put the corresponding code into two new functions, `generateNewMessage()` and `sendCopyOf()` and call them from `handleMessage()`.
    ```c
    cMessage *Tic9::generateNewMessage()
    {
        // Generate a message with a different name every time.
        char msgname[20];
        sprintf(msgname, "tic-%d", ++seq);
        cMessage *msg = new cMessage(msgname);
        return msg;
    }
    void Tic9::sendCopyOf(cMessage *msg)
    {
        // Duplicate message and send the copy.
        cMessage *copy = (cMessage *)msg->dup();
        send(copy, "out");
    }                   
    ```
## Turning it Into a Real Network:
   1. More than two nodes
   - the Txc module will need to have multiple input and output gates:
   ```c
       simple Txc10
    {
        parameters:
            @display("i=block/routing");
        gates:
            input in[];  // declare in[] and out[] to be vector gates
            output out[];
    }
   ```
   - The size of the vector (the number of gates) will be determined where we use Txc to build the network.
   ```c++
    network Tictoc10
    {
        submodules:
            tic[6]: Txc10;
        connections:
            tic[0].out++ --> {  delay = 100ms; } --> tic[1].in++;
            tic[0].in++ <-- {  delay = 100ms; } <-- tic[1].out++;

            tic[1].out++ --> {  delay = 100ms; } --> tic[2].in++;
            tic[1].in++ <-- {  delay = 100ms; } <-- tic[2].out++;

            tic[1].out++ --> {  delay = 100ms; } --> tic[4].in++;
            tic[1].in++ <-- {  delay = 100ms; } <-- tic[4].out++;

            tic[3].out++ --> {  delay = 100ms; } --> tic[4].in++;
            tic[3].in++ <-- {  delay = 100ms; } <-- tic[4].out++;

            tic[4].out++ --> {  delay = 100ms; } --> tic[5].in++;
            tic[4].in++ <-- {  delay = 100ms; } <-- tic[5].out++;
    }
   ```
   - `getIndex()` function which returns the index of the module in the vector.
   - `forwardMessage()` function which is invoked from `handleMessage()` whenever a message arrives at the node. It draws a random gate number, and sends out message on that gate.
   ```c
   void Txc10::forwardMessage(cMessage *msg)
    {
        // In this example, we just pick a random gate to send it on.
        // We draw a random number between 0 and the size of gate `out[]'.
        int n = gateSize("out");
        int k = intuniform(0, n-1);

        EV << "Forwarding message " << msg << " on port out[" << k << "]\n";
        send(msg, "out", k);
    }
   ```
   2. Channels and inner type definitions
   - create a channel type which specifies the delay parameter and we will use that type for all connections in the network
   ```c
   network Tictoc11
    {
        types:
            channel Channel extends ned.DelayChannel {
                delay = 100ms;
            }
        submodules:
   ```
   ```c
   connections:
        tic[0].out++ --> Channel --> tic[1].in++;
        tic[0].in++ <-- Channel <-- tic[1].out++;

        tic[1].out++ --> Channel --> tic[2].in++;
        tic[1].in++ <-- Channel <-- tic[2].out++;

        tic[1].out++ --> Channel --> tic[4].in++;
        tic[1].in++ <-- Channel <-- tic[4].out++;

        tic[3].out++ --> Channel --> tic[4].in++;
        tic[3].in++ <-- Channel <-- tic[4].out++;

        tic[4].out++ --> Channel --> tic[5].in++;
        tic[4].in++ <-- Channel <-- tic[5].out++;
   ```
   3. Using two-way connections
   -  define two-way (or so called inout) gates instead of the separate input and output gates we used previously.
   ```c
    simple Txc12
    {
        parameters:
            @display("i=block/routing");
        gates:
            inout gate[];  // declare two way connections
    }
   ```
   ```c
    connections:
        tic[0].gate++ <--> Channel <--> tic[1].gate++;
        tic[1].gate++ <--> Channel <--> tic[2].gate++;
        tic[1].gate++ <--> Channel <--> tic[4].gate++;
        tic[3].gate++ <--> Channel <--> tic[4].gate++;
        tic[4].gate++ <--> Channel <--> tic[5].gate++;
   ```
   ```c
   void Txc12::forwardMessage(cMessage *msg)
    {
        // In this example, we just pick a random gate to send it on.
        // We draw a random number between 0 and the size of gate `gate[]'.
        int n = gateSize("gate");
        int k = intuniform(0, n-1);

        EV << "Forwarding message " << msg << " on gate[" << k << "]\n";
        // $o and $i suffix is used to identify the input/output part of a two way gate
        send(msg, "gate$o", k);
    }
    ```
   4. Defining our message class
   - subclass cMessage and add destination as a data member
   ```c
   message TicTocMsg13
    {
        int source;
        int destination;
        int hopCount = 0;
    }
    ```
    ```c
    TicTocMsg13 *msg = new TicTocMsg13(msgname);
    msg->setSource(src);
    msg->setDestination(dest);
    return msg;
    ```
    ```c
    void Txc13::handleMessage(cMessage *msg)
    {
        TicTocMsg13 *ttmsg = check_and_cast<TicTocMsg13 *>(msg);

        if (ttmsg->getDestination() == getIndex()) {
    ```
    - include tictoc13_m.h into our C++ code `#include "tictoc13_m.h"`
12. Adding Statistics Collection:
13. Visualizing the Results: 
14. Parameter Studies:
15. Closing words:
