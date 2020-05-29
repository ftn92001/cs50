import csv
from sys import argv, exit

# argument error
if len(argv) != 3:
    print("Usage: python dna.py data.csv sequence.txt")
    exit(1)

# read text file
text = open(argv[2], "r")
sequence = text.read()
text.close()

# open csv file
with open(argv[1], "r") as csv_file:
    reader = csv.reader(csv_file)
    n = 0
    name = []
    for row in reader:
        if n == 0:
            dna_str = row[1:9]

        if n > 0:
            name.append(row)

        n += 1

# compute STR
ptr = 0
counter = 0
max_value = 0
max_list = []

for i in dna_str:
    for j in range(len(sequence)):

        if sequence[ptr:ptr + len(i)] == i:
            counter += 1
            ptr = ptr + len(i)
            if counter > max_value:
                max_value = counter

        else:
            counter = 0
            ptr += 1

    max_list.append(max_value)
    ptr = 0
    max_value = 0

# match name
no_match = True
for i in name:
    for j in range(len(i) - 1):
        i[j + 1] = int(i[j + 1])

    if i[1:] == max_list:
        no_match = False
        print(i[0])

if no_match:
    print("No match")