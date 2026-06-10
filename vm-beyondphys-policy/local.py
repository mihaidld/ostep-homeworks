import random

low = 0
high = 9
nb = 10000
current = low
times = 0

with open("temp.txt", "w") as f:
    for i in range(nb):
        # achieve temporal locality 3 times in sequence same page
        if times < 3:
            times += 1
        else:
            times = 1  # reset counter
            current = random.randint(low, high)  # reset current
        f.write(f"{str(current)}\n")

current = low
times = 0

with open("space.txt", "w") as f:
    for i in range(nb):
        # achieve space locality 5 consequent pages (if <= 9) in sequence
        if times < 3 and current < high:
            current += 1
            times += 1
        else:
            times = 1  # reset counter
            current = random.randint(low, high)
        f.write(f"{str(current)}\n")
