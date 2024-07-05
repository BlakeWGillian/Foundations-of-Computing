import random
import math
PI = 3.141592653589;
trials = 1

#RANDOM
print("\nPoints in a square.")
sizes = [4, 16, 256]
for size in sizes: #different input sizes
    for trial in range(0, trials): #number of trials per size.
        print(size)
        for x in range(0, size):
            print(f'{random.randint(-10000,10000)/1000} {random.randint(-10000,10000)/1000}')

print("\nPoints on a circle.")
radius = 10;
for size in sizes: #different input sizes
    for trial in range(0, trials): #number of trials per size.
        print(size)
        for x in range(0, size):
            angle = random.random() * PI * 2
            
            print(f'{math.sin(angle) * radius:.3f} {math.cos(angle) * radius:.3f}')

print("\nPoints within a 4 point hull.")
edges = [[10.000, 10.000], [10.000, -10.000], [-10.000, 10.000], [-10.000, -10.000]]
remd = 4
times = [random.randint(0, size)]

for size in sizes: #different input sizes
    for trial in range(0, trials): #number of trials per size.
        print(size)
        points = []
        points += edges
        for x in range(0, size-4):
            points.append([random.randint(-10000,10000)/1000, random.randint(-10000,10000)/1000])
        random.shuffle(points)
        for x in points:
            print(f'{x[0]} {x[1]}')
