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

### 3. Producer Function

```cpp
void producer(data* myData) {
    while (true) {
        sem_wait(&myData->empty);
        sem_wait(&myData->mutex);

        int produced = 0;
        for (int i = 0; i < 2; i++) {
            myData->buffer[i] = rand() % 100 + 1;
            produced += myData->buffer[i];
        }

        std::cout << "~<Producer>~ {|} Produced : " << produced << std::endl;

        sem_post(&myData->mutex); 
        sem_post(&myData->full); 
        sleep(1);
    }
}
```
This is the most important part of the code. This is what allows the looping and editing of the shared memory.

##### 1. The While Loop
```cpp
while (true) {}
```
It begins with a infinite loop that is never broken as we never want the processes to stop running. It then 

##### 2. Semaphore waiting
```cpp
sem_wait(&myData->empty);
sem_wait(&myData->mutex);
```
After this we wait until the there is an empty block of data and it is not being used. This is what the `empty` and `mutex` are for.

##### 3. The Produced loop
```cpp
int produced = 0;
for (int i = 0; i < 2; i++) {
	myData->buffer[i] = rand() % 100 + 1;
	produced += myData->buffer[i];
}
```
After this we then loop through all of the buffer indexes (which there are only two). As we loop we insert a value into these buffer indexes. We then add this to the produced value to see how much we produced.


##### 4. Printing
```cpp
std::cout << "~<Producer>~ {|} Produced : " << produced << std::endl;
```
After this we print the value that we produced.

##### 5. Updating the semaphores
```cpp
sem_post(&myData->mutex); 
sem_post(&myData->full); 
```
We then update the data in the semaphores to not using it anymore and full so that the consumer knows what to do next.
##### 6. Waiting
```cpp
sleep(1);
```
We then have it wait for a second. This is not important but makes it visually easier to understand in the terminal. This code could be deleted but I decided to leave it in for educational purposes.