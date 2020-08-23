global x
x = 0

def func():
    global x
    x = 1

func()
print x