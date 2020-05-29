from cs50 import get_string

text = get_string("Text: ")
L = 0
W = 0
S = 0
for c in text:
    if str.isalpha(c):
        L += 1
    if c == " ":
        W += 1
    if c == "?" or c == "." or c == "!":
        S += 1

W += 1
L = L / W * 100
S = S / W * 100
grade = round(0.0588 * L - 0.296 * S - 15.8)

if grade > 16:
    print("Grade 16+")
elif grade < 1:
    print("Before Grade 1")
else:
    print(f"Grade: {grade}")