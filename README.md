# Threads-and-Synchronization

# GOAL
The goal of this project is to implement a simulation to order tickets from the Tonight Show phone ordering system by multiple people at the same time. It involves illustrations of threads, synchronization, and critical sections (implementations of semaphores).

# DESCRIPTION
The project implements a simulation where there are five call lines, but only three operators. Each phone call represents a thread, and in this simulation we have more threads running than phone lines. When a phone call (thread) arrives, we see if there is a free phone line available. In order to do this, we check whether the connected number of lines is less than the number of lines available. If this condition satisfies, the phone will ring. Otherwise it will receive a busy signal. The connected number of lines and next_id is updated accordingly inside the thread worker function. Finally, it prints out the output with each caller’s id.

# IMPLEMENTATION
## jimmy_fallon.c

The following functions are used:

1. void *phonecall(void *threadId)
takes in a threadId (of type void*) and iterates through each thread. In order to do this, binary semaphores are used to protect critical regions of code. We iteratively check if the connected number of lines is less than the number of lines available. If so, the call will ring and a counting semaphore is used properly to restrict the use of resources (operators). Furthermore, people can speak to the operator, buy a ticket and hang up the call. The connected number of lines and next_id is updated accordingly. Otherwise, the call will receive a busy signal (and binary semaphore is closed).  We simulate the ticket order by sleeping both the conditions for a few seconds.

2. int main(int argc, char *argv[])
takes in an integer argc (count) and a character array argv (vector). It executes the program by initializing threads and handling errors. Furthermore, it waits for these threads to join and destroys both the binary and count semaphores.

Local variables:
int rc;

Global variables:
int next_id = 1;
sem_t connected_lock;
sem_t operators;
