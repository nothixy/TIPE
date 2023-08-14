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
edges = []
for elt in X:
    if elt['s1'] not in edges:
        edges.append(elt['s1'])
    if elt['s2'] not in edges:
        edges.append(elt['s2'])
Vus = [False] * (max(edges) + 1)
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
paths = np.array(paths)
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
xa = np.mean(paths)
print("avgPath =", xa)
xs = np.std(paths)
print("stdPath =", xs)
xM = np.max(paths)
print("maxPath =", xM)
xm = np.min(paths)
print("minPath =", xm)
xS = np.median(paths)
print("medPath =", xS)
plt.hist(Y, bins=100, color='b')
plt.axvline(x=ya, color='blue', label='AvgEdge = '+str(ya))
plt.axvline(x=yS, color='red', label='MedEdge = '+str(yS))
plt.axvline(x=yM, color='green', label='MaxEdge = '+str(yM))
plt.axvline(x=ym, color='black', label='MinEdge = '+str(ym))
plt.legend(loc='lower right')
plt.title("Nombre d'aretes par longueur")
plt.show()
plt.hist(paths, bins=100, color='r')
plt.axvline(x=xa, color='blue', label='AvgPath = '+str(xa))
plt.axvline(x=xS, color='red', label='MedPath = '+str(xS))
plt.axvline(x=xM, color='green', label='MaxPath = '+str(xM))
plt.axvline(x=xm, color='black', label='MinPath = '+str(xm))
plt.legend(loc='lower right')
plt.title("Nombre de chemins par longueur")
plt.show()
