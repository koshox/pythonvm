a = 1
b = 0
i = 0

print a
print b
print i

while i < 10:
    print a
    t = a
    a = a + b
    b = t

    i = i + 1

i = 0

while i < 10:
    i = i + 1
    if (i > 5):
        break

    print i
