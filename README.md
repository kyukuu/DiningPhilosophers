# DiningPhilosophers

This C program models the Dining Philosophers Problem with an additional modification of two bowls placed at the center of the table. Each philosopher thread performs eating and thinking tasks, and the time for each action is simulated using the `sleep()` function. The code uses Mutexes (locks) and Conditional Variables to ensure thread synchronization and avoid deadlocks.

### Code Structure:

1. **`philosopher` Function:**
   - Represents the philosopher thread, alternating between eating and thinking.
   - Uses Mutexes and Conditional Variables for synchronization.

2. **`eating` Function:**
   - Simulates the philosopher entering the eating state.
   - Acquires locks on two forks and a bowl before eating.
   - Prints the philosopher's eating action to the console.

3. **`thinking` Function:**
   - Simulates the philosopher entering the thinking state.
   - Releases locks on the forks and bowl after eating.
   - Prints the philosopher's thinking action to the console.

### Readme File:

#### Why Deadlocks Can Occur:
Deadlocks can occur when multiple philosophers attempt to acquire the locks on forks and bowls simultaneously. If each philosopher holds one fork and is waiting for another held by a neighboring philosopher, and they all wait for the bowl, a cyclic wait scenario can lead to a deadlock.

#### How the Solution Avoids Deadlock:
The code utilizes Mutexes and Conditional Variables to ensure that each philosopher can only enter the eating state if both forks and the bowl are available. This prevents cyclic waiting scenarios, avoiding deadlocks.

#### Fairness of the Solution:
The solution aims for fairness by allowing philosophers to enter the eating state only when all necessary resources (two forks and a bowl) are available. This prevents starvation and ensures that each philosopher gets an opportunity to eat. The fairness can be further improved by implementing a turn-based mechanism, allowing each philosopher to take turns eating.

### Estimated Philosopher Eating Frequency:
In this implementation, all five philosophers have a fair chance to eat, and the rough estimate of how often a philosopher can eat depends on the availability of resources and the scheduling policy of the operating system. A balanced distribution of resources and a fair scheduling algorithm contribute to a relatively equal opportunity for each philosopher to eat.
