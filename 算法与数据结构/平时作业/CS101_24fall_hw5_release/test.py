import math

def UCB (a, b, bp):
    return a/b + math.sqrt(math.log2(bp)/b)

a = 1
b = 2
bp = 7
print(UCB(a, b, bp))