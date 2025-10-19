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

int main() {

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


}