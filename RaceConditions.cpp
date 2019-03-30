/* Program to demonstrate how race conditions can occur with shared memory.
It will allocate a shared memory segment that contains an integer variable 
and set it to zero. It will create a clone that will start adding 1 to the 
shared memory segment's integer 100,000,000 times. The parent process will 
also add1 to the shared memory segment's integer 100,000,000 times. The 
parent will make sure that the child process has completed and then it will 
display the value of integer in shared memory. The parent will then remove 
the shared memory segment. */

#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

const int SHMSZ = 16000;


int main()
{
    int sid;
    key_t key;
    int* sharedInt;
    int status = 0;
    
    /*
    const int shareSize = sizeof(int) * (2 + ARRAY_SIZE);  
    //Allocate shared memory segment
    segmentId = shmget(IPC_PRIVATE, shareSize, S_IRUSR | S_IWUSR); 
    */
    
    if( (key = ftok( "ex.txt", 'a' )) == -1 ) {
        perror( "ftok" );
        return 1;
    }
    
    if( (sid = shmget( key, SHMSZ, IPC_CREAT | SHM_R | SHM_W | SHM_R >> 6 | SHM_W >> 6 )) == -1 ) {
        perror( "shmget" );
        return 1;
    }
    
    if( (sharedInt = (int *) shmat( sid, NULL, 0 )) == (void *) -1 ) {
        perror( "shmat" );
        return 1;
    }
    int A;
    if((A = fork()) == 0){
        for(int i = 0; i < 100000000; i++){
            *sharedInt++;
        }
    }
    
    else{
        for(int i = 0; i < 100000000; i++){
            *sharedInt++;
        }
        wait(&status);
        cout << *sharedInt << endl;
    }
    
    return 0;
}

