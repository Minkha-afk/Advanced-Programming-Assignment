/*
 * Assignment 15 - Part 1: WITHOUT Mutex (Race Condition Demo)
 * ===========================================================
 * The increment is written as 3 explicit steps to make interleaving
 * visible. A forced thread yield after LOAD encourages the OS to switch
 * threads right in the middle, making the race easy to see.
 *
 * Windows compile : gcc -O0 -Wall -Wextra -o assign15.exe assign15.c
 * Linux/macOS     : gcc -O0 -Wall -Wextra -o assign15 assign15.c -pthread
 */

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
typedef HANDLE thread_handle_t;
typedef DWORD thread_return_t;
#define THREAD_CALL WINAPI
#define COUNTER_FORMAT "%I64d"
#else
#include <pthread.h>
#include <sched.h>
typedef pthread_t thread_handle_t;
typedef void *thread_return_t;
#define THREAD_CALL
#define COUNTER_FORMAT "%lld"
#endif

#define NUM_THREADS           4
#define INCREMENTS_PER_THREAD 50000

volatile long long counter = 0;

static void force_context_switch(void)
{
#ifdef _WIN32
    SwitchToThread();
#else
    sched_yield();
#endif
}

static thread_return_t THREAD_CALL increment_counter(void *arg)
{
    int thread_id = *((int *)arg);

    for (int i = 0; i < INCREMENTS_PER_THREAD; i++) {
        long long tmp = counter;   /* Step 1: LOAD  - read into local  */
        force_context_switch();    /* OS may switch to another thread  */
        tmp = tmp + 1;             /* Step 2: ADD   - increment locally */
        counter = tmp;             /* Step 3: STORE - write back        */
    }

    printf("  Thread %d finished.\n", thread_id);

#ifdef _WIN32
    return 0;
#else
    return NULL;
#endif
}

static int start_thread(thread_handle_t *thread, void *arg)
{
#ifdef _WIN32
    *thread = CreateThread(NULL, 0, increment_counter, arg, 0, NULL);
    return *thread == NULL ? (int)GetLastError() : 0;
#else
    return pthread_create(thread, NULL, increment_counter, arg);
#endif
}

static int join_thread(thread_handle_t thread)
{
#ifdef _WIN32
    DWORD wait_result = WaitForSingleObject(thread, INFINITE);

    if (wait_result != WAIT_OBJECT_0) {
        return (int)GetLastError();
    }

    CloseHandle(thread);
    return 0;
#else
    return pthread_join(thread, NULL);
#endif
}

int main(void)
{
    thread_handle_t threads[NUM_THREADS];
    int             thread_ids[NUM_THREADS];

    long long expected = (long long)NUM_THREADS * INCREMENTS_PER_THREAD;

    printf("============================================================\n");
    printf("  Part 1: NO MUTEX (Race Condition Demo)\n");
    printf("============================================================\n");
    printf("  Threads            : %d\n", NUM_THREADS);
    printf("  Increments/thread  : %d\n", INCREMENTS_PER_THREAD);
    printf("  Expected counter   : " COUNTER_FORMAT "\n", expected);
    printf("------------------------------------------------------------\n");

    for (int i = 0; i < NUM_THREADS; i++) {
        int rc;

        thread_ids[i] = i + 1;
        rc = start_thread(&threads[i], &thread_ids[i]);

        if (rc != 0) {
            fprintf(stderr, "Could not create thread %d. Error code: %d\n",
                    i + 1, rc);
            exit(EXIT_FAILURE);
        }

        printf("  Thread %d created.\n", i + 1);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        int rc = join_thread(threads[i]);

        if (rc != 0) {
            fprintf(stderr, "Could not join thread %d. Error code: %d\n",
                    i + 1, rc);
            exit(EXIT_FAILURE);
        }
    }

    printf("------------------------------------------------------------\n");
    printf("  Expected counter   : " COUNTER_FORMAT "\n", expected);
    printf("  Actual   counter   : " COUNTER_FORMAT "\n", counter);
    printf("  Lost increments    : " COUNTER_FORMAT "\n", expected - counter);
    printf("\n");

    if (counter == expected) {
        printf("  Got correct value this run.\n");
    } else {
        printf("  Race condition CONFIRMED! Counter is WRONG.\n");
    }

    printf("============================================================\n");
    return 0;
}
