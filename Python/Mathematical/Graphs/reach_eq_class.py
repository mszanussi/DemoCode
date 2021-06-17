# Author : Milan Zanussi
# Project : Reachability Relation Calculator
# Description : Implements functions used to compute the reachability matrix of
# a directed graph using the Floyd-Warshall algorithm on the ajacency matrix of
# the graph (performing the transtitive closure). Furthermore the program includes 
# an implementation of the algorithm to find the "reachability classes" (ie, connected components) 
# of an undirected graph (for the general directed case, the reachability may not 
# form equivalence classes).

# Additional Notes : I imagine a minor augmentaton of the code can allow the reachability
# class function to instead give a list of the "reachable vertices" from each vertex. However
# I imagine this would just be a less terse way of expressing the reachability matrix.

# Implementation of Reachibility matrix calculation
# using the Floyd-Warshall Algorithm
# Assumes all entries are either 1 or 0 with 1 representing
# that two vertices are digraph adjacent and 0 otherwise
def Reachability(adj_matrix):
    reach_matrix = adj_matrix
    vertices = len(reach_matrix)
    
    # Turn reach_matrix into connectivity matrix
    for index in range(vertices):
        adj_matrix[index][index] = 1
        
    # Apply Floyd-Warshall process
    # For each row
    for i in range(vertices):
        # For each other row
        for j in range(vertices):
            if (reach_matrix[j][i] == 1 and i != j):
                # For every entry in each row
                for k in range(vertices):
                    reach_matrix[j][k] = int(bool(reach_matrix[i][k]) or bool(reach_matrix[j][k]))

    return reach_matrix

# Uses Warshall's algorithm to generate a reachability matrix
# for an undirected graph, and then uses the reachability matrix
# to partition the graph into the equivalence classes.
def Reachability_Classes(adj_matrix):
    vertices = len(adj_matrix)

    # Check if the matrix is symmetric
    for i in range(vertices):
        for j in range(i,vertices - i):
            if(adj_matrix[i][j] != adj_matrix[j][i]):
                return []

    # Compute the reachability matrix
    reach_matrix = Reachability(adj_matrix)

    # Compute equivalence classes
    eq_classes = [x for x in range(vertices)]
    for i in range(vertices):
        if(eq_classes.count(i) != 0):
            new_class = []
            for j in range(vertices):
                if(reach_matrix[i][j] == 1):
                    new_class.append(j)
                    eq_classes.remove(j)        
            eq_classes.append(new_class)

    return eq_classes
