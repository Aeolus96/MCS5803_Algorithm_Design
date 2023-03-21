# /usr/bin/python3
# // I have neither given nor received any unauthorizedaid in completing this work,
# // nor have I presented someone else's work as my own.
# // Your Name: Devson Butani
# // LTU ID: 000732711
# // Date: 11/13/2022

# Pseudocode from textbook:
# FLOYD-WARSHALL(W)
# n = W.rows
# D(0) = W
# for k = 1 to n
#   let D(k) = d(k)ij be a new (nxn) matrix
#   for i = 1 to n
#       for j = 1 to n
#           d(k)ij = min(d(k-1)ij, d(k-1)ik, d(k-1)kj)
# return D(n)

# Figure 25.1
# W = [
#     [0, 3, 8, inf, -4],
#     [inf, 0, inf, 1, 7],
#     [inf, 4, 0, inf, inf],
#     [2, inf, -5, 0, inf],
#     [inf, inf, inf, 6, 0],
# ]

# ---------------------------------------------------------------------------------
# Python Code:
from math import inf


# Floyd Warshall Algorithm - O(n^3)
def floyd_warshall(n, W):
    dist = list(map(lambda i: list(map(lambda j: j, i)), W))  # new (nxn) dist[][]
    for k in range(n):  # O(n) # go through n one by one
        for i in range(n):  # O(n^2) # go through n's destinations one by one
            for j in range(n):  # O(n^3) # shortest path (i to j) write in dist[i][j]
                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j])
                # Path through i->j OR i->k->j
    return dist  # Return the solved matrix with n vertices


# Just for displaying the answer, not part of the algorithm
def show_graph(matrix):
    print("Computed Matrix:")
    for i in range(n):  # Go through entire matrix
        for j in range(n):
            if matrix[i][j] == inf:  # print inf as string
                print("%3s" % ("inf"), end="")
            else:  # Print matrix value
                print("%2d\t" % (matrix[i][j]), end="")
            if j == n - 1:
                print()  # end


# Run the script in main
if __name__ == "__main__":
    # Input:
    n = 5  # Number of vertices (also nxn matrix size)
    W = [
        [0, 3, 8, inf, -4],
        [inf, 0, inf, 1, 7],
        [inf, 4, 0, inf, inf],
        [2, inf, -5, 0, inf],
        [inf, inf, inf, 6, 0],
    ]
    print(
        """Input Matrix:
0       3       8     inf      -4
inf     0     inf       1       7
inf     4       0     inf     inf
2     inf      -5       0     inf
inf   inf     inf       6       0
        """
    )

    # Output:
    show_graph(floyd_warshall(n, W))
