class A(object):
    def __init__(self, v):
        self.value = v

a = A(1)
b = A(2)
print a.value

a.field = 3
A.field = 4
print a.field

print b.field
