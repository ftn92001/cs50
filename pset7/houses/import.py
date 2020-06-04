# TODO
import csv
from sys import argv, exit
from cs50 import SQL

if len(argv) != 2:
    print("Usage: python import.py characters.csv")
    exit

db = SQL("sqlite:///students.db")

with open(argv[1], "r") as file:
    reader = csv.DictReader(file)
    for row in reader:
        name = row["name"].split(" ")
        if len(name) == 3:
            db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES (?, ?, ?, ?, ?)",
                       name[0], name[1], name[2], row["house"], row["birth"])
        else:
            db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES (?, ?, ?, ?, ?)",
                       name[0], None, name[1], row["house"], row["birth"])