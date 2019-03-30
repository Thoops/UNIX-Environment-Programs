/* Program to generate 100,000 prime numbers. The primes are stored in a file 
which is a command-line argument. Five threads are created to generate the 
primes as quickly as possible. */

#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <string.h>
#include <sstream>

using namespace std;

#define THREADS 5

pthread_mutex_t my_mutex;

int prime = 0;//The prime that gets written to the file

void* computePrime(void* file);

bool isPrime(int number);

int main (int argc, char **argv)
{
    if(argc != 2){
	    perror("Error: Wrong amount of arguments\n");
     	return 1;
    }
    
	if(!(argv[1])){
	    perror("Error: could not open file\n");
     	return 1;
    }
    
    char* fileName = argv[1];
	pthread_t thread[THREADS];
	
	for(int i = 0; i < THREADS; i++){
	    if( pthread_create(&thread[i], NULL, computePrime, (void*) fileName) == -1 ) {
			perror( "thread error" );
			exit(1);
		}
	}
	
	for(int i = 0; i < THREADS; i++){
	    pthread_join(thread[i], NULL);
	}
		
	return 0;
}

void* computePrime(void* file){
    char* fileName = (char*) file;
    int fd = open(fileName, O_WRONLY);
    srand(time(NULL));
    string str;
    
    
    for(int i = 0; i < 20000; i++){
        
        pthread_mutex_lock(&my_mutex);
        do
        {
            
            prime++;
            
        }while(!isPrime(prime));
        
        
        
        stringstream ss;
        ss << prime;
        
        str = ss.str();
        
        
        if(write(fd, str.c_str(), sizeof(str)) == -1){
            perror("Write failed\n");
	    	exit(0);
        }
        pthread_mutex_unlock(&my_mutex);
    }
}

bool isPrime(int number){
	int i = 2;
	if(number < 2){
		return false;
	}
	while (i < number) {
	  if (number % i == 0 && i != number)
		return false;
	  i++;
	}
	return true;
}