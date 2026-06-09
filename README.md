# Assignment 15

## Question

Write a multithreaded C program using POSIX threads (pthread) where multiple threads increment a shared global counter variable many times. First implement the program without any synchronization and observe the incorrect output caused by a race condition. Then modify the program using a mutex (pthread_mutex_t) to protect the critical section and produce the correct final counter value. Your program must demonstrate thread creation using pthread_create(), synchronization using pthread_mutex_lock() and pthread_mutex_unlock(), and thread completion using pthread_join(). Also explain briefly why the race condition occurs and how the mutex solves the problem.
