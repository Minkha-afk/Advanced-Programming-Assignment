# Assignment 13

## Question

In C, managing strings is a common source of buffer overflows and memory leaks. Implement a Dynamic String Buffer that automatically grows as needed.

Requirements:

1. Create a StringBuffer struct containing a char *data, a size_t length, and a size_t capacity.

2. Write a function sb_init(size_t initial_capacity) that allocates the struct and the data buffer on the heap. Handle NULL returns from malloc.

3. Write sb_append(StringBuffer *sb, const char *str).

4. If the new string exceeds current capacity, use realloc to double the capacity. Ensure you handle realloc safely (don't overwrite the original pointer if it returns NULL).

Write sb_free(StringBuffer *sb) which works as a destructor that frees both the internal data and the struct itself to prevent memory leaks.

Demonstrate the buffer growing at least twice and then free all memory.
