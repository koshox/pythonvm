def foo(a):
    b = a - 1
    bar(a, b)


def bar(a, b):
    raise Exception("something wrong")


foo(1)
