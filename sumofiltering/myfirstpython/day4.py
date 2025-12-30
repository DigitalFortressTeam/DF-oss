import time
try:

    a = int(input())
    start = time.perf_counter()

    print(2)
    print(3)
    prime = True
    nbs = 2
    listofprime = [2, 3]


    def primebt(c):
        b = 0
        numberprimeorno = True
        while numberprimeorno and listofprime[b] <= c / 2:
            if c % listofprime[b] == 0:
                numberprimeorno = False
            elif c % listofprime[b] > 0:
                b += 1
        if numberprimeorno:
            return True

    for i in range(5, a-1):
        if primebt(i):
            listofprime.append(i)
            print(i)
            nbs += 1
    print(f"{nbs} prime numbers between 2 and {a}")

except KeyboardInterrupt:

    print(nbs, "prime numbers between 2 and",i )
delta = time.perf_counter() - start
print("%.4f" % delta,"seconds")