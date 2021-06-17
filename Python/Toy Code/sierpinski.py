# Author : Milan Zanussi
# Date : January 2017
# Project : Sierpinski Shape generator
# Description : Some toy code I wrote during HackMT 2017. Planned on trying to implement a command in our
# project which would allow players in a game to arrange their characters they control into Sierpinski-like shape.


# REQUIREMENTS TO RUN : This test code was originally written in Python 2.7 and uses matplotlib

import matplotlib.pyplot as plt
import random
import math

def SierpinskiShape(verts, n):

    # Generate vertices of polygon for shape generation
    vertices = []
    for i in range(1, verts + 1):
        vertices.append((math.cos(i*((2*math.pi)/verts) + math.pi/2), math.sin(i*((2*math.pi)/verts) + math.pi/2)))

    x_vals = [0]
    y_vals = [0]
    for i in range(1, n):

        # Get previous point (current position, so to speak)
        last_x = x_vals[i-1]
        last_y = y_vals[i-1]

        # Randomly select pivot point
        select = random.randint(1,verts)
            
        pivot = vertices[select-1]

        # Append new point to data lists
        x_vals.append((last_x + pivot[0])/(verts-1))
        y_vals.append((last_y + pivot[1])/(verts-1))  

    plt.scatter(x_vals, y_vals)
    plt.show()



