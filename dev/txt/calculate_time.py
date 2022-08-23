import pandas as pd

with open("time.txt", "r") as f:
    hours = [float(line.split(":")[1].strip()[:-1]) for line in f.readlines()]

sum_ = sum(hours)

print(f"Hours spent: {sum_}")
