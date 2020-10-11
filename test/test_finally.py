def foo():
    try:
        print "hello"
        return
        print "hi"
    finally:
        print "world"


foo()
