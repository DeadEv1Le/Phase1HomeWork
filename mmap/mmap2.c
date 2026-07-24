#include <stdio.h>      // For perror(), printf(), fprintf(), stderr
#include <stdlib.h>     // For exit(), EXIT_FAILURE, EXIT_SUCCESS
#include <unistd.h>     // For fork(), pid_t, getpagesize(), usleep()
#include <sys/mman.h>   // For mmap(), munmap(), PROT_READ, PROT_WRITE, MAP_SHARED, MAP_ANON, MAP_FAILED
#include <sys/wait.h>   // For wait(), waitpid()
#include <sys/types.h>  // Standard POSIX system types
#include <string.h>


typedef struct {
    int numbers[100];
    int size;
    volatile int flag; // 👈 Flag-ը պետք է լինի Shared Memory-ում!
    int sum;           // Child-ի հաշվարկած գումարը
    int max;
} SharedData;

int main(){
    
    long pageSize = getpagesize();

    SharedData dat; 
    
    SharedData *data = mmap(NULL, sizeof(SharedData), 
                        PROT_READ | PROT_WRITE, 
                        MAP_SHARED | MAP_ANONYMOUS, 
                        -1, 0);

    if (data == MAP_FAILED) {
        perror("mmap failed");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if(pid < 0){
        perror("fork fail");
        munmap(data, sizeof(SharedData));
        exit(EXIT_FAILURE);
    }
    if(pid == 0){

        while(data->flag == 0){
           usleep(100);
        }
        int local_sum = 0;
        int local_max = data->numbers[0];

        for (int i = 0; i < data->size; i++) {
            local_sum += data->numbers[i];
            
            if (data->numbers[i] > local_max) {
                local_max = data->numbers[i];
            }
        }

      
        data->sum = local_sum;
        data->max = local_max;

        printf("[Child] Calculated Sum = %d, Max = %d\n", data->sum, data->max);

        data->flag = 2;

       
        munmap(data, sizeof(SharedData));
        exit(EXIT_SUCCESS);
        
    }else{
     
        char buf[] = "Parent: Enter array size and elements:\n";
        write(1, buf, strlen(buf)); 

        scanf("%d", &data->size);

    
        if (data->size > 100) {
            data->size = 100;
        }

        for (int i = 0; i < data->size; i++) {
            scanf("%d", &data->numbers[i]);
        }

        data->flag = 1;


        wait(NULL);

    }



    
    



    


    






    return 0;
}