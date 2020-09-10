class A(object):
    def __len__(self):
        print "len() called"
        return 1

a = A()
print len(a)