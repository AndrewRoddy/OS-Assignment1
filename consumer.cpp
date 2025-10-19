#include <iostream>
#include <semaphore.h>
#include <thread>
#include <ctime>
#include <sys/mman.h>
#include <unistd.h>
#include <cstdlib>
#include <fcntl.h>

int main() {
    int sharedMemory = shm_open("/shared_buffer", O_CREAT | O_RDWR, 0666);

    if (sharedMemory == -1) {
        std::cout << "Producer failed to open shared memory." << std::endl;
        return 1;
    }
}