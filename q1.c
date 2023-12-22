#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5

pthread_mutex_t fork_mutex[NUM_PHILOSOPHERS];
pthread_mutex_t bowl_mutex[2];
pthread_mutex_t waiter_mutex;

void eating(int philosopher_id, int left_fork, int right_fork, int bowl_id) {
    printf("Philosopher %d is eating using left fork: %d, right fork: %d, and bowl: %d.\n", philosopher_id, left_fork, right_fork, bowl_id);
    sleep(2); // Simulating eating time
}

void thinking(int philosopher_id) {
    printf("Philosopher %d is thinking.\n", philosopher_id);
    sleep(2); // Simulating thinking time
}

void pick_up_forks(int philosopher_id, int* left_fork, int* right_fork) {
    pthread_mutex_lock(&waiter_mutex);

    while (1) {
        // Attempt to acquire forks in a fixed order to prevent circular waiting
        *left_fork = philosopher_id;
        *right_fork = (philosopher_id + 1) % NUM_PHILOSOPHERS;

        if (!pthread_mutex_trylock(&fork_mutex[*left_fork]) && !pthread_mutex_trylock(&fork_mutex[*right_fork])) {
            pthread_mutex_unlock(&waiter_mutex);
            return;
        }

        pthread_mutex_unlock(&fork_mutex[*left_fork]);
        pthread_mutex_unlock(&fork_mutex[*right_fork]);
        sched_yield(); // Allowing other threads to execute
    }
}

void put_down_forks(int left_fork, int right_fork) {
    pthread_mutex_unlock(&fork_mutex[left_fork]);
    pthread_mutex_unlock(&fork_mutex[right_fork]);
}

void pick_up_bowl(int philosopher_id, int bowl_id) {
    pthread_mutex_lock(&bowl_mutex[bowl_id]);
}

void put_down_bowl(int bowl_id) {
    pthread_mutex_unlock(&bowl_mutex[bowl_id]);
}

void* philosopher(void* args) {
    int philosopher_id = *((int*)args);
    int left_fork, right_fork;
    int bowl_id = philosopher_id % 2; // Alternate between the two bowls

    while (1) {
        thinking(philosopher_id);

        pick_up_forks(philosopher_id, &left_fork, &right_fork);
        pick_up_bowl(philosopher_id, bowl_id);

        eating(philosopher_id, left_fork, right_fork, bowl_id);

        put_down_bowl(bowl_id);
        put_down_forks(left_fork, right_fork);
    }
}

int main() {
    pthread_t philosophers[NUM_PHILOSOPHERS];
    int philosopher_ids[NUM_PHILOSOPHERS];

    // Initialize mutexes
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_init(&fork_mutex[i], NULL);
    }

    for (int i = 0; i < 2; i++) {
        pthread_mutex_init(&bowl_mutex[i], NULL);
    }

    pthread_mutex_init(&waiter_mutex, NULL);

    // Create philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        philosopher_ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, (void*)&philosopher_ids[i]);
    }

    // Wait for philosopher threads to finish
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], NULL);
    }

    // Clean up
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_destroy(&fork_mutex[i]);
    }

    for (int i = 0; i < 2; i++) {
        pthread_mutex_destroy(&bowl_mutex[i]);
    }

    pthread_mutex_destroy(&waiter_mutex);

    return 0;
}
