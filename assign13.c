/*
 * Assignment 13 - Dynamic String Buffer in C
 * ==========================================
 *
 * This program demonstrates a heap-allocated dynamic string buffer.
 * It grows automatically by doubling its capacity when an append would
 * overflow the current allocation.
 *
 * Windows compile : gcc -O0 -Wall -Wextra -o assign13.exe assign13.c
 * Linux/macOS     : gcc -O0 -Wall -Wextra -o assign13 assign13.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE_FORMAT "lu"
#define SIZE_VALUE(value) ((unsigned long)(value))

#define SB_OK 0
#define SB_ERR -1
#define SB_MIN_CAPACITY 4

typedef struct {
    char *data;       /* heap buffer, always NUL-terminated */
    size_t length;    /* characters stored, excluding '\0' */
    size_t capacity;  /* bytes allocated, including '\0' */
} StringBuffer;

static void sb_print_state(const StringBuffer *sb, const char *label)
{
    double used_percent;

    if (sb == NULL) {
        printf("  %-22s | NULL pointer\n", label);
        return;
    }

    used_percent = sb->capacity > 1
                       ? (double)sb->length / (double)(sb->capacity - 1) * 100.0
                       : 0.0;

    printf("  %-22s | length=%-4" SIZE_FORMAT
           " | capacity=%-4" SIZE_FORMAT
           " | used=%5.1f%% | \"%s\"\n",
           label,
           SIZE_VALUE(sb->length),
           SIZE_VALUE(sb->capacity),
           used_percent,
           sb->data ? sb->data : "(null)");
}

StringBuffer *sb_init(size_t initial_capacity)
{
    StringBuffer *sb;

    if (initial_capacity < SB_MIN_CAPACITY) {
        initial_capacity = SB_MIN_CAPACITY;
    }

    sb = (StringBuffer *)malloc(sizeof(StringBuffer));
    if (sb == NULL) {
        fprintf(stderr,
                "sb_init: malloc failed for StringBuffer struct ("
                "%" SIZE_FORMAT " bytes)\n",
                SIZE_VALUE(sizeof(StringBuffer)));
        return NULL;
    }

    sb->data = (char *)malloc(initial_capacity);
    if (sb->data == NULL) {
        fprintf(stderr,
                "sb_init: malloc failed for data buffer ("
                "%" SIZE_FORMAT " bytes)\n",
                SIZE_VALUE(initial_capacity));
        free(sb);
        return NULL;
    }

    sb->data[0] = '\0';
    sb->length = 0;
    sb->capacity = initial_capacity;

    printf("  [sb_init] Allocated struct + data buffer. capacity="
           "%" SIZE_FORMAT "\n",
           SIZE_VALUE(sb->capacity));

    return sb;
}

static int sb_grow(StringBuffer *sb)
{
    size_t new_capacity = sb->capacity * 2;
    char *tmp;

    printf("  [sb_grow] capacity %" SIZE_FORMAT " -> %" SIZE_FORMAT
           " (doubling)\n",
           SIZE_VALUE(sb->capacity),
           SIZE_VALUE(new_capacity));

    tmp = (char *)realloc(sb->data, new_capacity);
    if (tmp == NULL) {
        fprintf(stderr,
                "sb_grow: realloc failed requesting "
                "%" SIZE_FORMAT " bytes\n",
                SIZE_VALUE(new_capacity));
        return SB_ERR;
    }

    sb->data = tmp;
    sb->capacity = new_capacity;
    return SB_OK;
}

int sb_append(StringBuffer *sb, const char *str)
{
    size_t str_len;

    if (sb == NULL) {
        fprintf(stderr, "sb_append: StringBuffer pointer is NULL\n");
        return SB_ERR;
    }

    if (str == NULL) {
        fprintf(stderr, "sb_append: input string pointer is NULL\n");
        return SB_ERR;
    }

    str_len = strlen(str);
    if (str_len == 0) {
        return SB_OK;
    }

    while (sb->length + str_len + 1 > sb->capacity) {
        if (sb_grow(sb) != SB_OK) {
            return SB_ERR;
        }
    }

    memcpy(sb->data + sb->length, str, str_len + 1);
    sb->length += str_len;

    return SB_OK;
}

void sb_free(StringBuffer **sb)
{
    if (sb == NULL || *sb == NULL) {
        return;
    }

    printf("  [sb_free] Freeing data buffer ("
           "%" SIZE_FORMAT " bytes) + struct.\n",
           SIZE_VALUE((*sb)->capacity));

    free((*sb)->data);
    (*sb)->data = NULL;
    (*sb)->length = 0;
    (*sb)->capacity = 0;

    free(*sb);
    *sb = NULL;
}

static int append_or_exit(StringBuffer *sb, const char *text)
{
    if (sb_append(sb, text) != SB_OK) {
        fprintf(stderr, "FATAL: sb_append failed.\n");
        return SB_ERR;
    }

    return SB_OK;
}

int main(void)
{
    StringBuffer *sb;
    int total_growths = 4;

    printf("\n");
    printf("============================================================\n");
    printf("  Assignment 13 - Dynamic String Buffer\n");
    printf("============================================================\n\n");

    printf("  %-22s | %-10s | %-12s | %-9s | content\n",
           "event", "length", "capacity", "used %");
    printf("  ----------------------------------------------------------"
           "---------------------\n");

    printf("\nSTEP 1 - Initialise with capacity 8\n\n");
    sb = sb_init(8);
    if (sb == NULL) {
        fprintf(stderr, "FATAL: sb_init returned NULL.\n");
        return EXIT_FAILURE;
    }
    sb_print_state(sb, "after sb_init(8)");

    printf("\nSTEP 2 - Append \"Hello\"\n\n");
    if (append_or_exit(sb, "Hello") != SB_OK) {
        sb_free(&sb);
        return EXIT_FAILURE;
    }
    sb_print_state(sb, "after append Hello");

    printf("\nSTEP 3 - Append \", World\" (8 -> 16)\n\n");
    if (append_or_exit(sb, ", World") != SB_OK) {
        sb_free(&sb);
        return EXIT_FAILURE;
    }
    sb_print_state(sb, "after append , World");

    printf("\nSTEP 4 - Append \"!\" (no growth)\n\n");
    if (append_or_exit(sb, "!") != SB_OK) {
        sb_free(&sb);
        return EXIT_FAILURE;
    }
    sb_print_state(sb, "after append !");

    printf("\nSTEP 5 - Append long phrase (16 -> 32 -> 64)\n\n");
    if (append_or_exit(sb, " How are you doing today?") != SB_OK) {
        sb_free(&sb);
        return EXIT_FAILURE;
    }
    sb_print_state(sb, "after long append");

    printf("\nSTEP 6 - Append empty string (no-op)\n\n");
    if (append_or_exit(sb, "") != SB_OK) {
        sb_free(&sb);
        return EXIT_FAILURE;
    }
    sb_print_state(sb, "after append empty");

    printf("\nSTEP 7 - Append longer text (64 -> 128)\n\n");
    if (append_or_exit(sb, " Hope you are having a great day") != SB_OK) {
        sb_free(&sb);
        return EXIT_FAILURE;
    }
    sb_print_state(sb, "after 3rd growth");

    printf("\nSTEP 8 - Use sb->data as a normal C string\n\n");
    printf("  strlen(sb->data) = %" SIZE_FORMAT
           " (matches sb->length = %" SIZE_FORMAT ")\n",
           SIZE_VALUE(strlen(sb->data)),
           SIZE_VALUE(sb->length));
    printf("  Full string      : \"%s\"\n", sb->data);

    printf("\nSTEP 9 - Free all memory with sb_free()\n\n");
    sb_free(&sb);
    printf("  sb pointer after sb_free = %s\n",
           sb == NULL ? "NULL [OK] (no dangling pointer)" : "NOT NULL [ERROR]");

    printf("\nSTEP 10 - Call sb_free() again on NULL\n\n");
    sb_free(&sb);
    printf("  Second sb_free(&sb) on NULL pointer: no crash [OK]\n");

    printf("\n");
    printf("============================================================\n");
    printf("  Growth Summary\n");
    printf("============================================================\n");
    printf("  %-14s | %-12s | %-12s | %-12s\n",
           "Growth event", "Old", "New", "Trigger");
    printf("  ----------------------------------------------------------\n");
    printf("  %-14s | %-12d | %-12d | %s\n",
           "1st doubling", 8, 16, "\", World\"");
    printf("  %-14s | %-12d | %-12d | %s\n",
           "2nd doubling", 16, 32, "long phrase");
    printf("  %-14s | %-12d | %-12d | %s\n",
           "3rd doubling", 32, 64, "long phrase");
    printf("  %-14s | %-12d | %-12d | %s\n",
           "4th doubling", 64, 128, "longer text");
    printf("============================================================\n");
    printf("  All heap memory freed. No leaks expected.\n");
    printf("  Total growth events shown: %d\n\n", total_growths);

    return EXIT_SUCCESS;
}
