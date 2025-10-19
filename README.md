# Operating Systems Assignment 1

This code was compiled and run on hornet servers.

# Running
### 1. Cloning the Repository
```
git clone https://github.com/AndrewRoddy/OS-Assignment1
```
This command is used to clone the repository before running it.

### 2. Running the code
```
make run
```
After this the user must use make run to run the code.
This will build the `producer.cpp` and `consumer.cpp`.
This is necessary to run the code.

### 3. Killing the processes
```
make clean
```
To stop the code in a different terminal run `make clean`. This will kill both processes. This is absolutely necessary or else the processes will run forever and ever.

# The Producer Code

### 1. Include Statements
```cpp
#include <iostream>
#include <semaphore.h> // adds semaphores (sem_t)
#include <sys/mman.h> // Adds shm_open
#include <unistd.h> // Adds ftruncate
#include <fcntl.h> // Used for O_CREAT and O_RDWR
```
These include statements are used to ensure the user can operate the proper functions. These allow the user of semaphores, shared memory, `fttruncate` and other important things like `O_CREAT` which is used in creating the shared memory.