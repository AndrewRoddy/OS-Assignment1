/////////////////////////////
// Andrew Roddy
// 10 / 18 / 2025
// consumer.cpp
/////////////////////////////

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

        std::cout << "~<Consumer>~ {|} Consumed : " << consumed << std::endl;

        sem_post(&myData->mutex); // release the buffer
        sem_post(&myData->empty); // signal that the buffer is empty and item has been consumed

        // Waits second before going again
        sleep(1);
    }
}

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