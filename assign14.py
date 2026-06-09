import gc
import sys

class Node:
    def __init__(self, name):

        self.name = name
        self.link = None

print("Step 1: Create two nodes")
A = Node("A")
B = Node("B")

print("Step 2: Create a cycle")
A.link = B
B.link = A

print("\nStep 3: Check reference counts")

print("A reference count:", sys.getrefcount(A) - 1)

print("B reference count:", sys.getrefcount(B) - 1)

print("Both are 2 because each has a variable name and a link from the other node.")

print("\nStep 4: Delete normal variable names")

# Stop automatic garbage collection so we can see the dead objects first.
gc.disable()

del A
del B

print("A and B names are deleted.")
print("But the objects still point to each other, so they are still in memory.")

print("\nStep 5: Investigate using gc.get_objects()")
dead_nodes = [obj for obj in gc.get_objects() if isinstance(obj, Node)]

print("Node objects still found in memory:", len(dead_nodes))
print("Names found:", [node.name for node in dead_nodes])

# Very important: remove this temporary list before calling gc.collect().
del dead_nodes

print("\nStep 6: Force garbage collection")
collected = gc.collect()
print("Unreachable objects collected:", collected)

remaining_nodes = [obj for obj in gc.get_objects() if isinstance(obj, Node)]
print("Node objects after gc.collect():", len(remaining_nodes))

gc.enable()
