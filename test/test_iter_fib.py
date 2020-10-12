class Fib(object):
    def __init__(self, n):
        self.n = n

    def __iter__(self):
        return FibIterator(self.n)


class FibIterator(object):
    def __init__(self, n):
        self.n = n
        self.a = 1
        self.b = 1
        self.cnt = 0

    def next(self):
        if (self.cnt > self.n):
            raise StopIteration

        self.cnt += 1
        t = self.a
        self.a = self.a + self.b
        self.b = t
        return t


fib = Fib(10)
itor = iter(fib)

for i in fib:
    print i

while True:
    try:
        print itor.next()
    except StopIteration, e:
        break