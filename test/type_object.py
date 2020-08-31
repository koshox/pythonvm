print list

t = type(1)
print t
print type(t)

i = 0
while i < 5:
    t = type(t)
    print t
    i = i + 1