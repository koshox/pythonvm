i = 0
while i < 5:
    try:
        i += 1
        if i > 3:
            continue
        print i
    finally:
        print "hello"
