# Assignment 14

## Question

Create a scenario where objects are "dead" but still have a reference count higher than zero, then force the Garbage Collector to clean them up. Do in python only.

Implementation Steps:

Create a Node class with a name and a link attribute.

Create a Cycle: Instantiate Node A and Node B.

Set A.link = B and B.link = A.

Check References: Use sys.getrefcount() to show that both objects have multiple references.

The "Deletion": Use del A and del B.

The Investigation: Use the gc module to show that these objects still exist in memory because of the cycle, even though you can no longer access them from your code.

The Cleanup: Call gc.collect() and print the number of "unreachable" objects collected.
