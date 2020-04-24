import sys

n = None
selection_ns = None
bubble_ns = None
insertion_ns = None
optimistic_insertion_ns = None

for line in sys.stdin:
    if line.startswith('n: '):
        n = int(line.split()[1])
    elif line.startswith('selection: '):
        selection_ns = float(line.split()[2])
    elif line.startswith('bubble: '):
        bubble_ns = float(line.split()[2])
    elif line.startswith('insertion: '):
        insertion_ns = float(line.split()[2])
    elif line.startswith('optimistic_insertion: '):
        optimistic_insertion_ns = float(line.split()[2])
        print(n, selection_ns, bubble_ns, insertion_ns, optimistic_insertion_ns)
