# Generate node coordinates with 15m spacing
spacing = 15
start = 1
num_nodes = 100
nodes_per_row = 10

output = []
for i in range(num_nodes):
    row = i // nodes_per_row
    col = i % nodes_per_row
    x = start + col * spacing
    y = start + row * spacing
    output.append(f"SN.node[{i}].xCoor = {x}")
    output.append(f"SN.node[{i}].yCoor = {y}")

# Print to file
with open('new_coords.txt', 'w') as f:
    for line in output:
        f.write(line + '\n')

print(f"Generated {len(output)} coordinate lines")
print(f"Field size should be: {start + (nodes_per_row-1)*spacing}x{start + (nodes_per_row-1)*spacing}")
