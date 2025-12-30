a = int(input())
b = 2
prime = True
if a <= 1:
    prime = False
    print("any number under 1 isn't prime")
else:
    for i in range(int(a/2)):
        if a % b == 0:
            prime = False
            break
        b += 1
        if a/2 == b:
            break
    if prime:
        print(str(a) + " is prime")
    if not prime:
        print(str(a) + " is not prime")
print("Thank you for processing, but you killed my cpu.")