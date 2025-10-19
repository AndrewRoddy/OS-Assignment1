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
We then have it wait for a second. This is not important but makes it visually easier to understand in the terminal. This code could be deleted but I decided to leave it in for educational purposes.This value can also only be a minimum of one second as it does not accept floats or doubles. This is also important as I selected the lowest possible amount for this function. Although I didn't mention this earlier this function comes from `unistd.h` which needs to be included to use it. This is okay though as we also need to include this library to run `ftruncate()` so we do not include anything just for this visual element.
### 4. The Main Function


##### 1. Starting up
```cpp
int main() {
    std::cout << "Starting up producer." << std::endl;

    // Sets srand to a random value 
    srand(time(0));

    // Creates or opens the shared memory
    int sharedMemory = shm_open("/shared_buffer", O_CREAT | O_RDWR, 0666);
    // If it failed to open send out an error message
    if (sharedMemory == -1) {
        std::cout << "Producer failed to open shared memory." << std::endl;
        return 1;
    }
```
We do a few initial things in int main to make this possible to run. The first thing we do is I send a starting up message so the user knows that this file is properly initialized. After this I set `srand()` to the current time to allow random numbers to be generated. Then I initialize the shared memory variable with `shm_open()`. This is stored in the `"/shared_buffer"`.
I then have an if statement that prints an error message if it failed to initialize.


##### 2. Starting up
```cpp
// Makes the size of the shared memory the same as data
ftruncate(sharedMemory, sizeof(data));

// Sends error message if failed to create memory
if (sharedMemory == -1) {
	std::cout << "Producer failed to create shared memory." << std::endl;
	return 1;
}
```
I then change the size of the shared memory to be the same as the data. I also include another error check to make sure this is changed properly. By cutting off the end of the shared memory we save space.

##### 3. Mapping
```cpp
// Makes map of a single data value
data* singleData = (data*) mmap(
	0,
	sizeof(data),
	PROT_READ | PROT_WRITE,
	MAP_SHARED,
	sharedMemory,
	0
);

// Sends error message if the map failed to initialize
if (singleData == MAP_FAILED) {
	std::cout << "Producer : Making the map failed" << std::endl;
	return 1;
}
```
##### 4. Initialing the Semaphores
```cpp
// Initializes the empty buffer
sem_init(&singleData->empty, 1, 2); 

// Initializes the full buffer as empty from the start
sem_init(&singleData->full, 1, 0);

// Makes sure the buffers are mutually exclusive
sem_init(&singleData->mutex, 1, 1);
```
This piece of code is only there for the producer and not the consumer. Everything else in the main function is exactly the same for both. This piece of code is to initialize the values in the producer, consumer, and mutex before we run the producer function.

##### 5. Starting the producer process
```cpp
    // Starts up the process
    producer(singleData);

    return 0;
}
```
This just starts the producer process.



# The Consumer Code
### 1. The Same Code
```cpp
#include <iostream>
#include <semaphore.h> // adds semaphores (sem_t)
#include <sys/mman.h> // Adds shm_open
#include <unistd.h> // Adds ftruncate
#include <fcntl.h> // Used for O_CREAT and O_RDWR

// The data that will be shared by the processes
struct data {
    // sem_t => semaphore
    sem_t empty; // tracks empty slots
    sem_t full; // tracks full slots
    sem_t mutex; // makes sure only one process at a time
    int buffer[2]; // Shared buffer
};

void consumer(data* myData) {
    while (true) {
```
This code is the exact same for the consumer as the producer so I will not explain it again.

### 2. Checking and Getting Semaphore Data
```cpp
// Waits until there is a full buffer
sem_wait(&myData->full);
// Waits until the consumer is done with the buffer
sem_wait(&myData->mutex);

int consumed = 0;
// consume the items in the buffer if there are any
for (int i = 0; i < 2; i++) {
	consumed += myData->buffer[i];
	myData->buffer[i] = 0;
}
```
We use `sem_wait` to wait until there is a full buffer and the producer is no longer using that buffer. We then loop through all of the data in the buffers to get the values and then set them back to zero.

### 3. Printing and Signaling
```cpp
		std::cout << "~<Consumer>~ {|} Consumed : " << consumed << std::endl;
		sem_post(&myData->mutex);
		sem_post(&myData->empty);
		sleep(1);
    }
}
```
I then print the results of getting the values. I then updated the mutex to say I am done and signal that the data is empty again and ready to be filled.

### 4. The Same Code Again
```cpp
int main() {
    std::cout << "Starting up consumer." << std::endl;

    // Sets srand to a random value 
    srand(time(0));

    // Creates or opens the shared memory
    int sharedMemory = shm_open("/shared_buffer", O_CREAT | O_RDWR, 0666);

    // If it failed to open send out an error message
    if (sharedMemory == -1) {
        std::cout << "Consumer failed to open shared memory." << std::endl;
        return 1;
    }

    // Makes the size of the shared memory the same as data
    ftruncate(sharedMemory, sizeof(data));
    
    // Sends error message if failed to create memory
    if (sharedMemory == -1) {
        std::cout << "Consumer failed to create shared memory." << std::endl;
        return 1;
    }

    // Makes map of a single data value
    data* singleData = (data*) mmap(
        0,
        sizeof(data),
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        sharedMemory,
        0
    );

    // Sends error message if the map failed to initialize
    if (singleData == MAP_FAILED) {
        std::cout << "Consumer : Making the map failed" << std::endl;
        return 1;
    }

    // Starts up the process
    consumer(singleData);

    return 0;
}
```

All of this code is the exact same or less than the `producer.cpp`. It is not needed to be explained again.
