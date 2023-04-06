import json
import sys
import numpy as np
import matplotlib.pyplot as plt

if len(sys.argv) != 2:
    print("Nom de fichier requis")
    sys.exit(1)

f = open(sys.argv[1], "r")
data = json.load(f)
X = np.array([])
for x in data['data']:
    X = np.append(X, [x])

# def mu(array):
#     m = 0
#     for x in array:
#         # print(x['w'])
#         m += x['w']
#     return m / len(array)

# def sigma(array):
#     s = 0
#     mu_val = mu(array)
#     for elt in array:
#         x = elt['w'] - mu_val
#         s += x**2
#     s /= len(array)
#     return s**(1/2)

# Calculate path length between two nodes
def calculatePath(node1, node2, array):
    # print("Calculating path between", node1, "and", node2)
    # print(array)
    path = 0
    current = node1
    while current != node2:
        # print("Current node:", current)
        # print("Path:", path)
        found = False
        for elt in array:
            if elt['s1'] == current:
                # print("Found edge", elt['s1'], elt['s2'], "with weight", elt['w'])
                path += elt['w']
                current = elt['s2']
                found = True
                break
        if not found:
            # print("No path found")
            return None
    return path

def maxPathLength(array):
    max = 0
    edges = []
    for elt in array:
        if elt['s1'] not in edges:
            edges.append(elt['s1'])
        if elt['s2'] not in edges:
            edges.append(elt['s2'])
    for edge1 in edges:
        for edge2 in edges:
            path = calculatePath(edge1, edge2, array)
            if path != None and path != 0:
                if path > max:
                    max = path
    return max

def minPathLength(array):
    min = -1
    edges = []
    for elt in array:
        if elt['s1'] not in edges:
            edges.append(elt['s1'])
        if elt['s2'] not in edges:
            edges.append(elt['s2'])
    for edge1 in edges:
        for edge2 in edges:
            path = calculatePath(edge1, edge2, array)
            if path != None and path != 0:
                if path < min or min == -1:
                    min = path
    return min

def avgPathLength(array):
    sum = 0
    cnt = 0
    edges = []
    for elt in array:
        if elt['s1'] not in edges:
            edges.append(elt['s1'])
        if elt['s2'] not in edges:
            edges.append(elt['s2'])
    for edge1 in edges:
        for edge2 in edges:
            path = calculatePath(edge1, edge2, array)
            if path != None and path != 0:
                cnt += 1
                sum += path
    if cnt != 0:
        return sum / cnt
    return 0

def medPathLength(array):
    edges = []
    for elt in array:
        if elt['s1'] not in edges:
            edges.append(elt['s1'])
        if elt['s2'] not in edges:
            edges.append(elt['s2'])
    paths = []
    for edge1 in edges:
        for edge2 in edges:
            if edge1 != edge2:
                path = calculatePath(edge1, edge2, array)
                if path != None and path != 0:
                    paths.append(path)
    return np.median(paths)

def sigmaPathLength(array):
    edges = []
    for elt in array:
        if elt['s1'] not in edges:
            edges.append(elt['s1'])
        if elt['s2'] not in edges:
            edges.append(elt['s2'])
    paths = []
    for edge1 in edges:
        for edge2 in edges:
            path = calculatePath(edge1, edge2, array)
            if path != None and path != 0:
                paths.append(path)
    return np.std(paths)

def getAllPathLength(array):
    edges = []
    for elt in array:
        if elt['s1'] not in edges:
            edges.append(elt['s1'])
        if elt['s2'] not in edges:
            edges.append(elt['s2'])
    paths = []
    for edge1 in edges:
        for edge2 in edges:
            path = calculatePath(edge1, edge2, array)
            if path != None and path != 0:
                paths.append(path)
    return np.array(paths)

Y = np.array(list(map(lambda elt : elt['w'], X)))
ya = Y.mean()
print("avgEdge =", ya)
ys = Y.std()
print("stdEdge =", ys)
yM = Y.max()
print("maxEdge =", yM)
ym = Y.min()
print("minEdge =", ym)
yS = np.median(Y)
print("medEdge =", yS)
xa = avgPathLength(X)
print("avgPath =", xa)
xs = sigmaPathLength(X)
print("stdPath =", xs)
xM = maxPathLength(X)
print("maxPath =", xM)
xm = minPathLength(X)
print("minPath =", xm)
xS = medPathLength(X)
print("medPath =", xS)


plt.hist(Y, bins=100, color='b')
plt.axvline(x=ya, color='b', label='Avg = '+str(ya))
plt.axvline(x=yS, color='b', label='Med = '+str(yS))
plt.axvline(x=yM, color='b', label='Max = '+str(yM))
plt.axvline(x=ym, color='b', label='Min = '+str(ym))
plt.legend(loc='upper right')
plt.show()
plt.hist(getAllPathLength(X), bins=100, color='r')
plt.axvline(x=xa, color='r', label='Avg = '+str(xa))
plt.axvline(x=xS, color='r', label='Med = '+str(xS))
plt.axvline(x=xM, color='r', label='Max = '+str(xM))
plt.axvline(x=xm, color='r', label='Min = '+str(xm))
plt.legend(loc='upper right')
plt.show()
