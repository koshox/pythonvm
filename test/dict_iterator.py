d = {1: "a", 2: "b"}

for k in d:
    print k, d[k]

for k in d.iterkeys():
    print k, d[k]

for v in d.itervalues():
    print v

for k, v in d.iteritems():
    print k, v