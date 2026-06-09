/*
 * Assignment 16 - Producer-Consumer with Semaphores and Condition Variables
 * =========================================================================
 *
 * Part A uses semaphores:
 *   - empty_sem counts free buffer slots
 *   - full_sem counts filled buffer slots
 *   - buffer_sem is a binary semaphore used as a mutex
 *
 * Part B uses a mutex plus condition variables:
 *   - not_full wakes producers when space is available
 *   - not_empty wakes consumers when an item is available
 *
 * Windows compile : gcc -O0 -Wall -Wextra -o assign16.exe assign16.c
 * Linux/macOS     : gcc -O0 -Wall -Wextra -o assign16 assign16.c -pthread
 */

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif
#define _WIN32_WINNT 0x0600
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#endif

#define BUFFER_SIZE      5
#define NUM_PRODUCERS    2
#define NUM_CONSUMERS    2
#define ITEMS_PER_THREAD 4

typedef struct {
    int data[BUFFER_SIZE];
    int head;
    int tail;
    int count;
} CircularBuffer;

#ifdef _WIN32
typedef HANDLE thread_handle_t;
typedef DWORD thread_return_t;
#define THREAD_CALL WINAPI
#define THREAD_OK 0

typedef HANDLE sync_sem_t;
typedef CRITICAL_SECTION sync_mutex_t;
typedef CONDITION_VARIABLE sync_cond_t;
#else
typedef pthread_t thread_handle_t;
typedef void *thread_return_t;
#define THREAD_CALL
#define THREAD_OK NULL

typedef sem_t sync_sem_t;
typedef pthread_mutex_t sync_mutex_t;
typedef pthread_cond_t sync_cond_t;
#endif

typedef thread_return_t(THREAD_CALL *thread_func_t)(void *);

static int create_thread(thread_handle_t *thread, thread_func_t func, void *arg)
{
#ifdef _WIN32
    *thread = CreateThread(NULL, 0, func, arg, 0, NULL);
    return *thread == NULL ? (int)GetLastError() : 0;
#else
    return pthread_create(thread, NULL, func, arg);
#endif
}

static int join_thread(thread_handle_t thread)
{
#ifdef _WIN32
    DWORD result = WaitForSingleObject(thread, INFINITE);

    if (result != WAIT_OBJECT_0) {
        return (int)GetLastError();
    }

    CloseHandle(thread);
    return 0;
#else
    return pthread_join(thread, NULL);
#endif
}

static int sem_create(sync_sem_t *sem, int initial_count)
{
#ifdef _WIN32
    *sem = CreateSemaphore(NULL, initial_count, BUFFER_SIZE, NULL);
    return *sem == NULL ? (int)GetLastError() : 0;
#else
    return sem_init(sem, 0, initial_count);
#endif
}

static int sem_wait_one(sync_sem_t *sem)
{
#ifdef _WIN32
    DWORD result = WaitForSingleObject(*sem, INFINITE);
    return result == WAIT_OBJECT_0 ? 0 : (int)GetLastError();
#else
    return sem_wait(sem);
#endif
}

static int sem_signal_one(sync_sem_t *sem)
{
#ifdef _WIN32
    return ReleaseSemaphore(*sem, 1, NULL) ? 0 : (int)GetLastError();
#else
    return sem_post(sem);
#endif
}

static int sem_close(sync_sem_t *sem)
{
#ifdef _WIN32
    return CloseHandle(*sem) ? 0 : (int)GetLastError();
#else
    return sem_destroy(sem);
#endif
}

static int mutex_create(sync_mutex_t *mutex)
{
#ifdef _WIN32
    InitializeCriticalSection(mutex);
    return 0;
#else
    return pthread_mutex_init(mutex, NULL);
#endif
}

static int mutex_lock_one(sync_mutex_t *mutex)
{
#ifdef _WIN32
    EnterCriticalSection(mutex);
    return 0;
#else
    return pthread_mutex_lock(mutex);
#endif
}

static int mutex_unlock_one(sync_mutex_t *mutex)
{
#ifdef _WIN32
    LeaveCriticalSection(mutex);
    return 0;
#else
    return pthread_mutex_unlock(mutex);
#endif
}

static int mutex_close(sync_mutex_t *mutex)
{
#ifdef _WIN32
    DeleteCriticalSection(mutex);
    return 0;
#else
    return pthread_mutex_destroy(mutex);
#endif
}

static int cond_create(sync_cond_t *cond)
{
#ifdef _WIN32
    InitializeConditionVariable(cond);
    return 0;
#else
    return pthread_cond_init(cond, NULL);
#endif
}

static int cond_wait_one(sync_cond_t *cond, sync_mutex_t *mutex)
{
#ifdef _WIN32
    return SleepConditionVariableCS(cond, mutex, INFINITE) ? 0 : (int)GetLastError();
#else
    return pthread_cond_wait(cond, mutex);
#endif
}

static int cond_signal_one(sync_cond_t *cond)
{
#ifdef _WIN32
    WakeConditionVariable(cond);
    return 0;
#else
    return pthread_cond_signal(cond);
#endif
}

static int cond_close(sync_cond_t *cond)
{
#ifdef _WIN32
    (void)cond;
    return 0;
#else
    return pthread_cond_destroy(cond);
#endif
}

static void sleep_ms(int milliseconds)
{
#ifdef _WIN32
    Sleep((DWORD)milliseconds);
#else
    usleep((useconds_t)milliseconds * 1000);
#endif
}

static void fail_if_error(int rc, const char *operation)
{
    if (rc != 0) {
        fprintf(stderr, "ERROR: %s failed with code %d\n", operation, rc);
        exit(EXIT_FAILURE);
    }
}

static void buffer_print(const CircularBuffer *buffer, const char *label)
{
    printf("      [Buffer %s] count=%d | slots: [", label, buffer->count);

    for (int i = 0; i < BUFFER_SIZE; i++) {
        int slot = (buffer->head + i) % BUFFER_SIZE;

        if (i < buffer->count) {
            printf("%3d", buffer->data[slot]);
        } else {
            printf("  _");
        }

        if (i < BUFFER_SIZE - 1) {
            printf(",");
        }
    }

    printf(" ]\n");
}

/* Part A: semaphore-based solution */
static CircularBuffer buffer_A = { {0}, 0, 0, 0 };
static sync_sem_t empty_sem;
static sync_sem_t full_sem;
static sync_sem_t buffer_sem;
static int item_counter_A = 1;

static thread_return_t THREAD_CALL producer_A(void *arg)
{
    int id = *((int *)arg);

    for (int i = 0; i < ITEMS_PER_THREAD; i++) {
        sleep_ms(rand() % 300 + 100);

        printf("  [P%d] Wants to produce - checking free slots...\n", id);
        fail_if_error(sem_wait_one(&empty_sem), "sem wait empty");
        fail_if_error(sem_wait_one(&buffer_sem), "sem wait mutex");

        int item = item_counter_A++;
        buffer_A.data[buffer_A.tail] = item;
        buffer_A.tail = (buffer_A.tail + 1) % BUFFER_SIZE;
        buffer_A.count++;

        printf("  [P%d] PRODUCED item %2d -> ", id, item);
        buffer_print(&buffer_A, "A");

        fail_if_error(sem_signal_one(&buffer_sem), "sem post mutex");
        fail_if_error(sem_signal_one(&full_sem), "sem post full");
    }

    printf("  [P%d] Done producing.\n", id);
    return THREAD_OK;
}

static thread_return_t THREAD_CALL consumer_A(void *arg)
{
    int id = *((int *)arg);
    int total = (NUM_PRODUCERS * ITEMS_PER_THREAD) / NUM_CONSUMERS;

    for (int i = 0; i < total; i++) {
        sleep_ms(rand() % 400 + 200);

        printf("  [C%d] Wants to consume - checking filled slots...\n", id);
        fail_if_error(sem_wait_one(&full_sem), "sem wait full");
        fail_if_error(sem_wait_one(&buffer_sem), "sem wait mutex");

        int item = buffer_A.data[buffer_A.head];
        buffer_A.head = (buffer_A.head + 1) % BUFFER_SIZE;
        buffer_A.count--;

        printf("  [C%d] CONSUMED item %2d <- ", id, item);
        buffer_print(&buffer_A, "A");

        fail_if_error(sem_signal_one(&buffer_sem), "sem post mutex");
        fail_if_error(sem_signal_one(&empty_sem), "sem post empty");

        printf("  [C%d] Processing item %d...\n", id, item);
    }

    printf("  [C%d] Done consuming.\n", id);
    return THREAD_OK;
}

static void run_part_A(void)
{
    thread_handle_t producer_threads[NUM_PRODUCERS];
    thread_handle_t consumer_threads[NUM_CONSUMERS];
    int producer_ids[NUM_PRODUCERS];
    int consumer_ids[NUM_CONSUMERS];

    buffer_A.head = 0;
    buffer_A.tail = 0;
    buffer_A.count = 0;
    item_counter_A = 1;

    fail_if_error(sem_create(&empty_sem, BUFFER_SIZE), "create empty semaphore");
    fail_if_error(sem_create(&full_sem, 0), "create full semaphore");
    fail_if_error(sem_create(&buffer_sem, 1), "create mutex semaphore");

    for (int i = 0; i < NUM_PRODUCERS; i++) {
        producer_ids[i] = i + 1;
        fail_if_error(create_thread(&producer_threads[i], producer_A,
                                    &producer_ids[i]),
                      "create producer thread");
        printf("  [MAIN] Producer P%d created.\n", i + 1);
    }

    for (int i = 0; i < NUM_CONSUMERS; i++) {
        consumer_ids[i] = i + 1;
        fail_if_error(create_thread(&consumer_threads[i], consumer_A,
                                    &consumer_ids[i]),
                      "create consumer thread");
        printf("  [MAIN] Consumer C%d created.\n", i + 1);
    }

    for (int i = 0; i < NUM_PRODUCERS; i++) {
        fail_if_error(join_thread(producer_threads[i]), "join producer thread");
    }

    for (int i = 0; i < NUM_CONSUMERS; i++) {
        fail_if_error(join_thread(consumer_threads[i]), "join consumer thread");
    }

    fail_if_error(sem_close(&empty_sem), "destroy empty semaphore");
    fail_if_error(sem_close(&full_sem), "destroy full semaphore");
    fail_if_error(sem_close(&buffer_sem), "destroy mutex semaphore");
}

/* Part B: condition-variable-based solution */
static CircularBuffer buffer_B = { {0}, 0, 0, 0 };
static sync_mutex_t mutex_B;
static sync_cond_t not_full;
static sync_cond_t not_empty;
static int item_counter_B = 100;

static thread_return_t THREAD_CALL producer_B(void *arg)
{
    int id = *((int *)arg);

    for (int i = 0; i < ITEMS_PER_THREAD; i++) {
        sleep_ms(rand() % 300 + 100);

        fail_if_error(mutex_lock_one(&mutex_B), "lock condition mutex");

        printf("  [P%d-B] Checking buffer space (count=%d/%d)...\n",
               id, buffer_B.count, BUFFER_SIZE);

        while (buffer_B.count == BUFFER_SIZE) {
            printf("  [P%d-B] Buffer FULL - waiting on not_full...\n", id);
            fail_if_error(cond_wait_one(&not_full, &mutex_B),
                          "wait on not_full");
            printf("  [P%d-B] Woken up - re-checking buffer space.\n", id);
        }

        int item = item_counter_B++;
        buffer_B.data[buffer_B.tail] = item;
        buffer_B.tail = (buffer_B.tail + 1) % BUFFER_SIZE;
        buffer_B.count++;

        printf("  [P%d-B] PRODUCED item %3d -> ", id, item);
        buffer_print(&buffer_B, "B");

        fail_if_error(cond_signal_one(&not_empty), "signal not_empty");
        fail_if_error(mutex_unlock_one(&mutex_B), "unlock condition mutex");
    }

    printf("  [P%d-B] Done producing.\n", id);
    return THREAD_OK;
}

static thread_return_t THREAD_CALL consumer_B(void *arg)
{
    int id = *((int *)arg);
    int total = (NUM_PRODUCERS * ITEMS_PER_THREAD) / NUM_CONSUMERS;

    for (int i = 0; i < total; i++) {
        sleep_ms(rand() % 400 + 200);

        fail_if_error(mutex_lock_one(&mutex_B), "lock condition mutex");

        printf("  [C%d-B] Checking buffer items (count=%d)...\n",
               id, buffer_B.count);

        while (buffer_B.count == 0) {
            printf("  [C%d-B] Buffer EMPTY - waiting on not_empty...\n", id);
            fail_if_error(cond_wait_one(&not_empty, &mutex_B),
                          "wait on not_empty");
            printf("  [C%d-B] Woken up - re-checking buffer items.\n", id);
        }

        int item = buffer_B.data[buffer_B.head];
        buffer_B.head = (buffer_B.head + 1) % BUFFER_SIZE;
        buffer_B.count--;

        printf("  [C%d-B] CONSUMED item %3d <- ", id, item);
        buffer_print(&buffer_B, "B");

        fail_if_error(cond_signal_one(&not_full), "signal not_full");
        fail_if_error(mutex_unlock_one(&mutex_B), "unlock condition mutex");

        printf("  [C%d-B] Processing item %d...\n", id, item);
    }

    printf("  [C%d-B] Done consuming.\n", id);
    return THREAD_OK;
}

static void run_part_B(void)
{
    thread_handle_t producer_threads[NUM_PRODUCERS];
    thread_handle_t consumer_threads[NUM_CONSUMERS];
    int producer_ids[NUM_PRODUCERS];
    int consumer_ids[NUM_CONSUMERS];

    buffer_B.head = 0;
    buffer_B.tail = 0;
    buffer_B.count = 0;
    item_counter_B = 100;

    fail_if_error(mutex_create(&mutex_B), "create condition mutex");
    fail_if_error(cond_create(&not_full), "create not_full condition");
    fail_if_error(cond_create(&not_empty), "create not_empty condition");

    for (int i = 0; i < NUM_PRODUCERS; i++) {
        producer_ids[i] = i + 1;
        fail_if_error(create_thread(&producer_threads[i], producer_B,
                                    &producer_ids[i]),
                      "create producer-B thread");
        printf("  [MAIN] Producer P%d-B created.\n", i + 1);
    }

    for (int i = 0; i < NUM_CONSUMERS; i++) {
        consumer_ids[i] = i + 1;
        fail_if_error(create_thread(&consumer_threads[i], consumer_B,
                                    &consumer_ids[i]),
                      "create consumer-B thread");
        printf("  [MAIN] Consumer C%d-B created.\n", i + 1);
    }

    for (int i = 0; i < NUM_PRODUCERS; i++) {
        fail_if_error(join_thread(producer_threads[i]), "join producer-B thread");
    }

    for (int i = 0; i < NUM_CONSUMERS; i++) {
        fail_if_error(join_thread(consumer_threads[i]), "join consumer-B thread");
    }

    fail_if_error(cond_close(&not_full), "destroy not_full condition");
    fail_if_error(cond_close(&not_empty), "destroy not_empty condition");
    fail_if_error(mutex_close(&mutex_B), "destroy condition mutex");
}

int main(void)
{
    int total_items = NUM_PRODUCERS * ITEMS_PER_THREAD;

    srand(42);

    printf("\n");
    printf("============================================================\n");
    printf("  Producer-Consumer Synchronization Demo\n");
    printf("============================================================\n");
    printf("  Buffer size      : %d\n", BUFFER_SIZE);
    printf("  Producers        : %d\n", NUM_PRODUCERS);
    printf("  Consumers        : %d\n", NUM_CONSUMERS);
    printf("  Items/producer   : %d\n", ITEMS_PER_THREAD);
    printf("  Total items      : %d\n", total_items);

    printf("\n");
    printf("------------------------------------------------------------\n");
    printf("  PART A: Semaphore-based solution\n");
    printf("  empty_sem=%d, full_sem=0, buffer_sem=1\n", BUFFER_SIZE);
    printf("------------------------------------------------------------\n");
    run_part_A();
    printf("\n  PART A complete. All %d items produced and consumed.\n", total_items);

    printf("\n");
    printf("------------------------------------------------------------\n");
    printf("  PART B: Condition-variable-based solution\n");
    printf("  not_full wakes producers, not_empty wakes consumers\n");
    printf("------------------------------------------------------------\n");
    run_part_B();
    printf("\n  PART B complete. All %d items produced and consumed.\n", total_items);

    printf("\n");
    printf("============================================================\n");
    printf("  Comparison\n");
    printf("============================================================\n");
    printf("  Semaphores remember signals by changing a count.\n");
    printf("  Condition variables do not remember signals, so each wait\n");
    printf("  is protected by a while loop that re-checks the condition.\n");
    printf("============================================================\n");

    return 0;
}
