import json
data = json.load(open('test.txt'))
arr = list(data.values())[0]
l1s = [x.l1 for x in arr]
l2s = [x.l2 for x in arr]
print(l1s)
#keys = [x.l1 for x in data.values().data]
#print(keys)
