from cs50 import get_int

n = 0
while(n < 1 or n > 8):
    n = get_int("Height: ")

for i in range(n):
    for j in range(n):
        if j >= n - i - 1:
            print("#", end="")
        else:
            print(" ", end="")
    print()