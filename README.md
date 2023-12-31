# projet
projet système d'exploitation
Q1:
the main data structures used are matrices (B, C, and A), and a linked list structure (Node) for the buffer. The matrices represent the input matrices, and the linked list acts as a buffer for communication between the producer and consumer threads.

Q2:
Access to shared data is protected using synchronization mechanisms. The code uses a mutex (pthread_mutex_t mutex) to control access to the shared buffer (head and tail pointers). Additionally, semaphores (sem_t empty and sem_t full) are employed to regulate the number of items in the buffer, ensuring that the producer and consumer threads operate in a synchronized manner.

Q3: 
Potential risks in this code include race conditions and deadlock situations. Race conditions may occur if multiple threads access shared data simultaneously without proper synchronization, leading to unpredictable behavior. Deadlocks might occur if threads acquire resources in a way that creates a circular wait, preventing the program from making progress. The use of mutexes and semaphores is designed to mitigate these risks, but it requires careful implementation and testing to ensure correctness. The use of a linked list as a buffer in a first-in-first-out (FIFO) manner helps control the order in which items are produced and consumed. The producer adds items to the tail of the linked list, and the consumer removes items from the head. This ensures that items are processed in the order they were produced, following the FIFO principle. The combination of mutexes and semaphores in the code helps enforce this order and prevents race conditions or conflicts between the producer and consumer threads.
