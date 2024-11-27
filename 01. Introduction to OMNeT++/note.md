C:/Users/vuduc/OneDrive/Documents/GitHub/IoTOMNetpp/omnetpp-6.0.3/samples/veins-master/sumo-launchd.py -vv -c 'C:/Users/vuduc/OneDrive/Documents/GitHub/IoTOMNetpp/sumo-1.21.0/bin/sumo.exe'
C:\Users\vuduc\OneDrive\Documents\GitHub\IoTOMNetpp\cookiecutter-veins-project\ -o, --output-dir "C:\Users\vuduc\OneDrive\Documents\GitHub\how-did-i-prepare-for-my-graduation-thesis\01. Introduction to OMNeT++\source\Project\Veins"
C:\Users\vuduc\AppData\Local\Programs\Python\Python313\Scripts>

netconvert --node-files=sumotest.nod.xml --edge-files-sumotest.edg.xml -o sumotest.net.xml
C:/Users/vuduc/OneDrive/Documents/GitHub/IoTOMNetpp/sumo-1.21.0/bin/netconvert --node-files=sumotest.nod.xml --edge-files=sumotest.edg.xml -o sumotest.net.xml

py C:/Users/vuduc/OneDrive/Documents/GitHub/IoTOMNetpp/sumo-1.21.0/tools/randomtrips.py -n sumotest.net.xml -e 600 -o sumotest.trips.xml

C:/Users/vuduc/OneDrive/Documents/GitHub/IoTOMNetpp/sumo-1.21.0/bin/duarouter -n sumotest.net.xml --route-files sumotest.trips.xml -o sumotest.rout.xml --ignore-errors