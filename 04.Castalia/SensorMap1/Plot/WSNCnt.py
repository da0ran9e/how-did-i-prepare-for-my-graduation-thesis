import matplotlib.pyplot as plt
import re

def parse_wsn_map(file_path):
    connections = []
    msg_type_colors = {1: 'blue', 2: 'yellow', 3: 'green', 4: 'red'}

    with open(file_path, 'r') as file:
        lines = file.readlines()

    # Improved Regex pattern to allow for potential extra spaces
    pattern = re.compile(r'Msg type (\d+)\s*org: \d+\s*from: (\d+)\s*to: (\d+)')

    for line in lines:
        print(f"Line: '{line.strip()}'")  # Print each line for debugging
        match = pattern.search(line)
        if match:
            msg_type = int(match.group(1))
            from_node = int(match.group(2))
            to_node = int(match.group(3))
            print(f"Found message type {msg_type} from node {from_node} to node {to_node}")
            connections.append((from_node, to_node, msg_type))
        else:
            print(f"No match for: '{line.strip()}'")  # Debugging non-matching lines

    return connections, msg_type_colors

def parse_omnetpp_ini(file_path):
    nodes = []

    with open(file_path, 'r') as file:
        lines = file.readlines()

    # Regex to match the coordinates from the 'SN.node[*].xCoor' and 'SN.node[*].yCoor'
    xcoord_pattern = re.compile(r'SN\.node\[(\d+)\]\.xCoor\s*=\s*(-?\d+\.\d+)')
    ycoord_pattern = re.compile(r'SN\.node\[(\d+)\]\.yCoor\s*=\s*(-?\d+\.\d+)')

    # Dictionary to hold coordinates and node IDs
    coords_dict = {}

    for line in lines:
        x_match = xcoord_pattern.search(line)
        y_match = ycoord_pattern.search(line)

        if x_match:
            node_id = int(x_match.group(1))
            x_coord = float(x_match.group(2))
            coords_dict[node_id] = [x_coord, None]  # Store x, but y is unknown yet

        if y_match:
            node_id = int(y_match.group(1))
            y_coord = float(y_match.group(2))

            if node_id in coords_dict:
                coords_dict[node_id][1] = y_coord  # Assign the y value to the corresponding node

    # Convert to list of (node_id, x, y)
    nodes = [(node_id, coords[0], coords[1]) for node_id, coords in coords_dict.items()]

    return nodes

# File path to your OMNeT++ ini file
omnet_file_path = '../omnetpp.ini'  # Replace with your actual file path
nodes = parse_omnetpp_ini(omnet_file_path)

# Extract node IDs, X and Y coordinates
node_ids = [node[0] for node in nodes]
x_coords = [node[1] for node in nodes]
y_coords = [node[2] for node in nodes]

# File path to your WSNMap.txt file
wsn_file_path = 'WSNMap4.txt'  # Replace with your actual file path

# Parse the file and get connections
connections, msg_type_colors = parse_wsn_map(wsn_file_path)

plt.figure(figsize=(10, 10))

# Plot sensor nodes as gray circles
plt.scatter(x_coords, y_coords, color='gray', label='Sensor Nodes', marker='o', s=500)    

# Annotate each point with its node ID
for i, (x, y) in enumerate(zip(x_coords, y_coords)):
    plt.text(x + 0.1, y + 0.1, str(node_ids[i]), fontsize=9, color='black')

# Iterate over the connections and plot lines with different colors based on message type
for from_node, to_node, msg_type in connections:
    # Get the coordinates for the from and to nodes
    x_from, y_from = next((x, y) for node_id, x, y in nodes if node_id == from_node)
    x_to, y_to = next((x, y) for node_id, x, y in nodes if node_id == to_node)

    # Plot the connection line with the appropriate color
    plt.plot([x_from, x_to], [y_from, y_to], color=msg_type_colors[msg_type], lw=2)

# Set the plot labels and title
plt.xlabel('X Coordinate')
plt.ylabel('Y Coordinate')
plt.title('Sensor Nodes Map with Node IDs and Connections')
plt.grid(True)

# Save and show the plot
plt.savefig("sensor_map_con_4.png")
plt.show()  # To display the plot interactively
