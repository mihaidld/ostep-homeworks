import random

low = 0
high = 9
nb = 10000

with open("src.txt", "w") as f:
    for i in range(nb):
        f.write(f"{str(random.randint(low, high))}\n")
