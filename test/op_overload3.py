class A(object):
    def __init__(self, v):
        self.value = v

    def __call__(self, a):
        if self.value > a.value:
            print "gt"
        elif self.value < a.value:
            print "lt"
        elif self.value == a.value:
            print "eq"
        else:
            print "can not compare"

a = A(1)
b = A(2)
c = A(0)

a(b)
a(a)
a(c)