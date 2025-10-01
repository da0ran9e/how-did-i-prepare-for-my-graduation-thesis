import matplotlib.pyplot as plt
import random

# Số node
num_nodes = 20
random.seed(42)

# Tạo vị trí node trong vùng 100x100
positions = {i: (random.randint(0, 100), random.randint(0, 100)) for i in range(num_nodes)}

# Đặt BS ở góc trên trái (0, 100)
positions['BS'] = (0, 100)

# Vẽ mạng node
plt.figure(figsize=(6,6))
for i, (x, y) in positions.items():
    if i == 'BS':
        plt.scatter(x, y, c='red', marker='s', s=200, label='BS')
        plt.text(x+2, y, "BS", fontsize=10, color='red')
    else:
        plt.scatter(x, y, c='blue', s=80)
        plt.text(x+1, y+1, str(i), fontsize=8, color='blue')

plt.xlim(-10, 110)
plt.ylim(-10, 110)
plt.title("Slide 1: Node Deployment (BS at corner)")
plt.xlabel("X coordinate")
plt.ylabel("Y coordinate")
plt.grid(True)
plt.legend()
plt.show()
