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

paths = []

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

edges = []
for elt in X:
    if elt['s1'] not in edges:
        edges.append(elt['s1'])
    if elt['s2'] not in edges:
        edges.append(elt['s2'])

Vus = [False] * (max(edges) + 1)
# print("len(Vus) = {}".format(len(Vus)))

def calculatePath(node1, node2, array):
    if node1 == node2:
        return 0
    x = 0
    for elt in array:
        if elt['s1'] == node1:
            if not Vus[elt['s2']]:
                Vus[elt['s2']] = True
                x = calculatePath(elt['s2'], node2, array)
                if x != None:
                    x = x + elt['w']
                    break
        if elt['s2'] == node1:
            if not Vus[elt['s1']]:
                Vus[elt['s1']] = True
                x = calculatePath(elt['s1'], node2, array)
                if x != None:
                    x = x + elt['w']
                    break
    return x

for edge1 in edges:
    for edge2 in edges:
        if edge1 != edge2:
            Vus = [False] * (max(edges) + 1)
            path = calculatePath(edge1, edge2, X)
            print(edge1, edge2, path)
            if path != None and path != 0:
                paths.append(path)
# print(paths)
paths = np.array(paths)

# def calculatePath(node1, node2, array):
#     # print("Calculating path between", node1, "and", node2)
#     # print(array)
#     path = 0
#     current = node1
#     while current != node2:
#         # print("Current node:", current)
#         # print("Path:", path)
#         found = False
#         for elt in array:
#             if elt['s1'] == current:
#                 # print("Found edge", elt['s1'], elt['s2'], "with weight", elt['w'])
#                 path += elt['w']
#                 current = elt['s2']
#                 found = True
#                 break
#         if not found:
#             # print("No path found")
#             return None
#     return path

def maxPathLength(array):
    return np.max(paths)

def minPathLength(array):
    return np.min(paths)

def avgPathLength(array):
    return np.mean(paths)

def medPathLength(array):
    return np.median(paths)

def sigmaPathLength(array):
    return np.std(paths)

def getAllPathLength(array):
    return paths

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
plt.axvline(x=ya, color='blue', label='AvgEdge = '+str(ya))
plt.axvline(x=yS, color='red', label='MedEdge = '+str(yS))
plt.axvline(x=yM, color='green', label='MaxEdge = '+str(yM))
plt.axvline(x=ym, color='black', label='MinEdge = '+str(ym))
plt.legend(loc='lower right')
plt.title("Nombre d'arêtes par longueur")
plt.show()
plt.hist(getAllPathLength(X), bins=100, color='r')
plt.axvline(x=xa, color='blue', label='AvgPath = '+str(xa))
plt.axvline(x=xS, color='red', label='MedPath = '+str(xS))
plt.axvline(x=xM, color='green', label='MaxPath = '+str(xM))
plt.axvline(x=xm, color='black', label='MinPath = '+str(xm))
plt.legend(loc='lower right')
plt.title("Nombre de chemins par longueur")
plt.show()
