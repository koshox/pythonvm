class A(object):
    def __init__(self, v):
        self.value = v

# bound method
a = A(1)
lst = []
lst.append(2)
a.foo = lst.append
a.foo(3)
print lst

#unbound method
a = A(1)
b = A(2)

def func(self, s):
    print self.value
    print s

a.bar = func
A.bar = func

a.bar(a, "hello")
b.bar("world")
