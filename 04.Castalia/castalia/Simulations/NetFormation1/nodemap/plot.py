import matplotlib
matplotlib.use('TkAgg')  # Use Tkinter instead of GTK
import matplotlib.pyplot as plt


# Function to parse the text file and extract node coordinates
def parse_node_coordinates(file_path):
    node_coordinates = {}
    with open(file_path, 'r') as file:
        for line in file:
            # Match lines with xCoor or yCoor
            if "xCoor" in line:
                node_id = int(line.split('[')[1].split(']')[0])
                x = float(line.split('=')[1].strip())
                node_coordinates[node_id] = {'x': x}
            elif "yCoor" in line:
                node_id = int(line.split('[')[1].split(']')[0])
                y = float(line.split('=')[1].strip())
                node_coordinates[node_id]['y'] = y
    return node_coordinates

# Function to visualize the nodes
def visualize_nodes(node_coordinates):
    plt.figure(figsize=(12, 10))
    for node_id, coords in node_coordinates.items():
        x, y = coords['x'], coords['y']
        plt.scatter(x, y, label=f"Node {node_id}")
        plt.text(x, y, f"{node_id}", fontsize=9, ha='right', va='bottom')
        plt.subplots_adjust(hspace=0.4, wspace=0.4)

    plt.xlabel("X Coordinate")
    plt.ylabel("Y Coordinate")
    plt.title("Node Visualization")
    plt.grid(True)
    plt.legend(loc='upper right', bbox_to_anchor=(1.2, 1.0), fontsize='small')
    plt.tight_layout()
    plt.show()

# File path to the text file
file_path = "map1.txt"  # Replace with your file path

# Parse the file and visualize
node_coordinates = parse_node_coordinates(file_path)
visualize_nodes(node_coordinates)
