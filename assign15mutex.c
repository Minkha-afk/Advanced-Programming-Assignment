/*
 * Assignment 15 - Part 2: WITH Mutex (Correct Synchronized Version)
 * =================================================================
 * Same scenario as Part 1, but the critical section is protected by
 * a mutex. Only one thread can increment at a time, so the final
 * counter is always NUM_THREADS * INCREMENTS_PER_THREAD.
 *
 * Windows compile : gcc -O0 -Wall -Wextra -o assign15mutex.exe assign15mutex.c
 * Linux/macOS     : gcc -O0 -Wall -Wextra -o assign15mutex assign15mutex.c -pthread
 */

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
typedef HANDLE thread_handle_t;
typedef CRITICAL_SECTION mutex_t;
typedef DWORD thread_return_t;
#define THREAD_CALL WINAPI
#define COUNTER_FORMAT "%I64d"
#else
#include <pthread.h>
typedef pthread_t thread_handle_t;
typedef pthread_mutex_t mutex_t;
typedef void *thread_return_t;
#define THREAD_CALL
#define COUNTER_FORMAT "%lld"
#endif

#define NUM_THREADS           4
#define INCREMENTS_PER_THREAD 50000

static long long counter = 0;
static mutex_t counter_mutex;

static void mutex_init(mutex_t *mutex)
{
#ifdef _WIN32
    InitializeCriticalSection(mutex);
#else
    pthread_mutex_init(mutex, NULL);
#endif
}

static void mutex_lock(mutex_t *mutex)
{
#ifdef _WIN32
    EnterCriticalSection(mutex);
#else
    pthread_mutex_lock(mutex);
#endif
}

static void mutex_unlock(mutex_t *mutex)
{
#ifdef _WIN32
    LeaveCriticalSection(mutex);
#else
    pthread_mutex_unlock(mutex);
#endif
}

static void mutex_destroy(mutex_t *mutex)
{
#ifdef _WIN32
    DeleteCriticalSection(mutex);
#else
    pthread_mutex_destroy(mutex);
#endif
}

static thread_return_t THREAD_CALL increment_counter(void *arg)
{
    int thread_id = *((int *)arg);

    for (int i = 0; i < INCREMENTS_PER_THREAD; i++) {
        mutex_lock(&counter_mutex);
        counter++;
        mutex_unlock(&counter_mutex);
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
    int thread_ids[NUM_THREADS];
    long long expected = (long long)NUM_THREADS * INCREMENTS_PER_THREAD;

    mutex_init(&counter_mutex);

    printf("========================================\n");
    printf("  Part 2: WITH MUTEX (Correct Output)\n");
    printf("========================================\n");
    printf("  Threads            : %d\n", NUM_THREADS);
    printf("  Increments/thread  : %d\n", INCREMENTS_PER_THREAD);
    printf("  Expected counter   : " COUNTER_FORMAT "\n", expected);
    printf("----------------------------------------\n");

    for (int i = 0; i < NUM_THREADS; i++) {
        int rc;

        thread_ids[i] = i + 1;
        rc = start_thread(&threads[i], &thread_ids[i]);

        if (rc != 0) {
            fprintf(stderr, "ERROR: could not create thread %d. Code: %d\n",
                    i + 1, rc);
            mutex_destroy(&counter_mutex);
            return EXIT_FAILURE;
        }

        printf("  Thread %d created.\n", i + 1);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        int rc = join_thread(threads[i]);

        if (rc != 0) {
            fprintf(stderr, "ERROR: could not join thread %d. Code: %d\n",
                    i + 1, rc);
            mutex_destroy(&counter_mutex);
            return EXIT_FAILURE;
        }
    }

    mutex_destroy(&counter_mutex);

    printf("----------------------------------------\n");
    printf("  Expected counter   : " COUNTER_FORMAT "\n", expected);
    printf("  Actual   counter   : " COUNTER_FORMAT "\n", counter);
    printf("  Difference         : " COUNTER_FORMAT "\n", expected - counter);
    printf("\n");

    if (counter == expected) {
        printf("  Mutex worked! Counter is CORRECT every time.\n");
    } else {
        printf("  Unexpected mismatch - check mutex usage.\n");
    }

    printf("========================================\n");
    return EXIT_SUCCESS;
}
