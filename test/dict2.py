# d = {1: "hello"}
# d.setdefault(1, 2)
# d.setdefault(2, 3)
#
# print d[1]
# print d[2]
#
# d.pop(1)
# print d
#
# del d[2]
# print d

d = {1: "a", 2: "b"}

print d.keys()
for k in d.keys():
    print k, d[k]

for v in d.values():
    print v

print d.items()
for k, v in d.items():
    print k, v

