def func():
    x = 2

    def say():
        print x

    return say

f = func()
f()
