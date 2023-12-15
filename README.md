# Youcef Badaoui

Notes:
N: is the MAX_SIZE of the buffer and number of consumer threads

Q1:
We use a two-dimensional array to represent the matrices A, B and C.
and a circular queue to represent the buffer T, (arrays of size N for the intermediate results and their indices.)

Q2:
Each producer thread must process a distinct section of matrix B and C. Thus, data sharing is done by dividing the tasks.
We also use semaphores to protect and synchronize access to shared data (result matrix and buffer)

Q3:
The only risk i could have think of in my implemntation is Unpredictable results due to the use of the same buffer by all threads in case an unexpected error or overflow occurs.
Also possible lack of error handling in case of unexpected errors or inputs.
