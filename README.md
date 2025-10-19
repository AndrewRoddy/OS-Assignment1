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

### 2. The data structure

```cpp
// The data that will be shared by the processes
struct data {
    // sem_t => semaphore
    sem_t empty; // tracks empty slots
    sem_t full; // tracks full slots
    sem_t mutex; // makes sure only one process at a time
    int buffer[2]; // Shared buffer
};
```
This structure is created in both the `producer.cpp` and the `consumer.cpp` this allows the storage of the semaphores. These check if the data is empty, full, being used by the other program, and what data is inside of them.
