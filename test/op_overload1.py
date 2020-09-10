class A(object):
    def __init__(self, v):
        self.value = v

    def __add__(self, other):
        print "exec operator +"
        return A(self.value + other.value)

a = A(1)
b = A(2)
c = a + b

print c.value