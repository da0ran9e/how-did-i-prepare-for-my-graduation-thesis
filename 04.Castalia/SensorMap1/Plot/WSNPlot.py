import re
import matplotlib.pyplot as plt

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

# Use the function to extract nodes (node_id, x, y)
file_path = '../omnetpp.ini'  # Replace with your file path
nodes = parse_omnetpp_ini(file_path)

# Extract node IDs, X and Y coordinates
node_ids = [node[0] for node in nodes]
x_coords = [node[1] for node in nodes]
y_coords = [node[2] for node in nodes]

# Plotting the map with nodes and their IDs
plt.figure(figsize=(10, 10))
plt.scatter(x_coords, y_coords, color='gray', label='Sensor Nodes', marker='o', s=500)

# Annotate each point with its node ID
for i, (x, y) in enumerate(zip(x_coords, y_coords)):
    plt.text(x + 0.1, y + 0.1, str(node_ids[i]), fontsize=9, color='black')

# Set the plot labels and title
plt.xlabel('X Coordinate')
plt.ylabel('Y Coordinate')
plt.title('Sensor Nodes Map with Node IDs')
plt.grid(True)

# Show the plot


plt.savefig("sensor_map.png")  # Save the plot to a PNG file
