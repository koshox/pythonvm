def foo(a, b):
    return a / b


print foo(b=2, a=6)


def sum(*args):
    t = 0
    for i in args:
        t += i

    return t


print sum(1, 2, 3, 4)


def foo(**kwargs):
    for k, v in kwargs.iteritems():
        print k
        print v


foo(a=1, b=2)
