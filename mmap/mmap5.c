#include <stdio.h>      // For perror(), printf(), fprintf(), stderr
#include <stdlib.h>     // For exit(), EXIT_FAILURE, EXIT_SUCCESS
#include <unistd.h>     // For fork(), pid_t
#include <sys/mman.h>   // For mmap(), munmap(), PROT_READ, PROT_WRITE, MAP_SHARED, MAP_ANON, MAP_FAILED
#include <sys/wait.h>   // For wait()
#include <sys/types.h>  // Standard POSIX system types
#include <string.h>

#define STATUS_NOT_FOUND 0
#define STATUS_SUCCESS   1

typedef struct {
    int id;
    char name[50];
    int age;
} User;

typedef struct {
    int users_count;
    User users[100];
} Database;

typedef struct {
    Database db;
    
    // Child 1: Search Result
    User search_result;
    int search_status; 
    
    // Child 2: Average Age
    double average_age; 
    
    // Child 3: Oldest User
    User oldest_user;
} SharedDB;


void input_Database(Database* db);
void input_User(User* us);

int main(void) {
    
    ssize_t mem_size = sizeof(SharedDB);

    SharedDB* data = mmap(NULL, mem_size, 
                          PROT_READ | PROT_WRITE, 
                          MAP_ANON | MAP_SHARED, 
                          -1, 0);
    
    if (data == MAP_FAILED) {
        perror("mmap() fail");
        exit(EXIT_FAILURE); 
    }

   
    input_Database(&data->db);

    // ==========================================
    // --- Child 1: User Search ---
    // ==========================================
    if (fork() == 0) {
        int target_id;
        printf("\n[Child 1] Enter User ID to search: ");
        if (scanf("%d", &target_id) != 1) {
            data->search_status = STATUS_NOT_FOUND;
            exit(EXIT_FAILURE);
        }

        data->search_status = STATUS_NOT_FOUND; // Default state

        for (int i = 0; i < data->db.users_count; i++) {
            if (data->db.users[i].id == target_id) {
                data->search_result = data->db.users[i];
                data->search_status = STATUS_SUCCESS;
                break;
            }
        }

        exit(EXIT_SUCCESS); // 🛑 Child 1-ը ավարտվեց
    }

    // ==========================================
    // --- Child 2: Average Age Calculation ---
    // ==========================================
    if (fork() == 0) {
        if (data->db.users_count > 0) {
            int sum = 0;
            for (int i = 0; i < data->db.users_count; i++) {
                sum += data->db.users[i].age;
            }
            data->average_age = (double)sum / data->db.users_count;
        } else {
            data->average_age = 0.0;
        }

        exit(EXIT_SUCCESS); // 🛑 Child 2-ը ավարտվեց
    }

    // ==========================================
    // --- Child 3: Oldest User Search ---
    // ==========================================
    if (fork() == 0) {
        if (data->db.users_count > 0) {
            User oldest = data->db.users[0];
            for (int i = 1; i < data->db.users_count; i++) {
                if (data->db.users[i].age > oldest.age) {
                    oldest = data->db.users[i];
                }
            }
            data->oldest_user = oldest;
        }

        exit(EXIT_SUCCESS); // 🛑 Child 3-ը ավարտվեց
    }

    // ==========================================
    // --- PARENT PROCESS ---
    // ==========================================
    
    for (int i = 0; i < 3; i++) {
        wait(NULL);
    }

  
    printf("\n==========================================");
    printf("\n           DATABASE FINAL REPORT          ");
    printf("\n==========================================");

    // Search Result
    if (data->search_status == STATUS_SUCCESS) {
        printf("\n[Search] Found User -> ID: %d, Name: %s, Age: %d", 
               data->search_result.id, data->search_result.name, data->search_result.age);
    } else {
        printf("\n[Search] User not found.");
    }

    // Average Age Result
    printf("\n[Average Age] %.2f years old", data->average_age);

    // Oldest User Result
    if (data->db.users_count > 0) {
        printf("\n[Oldest User] ID: %d, Name: %s, Age: %d", 
               data->oldest_user.id, data->oldest_user.name, data->oldest_user.age);
    }
    printf("\n==========================================\n");

   
    munmap(data, mem_size);
    return 0;
}

void input_Database(Database* db) {
    printf("Enter the count of users: ");
    if (scanf("%d", &db->users_count) != 1 || db->users_count <= 0 || db->users_count > 100) {
        printf("Invalid count!\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < db->users_count; i++) {
        printf("\n--- User %d ---\n", i + 1);
        input_User(&db->users[i]);
    }
}

void input_User(User* us) {
    printf("ID: ");
    scanf("%d", &us->id);
    printf("Name: ");
    scanf("%49s", us->name); 
    printf("Age: ");
    scanf("%d", &us->age);
}