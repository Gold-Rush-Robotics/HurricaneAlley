a = 100


def test():
    global a 
    a = 200
    
    
if __name__ == "__main__":
    test()
    print(a)