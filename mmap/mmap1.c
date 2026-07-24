#include <stdio.h>      // For perror(), printf(), fprintf(), stderr
#include <stdlib.h>     // For exit(), EXIT_FAILURE, EXIT_SUCCESS
#include <unistd.h>     // For fork(), pid_t, getpagesize(), usleep()
#include <sys/mman.h>   // For mmap(), munmap(), PROT_READ, PROT_WRITE, MAP_SHARED, MAP_ANON, MAP_FAILED
#include <sys/wait.h>   // For wait(), waitpid()
#include <sys/types.h>  // Standard POSIX system types
#include <string.h>

int main(void) {
    long pageSize = getpagesize();

    
    char *shared_memory = mmap(NULL, pageSize, 
                              PROT_READ | PROT_WRITE, 
                              MAP_ANON | MAP_SHARED, 
                              -1, 0);

    if (shared_memory == MAP_FAILED) {
        perror("mmap failed");
        exit(EXIT_FAILURE);
    }

    shared_memory[0] = '\0';

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork() fail");
        munmap(shared_memory, pageSize); 
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { 

        while (shared_memory[0] == '\0') {
            sleep(3);
        }

        printf("[Child] Read from shared memory: %s\n", shared_memory);

        munmap(shared_memory, pageSize);
        exit(EXIT_SUCCESS);
    } else {
        
        
        printf("Enter text: ");
        if (fgets(shared_memory, pageSize, stdin) != NULL) {
            shared_memory[strcspn(shared_memory, "\n")] = '\0';
        }   

        wait(NULL); 

        munmap(shared_memory, pageSize);

    }

    return EXIT_SUCCESS;
}