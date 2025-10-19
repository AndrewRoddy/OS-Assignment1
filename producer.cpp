/////////////////////////////
// Andrew Roddy
// 10/18/2025
// producer.cpp
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

void producer(data* myData) {
    while (true) {
        // Waits until there is an empty buffer
        sem_wait(&myData->empty);
        // Waits until the consumer is done with the buffer
        sem_wait(&myData->mutex);

        int produced = 0;
        // produce the items in the buffer if there are any
        for (int i = 0; i < 2; i++) {
            myData->buffer[i] = rand() % 100 + 1;
            produced += myData->buffer[i];
        }

        std::cout << "~<Producer>~ {|} Produced : " << produced << std::endl;

        // Updates to say we are not longer using the buffer
        sem_post(&myData->mutex); 

        // Updates to say the buffer is full
        sem_post(&myData->full); 

        // Waits a second before going again
        sleep(1);
    }
}

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

    // Makes the size of the shared memory the same as data
    ftruncate(sharedMemory, sizeof(data));

    // Sends error message if failed to create memory
    if (sharedMemory == -1) {
        std::cout << "Producer failed to create shared memory." << std::endl;
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
        std::cout << "Producer : Making the map failed" << std::endl;
        return 1;
    }

    //
    // Different for the producer
    //

    // Initializes the empty buffer
    sem_init(&singleData->empty, 1, 2); 

    // Initializes the full buffer as empty from the start
    sem_init(&singleData->full, 1, 0);

    // Makes sure the buffers are mutually exclusive
    sem_init(&singleData->mutex, 1, 1);

    //
    // End of things different for the producer
    //

    // Starts up the process
    producer(singleData);

    return 0;

}