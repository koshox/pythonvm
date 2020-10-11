i = 0
while i < 10:
    try:
        i += 1
        if i == 5:
            break
        print i
    finally:
        print "hello"
