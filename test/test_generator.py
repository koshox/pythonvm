def foo():
    i = 0
    while i < 10:
        yield i
        i += 1

    return


for i in foo():
    print i


def func():
    i = 0
    while i < 2:
        yield i
        i += 1

    return


g = func()
print g.next()
print g.next()
print g.next()
