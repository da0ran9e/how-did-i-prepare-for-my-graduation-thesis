import matplotlib.pyplot as plt
import networkx as nx
import random

# ---------------------------
# B1: Tạo ngẫu nhiên node và Base Station
# ---------------------------
random.seed(42)
num_nodes = 10
positions = {i: (random.randint(0, 100), random.randint(0, 100)) for i in range(num_nodes)}
positions['BS'] = (50, 120)  # Base Station

# ---------------------------
# B2: Tạo đồ thị đầy đủ với trọng số = khoảng cách Euclidean
# ---------------------------
A = nx.Graph()
for i in positions:
    A.add_node(i, pos=positions[i])

G = A
for i in positions:
    for j in positions:
        if i != j:
            xi, yi = positions[i]
            xj, yj = positions[j]
            dist = ((xi - xj)**2 + (yi - yj)**2)**0.5
            G.add_edge(i, j, weight=dist)

# ---------------------------
# B3: Tính cây MST (PEDAP)
# ---------------------------
mst = nx.minimum_spanning_tree(G)

# ---------------------------
# Hàm vẽ helper
# ---------------------------
def draw_step(step):
    plt.figure(figsize=(6,6))
    pos = positions

    if step == 1:
        nx.draw(A, pos, node_color="lightblue", node_size=600, with_labels=True)
        plt.title("B1: Mạng cảm biến ban đầu")

    elif step == 2:
        nx.draw(G, pos, node_color="lightblue", node_size=600, with_labels=True)
        nx.draw_networkx_edges(G, pos, alpha=0.3)
        plt.title("B2: Đồ thị đầy đủ (các kết nối có thể)")

    elif step == 3:
        nx.draw(mst, pos, node_color="lightgreen", node_size=600, with_labels=True)
        nx.draw_networkx_edges(mst, pos, edge_color="green", width=2)
        plt.title("B3: Cây Multihop PEDAP (MST)")

    elif step == 4:
        nx.draw(mst, pos, node_color="lightgreen", node_size=600, with_labels=True)
        # Vẽ mũi tên (luồng dữ liệu từ node -> parent)
        for u, v in mst.edges():
            # giả định dữ liệu đi về phía BS
            if v == 'BS':
                parent, child = v, u
            elif u == 'BS':
                parent, child = u, v
            else:
                # chọn node gần BS hơn làm parent
                if positions[u][1] > positions[v][1]:
                    parent, child = u, v
                else:
                    parent, child = v, u
            plt.arrow(positions[child][0], positions[child][1],
                      (positions[parent][0]-positions[child][0])*0.8,
                      (positions[parent][1]-positions[child][1])*0.8,
                      length_includes_head=True, head_width=2, color="red")
        plt.title("B4: Luồng dữ liệu multihop về BS")

    plt.show()

# ---------------------------
# Hiển thị lần lượt các bước
# ---------------------------
for s in range(1,5):
    draw_step(s)
