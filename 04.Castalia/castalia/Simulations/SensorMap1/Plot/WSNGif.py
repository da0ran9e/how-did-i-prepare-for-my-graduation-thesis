import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import re

def parse_wsn_map(file_path):
    connections = []
    msg_type_colors = {1: 'blue', 2: 'yellow', 3: 'green', 4: 'red'}

    with open(file_path, 'r') as file:
        lines = file.readlines()

    # Regex pattern to extract message type, from and to nodes
    pattern = re.compile(r'Msg type (\d+)\s*org: \d+\s*from: (\d+)\s*to: (\d+)')

    for line in lines:
        match = pattern.search(line)
        if match:
            msg_type = int(match.group(1))
            from_node = int(match.group(2))
            to_node = int(match.group(3))
            connections.append((from_node, to_node, msg_type))

    return connections, msg_type_colors

def parse_omnetpp_ini(file_path):
    nodes = []

    with open(file_path, 'r') as file:
        lines = file.readlines()

    # Regex to match coordinates
    xcoord_pattern = re.compile(r'SN\.node\[(\d+)\]\.xCoor\s*=\s*(-?\d+\.\d+)')
    ycoord_pattern = re.compile(r'SN\.node\[(\d+)\]\.yCoor\s*=\s*(-?\d+\.\d+)')

    coords_dict = {}

    for line in lines:
        x_match = xcoord_pattern.search(line)
        y_match = ycoord_pattern.search(line)

        if x_match:
            node_id = int(x_match.group(1))
            x_coord = float(x_match.group(2))
            coords_dict[node_id] = [x_coord, None]

        if y_match:
            node_id = int(y_match.group(1))
            y_coord = float(y_match.group(2))

            if node_id in coords_dict:
                coords_dict[node_id][1] = y_coord

    nodes = [(node_id, coords[0], coords[1]) for node_id, coords in coords_dict.items()]
    return nodes

# File paths
omnet_file_path = '../omnetpp.ini'  # Your OMNeT++ ini file path
wsn_file_path = 'WSNMap.txt'  # Your WSNMap.txt file path

# Parse the data
nodes = parse_omnetpp_ini(omnet_file_path)
connections, msg_type_colors = parse_wsn_map(wsn_file_path)

# Extract coordinates and node IDs
node_ids = [node[0] for node in nodes]
x_coords = [node[1] for node in nodes]
y_coords = [node[2] for node in nodes]

# Set up the plot
fig, ax = plt.subplots(figsize=(10, 10))
ax.scatter(x_coords, y_coords, color='gray', label='Sensor Nodes', marker='o', s=500)
for i, (x, y) in enumerate(zip(x_coords, y_coords)):
    ax.text(x + 0.1, y + 0.1, str(node_ids[i]), fontsize=9, color='black')

# Function to animate each step
def update(step):
    ax.clear()  # Clear previous frame
    ax.scatter(x_coords, y_coords, color='gray', label='Sensor Nodes', marker='o', s=500)
    for i, (x, y) in enumerate(zip(x_coords, y_coords)):
        ax.text(x + 0.1, y + 0.1, str(node_ids[i]), fontsize=9, color='black')
    
    # Plot connections up to the current step
    for from_node, to_node, msg_type in connections[:step + 1]:
        x_from, y_from = next((x, y) for node_id, x, y in nodes if node_id == from_node)
        x_to, y_to = next((x, y) for node_id, x, y in nodes if node_id == to_node)
        ax.plot([x_from, x_to], [y_from, y_to], color=msg_type_colors[msg_type], lw=2)
    
    ax.set_xlabel('X Coordinate')
    ax.set_ylabel('Y Coordinate')
    ax.set_title('Sensor Nodes Map with Node IDs and Connections')
    ax.grid(True)

# Limit frames or the number of connections (whichever is smaller)
max_frames = min(1000, len(connections))

# Create the animation
ani = FuncAnimation(fig, update, frames=max_frames, repeat=False)

# Save the animation as GIF
ani.save('sensor_map_animation1.gif', writer='imagemagick', fps=10)
