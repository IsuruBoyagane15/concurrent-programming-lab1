The project contains three files with the 3 implementations Serial program, Parallel program with one mutex and Parallel program with read-write locks.

To compile the Serial program, open a cmd and enter,
	gcc -g -Wall -o serial SerialProgram.c -lpthread -lm
To compile the Mutex program, open a cmd and enter,
	gcc -g -Wall -o mutex MutexProgram.c -lpthread -lm
To compile the Read Write Lock program, open a cmd and enter,
	gcc -g -Wall -o rwlock SerialProgram.c -lpthread -lm


Once the program is compiled, enter ./serial or ./mutex, ./rwlock to execute the program respective program.

This will ask some inputs from user as,

Case number - (1/2/3)
This is the number related to the 3 cases given in the assignment.
1 - 99% Member, 0.5% Insert and 0.5% Delete
2 - 90% Member, 5% Insert and 5% Delete
3 - 50% Member, 25% Insert and 25% Delete

Test run count - 1 to any positive number
This is the number of times that the program has to run with the configurations of the above given case.

Number of threads (Only for mutex and rwlock programs) - No of threads that has to run

Here, the amount of operations(m=10000) and amount of random values to populate the linked list (n=1000) are already hardcoded in the program.

Once the above inputs are given, the program will return the average execution time and the standard deviation of the execution.