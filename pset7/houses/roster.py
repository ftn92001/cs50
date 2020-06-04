# TODO
from sys import argv, exit
from cs50 import SQL

if len(argv) != 2:
    print("Usage: python roster.py characters.csv")
    exit

db = SQL("sqlite:///students.db")
house = argv[1]
data = db.execute("SELECT * FROM students WHERE house = ? ORDER BY last, first", house)

for row in data:
    if row["middle"] == None:
        print(row["first"], row["last"] + ", born", row["birth"])
    else:
        print(row["first"], row["middle"], row["last"] + ", born", row["birth"])