# Assignment 16

## Question

Develop a multithreaded C program using POSIX threads where multiple threads coordinate access to a shared resource using either semaphores or condition variables. You may implement a simple producer-consumer system, limited resource access system, or thread scheduling simulation. The program should ensure that threads wait correctly when the resource is unavailable and continue execution only when signaled. Demonstrate proper synchronization, safe shared-memory access, and thread communication using functions such as sem_wait(), sem_post(), pthread_cond_wait(), or pthread_cond_signal(). Print messages showing thread execution order and explain how synchronization prevents inconsistent behavior.
