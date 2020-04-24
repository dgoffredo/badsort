import sys

n=None
insertion_ns=None
bubble_ns=None

for line in sys.stdin:
    if line.startswith('n: '):
        n = int(line.split()[1])
    elif line.startswith('insertion: '):
        insertion_ns = int(line.split()[1])
    elif line.startswith('bubble: '):
        bubble_ns = int(line.split()[1])
        print(n, insertion_ns, bubble_ns)
