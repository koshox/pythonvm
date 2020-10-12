try:
    raise Exception("something wrong")
except Exception, e:
    print e

try:
    raise StopIteration("end of iteration")
except ZeroDivisionError, e:
    print "handled"
except Exception, e:
    print e
