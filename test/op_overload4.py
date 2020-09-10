class A(object):
    def __getitem__(self, key):
        if key == "hello":
            return "hi"
        elif key == "how are you":
            return "fine"

    def __setitem__(self, key, value):
        print self
        print key
        print value

a = A()
print a["hello"]
print a["how are you"]
a["one"] = 1