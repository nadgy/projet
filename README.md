# Youcef Badaoui

# Variables:

- BUFFER_SIZE: is the size of the buffer, greater than 0 and entered by the user.
- NUM_CONSUMERS: is the number of consumer threads, greater than 0 and entered by the user.
- shouldExit: is a boolean variable that indicates whether the consumer threads should exit (when all items are consumed).

# Structures:

- BufferItem: is a structure that represents an item in the buffer, it contains the index of the result matrix (i,j) and the value of the result.
- BufferQueue: is a structure that represents the buffer, it contains the buffer items, the head and tail indices, and the number of items produced and consumed to keep track of the buffer state.

# Functions:

- insertItem: is a function that inserts an item in the buffer, it takes the value of the result and the index of the result matrix (i,j) as parameters. (Used by the producer threads)
- removeItem: is a function that removes an item from the buffer, it returns the value of the result and the index of the result matrix (i,j). (Used by the consumer threads)
- Helper functions: are functions that are used to send helpful debug messages to the console. and generate, print and clear matrices.

# Q1:

We use a two-dimensional array to represent the matrices A, B and C.
and a circular queue to represent the buffer T (BufferQueue and the items inside it as BufferItem).

# Q2:

Each producer thread must process a distinct section of matrix B and C. Thus, data sharing is done by dividing the tasks.
We also use semaphores to protect and synchronize access to shared data (result matrix and buffer) and lock the critical sections.
And suspend the producer threads when the buffer is full and the consumer threads when the buffer is empty.

# Q3:

Current implementation does not have any predictable risks, because we are using semaphores to carefully protect and synchronize access to shared data (result matrix and buffer) and lock the critical sections.
