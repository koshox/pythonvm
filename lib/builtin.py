def map(func, iterable):
    l = []
    for i in iterable:
        l.append(func(i))

    return l


def filter(func, iterable):
    l = []
    for i in iterable:
        if func(i):
            l.append(i)

    return l


def sum(iterable, i):
    temp = i
    for e in iterable:
        temp = temp + e

    return temp


def range(*args):
    start = 0
    step = 1

    if len(args) == 1:
        end = args[0]
    elif len(args) == 2:
        start = args[0]
        end = args[1]
    else:
        start = args[0]
        end = args[1]
        step = args[2]

    lst = []
    if (start < end and step > 0):
        while start < end:
            lst.append(start)
            start += step
    elif (start > end and step < 0):
        while start > end:
            lst.append(start)
            start += step
    else:
        print "Error"

    return lst


class Exception(object):
    def __init__(self, *args):
        self.info = args

    def __repr__(self):
        return " ".join(self.info)


class StopIteration(Exception):
    pass
