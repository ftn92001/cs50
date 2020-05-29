from cs50 import get_float

dollars = -1
while dollars < 0:
    dollars = get_float("Change owed: ")
cent = round(dollars * 100)

n = cent // 25
cent %= 25
n = n + cent // 10
cent %= 10
n = n + cent // 5
cent %= 5
n = n + cent // 1

print(n)